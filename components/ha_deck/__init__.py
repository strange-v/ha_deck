import re

import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.core as core
import esphome.core.config as cfg
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_POSITION,
    CONF_DIMENSIONS,
    CONF_VISIBLE,
    CONF_TYPE,
    CONF_TRIGGER_ID,
    CONF_PERIOD,
)
from esphome.core import CORE, coroutine_with_priority
from .ha_deck import ha_deck_ns, HaDeck, HaDeckScreen
from .hd_button import BUTTON_CONFIG_SCHEMA, build_button
from .hd_slider import SLIDER_CONFIG_SCHEMA, build_slider
from .hd_value_card import VALUE_CARD_CONFIG_SCHEMA, build_value_card

CODEOWNERS = ["@strange-v"]

CONF_SCREENS = "screens"
CONF_WIDGETS = "widgets"
CONF_TEXT = "text"
CONF_ENABLED = "enabled"
CONF_TOGGLE = "toggle"
CONF_CHECKED = "checked"
CONF_MAIN_SCREEN = "main_screen"
CONF_INACTIVITY = "inactivity"
CONF_BLANK_SCREEN = "blank_screen"
CONF_ON_INACTIVITY_CHANGE = "on_inactivity_change"

CONF_BUTTON = "button"
CONF_SLIDER = "slider"
CONF_VALUE_CARD = "value-card"

InactivityChangeTrigger = ha_deck_ns.class_(
    "HaDeckInactivityChangeTrigger", automation.Trigger.template(cg.bool_)
)

def validate_position(position):
    r = re.match(r"^([0-9]*),[ ]*([0-9]*)", position)
    if r is None:
        raise cv.Invalid(
            f"{CONF_POSITION}: has to specify x and y coordinates delimited by a comma, '{position}' provided"
        )
    else:
        x, y = r.groups()
        return [int(x), int(y)]

WIDGET_BUILDERS = {
    CONF_BUTTON: build_button,
    CONF_SLIDER: build_slider,
    CONF_VALUE_CARD: build_value_card,
}

COMMON_WIDGET_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_POSITION): validate_position,
        cv.Optional(CONF_DIMENSIONS): cv.dimensions,
        cv.Optional(CONF_ENABLED): cv.returning_lambda,
        cv.Optional(CONF_VISIBLE): cv.returning_lambda,
    }
)
WIDGET_SCHEMA = cv.typed_schema({
    CONF_BUTTON: COMMON_WIDGET_SCHEMA.extend(BUTTON_CONFIG_SCHEMA),
    CONF_SLIDER: COMMON_WIDGET_SCHEMA.extend(SLIDER_CONFIG_SCHEMA),
    CONF_VALUE_CARD: COMMON_WIDGET_SCHEMA.extend(VALUE_CARD_CONFIG_SCHEMA),
})
SCREEN_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HaDeckScreen),
        cv.Required(CONF_NAME): cv.string,
        cv.Required(CONF_WIDGETS): cv.All(
            cv.ensure_list(WIDGET_SCHEMA),
        ),
        cv.Optional(CONF_INACTIVITY): cv.int_,
    }
)
DECK_INACTIVITY_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_PERIOD): cv.int_,
        cv.Optional(CONF_BLANK_SCREEN): cv.boolean,
    }
)
DECK_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ID): cv.declare_id(HaDeck),
        cv.Required(CONF_MAIN_SCREEN): cv.string,
        cv.Required(CONF_SCREENS): cv.All(
            cv.ensure_list(SCREEN_SCHEMA),
        ),
        cv.Optional(CONF_INACTIVITY): DECK_INACTIVITY_SCHEMA,
        cv.Optional(CONF_ON_INACTIVITY_CHANGE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(InactivityChangeTrigger),
            }
        ),
    }
)
CONFIG_SCHEMA = cv.All(
    DECK_SCHEMA,
)

async def widgets_to_code(screen, config):
    for item in config:
        obj = cg.new_Pvariable(item[CONF_ID])
        await cg.register_component(obj, item)

        if CONF_POSITION in item:
            x, y = item[CONF_POSITION]
            cg.add(obj.set_position(x, y))

        if CONF_DIMENSIONS in item:
            w, h = item[CONF_DIMENSIONS]
            cg.add(obj.set_dimensions(w, h))

        if CONF_VISIBLE in item:
            visible = await cg.process_lambda(
                    item[CONF_VISIBLE], [], return_type=cg.optional.template(bool)
                )
            cg.add(obj.add_visible_lambda(visible))
        
        if CONF_ENABLED in item:
            enabled = await cg.process_lambda(
                item[CONF_ENABLED], [], return_type=cg.optional.template(bool)
            )
            cg.add(obj.add_enabled_lambda(enabled))

        cg.add(screen.add_widget(obj))

        if item[CONF_TYPE] in WIDGET_BUILDERS.keys():
            await WIDGET_BUILDERS[item[CONF_TYPE]](obj, item)

async def screens_to_code(var, config):
    for item in config:
        screen = cg.new_Pvariable(item[CONF_ID])
        await cg.register_component(screen, item)

        name = item.get(CONF_NAME)
        cg.add(screen.set_name(name))

        if inactivity := item.get(CONF_INACTIVITY):
            cg.add(screen.set_inactivity(inactivity))

        cg.add(var.add_screen(screen))

        if CONF_WIDGETS in item:
            await widgets_to_code(screen, item[CONF_WIDGETS])

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    main_screen = config.get(CONF_MAIN_SCREEN)
    cg.add(var.set_main_screen(main_screen))

    if inactivity := config.get(CONF_INACTIVITY):
        if period := inactivity.get(CONF_PERIOD):
            cg.add(var.set_inactivity_period(period))
        if bs := inactivity.get(CONF_BLANK_SCREEN):
            cg.add(var.set_inactivity_blank_screen(bs))
    
    for conf in config.get(CONF_ON_INACTIVITY_CHANGE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [(bool, "x")], conf)

    if CONF_SCREENS in config:
        await screens_to_code(var, config[CONF_SCREENS])
