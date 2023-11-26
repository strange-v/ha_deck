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

void HdSlider::set_show_value_label(bool value) {
    show_value_ = value;
}

void HdSlider::set_value_label_tpl(std::string tpl) {
    value_label_tpl_ = tpl;
}

void HdSlider::set_value(int value) {
    value_ = value;
    
    ESP_LOGD(this->TAG, "set_value: %d", value);
    if (!lv_widget_)
        return;
    
    lv_slider_set_value(lv_slider_, value_, LV_ANIM_OFF);
    update_value_label_(value_);
}

int HdSlider::get_value() {
    return value_;
}

void HdSlider::render_() {
    int16_t pad_row = 8;

    lv_widget_ = lv_obj_create(lv_scr_act());
    lv_obj_set_width(lv_widget_, w_);
    lv_obj_set_height(lv_widget_, h_);
    lv_obj_set_x(lv_widget_, x_);
    lv_obj_set_y(lv_widget_, y_);
    lv_obj_set_flex_flow(lv_widget_, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_widget_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(lv_widget_, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_pad_left(lv_widget_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lv_widget_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lv_widget_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lv_widget_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_row(lv_widget_, pad_row, LV_PART_MAIN | LV_STATE_DEFAULT);

    int16_t value_space = 0;
    if (show_value_) {
        lv_value_ = lv_label_create(lv_widget_);
        lv_obj_set_width(lv_value_, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_value_, LV_SIZE_CONTENT);
        lv_obj_set_align(lv_value_, LV_ALIGN_CENTER);

        lv_obj_set_style_pad_left(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_text_font(lv_value_, &Font16, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_update_layout(lv_value_);
        value_space = lv_obj_get_height(lv_value_) + pad_row;
    }

    lv_slider_ = lv_slider_create(lv_widget_);
    lv_slider_set_range(lv_slider_, min_, max_);
    lv_slider_set_value(lv_slider_, value_, LV_ANIM_OFF);
    lv_obj_set_width(lv_slider_, lv_pct(100));

    lv_obj_set_style_pad_left(lv_slider_, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lv_slider_, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lv_slider_, -45, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lv_slider_, -45, LV_PART_KNOB | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(lv_slider_, HdSlider::on_value_change_, LV_EVENT_VALUE_CHANGED, this);
    lv_obj_add_event_cb(lv_slider_, HdSlider::on_value_changed_, LV_EVENT_RELEASED, this);

    int16_t btn_space = 0;
    if (!icon_.empty() || !text_.empty()) {
        lv_button_ = lv_btn_create(lv_widget_);
        lv_obj_set_width(lv_button_, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_button_, LV_SIZE_CONTENT);

        lv_obj_set_flex_flow(lv_button_, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(lv_button_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

        lv_obj_set_style_pad_left(lv_button_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(lv_button_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(lv_button_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(lv_button_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        if (!icon_.empty()) {
            lv_button_icon_ = lv_label_create(lv_button_);
            lv_obj_set_width(lv_button_icon_, 48);
            lv_obj_set_height(lv_button_icon_, 48);
            lv_obj_set_align(lv_button_icon_, LV_ALIGN_CENTER);
            lv_label_set_text(lv_button_icon_, icon_.c_str());
            lv_obj_set_style_text_align(lv_button_icon_, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(lv_button_icon_, &Material48, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        if (!text_.empty())
        {
            lv_button_text_ = lv_label_create(lv_button_);
            lv_obj_set_width(lv_button_text_, LV_SIZE_CONTENT);
            lv_obj_set_height(lv_button_text_, LV_SIZE_CONTENT);
            lv_obj_set_align(lv_button_text_, LV_ALIGN_BOTTOM_MID);
            lv_label_set_text(lv_button_text_, text_.c_str());
            lv_obj_set_style_text_font(lv_button_text_, &Font16, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        lv_obj_update_layout(lv_button_);
        btn_space = lv_obj_get_height(lv_button_) + pad_row;
    }

    lv_obj_set_height(lv_slider_, h_ - value_space - btn_space - 10 * 2);
}

void HdSlider::destroy_() {
    lv_obj_del(lv_widget_);
    lv_widget_ = nullptr;
}

void HdSlider::update_() {
    if (this->value_fn_) {
        auto s = this->value_fn_();
        if (s.has_value() && s.value() != this->value_)
            this->set_value(s.value());
    }
}

void HdSlider::set_visible_(bool visible) {
    if (!lv_widget_)
        return;
    
    if (visible) {
        lv_obj_clear_flag(lv_widget_, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(lv_widget_, LV_OBJ_FLAG_HIDDEN);
    }
}

void HdSlider::set_enabled_(bool enabled) {
    if (!lv_widget_)
        return;
    
    if (enabled) {
        lv_obj_clear_state(lv_widget_, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(lv_widget_, LV_STATE_DISABLED);
    }
}

void HdSlider::apply_style_(HaDeckWidgetStyle *style) {
    auto slider_style = static_cast<HdSliderStyle *>(style);
    if (!slider_style)
        return;

    auto main = slider_style->get_style("main");
    lv_obj_add_style(lv_widget_, main.def, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(lv_widget_, main.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_add_style(lv_widget_, main.checked, LV_PART_MAIN | LV_STATE_CHECKED);

    if (show_value_) {
        auto value = slider_style->get_style("value");
        lv_obj_add_style(lv_value_, value.def, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_style(lv_value_, value.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
        lv_obj_add_style(lv_value_, value.checked, LV_PART_MAIN | LV_STATE_CHECKED);
    }

    auto slider = slider_style->get_style("slider");
    lv_obj_add_style(lv_slider_, slider.def, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(lv_slider_, slider.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_add_style(lv_slider_, slider.checked, LV_PART_MAIN | LV_STATE_CHECKED);

    auto indicator = slider_style->get_style("indicator");
    lv_obj_add_style(lv_slider_, indicator.def, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(lv_slider_, indicator.disabled, LV_PART_INDICATOR | LV_STATE_DISABLED);
    lv_obj_add_style(lv_slider_, indicator.checked, LV_PART_INDICATOR | LV_STATE_CHECKED);

    auto knob = slider_style->get_style("knob");
    lv_obj_add_style(lv_slider_, knob.def, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_add_style(lv_slider_, knob.disabled, LV_PART_KNOB | LV_STATE_DISABLED);
    lv_obj_add_style(lv_slider_, knob.checked, LV_PART_KNOB | LV_STATE_CHECKED);

    bool has_text = !text_.empty();
    bool has_icon = !icon_.empty();

    if (has_icon || has_text) {
        auto button = slider_style->get_style("button");
        lv_obj_add_style(lv_button_, button.def, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_style(lv_button_, button.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
        lv_obj_add_style(lv_button_, button.checked, LV_PART_MAIN | LV_STATE_CHECKED);
    }
}

void HdSlider::on_value_change_(lv_event_t *e) {
    auto obj = (HdSlider*)lv_event_get_user_data(e);
    auto value = lv_slider_get_value(obj->lv_slider_);
    
    obj->update_value_label_(value);
}

void HdSlider::on_value_changed_(lv_event_t *e) {
    auto obj = (HdSlider*)lv_event_get_user_data(e);
    auto value = lv_slider_get_value(obj->lv_slider_);
    
    obj->value_change_callback_.call(value);
}

void HdSlider::update_value_label_(int value) {
    if (show_value_) {
        char buff[32] = {};

        if (value_label_tpl_.empty())
            sprintf(buff, "%d", value);
        else
            sprintf(buff, value_label_tpl_.c_str(), value);
        
        lv_label_set_text(lv_value_, buff);
    }
}

void HdSlider::add_value_lambda(std::function<optional<int>()> &&f) {
    this->value_fn_ = f;
}

void HdSlider::add_on_value_change_callback(std::function<void(int)> &&callback) {
    this->value_change_callback_.add(std::move(callback));
}


HdSliderStyle::HdSliderStyle() {
    StyleGroup value = {
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
    };
    lv_style_init(value.def);
    lv_style_init(value.disabled);
    lv_style_init(value.checked);
    styles_["value"] = value;

    StyleGroup slider = {
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
    };
    lv_style_init(slider.def);
    lv_style_init(slider.disabled);
    lv_style_init(slider.checked);
    styles_["slider"] = slider;

    StyleGroup indicator = {
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
    };
    lv_style_init(indicator.def);
    lv_style_init(indicator.disabled);
    lv_style_init(indicator.checked);
    styles_["indicator"] = indicator;

    StyleGroup knob = {
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
    };
    lv_style_init(knob.def);
    lv_style_init(knob.disabled);
    lv_style_init(knob.checked);
    styles_["knob"] = knob;

    StyleGroup button = {
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
    };
    lv_style_init(button.def);
    lv_style_init(button.disabled);
    lv_style_init(button.checked);
    styles_["button"] = button;
}

}  // namespace ha_deck
}  // namespace esphome