/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "opentx.h"
#include "sliders.h"
#include "trims.h"

#define HAS_TOPBAR()      (persistentData->options[0].value.boolValue == true)
#define HAS_FM()          (persistentData->options[1].value.boolValue == true)
#define HAS_SLIDERS()     (persistentData->options[2].value.boolValue == true)
#define HAS_TRIMS()       (persistentData->options[3].value.boolValue == true)
#define IS_MIRRORED()     (persistentData->options[4].value.boolValue == true)

const uint8_t LBM_LAYOUT_2x1[] = {
#include "mask_layout2x1.lbm"
};

const ZoneOption OPTIONS_LAYOUT_2x1[] =  {
  { STR_TOP_BAR, ZoneOption::Bool },
  { STR_FLIGHT_MODE, ZoneOption::Bool },
  { STR_SLIDERS, ZoneOption::Bool },
  { STR_TRIMS, ZoneOption::Bool },
  { STR_MIRROR, ZoneOption::Bool },
  { nullptr, ZoneOption::Bool }
};

class Layout2x1: public Layout
{
  public:
    Layout2x1(const LayoutFactory * factory, Layout::PersistentData * persistentData):
      Layout(factory, persistentData)
    {
      decorate();
    }

    void create() override
    {
      Layout::create();
      persistentData->options[0].value.boolValue = true;
      persistentData->options[1].value.boolValue = true;
      persistentData->options[2].value.boolValue = true;
      persistentData->options[3].value.boolValue = true;
      persistentData->options[4].value.boolValue = false;
      persistentData->options[5].value.boolValue = false;
      decorate();
    }

    void decorate()
    {
      Layout::decorate(HAS_TOPBAR(), HAS_SLIDERS(), HAS_TRIMS(), HAS_FM());
    }


    unsigned int getZonesCount() const override
    {
      return 2;
    }

    rect_t getZone(unsigned int index) const override
    {
      rect_t zone;
      zone.w = (LCD_W-3*10) / 2;
      zone.x = (index & 1) ? 20 + zone.w : 10;
      if (persistentData->options[0].value.boolValue) {
        zone.h = (LCD_H-MENU_HEADER_HEIGHT-2*10);
        zone.y = MENU_HEADER_HEIGHT + 10;
      }
      else {
        zone.h = (LCD_H-2*10);
        zone.y = 10;
      }
      return zone;
    }

    void checkEvents() override
    {
      Layout::checkEvents();
      uint8_t newValue = persistentData->options[4].value.boolValue << 4 | persistentData->options[3].value.boolValue << 3 | persistentData->options[2].value.boolValue << 2
                         | persistentData->options[1].value.boolValue << 1 | persistentData->options[0].value.boolValue;
      if (value != newValue) {
        value = newValue;
        // TODO call this from the Layout config window
        this->clear();
        decorate();
      }
    }
  protected:
    uint8_t value = 0;
};

BaseLayoutFactory<Layout2x1> Layout2x1("Layout2x1", "2 x 1", LBM_LAYOUT_2x1, OPTIONS_LAYOUT_2x1);
