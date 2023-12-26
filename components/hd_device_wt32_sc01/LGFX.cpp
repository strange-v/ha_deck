#include "LGFX.h"

namespace esphome {
namespace hd_device {

LGFX::LGFX(void)
{
  { // Configure bus control settings.
      auto cfg = _bus_instance.config(); // Get the bus configuration structure.
      cfg.spi_host = VSPI_HOST;// Select the SPI to use (VSPI_HOST or HSPI_HOST)
      cfg.spi_mode = 0; // Set SPI communication mode (0 ~ 3)
      cfg.freq_write = 40000000; // SPI clock during transmission (maximum 80MHz, rounded to 80MHz divided by an integer)
      cfg.freq_read = 16000000; // SPI clock when receiving
      cfg.spi_3wire = false; // Set true if receiving is done using MOSI pin
      cfg.use_lock = true; // Set true to use transaction lock
      cfg.dma_channel = 1; // Set the DMA channel(1or2.0=disable)Set the DMA channel to use(0=DMA not used)
      cfg.pin_sclk = 14; // Set SPI SCLK pin number
      cfg.pin_mosi = 13; // Set the SPI MOSI pin number
      cfg.pin_miso = 12; // Set SPI MISO pin number (-1 = disable)
      cfg.pin_dc = 21; // Set SPI D/C pin number (-1 = disable)
      _bus_instance.config(cfg); // Apply the settings to the bus.
      _panel_instance.setBus(&_bus_instance); // Set the bus to the panel.
  }
  { // Set display panel control.
      auto cfg = _panel_instance.config(); // Get the structure for display panel settings.
      cfg.pin_cs = 15; // Pin number to which CS is connected (-1 = disable)
      cfg.pin_rst = 22; // Pin number to which RST is connected (-1 = disable)
      cfg.pin_busy = -1; // Pin number to which BUSY is connected (-1 = disable)
      cfg.memory_width = 320; // Maximum width supported by the driver IC
      cfg.memory_height = 480; // Maximum height supported by the driver IC
      cfg.panel_width = 320; // Actual displayable width
      cfg.panel_height = 480; // Actual display height
      cfg.offset_x = 0; // Panel X direction offset amount
      cfg.offset_y = 0; // Panel Y direction offset amount
      cfg.offset_rotation = 0; // Offset of rotation direction value 0~7 (4~7 are upside down)
      cfg.dummy_read_pixel = 8; // Number of bits for dummy read before pixel reading
      cfg.dummy_read_bits = 1; // Number of bits for dummy read before reading data other than pixels
      cfg.readable = true; // Set to true if data can be read
      cfg.invert = false; // Set to true if the brightness of the panel is inverted
      cfg.rgb_order = false; // Set to true if the red and blue colors of the panel are swapped
      cfg.dlen_16bit = false; // Set to true for panels that send data length in 16-bit units
      cfg.bus_shared = true; // Set to true if the bus is shared with the SD card (control the bus with drawJpgFile, etc.)
        _panel_instance.config(cfg);
  }
  { // Set backlight control. (Delete if unnecessary)
      auto cfg = _light_instance.config(); // Get the structure for backlight configuration.
      cfg.pin_bl = 23; // Pin number to which the backlight is connected
      cfg.invert = false; // true to invert the backlight brightness
      cfg.freq = 44100; // Backlight PWM frequency
      cfg.pwm_channel = 7; // PWM channel number to use
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance); // Set the backlight on the panel.
  }
  {// Configure touch screen control. (Delete if unnecessary)
      auto cfg = _touch_instance.config();
      cfg.x_min = 0; // Minimum X value obtained from the touch screen (raw value)
      cfg.x_max = 320; // Maximum X value obtained from the touch screen (raw value)
      cfg.y_min = 0; // Minimum Y value obtained from the touch screen (raw value)
      cfg.y_max = 480; // Maximum Y value obtained from the touch screen (raw value)
      cfg.pin_int = 39; // Pin number to which INT is connected
      cfg.bus_shared = false; // Set true if using the same bus as the screen
      cfg.offset_rotation = 0; // Adjust display and touch direction Set with a value from 0 to 7
      // For I2C connection
      cfg.i2c_port = 1; // Select I2C to use (0 or 1)
      cfg.i2c_addr = 0x38; // I2C device address number or 14
      cfg.pin_sda = 18; // Pin number to which SDA is connected
      cfg.pin_scl = 19; // Pin number to which SCL is connected
      cfg.freq = 400000; // Set I2C clock
     _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);//Set the touch screen on the panel
  }
  setPanel(&_panel_instance); // Set the panel to use.
}

}  // namespace ha_deck
}  // namespace esphome
