#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "hd_slider.h"

namespace esphome {
namespace ha_deck {

template<typename... Ts> class HdSliderCondition : public Condition<Ts...> {
 public:
  HdSliderCondition(HdSlider *parent, int32_t state) : parent_(parent), state_(state) {}
  int32_t check(Ts... x) override { return this->parent_->get_value() == this->state_; }

 protected:
  HdSlider *parent_;
  int32_t state_;
};

class HdSliderChangeTrigger : public Trigger<int32_t> {
 public:
  HdSliderChangeTrigger(HdSlider *slider) {
    slider->add_on_value_change_callback([this](int32_t value) { this->trigger(value); });
  }
};

}  // namespace ha_deck
}  // namespace esphome