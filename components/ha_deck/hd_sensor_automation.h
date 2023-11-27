#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "hd_sensor.h"

namespace esphome {
namespace ha_deck {

class HdSensorClickTrigger : public Trigger<> {
 public:
  HdSensorClickTrigger(HdSensor *widget) {
    widget->add_on_click_callback([this]() { this->trigger(); });
  }
};

}  // namespace ha_deck
}  // namespace esphome
