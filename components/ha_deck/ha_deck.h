#pragma once

#include <map>
#include <string>
#include "ha_deck_screen.h"

namespace esphome {
namespace ha_deck {

class HaDeck : public Component
{
public:
    void setup() override;
    void loop() override;
    float get_setup_priority() const override;
    void set_main_screen(std::string value);
    void set_inactivity_period(uint32_t value);
    void set_inactivity_blank_screen(bool value);
    bool get_inactivity();
    void add_screen(HaDeckScreen *screen);
    void switch_screen(std::string name);
    void add_on_inactivity_change_callback(std::function<void(bool)> &&callback);
private:
    void set_inactivity_(bool value);
    void create_inactivity_screen_();

    const char *TAG = "HA_DECK";
    std::string main_screen_name_;
    CallbackManager<void(bool)> inactivity_change_callback_{};
    uint32_t inactivity_timeout_default_ = 0;
    bool inactivity_blank_screen_ = false;
    uint32_t inactivity_timeout_ = 0;
    bool inactivity_ = false;

    std::map<std::string, HaDeckScreen*> screens_ = {};
    HaDeckScreen *active_screen_ = nullptr;
    lv_obj_t *lvgl_main_screen_ = nullptr;
    lv_obj_t *lvgl_inactivity_screen_ = nullptr;
};

}  // namespace ha_deck
}  // namespace esphome