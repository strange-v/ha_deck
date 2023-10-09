#pragma once

#include <functional>
#include <string>
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "lvgl.h"

LV_FONT_DECLARE(Font16);
LV_FONT_DECLARE(Font24);
LV_FONT_DECLARE(Material24);
LV_FONT_DECLARE(Material48);

namespace esphome {
namespace ha_deck {

class HaDeckWidget : public Component
{
public:
    void setup() override;
    void loop() override;
    float get_setup_priority() const override;

    void set_position(int16_t x, int16_t y);
    void set_dimensions(int16_t w, int16_t h);
    void render();
    void destroy();
    void set_visible(bool visible);
    void set_enabled(bool enabled);

    void add_enabled_lambda(std::function<optional<bool>()> &&f);
    void add_visible_lambda(std::function<optional<bool>()> &&f);
protected:
    virtual void update_() = 0;
    virtual void render_() = 0;
    virtual void destroy_() = 0;
    virtual void set_visible_(bool visible) = 0;
    virtual void set_enabled_(bool enabled) = 0;
    
    int16_t x_ = 0;
    int16_t y_ = 0;
    int16_t w_ = 0;
    int16_t h_ = 0;
    bool visible_ = true;
    bool enabled_ = false;
    bool rendered_ = false;

    std::function<optional<bool>()> visible_fn_ = nullptr;
    std::function<optional<bool>()> enabled_fn_ = nullptr;
};

}  // namespace ha_deck
}  // namespace esphome