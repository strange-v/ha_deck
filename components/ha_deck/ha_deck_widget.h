#pragma once

#include <functional>
#include <string>
#include <map>
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "lvgl.h"

LV_FONT_DECLARE(Font16);
LV_FONT_DECLARE(Font24);
LV_FONT_DECLARE(Material24);
LV_FONT_DECLARE(Material48);
LV_FONT_DECLARE(Numeric42);

namespace esphome {
namespace ha_deck {

enum HaDeckWidgetScale {
    HD_SCALE_NORMAL,
    HD_SCALE_SMALL
};

enum HaDeckWidgetPosition {
    HD_POSITION_TOP,
    HD_POSITION_BOTTOM
};

struct StyleGroup {
    lv_style_t *def;
    lv_style_t *disabled;
    lv_style_t *checked;
};

class HaDeckWidgetStyle
{
public:
    HaDeckWidgetStyle();
    
    StyleGroup get_style(std::string style);

    void set_border_radius(std::string style, uint8_t value, lv_state_t selector);
    void set_bg_color(std::string style, uint32_t value, lv_state_t selector);
    void set_bg_opacity(std::string style, uint8_t value, lv_state_t selector);
    void set_color(std::string style, uint32_t value, lv_state_t selector);
    void set_opacity(std::string style, uint8_t value, lv_state_t selector);
protected:
    void set_border_radius_(StyleGroup group, uint8_t value, lv_state_t selector);
    void set_bg_color_(StyleGroup group, uint32_t value, lv_state_t selector);
    void set_bg_opacity_(StyleGroup group, uint8_t value, lv_state_t selector);
    void set_color_(StyleGroup group, uint32_t value, lv_state_t selector);
    void set_opacity_(StyleGroup group, uint8_t value, lv_state_t selector);

    std::map<std::string, StyleGroup> styles_ = {};
    const char *TAG = "HA_DECK";
};

class StyleManager
{
public:
    virtual HaDeckWidgetStyle *get_style(std::string name) = 0;
};

class HaDeckWidget : public Component
{
public:
    HaDeckWidget();
    void setup() override;
    void loop() override;
    float get_setup_priority() const override;

    void set_position(int16_t x, int16_t y);
    void set_dimensions(int16_t w, int16_t h);
    void set_bg_color(uint32_t color);
    void render();
    void destroy();
    void set_visible(bool visible);
    void set_enabled(bool enabled);
    void set_style(std::string name);

    void add_enabled_lambda(std::function<optional<bool>()> &&f);
    void add_visible_lambda(std::function<optional<bool>()> &&f);
    void add_style_manager(StyleManager *style_manager);
protected:
    virtual void update_() = 0;
    virtual void render_() = 0;
    virtual void destroy_() = 0;
    virtual void set_visible_(bool visible) = 0;
    virtual void set_enabled_(bool enabled) = 0;
    virtual void apply_style_(HaDeckWidgetStyle *style) = 0;
    
    int16_t x_ = 0;
    int16_t y_ = 0;
    int16_t w_ = 0;
    int16_t h_ = 0;
    bool visible_ = true;
    bool enabled_ = false;
    bool rendered_ = false;
    std::string style_;

    std::function<optional<bool>()> visible_fn_ = nullptr;
    std::function<optional<bool>()> enabled_fn_ = nullptr;
    StyleManager *style_manager_ = nullptr;
};

}  // namespace ha_deck
}  // namespace esphome