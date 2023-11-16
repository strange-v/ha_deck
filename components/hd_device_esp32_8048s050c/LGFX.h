#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.h>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

#define TFT_WIDTH 800
#define TFT_HEIGHT 480

namespace esphome {
namespace hd_device {

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Touch_GT911 _touch_instance;    
    lgfx::Light_PWM   _light_instance;
    lgfx::Bus_RGB     _bus_instance;
    lgfx::Panel_RGB   _panel_instance;
public:
  LGFX(void);
};

}  // namespace ha_deck
}  // namespace esphome
