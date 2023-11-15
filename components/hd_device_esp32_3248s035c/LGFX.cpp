#include "LGFX.h"

namespace esphome {
namespace hd_device {

LGFX::LGFX(void)
{
  {
    auto cfg = _bus_instance.config();

    // SPI Display
    cfg.spi_host         = HSPI_HOST;
    cfg.spi_mode         = 0;
    cfg.freq_write       = 40000000;
    cfg.freq_read        = 16000000;
    cfg.spi_3wire        = true;
    cfg.use_lock         = true;
    cfg.dma_channel      = SPI_DMA_CH_AUTO;
    cfg.pin_sclk         = 14;
    cfg.pin_mosi         = 13;
    cfg.pin_miso         = 12;
    cfg.pin_dc           =  2;

    _bus_instance.config(cfg);
    _panel_instance.setBus(&_bus_instance);
  }

  {
    auto cfg = _panel_instance.config();

    cfg.pin_cs           =    15;
    cfg.pin_rst          =    -1;
    cfg.pin_busy         =    -1;

    cfg.panel_width      =   320;
    cfg.panel_height     =   480;
    cfg.offset_x         =     0;
    cfg.offset_y         =     0;
    cfg.offset_rotation  =     0;
    cfg.dummy_read_pixel =     8;
    cfg.dummy_read_bits  =     1;
    cfg.readable         =  true;
    cfg.invert           = false;
    cfg.rgb_order        = false;
    cfg.dlen_16bit       = false;
    cfg.bus_shared       = false;

    _panel_instance.config(cfg);
  }

  {
    auto cfg = _light_instance.config();

    cfg.pin_bl = 27;
    cfg.invert = false;
    cfg.freq   = 44100;
    cfg.pwm_channel = 7;

    _light_instance.config(cfg);
    _panel_instance.setLight(&_light_instance);
  }

  {
    auto cfg = _touch_instance.config();
    cfg.pin_int  = GPIO_NUM_36;   // INT pin number
    cfg.pin_sda  = GPIO_NUM_33;   // I2C SDA pin number
    cfg.pin_scl  = GPIO_NUM_32;   // I2C SCL pin number
    cfg.i2c_addr = 0x5D;          // I2C device addr
    cfg.i2c_port = 1;             // I2C port number
    cfg.freq     = 400000;        // I2C freq
    cfg.x_min    =  14;
    cfg.x_max    = 310;
    cfg.y_min    =   5;
    cfg.y_max    = 448;
    cfg.offset_rotation = 0;
    cfg.bus_shared = false;

    _touch_instance.config(cfg);
    _panel_instance.setTouch(&_touch_instance);
  }

  setPanel(&_panel_instance);
}

}  // namespace ha_deck
}  // namespace esphome
