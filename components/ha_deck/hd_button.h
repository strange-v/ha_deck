#pragma once

#include "ha_deck_widget.h"

namespace esphome {
namespace ha_deck {

class HdButton : public HaDeckWidget
{
public:
    void set_text(std::string text);
    void set_icon(std::string icon);
    void set_toggle(bool toggle);
    bool is_checked();

    void add_checked_lambda(std::function<optional<bool>()> &&f);
    void add_on_click_callback(std::function<void()> &&callback);
    void add_on_turn_on_callback(std::function<void()> &&callback);
    void add_on_turn_off_callback(std::function<void()> &&callback);
    void add_on_long_press_callback(std::function<void()> &&callback);
protected:
    void render_();
    void destroy_();
    void update_();
    void set_visible_(bool visible);
    void set_enabled_(bool enabled);
private:
    static void on_click_(lv_event_t *e);
    static void on_long_press_(lv_event_t *e);
    static void on_released_(lv_event_t *e);
    
    void set_checked_(bool enabled);

    const char *TAG = "HD_BUTTON";
    bool toggle_ = false;
    bool checked_ = false;
    bool long_pressed_ = false;
    std::string text_;
    std::string icon_;
    lv_obj_t *button_;

    std::function<optional<bool>()> checked_fn_ = nullptr;
    CallbackManager<void()> click_callback_{};
    CallbackManager<void()> turn_on_callback_{};
    CallbackManager<void()> turn_off_callback_{};
    CallbackManager<void()> long_press_callback_{};
};

}  // namespace ha_deck
}  // namespace esphome