#pragma once

#include "ha_deck_widget.h"

namespace esphome {
namespace ha_deck {

class HdValueCard : public HaDeckWidget
{
public:
    void set_text(std::string text);
    void set_icon(std::string icon);
    void set_unit(std::string unit);
    void set_value(std::string value);

    void add_value_lambda(std::function<optional<std::string>()> &&f);
    void add_on_click_callback(std::function<void()> &&callback);
protected:
    void render_();
    void destroy_();
    void update_();
    void set_visible_(bool visible);
    void set_enabled_(bool enabled);
    void set_value_(std::string value);
private:
    static void on_click_(lv_event_t *e);

    const char *TAG = "HD_VALUE_CARD";
    std::string text_;
    std::string icon_;
    std::string unit_;
    std::string value_;

    lv_obj_t *lv_main_;
    lv_obj_t *lv_value_;

    std::function<optional<std::string>()> value_fn_ = nullptr;
    CallbackManager<void()> click_callback_{};
};

}  // namespace ha_deck
}  // namespace esphome