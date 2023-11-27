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

void HdButton::add_icon_color_lambda(std::function<optional<uint32_t>()> &&f) {
    icon_color_fn_ = f;
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
    lv_widget_ = lv_btn_create(lv_scr_act());

    lv_obj_add_state(lv_widget_, LV_STATE_DISABLED);
    lv_obj_add_event_cb(lv_widget_, HdButton::on_click_, LV_EVENT_SHORT_CLICKED, this);
    lv_obj_add_event_cb(lv_widget_, HdButton::on_long_press_, LV_EVENT_LONG_PRESSED, this);
    lv_obj_add_event_cb(lv_widget_, HdButton::on_released_, LV_EVENT_RELEASED, this);

    lv_obj_set_height(lv_widget_, h_ > 0 ? h_ : 96);
    lv_obj_set_width(lv_widget_, w_ > 0 ? w_ : 110);
    lv_obj_align(lv_widget_, LV_ALIGN_TOP_LEFT, x_, y_);
    lv_obj_add_flag(lv_widget_, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(lv_widget_, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_shadow_width(lv_widget_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(lv_widget_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lv_widget_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lv_widget_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lv_widget_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(lv_widget_, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    bool has_text = !text_.empty();
    bool has_icon = !icon_.empty();

    if (has_icon) {
        lv_icon_ = lv_label_create(lv_widget_);
        lv_obj_set_width(lv_icon_, 48);
        lv_obj_set_height(lv_icon_, 48);
        lv_obj_set_align(lv_icon_, has_text ? LV_ALIGN_TOP_MID : LV_ALIGN_CENTER);
        lv_label_set_text(lv_icon_, icon_.c_str());
        lv_obj_set_style_text_align(lv_icon_, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(lv_icon_, &Material48, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    if (has_text) {
        lv_label_ = lv_label_create(lv_widget_);
        lv_obj_set_width(lv_label_, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_label_, LV_SIZE_CONTENT);
        lv_obj_set_align(lv_label_, has_icon ? LV_ALIGN_BOTTOM_MID : LV_ALIGN_CENTER);
        lv_label_set_text(lv_label_, text_.c_str());
        lv_obj_set_style_text_font(lv_label_, &Font16, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    set_visible(visible_);
    set_enabled(enabled_);
    if (toggle_) {
        lv_obj_add_flag(lv_widget_, LV_OBJ_FLAG_CHECKABLE);
        set_checked_(checked_);
    }
}

void HdButton::destroy_() {
    lv_obj_del(lv_widget_);
    lv_widget_ = nullptr;
}

void HdButton::update_() {
    if (this->checked_fn_) {
        auto s = this->checked_fn_();
        if (s.has_value() && s.value() != this->checked_) {
            this->set_checked_(s.value());
        }
    }
    // if (this->icon_color_fn_) {
    //     auto s = this->icon_color_fn_();
    //     if (s.has_value() && s.value() != this->icon_color_) {
    //         this->set_icon_color(s.value());
    //     }
    // }
}

void HdButton::set_visible_(bool visible) {
    if (!lv_widget_)
        return;
    
    if (visible) {
        lv_obj_clear_flag(lv_widget_, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(lv_widget_, LV_OBJ_FLAG_HIDDEN);
    }
}

void HdButton::set_enabled_(bool enabled) {
    if (!lv_widget_)
        return;
    
    if (enabled) {
        lv_obj_clear_state(lv_widget_, LV_STATE_DISABLED);
        if (lv_label_)
            lv_obj_clear_state(lv_label_, LV_STATE_DISABLED);
        if (lv_icon_)
            lv_obj_clear_state(lv_icon_, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(lv_widget_, LV_STATE_DISABLED);
        if (lv_label_)
            lv_obj_add_state(lv_label_, LV_STATE_DISABLED);
        if (lv_icon_)
            lv_obj_add_state(lv_icon_, LV_STATE_DISABLED);
    }
}

void HdButton::apply_style_(HaDeckWidgetStyle *style)
{
    auto button_style = static_cast<HdButtonStyle *>(style);
    if (!button_style)
        return;

    auto main = button_style->get_style("main");
    lv_obj_add_style(lv_widget_, main.def, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(lv_widget_, main.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_add_style(lv_widget_, main.checked, LV_PART_MAIN | LV_STATE_CHECKED);

    bool has_text = !text_.empty();
    bool has_icon = !icon_.empty();

    if (has_text) {
        auto text = button_style->get_style("text");
        lv_obj_add_style(lv_label_, text.def, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_style(lv_label_, text.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
        lv_obj_add_style(lv_label_, text.checked, LV_PART_MAIN | LV_STATE_CHECKED);
    }
    if (has_icon) {
        auto icon = button_style->get_style("icon");
        lv_obj_add_style(lv_icon_, icon.def, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_style(lv_icon_, icon.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
        lv_obj_add_style(lv_icon_, icon.checked, LV_PART_MAIN | LV_STATE_CHECKED);
    }
}

void HdButton::set_checked_(bool checked) {
    checked_ = checked;
    
    if (!lv_widget_)
        return;
    
    if (checked) {
        lv_obj_add_state(lv_widget_, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(lv_widget_, LV_STATE_CHECKED);
    }
}

void HdButton::on_click_(lv_event_t *e) {
    auto obj = (HdButton*)lv_event_get_user_data(e);
    obj->click_callback_.call();

    if (!obj->toggle_)
        return;

    auto checked = lv_obj_has_state(obj->lv_widget_, LV_STATE_CHECKED);
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


HdButtonStyle::HdButtonStyle() {
    StyleGroup text = {
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
    };
    lv_style_init(text.def);
    lv_style_init(text.disabled);
    lv_style_init(text.checked);
    styles_["text"] = text;

    StyleGroup icon = {
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
    };
    lv_style_init(icon.def);
    lv_style_init(icon.disabled);
    lv_style_init(icon.checked);
    styles_["icon"] = icon;
}

}  // namespace ha_deck
}  // namespace esphome