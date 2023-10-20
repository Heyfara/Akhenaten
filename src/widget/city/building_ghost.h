#pragma once

#include "graphics/color.h"
#include "grid/point.h"
#include "core/vec2i.h"

struct view_context;

void draw_building(view_context &ctx, int image_id, vec2i tile, color color_mask = COLOR_MASK_GREEN);
bool city_building_ghost_mark_deleting(tile2i tile);
