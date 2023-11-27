#pragma once

#include "ha_deck_widget.h"

namespace esphome {
namespace ha_deck {

class HdSensor : public HaDeckWidget
{
public:
    void set_text(std::string icon);
    void set_icon(std::string icon);
    void set_unit(std::string unit);
    void set_value(std::string value);
    void set_scale(HaDeckWidgetScale scale);
    void set_text_position(HaDeckWidgetPosition position);

    void add_value_lambda(std::function<optional<std::string>()> &&f);
    void add_on_click_callback(std::function<void()> &&callback);
protected:
    void render_();
    void destroy_();
    void update_();
    void set_visible_(bool visible);
    void set_enabled_(bool enabled);
    void set_value_(std::string value);
    void apply_style_(HaDeckWidgetStyle *style);
private:
    static void on_click_(lv_event_t *e);

    std::string text_;
    std::string icon_;
    std::string unit_;
    std::string value_;
    HaDeckWidgetScale scale_ = HD_SCALE_NORMAL;
    HaDeckWidgetPosition text_position_ = HD_POSITION_BOTTOM;

    lv_obj_t *lv_widget_;
    lv_obj_t *lv_value_container_;
    lv_obj_t *lv_text_container_;
    lv_obj_t *lv_icon_;
    lv_obj_t *lv_value_;
    lv_obj_t *lv_unit_;
    lv_obj_t *lv_text_;

    std::function<optional<std::string>()> value_fn_ = nullptr;
    CallbackManager<void()> click_callback_{};
};

class HdSensorStyle : public HaDeckWidgetStyle
{
public:
    HdSensorStyle();
};

}  // namespace ha_deck
}  // namespace esphome