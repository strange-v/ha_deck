#include "hd_slider.h"

namespace esphome {
namespace ha_deck {

void HdSlider::set_text(std::string text) {
    text_ = text;
}

void HdSlider::set_icon(std::string icon) {
    icon_ = icon;
}

void HdSlider::set_min(int min) {
    min_ = min;
}

void HdSlider::set_max(int max) {
    max_ = max;
}

void HdSlider::set_value(int value) {
    value_ = value;
    
    ESP_LOGD(this->TAG, "set_value: %d", value);
    if (!slider_panel_)
        return;
    
    lv_slider_set_value(slider_, value_, LV_ANIM_OFF);
}

int HdSlider::get_value() {
    return value_;
}

void HdSlider::render_() {
    slider_panel_ = lv_obj_create(lv_scr_act());
    lv_obj_set_width(slider_panel_, w_ > 0 ? w_ : 64);
    lv_obj_set_height(slider_panel_, h_ > 0 ? h_ : 304);
    lv_obj_set_x(slider_panel_, x_);
    lv_obj_set_y(slider_panel_, y_);
    lv_obj_clear_flag(slider_panel_, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(slider_panel_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(slider_panel_, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(slider_panel_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(slider_panel_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(slider_panel_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(slider_panel_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(slider_panel_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(slider_panel_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_left(slider_panel_, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(slider_panel_, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(slider_panel_, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(slider_panel_, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    slider_ = lv_slider_create(slider_panel_);
    lv_slider_set_range(slider_, min_, max_);
    lv_slider_set_value(slider_, value_, LV_ANIM_OFF);
    if (lv_slider_get_mode(slider_) == LV_SLIDER_MODE_RANGE)
        lv_slider_set_left_value(slider_, 0, LV_ANIM_OFF);

    lv_obj_add_event_cb(slider_, HdSlider::on_value_change_, LV_EVENT_RELEASED, this);

    lv_obj_set_height(slider_, 276);
    lv_obj_set_width(slider_, lv_pct(100));
    lv_obj_set_style_radius(slider_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(slider_, lv_color_hex(0xFFE082), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(slider_, 25, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(slider_, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(slider_, lv_color_hex(0xFFE082), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(slider_, 200, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(slider_, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(slider_, lv_color_hex(0xFFE082), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(slider_, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(slider_, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(slider_, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(slider_, -45, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(slider_, -45, LV_PART_KNOB | LV_STATE_DEFAULT);

    if (text_ != "" || icon_ != "") {
        auto label = lv_label_create(slider_panel_);
        lv_obj_set_width(label, LV_SIZE_CONTENT);
        lv_obj_set_height(label, LV_SIZE_CONTENT);
        lv_obj_set_align(label, LV_ALIGN_BOTTOM_MID);

        if (text_ != "") {
            lv_label_set_text(label, text_.c_str());
            lv_obj_set_style_text_font(label, &Font16, LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            lv_label_set_text(label, icon_.c_str());
            lv_obj_set_style_text_font(label, &Material24, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void HdSlider::destroy_() {
    lv_obj_del(slider_panel_);
    slider_panel_ = nullptr;
}

void HdSlider::update_() {
    if (this->value_fn_) {
        auto s = this->value_fn_();
        if (s.has_value() && s.value() != this->value_)
            this->set_value(s.value());
    }
}

void HdSlider::set_visible_(bool visible) {
    if (!slider_panel_)
        return;
    
    if (visible) {
        lv_obj_clear_flag(slider_panel_, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(slider_panel_, LV_OBJ_FLAG_HIDDEN);
    }
}

void HdSlider::set_enabled_(bool enabled) {
    if (!slider_panel_)
        return;
    
    if (enabled) {
        lv_obj_clear_state(slider_panel_, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(slider_panel_, LV_STATE_DISABLED);
    }
}

void HdSlider::on_value_change_(lv_event_t *e) {
    auto obj = (HdSlider*)lv_event_get_user_data(e);
    auto value = lv_slider_get_value(obj->slider_);
    
    obj->value_change_callback_.call(value);
}

void HdSlider::add_value_lambda(std::function<optional<int>()> &&f) {
    this->value_fn_ = f;
}

void HdSlider::add_on_value_change_callback(std::function<void(int)> &&callback) {
    this->value_change_callback_.add(std::move(callback));
}

}  // namespace ha_deck
}  // namespace esphome