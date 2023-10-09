#pragma once

#include "hd_button_automation.h"
#include "hd_slider_automation.h"
#include "hd_value_card_automation.h"
#include "ha_deck.h"

namespace esphome {
namespace ha_deck {

class HaDeckInactivityChangeTrigger : public Trigger<bool> {
 public:
  HaDeckInactivityChangeTrigger(HaDeck *deck) {
    deck->add_on_inactivity_change_callback([this](bool value) { this->trigger(value); });
  }
};

}  // namespace ha_deck
}  // namespace esphome