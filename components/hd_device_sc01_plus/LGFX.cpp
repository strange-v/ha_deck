#include "LGFX.h"

namespace esphome {
namespace hd_device {

LGFX::LGFX(void)
{
  {
    auto cfg = _bus_instance.config();
    cfg.freq_write = 20000000;
    cfg.pin_wr = 47;
    cfg.pin_rd = -1;
    cfg.pin_rs = 0;

    // LCD data interface, 8bit MCU (8080)
    cfg.pin_d0 = 9;
    cfg.pin_d1 = 46;
    cfg.pin_d2 = 3;
    cfg.pin_d3 = 8;
    cfg.pin_d4 = 18;
    cfg.pin_d5 = 17;
    cfg.pin_d6 = 16;
    cfg.pin_d7 = 15;

    _bus_instance.config(cfg);
    _panel_instance.setBus(&_bus_instance);
  }

  {
    auto cfg = _panel_instance.config();

    cfg.pin_cs = -1;
    cfg.pin_rst = 4;
    cfg.pin_busy = -1;

    cfg.panel_width = TFT_WIDTH;
    cfg.panel_height = TFT_HEIGHT;
    cfg.offset_x = 0;
    cfg.offset_y = 0;
    cfg.offset_rotation = 0;
    cfg.dummy_read_pixel = 8;
    cfg.dummy_read_bits = 1;
    cfg.readable = true;
    cfg.invert = true;
    cfg.rgb_order = false;
    cfg.dlen_16bit = false;
    cfg.bus_shared = true;

    _panel_instance.config(cfg);
  }

  {
    auto cfg = _light_instance.config();

    cfg.pin_bl = 45;
    cfg.invert = false;
    cfg.freq = 44100;
    cfg.pwm_channel = 7;

    _light_instance.config(cfg);
    _panel_instance.setLight(&_light_instance);
  }

  {
    auto cfg = _touch_instance.config();

    cfg.x_min = 0;
    cfg.x_max = 319;
    cfg.y_min = 0;
    cfg.y_max = 479;
    cfg.pin_int = 7;
    cfg.bus_shared = true;
    cfg.offset_rotation = 0;

    cfg.i2c_port = 1;
    cfg.i2c_addr = 0x38;
    cfg.pin_sda = 6;
    cfg.pin_scl = 5;
    cfg.freq = 400000;

    _touch_instance.config(cfg);
    _panel_instance.setTouch(&_touch_instance);
  }
  setPanel(&_panel_instance);
}

}  // namespace ha_deck
}  // namespace esphome
