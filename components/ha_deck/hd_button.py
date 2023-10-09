import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import Condition, maybe_simple_id
from esphome.const import (
    CONF_ID,
    CONF_ICON,
    CONF_TRIGGER_ID,
)

from .ha_deck import ha_deck_ns, HaDeckWidget

HdButton = ha_deck_ns.class_("HdButton", HaDeckWidget)
ClickAction = ha_deck_ns.class_("ClickAction", automation.Action)
TurnOnAction = ha_deck_ns.class_("TurnOnAction", automation.Action)
TurnOffAction = ha_deck_ns.class_("TurnOffAction", automation.Action)
LongPressAction = ha_deck_ns.class_("LongPressAction", automation.Action)

ButtonCondition = ha_deck_ns.class_("HdButtonCondition", Condition)
ButtonClickTrigger = ha_deck_ns.class_(
    "HdButtonClickTrigger", automation.Trigger.template()
)
ButtonTurnOnTrigger = ha_deck_ns.class_(
    "HdButtonTurnOnTrigger", automation.Trigger.template()
)
ButtonTurnOffTrigger = ha_deck_ns.class_(
    "HdButtonTurnOffTrigger", automation.Trigger.template()
)
ButtonLongPressTrigger = ha_deck_ns.class_(
    "HdButtonLongPressTrigger", automation.Trigger.template()
)

CONF_TEXT = "text"
CONF_TOGGLE = "toggle"
CONF_CHECKED = "checked"
CONF_ON_CLICK = "on_click"
CONF_ON_TURN_ON = "on_turn_on"
CONF_ON_TURN_OFF = "on_turn_off"
CONF_ON_LONG_PRESS = "on_long_press"

BUTTON_CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HdButton),
        cv.Optional(CONF_TEXT): cv.string,
        cv.Optional(CONF_ICON): cv.string,
        cv.Optional(CONF_TOGGLE): cv.boolean,
        cv.Optional(CONF_CHECKED): cv.returning_lambda,
        cv.Optional(CONF_ON_CLICK): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ButtonClickTrigger),
            }
        ),
        cv.Optional(CONF_ON_TURN_ON): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ButtonTurnOnTrigger),
            }
        ),
        cv.Optional(CONF_ON_TURN_OFF): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ButtonTurnOffTrigger),
            }
        ),
        cv.Optional(CONF_ON_LONG_PRESS): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ButtonLongPressTrigger),
            }
        ),
    }
)

async def build_button(var, config):
    if text := config.get(CONF_TEXT):
        cg.add(var.set_text(text))
    if icon := config.get(CONF_ICON):
        cg.add(var.set_icon(icon))
    if toggle := config.get(CONF_TOGGLE):
        cg.add(var.set_toggle(toggle))
    
    if CONF_CHECKED in config:
        checked = await cg.process_lambda(
                config[CONF_CHECKED], [], return_type=cg.optional.template(bool)
            )
        cg.add(var.add_checked_lambda(checked))

    for conf in config.get(CONF_ON_CLICK, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

    for conf in config.get(CONF_ON_TURN_ON, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

    for conf in config.get(CONF_ON_TURN_OFF, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

    for conf in config.get(CONF_ON_LONG_PRESS, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

BUTTON_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(HdButton),
    }
)

@automation.register_condition("hd_button.is_checked", ButtonCondition, BUTTON_ACTION_SCHEMA)
async def switch_is_on_to_code(config, condition_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(condition_id, template_arg, paren, True)

@automation.register_action("hd_button.click", ClickAction, BUTTON_ACTION_SCHEMA)
@automation.register_action("hd_button.turn_on", TurnOnAction, BUTTON_ACTION_SCHEMA)
@automation.register_action("hd_button.turn_off", TurnOffAction, BUTTON_ACTION_SCHEMA)
@automation.register_action("hd_button.long_press", LongPressAction, BUTTON_ACTION_SCHEMA)
async def button_click_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)
