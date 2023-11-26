import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import Condition, maybe_simple_id
from esphome.const import (
    CONF_ID,
    CONF_ICON,
    CONF_TRIGGER_ID,
)

from .ha_deck import ha_deck_ns, HaDeckWidget, HaDeckWidgetStyle, common_styles_to_code
from .ha_deck import COMMON_STYLE_PROPERTIES_SCHEMA

HdSlider = ha_deck_ns.class_("HdSlider", HaDeckWidget)
HdSliderStyle = ha_deck_ns.class_("HdSliderStyle", HaDeckWidgetStyle)
ChangeAction = ha_deck_ns.class_("HdSlider", automation.Action)

SliderCondition = ha_deck_ns.class_("HdButtonCondition", Condition)
SliderChangeTrigger = ha_deck_ns.class_(
    "HdSliderChangeTrigger", automation.Trigger.template(cg.float_)
)

CONF_TEXT = "text"
CONF_MIN = "min"
CONF_MAX = "max"
CONF_SHOW_VALUE_LABEL = "show_value_label"
CONF_VALUE_LABEL_TPL = "value_label_tpl"
CONF_VALUE = "value"
CONF_SLIDER = "slider"
CONF_INDICATOR = "indicator"
CONF_KNOB = "knob"
CONF_BUTTON = "button"
CONF_ON_CHANGE = "on_change"

SLIDER_STYLE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HdSliderStyle),
        cv.Optional(CONF_VALUE): cv.All(
            cv.ensure_list(COMMON_STYLE_PROPERTIES_SCHEMA),
        ),
        cv.Optional(CONF_SLIDER): cv.All(
            cv.ensure_list(COMMON_STYLE_PROPERTIES_SCHEMA),
        ),
        cv.Optional(CONF_INDICATOR): cv.All(
            cv.ensure_list(COMMON_STYLE_PROPERTIES_SCHEMA),
        ),
        cv.Optional(CONF_KNOB): cv.All(
            cv.ensure_list(COMMON_STYLE_PROPERTIES_SCHEMA),
        ),
        cv.Optional(CONF_BUTTON): cv.All(
            cv.ensure_list(COMMON_STYLE_PROPERTIES_SCHEMA),
        ),
    }
)

SLIDER_CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HdSlider),
        cv.Optional(CONF_TEXT): cv.string,
        cv.Optional(CONF_ICON): cv.string,
        cv.Required(CONF_MIN): cv.int_,
        cv.Required(CONF_MAX): cv.int_,
        cv.Optional(CONF_SHOW_VALUE_LABEL): cv.boolean,
        cv.Optional(CONF_VALUE_LABEL_TPL): cv.string,
        cv.Optional(CONF_VALUE): cv.returning_lambda,
        cv.Optional(CONF_ON_CHANGE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(SliderChangeTrigger),
            }
        ),
    }
)

async def build_slider_style(obj, config):
    if CONF_VALUE in config:
        await common_styles_to_code(obj, config[CONF_VALUE], CONF_VALUE)
    if CONF_SLIDER in config:
        await common_styles_to_code(obj, config[CONF_SLIDER], CONF_SLIDER)
    if CONF_INDICATOR in config:
        await common_styles_to_code(obj, config[CONF_INDICATOR], CONF_INDICATOR)
    if CONF_KNOB in config:
        await common_styles_to_code(obj, config[CONF_KNOB], CONF_KNOB)
    if CONF_BUTTON in config:
        await common_styles_to_code(obj, config[CONF_BUTTON], CONF_BUTTON)

async def build_slider(var, config):
    if text := config.get(CONF_TEXT):
        cg.add(var.set_text(text))

    if icon := config.get(CONF_ICON):
        cg.add(var.set_icon(icon))

    min_ = config.get(CONF_MIN)
    cg.add(var.set_min(min_))

    max_ = config.get(CONF_MAX)
    cg.add(var.set_max(max_))

    if show_value := config.get(CONF_SHOW_VALUE_LABEL):
        cg.add(var.set_show_value_label(show_value))
    
    if value_tpl := config.get(CONF_VALUE_LABEL_TPL):
        cg.add(var.set_value_label_tpl(value_tpl))

    if CONF_VALUE in config:
        val = await cg.process_lambda(
                config[CONF_VALUE], [], return_type=cg.optional.template(int)
            )
        cg.add(var.add_value_lambda(val))

    for conf in config.get(CONF_ON_CHANGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(int, "x")], conf)

SLIDER_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(HdSlider),
    }
)

@automation.register_condition("hd_slider.state", SliderCondition, SLIDER_ACTION_SCHEMA)
async def switch_is_on_to_code(config, condition_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(condition_id, template_arg, paren, True)

@automation.register_action("hd_button.change", ChangeAction, SLIDER_ACTION_SCHEMA)
async def button_click_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)