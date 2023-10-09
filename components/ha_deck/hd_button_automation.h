#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "hd_button.h"

namespace esphome {
namespace ha_deck {

template<typename... Ts> class HdButtonCondition : public Condition<Ts...> {
 public:
  HdButtonCondition(HdButton *parent, bool state) : parent_(parent), state_(state) {}
  bool check(Ts... x) override { return this->parent_->is_checked() == this->state_; }

 protected:
  HdButton *parent_;
  bool state_;
};

class HdButtonClickTrigger : public Trigger<> {
 public:
  HdButtonClickTrigger(HdButton *button) {
    button->add_on_click_callback([this]() { this->trigger(); });
  }
};

class HdButtonTurnOnTrigger : public Trigger<> {
 public:
  HdButtonTurnOnTrigger(HdButton *button) {
    button->add_on_turn_on_callback([this]() { this->trigger(); });
  }
};

class HdButtonTurnOffTrigger : public Trigger<> {
 public:
  HdButtonTurnOffTrigger(HdButton *button) {
    button->add_on_turn_off_callback([this]() { this->trigger(); });
  }
};

class HdButtonLongPressTrigger : public Trigger<> {
 public:
  HdButtonLongPressTrigger(HdButton *button) {
    button->add_on_long_press_callback([this]() { this->trigger(); });
  }
};

}  // namespace ha_deck
}  // namespace esphome
