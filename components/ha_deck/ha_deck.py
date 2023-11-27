import re
import esphome.codegen as cg
import esphome.config_validation as cv

ha_deck_ns = cg.esphome_ns.namespace("ha_deck")

HaDeck = ha_deck_ns.class_("HaDeck", cg.Component)
HaDeckScreen = ha_deck_ns.class_("HaDeckScreen", cg.Component)
HaDeckWidget = ha_deck_ns.class_("HaDeckWidget", cg.Component)
HaDeckWidgetStyle = ha_deck_ns.class_("HaDeckWidgetStyle")

CONF_STATE = "state"
CONF_BORDER_RADIUS = "border-radius"
CONF_COLOR = "color"
CONF_BG_COLOR = "bg-color"
CONF_OPACITY = "opacity"
CONF_BG_OPACITY = "bg-opacity"

supported_state = {
    "DEFAULT": cg.global_ns.LV_STATE_DEFAULT,
    "CHECKED": cg.global_ns.LV_STATE_CHECKED,
    "FOCUSED": cg.global_ns.LV_STATE_FOCUSED,
    "PRESSED": cg.global_ns.LV_STATE_PRESSED,
    "DISABLED": cg.global_ns.LV_STATE_DISABLED,
}

supported_scale = {
    "NORMAL": ha_deck_ns.HD_SCALE_NORMAL,
    "SMALL": ha_deck_ns.HD_SCALE_SMALL,
}

supported_v_position = {
    "TOP": ha_deck_ns.HD_POSITION_TOP,
    "BOTTOM": ha_deck_ns.HD_POSITION_BOTTOM,
}

def get_style_state(value):
    # states = [state.strip() for state in value.split('|')]
    # state_values = [supported_state.get(s, None) for s in states]
    # state_values = [val for val in state_values if val is not None]
    # return ' | '.join(state_values)
    return cv.one_of(*supported_state, upper=True)(value)

def validate_style_state(value):
    # states = [state.strip() for state in value.split('|')]

    # for s in states:
    #     if s not in supported_state.keys():
    #         raise cv.Invalid(f"Not supported value '{value}' provided")
    # return get_style_state(value)
    return cv.one_of(*supported_state, upper=True)(value)

def validate_scale(value):
    return cv.one_of(*supported_scale, upper=True)(value)

def get_scale(value):
    return supported_scale[value]

def validate_v_position(value):
    return cv.one_of(*supported_v_position, upper=True)(value)

def get_v_position(value):
    return supported_v_position[value]

def validate_opacity(value):
    cv.float_range(0, 1)
    return int(255 * float(value))

COMMON_STYLE_PROPERTIES_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_STATE): validate_style_state,
        cv.Optional(CONF_BORDER_RADIUS): cv.int_,
        cv.Optional(CONF_BG_COLOR): cv.hex_int,
        cv.Optional(CONF_BG_OPACITY): validate_opacity,
        cv.Optional(CONF_COLOR): cv.hex_int,
        cv.Optional(CONF_OPACITY): validate_opacity,
    }
)

async def common_styles_to_code(obj, config, name):
    for item in config:
        state = supported_state["DEFAULT"]
        if CONF_STATE in item:
            state = supported_state[item[CONF_STATE]]
        
        if CONF_BORDER_RADIUS in item:
            cg.add(obj.set_border_radius(name, item[CONF_BORDER_RADIUS], state))
        if CONF_BG_COLOR in item:
            cg.add(obj.set_bg_color(name, item[CONF_BG_COLOR], state))
        if CONF_BG_OPACITY in item:
            cg.add(obj.set_bg_opacity(name, item[CONF_BG_OPACITY], state))
        if CONF_COLOR in item:
            cg.add(obj.set_color(name, item[CONF_COLOR], state))
        if CONF_OPACITY in item:
            cg.add(obj.set_opacity(name, item[CONF_OPACITY], state))
            