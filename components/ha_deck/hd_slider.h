#pragma once

#include "ha_deck_widget.h"

namespace esphome {
namespace ha_deck {

class HdSlider : public HaDeckWidget
{
public:
    void set_text(std::string text);
    void set_icon(std::string icon);
    void set_min(int min);
    void set_max(int max);
    void set_value(int value);
    int get_value();

    void add_value_lambda(std::function<optional<int>()> &&f);
    void add_on_value_change_callback(std::function<void(int)> &&callback);
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
    int value_ = 0;
    int min_ = 0;
    int max_ = 1;

    lv_obj_t *slider_panel_;
    lv_obj_t *slider_;

    std::function<optional<int>()> value_fn_ = nullptr;
    CallbackManager<void(int)> value_change_callback_{};
};

}  // namespace ha_deck
}  // namespace esphome