#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "ESP_Panel_Library.h"
#include "lvgl.h"

LV_IMG_DECLARE(bg_480x320);

#define TFT_WIDTH ESP_PANEL_LCD_H_RES
#define TFT_HEIGHT ESP_PANEL_LCD_V_RES

namespace esphome {
namespace hd_device {

class lvglDeck {
 public:
  void setup();
  void loop();
  void set_brightness(unit8_t value);
};

class HaDeckDevice : public Component
{
public:
    void setup() override;
    void loop() override;
    float get_setup_priority() const override;
    uint8_t get_brightness();
    void set_brightness(uint8_t value);
private:
    lvglDeck deck;
    unsigned long time_ = 0;
    uint8_t brightness_ = 0;
};

}  // namespace hd_device
}  // namespace esphome