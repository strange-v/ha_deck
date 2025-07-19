#include "hd_device_s3_Touch_LCD_43.h"
#include "esphome/core/hal.h"
#include "ESP_IOExpander_Library.h"


namespace esphome {
namespace hd_device {

// Extend IO Pin define
#define TP_RST 1
#define LCD_BL 2
#define LCD_RST 3
#define SD_CS 4
#define USB_SEL 5

// I2C Pin define
#define I2C_MASTER_NUM (i2c_port_t)0
#define I2C_MASTER_SDA_IO 8
#define I2C_MASTER_SCL_IO 9

/* LVGL porting configurations */
#define LVGL_TICK_PERIOD_MS     (2)
#define LVGL_TASK_MAX_DELAY_MS  (500)
#define LVGL_TASK_MIN_DELAY_MS  (1)
#define LVGL_TASK_STACK_SIZE    (4 * 1024)
#define LVGL_TASK_PRIORITY      (2)
#define LVGL_BUF_SIZE           (ESP_PANEL_LCD_H_RES * 20)

static const char *const TAG = "HD_DEVICE";
static lv_disp_draw_buf_t draw_buf;

ESP_Panel panel;
ESP_IOExpander* expander;

lv_disp_t *indev_disp;
lv_group_t *group;
unsigned long flush_time = 0;

void IRAM_ATTR flush_pixels(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    panel.getLcd()->drawBitmap(area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_p);
    lv_disp_flush_ready(disp);
}

void IRAM_ATTR touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    auto* touch = panel.getLcdTouch();
    touch->readData();
    if (touch->getTouchState()) {
        TouchPoint point = touch->getPoint();
        data->point.x = point.x;
        data->point.y = point.y;
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void lvglDeck::setup() {
    lv_init();
    lv_theme_default_init(NULL, lv_color_hex(0xFFEB3B), lv_color_hex(0xFF7043), 1, LV_FONT_DEFAULT);

    uint8_t* buf = (uint8_t *)heap_caps_calloc(1, LVGL_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_INTERNAL);

    lv_disp_draw_buf_init(&draw_buf, buf, nullptr, LVGL_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = ESP_PANEL_LCD_H_RES;
    disp_drv.ver_res = ESP_PANEL_LCD_V_RES;
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

    panel.init();

    expander = new ESP_IOExpander_CH422G(I2C_MASTER_NUM, ESP_IO_EXPANDER_I2C_CH422G_ADDRESS_000);
    expander->init();
    expander->begin();
    expander->multiPinMode(IO_EXPANDER_PIN_NUM_1 | IO_EXPANDER_PIN_NUM_2 | IO_EXPANDER_PIN_NUM_3 | IO_EXPANDER_PIN_NUM_4 | IO_EXPANDER_PIN_NUM_5, OUTPUT);
    expander->multiDigitalWrite(IO_EXPANDER_PIN_NUM_1 | IO_EXPANDER_PIN_NUM_2 | IO_EXPANDER_PIN_NUM_3 | IO_EXPANDER_PIN_NUM_4, HIGH);
    expander->multiDigitalWrite(IO_EXPANDER_PIN_NUM_5, LOW);

    /* Add into panel */
    panel.addIOExpander(expander);

    /* Start panel */
    panel.begin();

    group = lv_group_create();
    lv_group_set_default(group);

    auto bg_image = lv_img_create(lv_scr_act());
    lv_img_set_src(bg_image, &bg_800x480);
    lv_obj_set_parent(bg_image, lv_scr_act());
}

void lvglDeck::loop() {
    lv_timer_handler();

    unsigned long ms = esphome::millis();
    if (ms - time_ > 60000) {
        time_ = ms;
        ESP_LOGCONFIG(TAG, "Free memory: %d bytes", esp_get_free_heap_size());
    }
}

void lvglDeck::set_brightness(uint8_t value) {
    if (expander) {
      expander->multiDigitalWrite(IO_EXPANDER_PIN_NUM_2, value > 0 ? HIGH : LOW);
    }
}

void HaDeckDevice::setup() {
    ESP_LOGCONFIG(TAG, "HaDeckDevice::Setup()");
    deck.setup();
}

void HaDeckDevice::loop() {
    deck.loop();
}

float HaDeckDevice::get_setup_priority() const { return setup_priority::DATA; }

uint8_t HaDeckDevice::get_brightness() {
    return brightness_;
}

void HaDeckDevice::set_brightness(uint8_t value) {
    brightness_ = value;
    deck.set_brightness(brightness_);
}

}  // namespace hd_device
}  // namespace esphome