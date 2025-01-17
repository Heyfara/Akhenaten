#include "building_plaza.h"

#include "building/building.h"
#include "city/object_info.h"
#include "figure/figure.h"
#include "game/resource.h"
#include "graphics/elements/panel.h"
#include "graphics/elements/lang_text.h"
#include "graphics/boilerplate.h"
#include "io/gamefiles/lang.h"
#include "config/config.h"
#include "window/building/common.h"
#include "window/building/figures.h"
#include "sound/sound_building.h"

void building_work_camp_draw_info(object_info &c) {
    const int32_t group_id = 179;
    c.help_id = 81;
    window_building_play_sound(&c, snd::get_building_info_sound("work_camp")); // TODO: change to work_camp
    outer_panel_draw(c.x_offset, c.y_offset, c.width_blocks, c.height_blocks);
    lang_text_draw_centered(group_id, 0, c.x_offset, c.y_offset + 10, 16 * c.width_blocks, FONT_LARGE_BLACK_ON_LIGHT);

    building* b = building_get(c.building_id);

    if (!c.has_road_access) {
        window_building_draw_description(c, 69, 25);
    } else {
        if (!b->num_workers) {
            window_building_draw_description(c, group_id, 2); // not enough workers
        } else {
            if (b->has_figure(0)) {
                figure* f = b->get_figure(0);
                building* b_dest = f->destination();
                if (building_is_farm(b_dest->type))
                    window_building_draw_description(c, group_id, 5); // working on floodplains
                else if (building_is_monument(b_dest->id))
                    window_building_draw_description(c, group_id, 6); // working on monuments
                else
                    window_building_draw_description(c, group_id, 4); // looking for work
                //                window_building_draw_description(c, group_id, 7); // working on both floodplains and
                //                monuments (unused?)
            } else {
                window_building_draw_description(c, group_id, 3);
            }
            //            if (c.worker_percentage >= 100)
            //                window_building_draw_description_at(c, 72, group_id, 4);
            //            else if (c.worker_percentage >= 75)
            //                window_building_draw_description_at(c, 72, group_id, 5);
            //            else if (c.worker_percentage >= 50)
            //                window_building_draw_description_at(c, 72, group_id, 6);
            //            else if (c.worker_percentage >= 25)
            //                window_building_draw_description_at(c, 72, group_id, 7);
            //            else
            //                window_building_draw_description_at(c, 72, group_id, 8);

            window_building_draw_description_at(c, 16 * c.height_blocks - 120, group_id, 1);
        }
    }

    inner_panel_draw(c.x_offset + 16, c.y_offset + 136, c.width_blocks - 2, 4);
    window_building_draw_employment(&c, 142);
}
