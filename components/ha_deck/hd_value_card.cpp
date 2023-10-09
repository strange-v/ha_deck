#include "hd_value_card.h"

namespace esphome {
namespace ha_deck {

void HdValueCard::set_text(std::string text) {
    text_ = text;
}

void HdValueCard::set_icon(std::string icon) {
    icon_ = icon;
}

void HdValueCard::set_unit(std::string unit) {
    unit_ = unit;
}

void HdValueCard::set_value(std::string value) {
    set_value_(value);
}

void HdValueCard::set_value_(std::string value) {
    value_ = value;
    
    if (!lv_main_)
        return;
    
    lv_label_set_text(lv_value_, value_.c_str());
}

void HdValueCard::render_() {
    lv_main_ = lv_obj_create(lv_scr_act());
    lv_obj_add_flag(lv_main_, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_event_cb(lv_main_, HdValueCard::on_click_, LV_EVENT_SHORT_CLICKED, this);

    lv_obj_set_x(lv_main_, x_);
    lv_obj_set_y(lv_main_, y_);
    lv_obj_set_height(lv_main_, h_ > 0 ? h_ : 96);
    lv_obj_set_width(lv_main_, w_ > 0 ? w_ : 110);
    lv_obj_clear_flag(lv_main_, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(lv_main_, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_main_, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(lv_main_, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(lv_main_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(lv_main_, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(lv_main_, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(lv_main_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lv_main_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lv_main_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lv_main_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    auto value_container = lv_obj_create(lv_main_);
    lv_obj_clear_flag(value_container, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_width(value_container, lv_pct(100));
    lv_obj_set_height(value_container, 64);
    lv_obj_set_align(value_container, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(value_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(value_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(value_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(value_container, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(value_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(value_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(value_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(value_container, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_value_ = lv_label_create(value_container);
    lv_obj_set_width(lv_value_, LV_SIZE_CONTENT);
    lv_obj_set_height(lv_value_, LV_SIZE_CONTENT);
    lv_obj_set_x(lv_value_, 0);
    lv_obj_set_y(lv_value_, 5);
    lv_obj_set_align(lv_value_, LV_ALIGN_CENTER);
    lv_label_set_text(lv_value_, value_.c_str());
    lv_obj_set_style_text_color(lv_value_, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(lv_value_, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lv_value_, &Font24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (!unit_.empty())
    {
        auto unit_lbl = lv_label_create(value_container);
        lv_obj_set_width(unit_lbl, LV_SIZE_CONTENT);
        lv_obj_set_height(unit_lbl, LV_SIZE_CONTENT);
        lv_obj_set_x(unit_lbl, 0);
        lv_obj_set_y(unit_lbl, 5);
        lv_obj_set_align(unit_lbl, LV_ALIGN_BOTTOM_RIGHT);
        lv_label_set_text(unit_lbl, unit_.c_str());
        lv_obj_set_style_text_color(unit_lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(unit_lbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(unit_lbl, &Font16, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    auto text_container = lv_obj_create(lv_main_);
    lv_obj_clear_flag(text_container, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_x(text_container, 0);
    lv_obj_set_y(text_container, 1);
    lv_obj_set_width(text_container, lv_pct(100));
    lv_obj_set_height(text_container, 24);
    lv_obj_set_align(text_container, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_flex_flow(text_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(text_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(text_container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(text_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(text_container, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(text_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(text_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(text_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(text_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(text_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(text_container, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(text_container, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

    if (!icon_.empty()) {
        auto icon_lbl = lv_label_create(text_container);
        lv_obj_set_width(icon_lbl, LV_SIZE_CONTENT);
        lv_obj_set_height(icon_lbl, LV_SIZE_CONTENT);
        lv_obj_set_align(icon_lbl, LV_ALIGN_BOTTOM_MID);
        lv_label_set_text(icon_lbl, icon_.c_str());
        lv_obj_set_style_text_color(icon_lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(icon_lbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(icon_lbl, &Material24, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    if (!text_.empty())
    {
        auto text_lbl = lv_label_create(text_container);
        lv_obj_set_width(text_lbl, LV_SIZE_CONTENT);
        lv_obj_set_height(text_lbl, LV_SIZE_CONTENT);
        lv_obj_set_align(text_lbl, LV_ALIGN_BOTTOM_MID);
        lv_label_set_text(text_lbl, text_.c_str());
        lv_obj_set_style_text_color(text_lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(text_lbl, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(text_lbl, &Font16, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if (enabled_) {
        lv_obj_clear_state(lv_main_, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(lv_main_, LV_STATE_DISABLED);
    }
}

void HdValueCard::destroy_() {
    lv_obj_del(lv_main_);
    lv_main_ = nullptr;
}

void HdValueCard::update_() {
    if (this->value_fn_) {
        auto s = this->value_fn_();
        if (s.has_value() && s.value() != this->value_)
            this->set_value_(s.value());
    }
}

void HdValueCard::set_visible_(bool visible) {
    if (!lv_main_)
        return;
    
    if (visible) {
        lv_obj_clear_flag(lv_main_, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(lv_main_, LV_OBJ_FLAG_HIDDEN);
    }
}

void HdValueCard::set_enabled_(bool enabled) {
    if (!lv_main_)
        return;
    
    if (enabled) {
        lv_obj_clear_state(lv_main_, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(lv_main_, LV_STATE_DISABLED);
    }
}

void HdValueCard::add_value_lambda(std::function<optional<std::string>()> &&f) {
    this->value_fn_ = f;
}

void HdValueCard::add_on_click_callback(std::function<void()> &&callback) {
    this->click_callback_.add(std::move(callback));
}

void HdValueCard::on_click_(lv_event_t *e) {
    auto obj = (HdValueCard*)lv_event_get_user_data(e);
    obj->click_callback_.call();
}

}  // namespace ha_deck
}  // namespace esphome