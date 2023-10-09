import esphome.codegen as cg

ha_deck_ns = cg.esphome_ns.namespace("ha_deck")

HaDeck = ha_deck_ns.class_("HaDeck", cg.Component)
HaDeckScreen = ha_deck_ns.class_("HaDeckScreen", cg.Component)
HaDeckWidget = ha_deck_ns.class_("HaDeckWidget", cg.Component)
