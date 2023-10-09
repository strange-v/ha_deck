#include "ha_deck_widget.h"

namespace esphome {
namespace ha_deck {

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

void HaDeckWidget::add_visible_lambda(std::function<optional<bool>()> &&f) {
    visible_fn_ = f;
}

void HaDeckWidget::add_enabled_lambda(std::function<optional<bool>()> &&f) {
    enabled_fn_ = f;
}

}  // namespace ha_deck
}  // namespace esphome