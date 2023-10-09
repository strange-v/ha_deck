#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "hd_value_card.h"

namespace esphome {
namespace ha_deck {

class HdValueCardClickTrigger : public Trigger<> {
 public:
  HdValueCardClickTrigger(HdValueCard *card) {
    card->add_on_click_callback([this]() { this->trigger(); });
  }
};

}  // namespace ha_deck
}  // namespace esphome
