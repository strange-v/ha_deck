#include "hd_sensor.h"

namespace esphome {
namespace ha_deck {

void HdSensor::set_text(std::string text) {
    text_ = text;
}

void HdSensor::set_icon(std::string icon) {
    icon_ = icon;
}

void HdSensor::set_unit(std::string unit) {
    unit_ = unit;
}

void HdSensor::set_value(std::string value) {
    set_value_(value);
}

void HdSensor::set_scale(HaDeckWidgetScale scale) {
    scale_ = scale;
}

void HdSensor::set_text_position(HaDeckWidgetPosition position) {
    text_position_ = position;
}

void HdSensor::set_value_(std::string value) {
    value_ = value;
    
    if (!lv_widget_)
        return;
    
    lv_label_set_text(lv_value_, value_.c_str());
}

void HdSensor::render_() {
    lv_widget_ = lv_btn_create(lv_scr_act());

    lv_obj_add_event_cb(lv_widget_, HdSensor::on_click_, LV_EVENT_SHORT_CLICKED, this);

    lv_obj_set_width(lv_widget_, w_);
    lv_obj_set_height(lv_widget_, h_);
    lv_obj_set_x(lv_widget_, x_);
    lv_obj_set_y(lv_widget_, y_);

    lv_obj_set_style_shadow_width(lv_widget_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(lv_widget_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(lv_widget_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lv_widget_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lv_widget_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lv_widget_, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    int16_t text_space = 0;
    if (!text_.empty()) {
        lv_text_container_ = lv_obj_create(lv_widget_);

        lv_obj_set_width(lv_text_container_, lv_pct(100));
        lv_obj_set_height(lv_text_container_, LV_SIZE_CONTENT);
        lv_obj_clear_flag(lv_text_container_, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_clear_flag(lv_text_container_, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_align(lv_text_container_, text_position_ == HD_POSITION_TOP ? LV_ALIGN_TOP_MID : LV_ALIGN_BOTTOM_MID);

        lv_obj_set_style_border_width(lv_text_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_pad_left(lv_text_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(lv_text_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(lv_text_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(lv_text_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_flex_flow(lv_text_container_, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(lv_text_container_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_style_pad_column(lv_text_container_, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

        if (!icon_.empty() && scale_ == HD_SCALE_SMALL) {
            lv_icon_ = lv_label_create(lv_text_container_);
            lv_obj_set_width(lv_icon_, LV_SIZE_CONTENT);
            lv_obj_set_height(lv_icon_, LV_SIZE_CONTENT);
            lv_label_set_text(lv_icon_, icon_.c_str());
            lv_obj_set_style_text_align(lv_icon_, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(lv_icon_, &Material24, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        lv_text_ = lv_label_create(lv_text_container_);
        lv_obj_set_width(lv_text_, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_text_, LV_SIZE_CONTENT);
        lv_label_set_text(lv_text_, text_.c_str());
        lv_obj_set_style_text_font(lv_text_, &Font16, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_update_layout(lv_text_container_);
        text_space = lv_obj_get_height(lv_text_container_);
    }

    lv_value_container_ = lv_obj_create(lv_widget_);

    lv_obj_set_width(lv_value_container_, lv_pct(100));
    lv_obj_set_height(lv_value_container_, LV_SIZE_CONTENT);
    lv_obj_set_align(lv_value_container_, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(lv_value_container_, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(lv_value_container_, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_border_width(lv_value_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_left(lv_value_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lv_value_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lv_value_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lv_value_container_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_flex_flow(lv_value_container_, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(lv_value_container_, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (scale_ == HD_SCALE_SMALL)
        lv_obj_set_flex_align(lv_value_container_, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER);
    else
        lv_obj_set_flex_align(lv_value_container_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER);

    if (!icon_.empty() && scale_ == HD_SCALE_NORMAL) {
        lv_icon_ = lv_label_create(lv_value_container_);
        lv_obj_set_width(lv_icon_, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_icon_, LV_SIZE_CONTENT);
        lv_label_set_text(lv_icon_, icon_.c_str());
        lv_obj_set_style_text_align(lv_icon_, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(lv_icon_, &Material48, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    lv_value_ = lv_label_create(lv_value_container_);
    lv_obj_set_width(lv_value_, LV_SIZE_CONTENT);
    lv_obj_set_height(lv_value_, LV_SIZE_CONTENT);

    lv_obj_set_style_pad_left(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lv_value_, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_label_set_text(lv_value_, value_.c_str());
    if (scale_ == HD_SCALE_SMALL)
        lv_obj_set_style_text_font(lv_value_, &Font24, LV_PART_MAIN | LV_STATE_DEFAULT);
    else
        lv_obj_set_style_text_font(lv_value_, &Numeric42, LV_PART_MAIN | LV_STATE_DEFAULT); 

    if (!unit_.empty()) {
        lv_unit_ = lv_label_create(lv_value_container_);
        lv_obj_set_width(lv_unit_, LV_SIZE_CONTENT);
        lv_obj_set_height(lv_unit_, LV_SIZE_CONTENT);
        lv_obj_set_align(lv_unit_, LV_ALIGN_BOTTOM_MID);
        lv_label_set_text(lv_unit_, unit_.c_str());
        lv_obj_set_style_text_font(lv_unit_, &Font16, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (scale_ == HD_SCALE_SMALL)
            lv_obj_set_style_pad_bottom(lv_unit_, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        else
            lv_obj_set_style_pad_bottom(lv_unit_, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    lv_obj_update_layout(lv_value_container_);
    int16_t value_space = lv_obj_get_height(lv_value_container_);

    lv_obj_update_layout(lv_value_container_);
    int16_t y = 0;
    if (text_position_ == HD_POSITION_TOP) {
        y = (h_ - 10 * 2 - text_space) / 2 - value_space / 2 + text_space;
    }
    else {
        y = (h_ - 10 * 2 - text_space) / 2 - value_space / 2;
    }
    lv_obj_set_y(lv_value_container_, y);
}

void HdSensor::destroy_() {
    lv_obj_del(lv_widget_);
    lv_widget_ = nullptr;
}

void HdSensor::update_() {
    if (this->value_fn_) {
        auto s = this->value_fn_();
        if (s.has_value() && s.value() != this->value_)
            this->set_value_(s.value());
    }
}

void HdSensor::set_visible_(bool visible) {
    if (!lv_widget_)
        return;
    
    if (visible) {
        lv_obj_clear_flag(lv_widget_, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(lv_widget_, LV_OBJ_FLAG_HIDDEN);
    }
}

void HdSensor::set_enabled_(bool enabled) {
    if (!lv_widget_)
        return;
    
    if (enabled) {
        lv_obj_clear_state(lv_widget_, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(lv_widget_, LV_STATE_DISABLED);
    }
}

void HdSensor::add_value_lambda(std::function<optional<std::string>()> &&f) {
    this->value_fn_ = f;
}

void HdSensor::add_on_click_callback(std::function<void()> &&callback) {
    this->click_callback_.add(std::move(callback));
}

void HdSensor::on_click_(lv_event_t *e) {
    auto obj = (HdSensor*)lv_event_get_user_data(e);
    obj->click_callback_.call();
}

void HdSensor::apply_style_(HaDeckWidgetStyle *style) {
    auto sensor_style = static_cast<HdSensorStyle *>(style);
    if (!sensor_style)
        return;

    auto main = sensor_style->get_style("main");
    lv_obj_add_style(lv_widget_, main.def, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(lv_widget_, main.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_add_style(lv_widget_, main.checked, LV_PART_MAIN | LV_STATE_CHECKED);

    auto value = sensor_style->get_style("value");
    lv_obj_add_style(lv_value_container_, value.def, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(lv_value_container_, value.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_add_style(lv_value_container_, value.checked, LV_PART_MAIN | LV_STATE_CHECKED);

    bool has_text = !text_.empty();
    bool has_icon = !icon_.empty();

    if (has_text) {
        auto text = sensor_style->get_style("text");
        lv_obj_add_style(lv_text_container_, text.def, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_style(lv_text_container_, text.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
        lv_obj_add_style(lv_text_container_, text.checked, LV_PART_MAIN | LV_STATE_CHECKED);
    }
    if (has_icon) {
        auto icon = sensor_style->get_style("icon");
        lv_obj_add_style(lv_icon_, icon.def, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_style(lv_icon_, icon.disabled, LV_PART_MAIN | LV_STATE_DISABLED);
        lv_obj_add_style(lv_icon_, icon.checked, LV_PART_MAIN | LV_STATE_CHECKED);
    }
}

HdSensorStyle::HdSensorStyle() {
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

    StyleGroup value = {
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
        (lv_style_t *)malloc(sizeof(lv_style_t)),
    };
    lv_style_init(value.def);
    lv_style_init(value.disabled);
    lv_style_init(value.checked);
    styles_["value"] = value;
}

}  // namespace ha_deck
}  // namespace esphome