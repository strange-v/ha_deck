import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.const import (
    CONF_ID,
    CONF_ICON,
    CONF_TRIGGER_ID,
)

from .ha_deck import ha_deck_ns, HaDeckWidget, HaDeckWidgetStyle, common_styles_to_code, validate_scale, validate_v_position, get_scale, get_v_position
from .ha_deck import COMMON_STYLE_PROPERTIES_SCHEMA

HdSensor = ha_deck_ns.class_("HdSensor", HaDeckWidget)
HdSensorStyle = ha_deck_ns.class_("HdSensorStyle", HaDeckWidgetStyle)

ClickAction = ha_deck_ns.class_("ClickAction", automation.Action)

SensorClickTrigger = ha_deck_ns.class_(
    "HdSensorClickTrigger", automation.Trigger.template()
)

CONF_TEXT = "text"
CONF_UNIT = "unit"
CONF_SCALE = "scale"
CONF_TEXT_POSITION = "text_position"
CONF_VALUE = "value"
CONF_ON_CLICK = "on_click"

SENSOR_STYLE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HdSensorStyle),
        cv.Optional(CONF_TEXT): cv.All(
            cv.ensure_list(COMMON_STYLE_PROPERTIES_SCHEMA),
        ),
        cv.Optional(CONF_ICON): cv.All(
            cv.ensure_list(COMMON_STYLE_PROPERTIES_SCHEMA),
        ),
        cv.Optional(CONF_VALUE): cv.All(
            cv.ensure_list(COMMON_STYLE_PROPERTIES_SCHEMA),
        ),
    }
)

SENSOR_CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HdSensor),
        cv.Optional(CONF_TEXT): cv.string,
        cv.Optional(CONF_ICON): cv.string,
        cv.Optional(CONF_UNIT): cv.string,
        cv.Optional(CONF_SCALE): validate_scale,
        cv.Optional(CONF_TEXT_POSITION): validate_v_position,
        cv.Optional(CONF_VALUE): cv.returning_lambda,
        cv.Optional(CONF_ON_CLICK): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(SensorClickTrigger),
            }
        ),
    }
)

async def build_sensor_style(obj, config):
    if CONF_TEXT in config:
        await common_styles_to_code(obj, config[CONF_TEXT], CONF_TEXT)
    if CONF_ICON in config:
        await common_styles_to_code(obj, config[CONF_ICON], CONF_ICON)
    if CONF_VALUE in config:
        await common_styles_to_code(obj, config[CONF_VALUE], CONF_VALUE)

async def build_sensor(var, config):
    if text := config.get(CONF_TEXT):
        cg.add(var.set_text(text))
    if icon := config.get(CONF_ICON):
        cg.add(var.set_icon(icon))
    if unit := config.get(CONF_UNIT):
        cg.add(var.set_unit(unit))
    if scale := config.get(CONF_SCALE):
        cg.add(var.set_scale(get_scale(scale)))
    if text_position := config.get(CONF_TEXT_POSITION):
        cg.add(var.set_text_position(get_v_position(text_position)))
    
    if CONF_VALUE in config:
        val = await cg.process_lambda(
                config[CONF_VALUE], [], return_type=cg.optional.template(cg.std_string)
            )
        cg.add(var.add_value_lambda(val))

    for conf in config.get(CONF_ON_CLICK, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

SENSOR_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(HdSensor),
    }
)

@automation.register_action("hd_sensor.click", ClickAction, SENSOR_ACTION_SCHEMA)
async def sensor_click_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)
