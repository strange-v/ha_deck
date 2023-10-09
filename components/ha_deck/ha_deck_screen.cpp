#include "ha_deck_screen.h"

namespace esphome {
namespace ha_deck {

void HaDeckScreen::setup() { 
    
}

void HaDeckScreen::loop() {
    
}

float HaDeckScreen::get_setup_priority() const { return setup_priority::AFTER_CONNECTION; }

void HaDeckScreen::set_name(std::string name) {
    name_ = name;
}
std::string HaDeckScreen::get_name() {
    return name_;
}

void HaDeckScreen::set_inactivity(uint32_t inactivity) {
    inactivity_ = inactivity * 1000;
}

uint32_t HaDeckScreen::get_inactivity() {
    return inactivity_;
}

void HaDeckScreen::add_widget(HaDeckWidget *widget) {
    widgets_.push_back(widget);
}

void HaDeckScreen::set_active(bool active) {
    ESP_LOGD(this->TAG, "screen %s, set_active: %d", this->get_name().c_str(), active);
    active_ = active;

    for (auto widget : widgets_) {
        if (active) {
            widget->render();
        } else {
            widget->destroy();
        }
    }
}

}  // namespace ha_deck
}  // namespace esphome