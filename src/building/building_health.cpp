#include "building_health.h"

#include "building/building.h"
#include "city/object_info.h"
#include "game/resource.h"
#include "graphics/elements/panel.h"
#include "graphics/elements/lang_text.h"
#include "graphics/boilerplate.h"
#include "io/gamefiles/lang.h"
#include "config/config.h"
#include "window/building/common.h"
#include "window/building/figures.h"
#include "sound/sound_building.h"

static void building_health_draw_info(object_info& c, int help_id, const char* type, int group_id, e_figure_type ftype) {
    c.help_id = help_id;
    window_building_play_sound(&c, snd::get_building_info_sound(type));
    outer_panel_draw(c.x_offset, c.y_offset, c.width_blocks, c.height_blocks);
    lang_text_draw_centered(group_id, 0, c.x_offset, c.y_offset + 10, 16 * c.width_blocks, FONT_LARGE_BLACK_ON_LIGHT);

    building *b = building_get(c.building_id);

    if (ftype != FIGURE_NONE && b->has_figure_of_type(BUILDING_SLOT_SERVICE, ftype)) {
        window_building_draw_description(c, group_id, e_text_figure_on_patrol);
    } else if (!c.has_road_access) {
        window_building_draw_description(c, e_text_building, e_text_building_no_roads);
    } else if (building_get(c.building_id)->num_workers <= 0) {
        window_building_draw_description(c, group_id, e_text_no_workers);
    } else {
        window_building_draw_description(c, group_id, e_text_works_fine);
    }

    inner_panel_draw(c.x_offset + 16, c.y_offset + 136, c.width_blocks - 2, 4);
    window_building_draw_employment(&c, 142);
}

void building_apothecary_draw_info(object_info& c) {
    building_health_draw_info(c, 63, "apothecary", e_text_building_apothecary, FIGURE_HERBALIST);
}

void building_dentist_draw_info(object_info& c) {
    building_health_draw_info(c, 65, "dentist", e_text_building_dentist, FIGURE_DENTIST);
}

void building_mortuary_draw_info(object_info& c) {
    building_health_draw_info(c, 66, "mortuary", e_text_building_mortuary, FIGURE_EMBALMER);
}

void building_physician_draw_info(object_info& c) {
    c.help_id = 64;
    window_building_play_sound(&c, snd::get_building_info_sound("physician"));
    outer_panel_draw(c.x_offset, c.y_offset, c.width_blocks, c.height_blocks);
    lang_text_draw_centered(e_text_building_physician, e_text_title, c.x_offset, c.y_offset + 10, 16 * c.width_blocks, FONT_LARGE_BLACK_ON_LIGHT);

    building *b = building_get(c.building_id);
    //if (!b->has_water_access)
    //    window_building_draw_description(c, 83, 4);

    if (b->has_figure_of_type(BUILDING_SLOT_SERVICE, FIGURE_PHYSICIAN)) {
        window_building_draw_description(c, e_text_building_physician, e_text_figure_on_patrol);
    } else if (!c.has_road_access) {
        window_building_draw_description(c, e_text_building, e_text_building_no_roads);
    } else if (b->num_workers <= 0) {
        window_building_draw_description(c, e_text_building_physician, e_text_no_workers);
    } else {
        window_building_draw_description(c, e_text_building_physician, e_text_works_fine);
    }

    inner_panel_draw(c.x_offset + 16, c.y_offset + 136, c.width_blocks - 2, 4);
    window_building_draw_employment(&c, 142);
}
