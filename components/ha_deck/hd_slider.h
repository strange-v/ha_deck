#pragma once

#include "ha_deck_widget.h"

namespace esphome {
namespace ha_deck {

class HdSlider : public HaDeckWidget
{
public:
    void set_text(std::string text);
    void set_icon(std::string icon);
    void set_min(int32_t min);
    void set_max(int32_t max);
    void set_value(int32_t value);
    int32_t get_value();

    void add_value_lambda(std::function<optional<int32_t>()> &&f);
    void add_on_value_change_callback(std::function<void(int32_t)> &&callback);
protected:
    void render_();
    void destroy_();
    void update_();
    void set_visible_(bool visible);
    void set_enabled_(bool enabled);
private:
    static void on_value_change_(lv_event_t *e);

    const char *TAG = "HD_SLIDER";
    std::string text_;
    std::string icon_;
    int32_t value_ = 0;
    int32_t min_ = 0;
    int32_t max_ = 1;

    lv_obj_t *slider_panel_;
    lv_obj_t *slider_;

    std::function<optional<int32_t>()> value_fn_ = nullptr;
    CallbackManager<void(int32_t)> value_change_callback_{};
};

}  // namespace ha_deck
}  // namespace esphome