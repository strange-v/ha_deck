# Home Assistant Deck

Config-driven Home Assistant dashboard (built on top of ESPHome using LVGL)

## How it works

The HA Deck comprises of two components: hd_device_[name] and ha_deck itself. The first one has such responsibilities:

- Integrates with the hardware (configures display, touch screen, LVGL)
- Exposes display brightness so it can be changed by other components.
- Incapsulates a background image and fonts.

As of today, only the WT32-SC01 PLUS (ESP32-S3, 3.5", 480x320) component exists. The ZX3D95CE01S-AR-4848 (ESP32-S3, 4", 480x480) will be added next.

The ha_deck  component renders all widgets, switches screens, and controls inactivity behavior.

## How to use

First, import components using the external [components feature](https://esphome.io/components/external_components.html).

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/strange-v/ha_deck
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

## Further development

I'll use HA Deck to build per-room dashboards in my apartment. So, for sure, fixes, additional widgets (e.g., to control warm floors), and at least one new hardware component (for the mentioned 4" 480x480 panel) will be added.

But overall, the approach implemented here isn't correct and goes against ESPHome's architecture. Additionally, it has many limitations, including no support for user-defined images and fonts. Currently, [Clyde Stubbs](https://github.com/clydebarrow) is working on bringing LVGL support to ESPHome in the correct way (through the display component). After this is done (including further performance improvements in the ESPHome display part), the HA Deck may become useless. If not, I'll consider migrating it to use the display component.
