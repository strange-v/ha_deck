#pragma once

#include "ha_deck_widget.h"

namespace esphome {
namespace ha_deck {

class HdSlider : public HaDeckWidget
{
public:
    void set_text(std::string text);
    void set_icon(std::string icon);
    void set_min(int min);
    void set_max(int max);
    void set_show_value_label(bool value);
    void set_value_label_tpl(std::string tpl);
    void set_value(int value);
    int get_value();

    void add_value_lambda(std::function<optional<int>()> &&f);
    void add_on_value_change_callback(std::function<void(int)> &&callback);
protected:
    void render_();
    void destroy_();
    void update_();
    void set_visible_(bool visible);
    void set_enabled_(bool enabled);
    void apply_style_(HaDeckWidgetStyle *style);
private:
    static void on_value_change_(lv_event_t *e);
    static void on_value_changed_(lv_event_t *e);

    void update_value_label_(int value);

    const char *TAG = "HD_SLIDER";
    std::string text_;
    std::string icon_;
    std::string value_label_tpl_;
    int value_ = 0;
    int min_ = 0;
    int max_ = 1;
    bool show_value_ = false;

    lv_obj_t *lv_widget_;
    lv_obj_t *lv_value_;
    lv_obj_t *lv_slider_;
    lv_obj_t *lv_button_;
    lv_obj_t *lv_button_icon_;
    lv_obj_t *lv_button_text_;

    std::function<optional<int>()> value_fn_ = nullptr;
    CallbackManager<void(int)> value_change_callback_{};
};

class HdSliderStyle : public HaDeckWidgetStyle
{
public:
    HdSliderStyle();
private:
};

}  // namespace ha_deck
}  // namespace esphome