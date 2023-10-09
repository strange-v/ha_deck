import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.const import (
    CONF_ID,
    CONF_ICON,
    CONF_TRIGGER_ID,
)

from .ha_deck import ha_deck_ns, HaDeckWidget

HdValueCard = ha_deck_ns.class_("HdValueCard", HaDeckWidget)
ClickAction = ha_deck_ns.class_("ClickAction", automation.Action)

ValueCardClickTrigger = ha_deck_ns.class_(
    "HdValueCardClickTrigger", automation.Trigger.template()
)

CONF_TEXT = "text"
CONF_UNIT = "unit"
CONF_VALUE = "value"
CONF_ON_CLICK = "on_click"

VALUE_CARD_CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HdValueCard),
        cv.Optional(CONF_TEXT): cv.string,
        cv.Optional(CONF_ICON): cv.string,
        cv.Optional(CONF_UNIT): cv.string,
        cv.Optional(CONF_VALUE): cv.returning_lambda,
        cv.Optional(CONF_ON_CLICK): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ValueCardClickTrigger),
            }
        ),
    }
)

async def build_value_card(var, config):
    if text := config.get(CONF_TEXT):
        cg.add(var.set_text(text))

    if icon := config.get(CONF_ICON):
        cg.add(var.set_icon(icon))

    if unit := config.get(CONF_UNIT):
        cg.add(var.set_unit(unit))

    if CONF_VALUE in config:
        val = await cg.process_lambda(
                config[CONF_VALUE], [], return_type=cg.optional.template(cg.std_string)
            )
        cg.add(var.add_value_lambda(val))

    for conf in config.get(CONF_ON_CLICK, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

VALUE_CARD_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(HdValueCard),
    }
)

@automation.register_action("hd_value_card.click", ClickAction, VALUE_CARD_ACTION_SCHEMA)
async def value_card_click_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)