# Home Assistant Deck, made by strange-v

Config-driven Home Assistant dashboard (built on top of ESPHome using LVGL).

![HA Deck dashboard exampl](/images/ha_deck.jpg)
[Demo video](https://www.youtube.com/watch?v=NPr9ryyTVB4)

## How it works

The HA Deck comprises of two components: hd_device_[name] and ha_deck itself. The first one has such responsibilities:

- Integrates with the hardware (configures display, touch screen, LVGL)
- Exposes display brightness so it can be changed by other components.
- Incapsulates a background image and fonts.

The ha_deck component renders all widgets, switches screens, and controls inactivity behavior.

## Supported devices

- WT32-SC01 PLUS (ESP32-S3, 3.5", 480x320)
- WT32S3-86S/ZX3D95CE01S-AR-4848 (ESP32-S3, 4", 480x480) - **not fully tested**
  *As of today, only the hardware component has been added. HA Deck doesn't auto-scale widget size based on screen resolution, so sliders will look odd when the default parameters are used with a larger (480x480) display.*

**Added by ToViNi (Se example yaml code for board config):**

- Sunton ESP32-3248S035C (ESP32, 3.5", 480x320) - **Only 4MB flash!**
- Sunton Esp32-8048S043C (ESP3-S3, 4.3", 800x480) - **no background image provided**
- Sunton Esp32-8048S050C (ESP3-S3, 5.0", 800x480) - **no background image provided**
- Sunton Esp32-8048S070C (ESP3-S3, 7.0", 800x480) - **no background image provided**
- WT32-SC01 (ESP32, 3.5", 480x320) - **Only 4MB flash!**

## How to use

First, import components using the external [components feature](https://esphome.io/components/external_components.html).

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/tovini/ha_deck
      ref: main
    components: [ hd_device_sc01_plus, ha_deck ]
```

Then, declare the hardware component.

```yaml
hd_device_sc01_plus:
  id: device
  brightness: 75
```

After this, the main component with screens and widgets can be declared (see complete example [here](examples)).

```yaml
ha_deck:
  id: deck
  main_screen: scr_main
  screens:
    - name: scr_main
      widgets:
        - type: button
          position: 8, 8
          text: Rooms
          icon: 󰠡
          enabled: return true;
          on_click:
            lambda: id(deck).switch_screen("scr_rooms");
    - name: scr_rooms
      widgets:
      ... ... ...
```

Make sure you have enough space on the app partitions, as pre-compiled fonts require a lot of space. The icon font contains [common home-based icons](https://pictogrammers.com/docs/library/mdi/guides/home-assistant). You can search icons [here](https://pictogrammers.com/library/mdi/), but make sure they belong to a common home-based icons subset.

## Important note about PSRAM

If a device has PSRAM, it is better to enable it. In the case of a big screen, the display won't work without PSRAM. The configuration below is for WT32S3-86S.

```yaml
esphome:
  name: device_name
  friendly_name: device_friendly_name
  platformio_options:
    board_build.arduino.memory_type: qio_opi
```

## Further development

I'll use HA Deck to build per-room dashboards in my apartment. So, for sure, fixes and additional widgets (e.g., to control warm floors) will be added.

But overall, the approach implemented here isn't correct and goes against ESPHome's architecture. Additionally, it has many limitations, including no support for user-defined images and fonts. Currently, [Clyde Stubbs](https://github.com/clydebarrow) is working on bringing LVGL support to ESPHome in the correct way (through the display component). After this is done (including further performance improvements in the ESPHome display part), the HA Deck may become useless. If not, I'll consider migrating it to use the display component.
