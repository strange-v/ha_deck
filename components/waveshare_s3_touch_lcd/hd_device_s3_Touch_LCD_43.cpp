#include "hd_device_s3_Touch_LCD_43.h"
#include "arduino.h"

namespace esphome {
namespace hd_device {

static const char *const TAG = "HD_DEVICE";
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf = (lv_color_t *)heap_caps_malloc(TFT_HEIGHT * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);

ESP_Panel lcd;

lv_disp_t *indev_disp;
lv_group_t *group;

void IRAM_ATTR flush_pixels(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    ESP_LOGCONFIG(TAG, "flush_pixels()");

    //uint32_t w = (area->x2 - area->x1 + 1);
    //uint32_t h = (area->y2 - area->y1 + 1);
    //uint32_t len = w * h;

    auto* display = lcd.getLcd();
    display->drawBitmap(area->x1, area->y1, area->x2, area->y2, static_cast<void*>(color_p));
    
    // lcd.startWrite();                            /* Start new TFT transaction */
    // lcd.setAddrWindow(area->x1, area->y1, w, h); /* set the working window */
    // lcd.writePixels((uint16_t *)&color_p->full, len, true);
    // lcd.endWrite();                              /* terminate TFT transaction */

    lv_disp_flush_ready(disp);
}

void IRAM_ATTR touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    auto* touch = lcd.getLcdTouch();

    if (touch->getTouchState()) {
        TouchPoint point = touch->getPoint(0);
        data->point.x = point.x;
        data->point.y = point.y;
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void HaDeckDevice::setup() {
    ESP_LOGCONFIG(TAG, "HaDeckDevice::Setup()");
    lv_init();
    lv_theme_default_init(NULL, lv_color_hex(0xFFEB3B), lv_color_hex(0xFF7043), 1, LV_FONT_DEFAULT);

    lcd.init();
//    lcd.getBacklight()->on();
//    lcd.getBacklight()->setBrightness(80);

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_HEIGHT * 20);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.rotated = 1;
    disp_drv.sw_rotate = 1;
    disp_drv.flush_cb = flush_pixels;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.long_press_time = 1000;
    indev_drv.long_press_repeat_time = 300;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);

    lcd.begin();

    group = lv_group_create();
    lv_group_set_default(group);

    ESP_LOGCONFIG(TAG, "Setting background image");
    auto bg_image = lv_img_create(lv_scr_act());
    lv_img_set_src(bg_image, &bg_480x320);
    lv_obj_set_parent(bg_image, lv_scr_act());
}

void HaDeckDevice::loop() {
    lv_timer_handler();

    unsigned long ms = millis();
    if (ms - time_ > 60000) {
        time_ = ms;
        ESP_LOGCONFIG(TAG, "Free memory: %d bytes", esp_get_free_heap_size());
    }
}

float HaDeckDevice::get_setup_priority() const { return setup_priority::DATA; }

uint8_t HaDeckDevice::get_brightness() {
    return brightness_;
}

void HaDeckDevice::set_brightness(uint8_t value) {
    brightness_ = value;
    lcd.getBacklight()->setBrightness(brightness_);
}

}  // namespace hd_device
}  // namespace esphome