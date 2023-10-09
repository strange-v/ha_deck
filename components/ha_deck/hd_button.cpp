#include "hd_button.h"

namespace esphome {
namespace ha_deck {

void HdButton::set_text(std::string text) {
    text_ = text;
}

void HdButton::set_icon(std::string icon) {
    icon_ = icon;
}

void HdButton::set_toggle(bool toggle) {
    toggle_ = toggle;
}

bool HdButton::is_checked() {
    return checked_;
}

void HdButton::add_checked_lambda(std::function<optional<bool>()> &&f) {
    checked_fn_ = f;
}

void HdButton::add_on_click_callback(std::function<void()> &&callback) {
    this->click_callback_.add(std::move(callback));
}

void HdButton::add_on_turn_on_callback(std::function<void()> &&callback) {
    this->turn_on_callback_.add(std::move(callback));
}

void HdButton::add_on_turn_off_callback(std::function<void()> &&callback) {
    this->turn_off_callback_.add(std::move(callback));
}

void HdButton::add_on_long_press_callback(std::function<void()> &&callback) {
    this->long_press_callback_.add(std::move(callback));
}

void HdButton::render_() {
    lv_obj_t *icon;
    lv_obj_t *label;

    button_ = lv_btn_create(lv_scr_act());

    lv_obj_add_state(button_, LV_STATE_DISABLED);
    lv_obj_add_event_cb(button_, HdButton::on_click_, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(button_, HdButton::on_long_press_, LV_EVENT_LONG_PRESSED, this);
    lv_obj_add_event_cb(button_, HdButton::on_released_, LV_EVENT_RELEASED, this);

    lv_obj_set_height(button_, h_ > 0 ? h_ : 96);
    lv_obj_set_width(button_, w_ > 0 ? w_ : 110);
    lv_obj_align(button_, LV_ALIGN_TOP_LEFT, x_, y_);
    lv_obj_add_flag(button_, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(button_, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_shadow_width(button_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(button_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(button_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(button_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(button_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(button_, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(button_, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(button_, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(button_, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(button_, 5, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_color(button_, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_opa(button_, 150, LV_PART_MAIN | LV_STATE_DISABLED);
    // Checked
    lv_obj_set_style_bg_color(button_, lv_color_hex(0xFCD663), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(button_, 125, LV_PART_MAIN | LV_STATE_CHECKED);
    // Disabled
    lv_obj_set_style_bg_color(button_, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_opa(button_, 5, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_color(button_, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_opa(button_, 150, LV_PART_MAIN | LV_STATE_DISABLED);

    bool has_text = text_ != "";
    bool has_icon = icon_ != "";

    if (has_icon) {
        icon = lv_label_create(button_);
        lv_obj_set_width(icon, 48);
        lv_obj_set_height(icon, 48);
        lv_obj_set_style_bg_color(icon, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DISABLED);
        lv_obj_set_align(icon, has_text ? LV_ALIGN_TOP_MID : LV_ALIGN_CENTER);
        lv_label_set_text(icon, icon_.c_str());
        lv_obj_set_style_text_align(icon, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(icon, &Material48, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if (has_text) {
        label = lv_label_create(button_);
        lv_obj_set_width(label, LV_SIZE_CONTENT);
        lv_obj_set_height(label, LV_SIZE_CONTENT);
        lv_obj_set_align(label, has_icon ? LV_ALIGN_BOTTOM_MID : LV_ALIGN_CENTER);
        lv_label_set_text(label, text_.c_str());
        lv_obj_set_style_text_font(label, &Font16, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    set_visible(visible_);
    set_enabled(enabled_);
    if (toggle_) {
        lv_obj_add_flag(button_, LV_OBJ_FLAG_CHECKABLE);
        set_checked_(checked_);
    }
}

void HdButton::destroy_() {
    lv_obj_del(button_);
    button_ = nullptr;
}

void HdButton::update_() {
    if (this->checked_fn_) {
        auto s = this->checked_fn_();
        if (s.has_value() && s.value() != this->checked_) {
            this->set_checked_(s.value());
        }
    }
}

void HdButton::set_visible_(bool visible) {
    if (!button_)
        return;
    
    if (visible) {
        lv_obj_clear_flag(button_, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(button_, LV_OBJ_FLAG_HIDDEN);
    }
}

void HdButton::set_enabled_(bool enabled) {
    if (!button_)
        return;
    
    if (enabled) {
        lv_obj_clear_state(button_, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(button_, LV_STATE_DISABLED);
    }
}

void HdButton::set_checked_(bool checked) {
    checked_ = checked;
    
    if (!button_)
        return;
    
    if (checked) {
        lv_obj_add_state(button_, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(button_, LV_STATE_CHECKED);
    }
}

void HdButton::on_click_(lv_event_t *e) {
    auto obj = (HdButton*)lv_event_get_user_data(e);
    obj->click_callback_.call();

    if (!obj->toggle_)
        return;

    auto checked = lv_obj_has_state(obj->button_, LV_STATE_CHECKED);
    if (obj->checked_) {
        obj->turn_off_callback_.call();
    } else {
        obj->turn_on_callback_.call();
    }
}

void HdButton::on_long_press_(lv_event_t *e) {
    auto obj = (HdButton*)lv_event_get_user_data(e);
    obj->long_pressed_ = true;
    obj->long_press_callback_.call();
}

void HdButton::on_released_(lv_event_t *e) {
    auto obj = (HdButton*)lv_event_get_user_data(e);
    if (obj->long_pressed_) {
        obj->long_pressed_ = false;
        obj->set_checked_(obj->checked_);
    }
}

}  // namespace ha_deck
}  // namespace esphome