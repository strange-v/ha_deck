#include "ha_deck_widget.h"

namespace esphome {
namespace ha_deck {

HaDeckWidget::HaDeckWidget() {
    auto buff = heap_caps_malloc(1024, MALLOC_CAP_DMA);
}

void HaDeckWidget::setup() {
}

void HaDeckWidget::loop() {
    if (this->visible_fn_) {
        auto s = this->visible_fn_();
        if (s.has_value() && s.value() != this->visible_)
            this->set_visible(s.value());
    }
    if (this->enabled_fn_) {
        auto s = this->enabled_fn_();
        if (s.has_value() && s.value() != this->enabled_)
            this->set_enabled(s.value());
    }
    update_();
}

float HaDeckWidget::get_setup_priority() const { return setup_priority::AFTER_CONNECTION; }

void HaDeckWidget::set_position(int16_t x, int16_t y) {
    x_ = x;
    y_ = y;
}

void HaDeckWidget::set_dimensions(int16_t w, int16_t h) {
    w_ = w;
    h_ = h;
}

void HaDeckWidget::render() {
    if (rendered_)
        return;

    rendered_ = true;
    render_();

    auto style = style_manager_->get_style(style_);
    if (style)
        apply_style_(style);
}

void HaDeckWidget::destroy() {
    if (!rendered_)
        return;

    rendered_ = false;
    destroy_();
}

void HaDeckWidget::set_visible(bool visible) {
    visible_ = visible;
    set_visible_(visible);
}

void HaDeckWidget::set_enabled(bool enabled) {
    enabled_ = enabled;
    set_enabled_(enabled);
}

void HaDeckWidget::set_style(std::string name) {
    style_ = name;
}

void HaDeckWidget::add_visible_lambda(std::function<optional<bool>()> &&f) {
    visible_fn_ = f;
}

void HaDeckWidget::add_enabled_lambda(std::function<optional<bool>()> &&f) {
    enabled_fn_ = f;
}

void HaDeckWidget::add_style_manager(StyleManager *style_manager)
{
    style_manager_ = style_manager;
}


HaDeckWidgetStyle::HaDeckWidgetStyle()
{
    lv_style_init(&main_def_);
    lv_style_init(&main_disabled_);
    lv_style_init(&main_checked_);
    main_.def = &main_def_;
    main_.disabled = &main_disabled_;
    main_.checked = &main_checked_;
}
StyleGroup *HaDeckWidgetStyle::get_main() {
    return &main_;
}
void HaDeckWidgetStyle::set_border_radius(uint8_t value, lv_state_t selector) {
    set_border_radius_(&main_, value, selector);
}
void HaDeckWidgetStyle::set_bg_color(uint32_t value, lv_state_t selector) {
    set_bg_color_(&main_, value, selector);
}
void HaDeckWidgetStyle::set_bg_opacity(uint8_t value, lv_state_t selector) {
    set_bg_opacity_(&main_, value, selector);
}
void HaDeckWidgetStyle::set_color(uint32_t value, lv_state_t selector)
{
    set_color_(&main_, value, selector);
}
void HaDeckWidgetStyle::set_opacity(uint8_t value, lv_state_t selector)
{
    set_opacity_(&main_, value, selector);
}

void HaDeckWidgetStyle::set_border_radius_(StyleGroup *style, uint8_t value, lv_state_t selector) {
    if (selector & LV_STATE_DEFAULT) {
        lv_style_set_radius(style->def, value);
    }
    if (selector & LV_STATE_DISABLED) {
        lv_style_set_radius(style->disabled, value);
    }
    if (selector & LV_STATE_CHECKED) {
        lv_style_set_radius(style->checked, value);
    }
}
void HaDeckWidgetStyle::set_bg_color_(StyleGroup *style, uint32_t value, lv_state_t selector) {
    if (selector == LV_STATE_DEFAULT) {
        lv_style_set_bg_color(style->def, lv_color_hex(value));
    }
    if (selector & LV_STATE_DISABLED) {
        lv_style_set_bg_color(style->disabled, lv_color_hex(value));
    }
    if (selector & LV_STATE_CHECKED) {
        lv_style_set_bg_color(style->checked, lv_color_hex(value));
    }
}
void HaDeckWidgetStyle::set_bg_opacity_(StyleGroup *style, uint8_t value, lv_state_t selector) {
    if (selector == LV_STATE_DEFAULT) {
        lv_style_set_bg_opa(style->def, value);
    }
    if (selector & LV_STATE_DISABLED) {
        lv_style_set_bg_opa(style->disabled, value);
    }
    if (selector & LV_STATE_CHECKED) {
        lv_style_set_bg_opa(style->checked, value);
    }
}
void HaDeckWidgetStyle::set_color_(StyleGroup *style, uint32_t value, lv_state_t selector)
{
    if (selector == LV_STATE_DEFAULT) {
        lv_style_set_text_color(style->def, lv_color_hex(value));
    }
    if (selector & LV_STATE_DISABLED) {
        lv_style_set_text_color(style->disabled, lv_color_hex(value));
    }
    if (selector & LV_STATE_CHECKED) {
        lv_style_set_text_color(style->checked, lv_color_hex(value));
    }
}
void HaDeckWidgetStyle::set_opacity_(StyleGroup *style, uint8_t value, lv_state_t selector)
{
    if (selector == LV_STATE_DEFAULT) {
        lv_style_set_text_opa(style->def, value);
    }
    if (selector & LV_STATE_DISABLED) {
        lv_style_set_text_opa(style->disabled, value);
    }
    if (selector & LV_STATE_CHECKED) {
        lv_style_set_text_opa(style->checked, value);
    }
}

}  // namespace ha_deck
}  // namespace esphome