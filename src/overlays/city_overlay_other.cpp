#include "city_overlay_other.h"

#include "building/building.h"
#include "building/model.h"
#include "city/constants.h"
#include "city/finance.h"
#include "core/calc.h"
#include "figure/figure.h"
#include "game/resource.h"
#include "game/state.h"
#include "graphics/boilerplate.h"
#include "graphics/elements/tooltip.h"
#include "grid/building.h"
#include "grid/desirability.h"
#include "grid/image.h"
#include "grid/property.h"
#include "grid/random.h"
#include "grid/terrain.h"
#include "io/config/config.h"
#include "overlays/city_overlay.h"

static int show_building_food_stocks(const building* b) {
    return b->type == BUILDING_MARKET || b->type == BUILDING_FISHING_WHARF || b->type == BUILDING_GRANARY;
}
static int show_building_tax_income(const building* b) {
    return b->type == BUILDING_TAX_COLLECTOR || b->type == BUILDING_SENATE_UPGRADED;
}
static int show_building_water(const building* b) {
    return b->type == BUILDING_WELL || b->type == BUILDING_MENU_BEAUTIFICATION || b->type == BUILDING_WATER_LIFT || b->type == BUILDING_WATER_SUPPLY;
}

static int show_figure_food_stocks(const figure* f) {
    if (f->type == FIGURE_MARKET_BUYER || f->type == FIGURE_MARKET_TRADER || f->type == FIGURE_DELIVERY_BOY
        || f->type == FIGURE_FISHING_BOAT) {
        return 1;
    } else if (f->type == FIGURE_CART_PUSHER) {
        return resource_is_food(f->get_resource());
    }

    return 0;
}
static int show_figure_tax_income(const figure* f) {
    return f->type == FIGURE_TAX_COLLECTOR;
}
static int show_figure_water(const figure* f) {
    return f->type == FIGURE_WATER_CARRIER;
}

static int get_column_height_food_stocks(const building* b) {
    if (b->house_size && model_get_house(b->subtype.house_level)->food_types) {
        int pop = b->house_population;
        int stocks = 0;
        for (int i = INVENTORY_MIN_FOOD; i < INVENTORY_MAX_FOOD; i++)
            stocks += b->data.house.inventory[i];
        int pct_stocks = calc_percentage(stocks, pop);
        if (pct_stocks <= 0)
            return 10;
        else if (pct_stocks < 100)
            return 5;
        else if (pct_stocks <= 200)
            return 1;
    }
    return NO_COLUMN;
}

static int get_column_height_tax_income(const building* b) {
    if (b->house_size) {
        int pct = calc_adjust_with_percentage(b->tax_income_or_storage / 2, city_finance_tax_percentage());
        if (pct > 0)
            return pct / 25;
    }
    return NO_COLUMN;
}

static int get_column_height_water(const building* b) {
    return b->house_size ? b->data.house.water_supply * 17 / 10 : NO_COLUMN;
}

static int get_tooltip_food_stocks(tooltip_context* c, const building* b) {
    if (b->house_population <= 0) {
        return 0;
    }

    if (!model_get_house(b->subtype.house_level)->food_types) {
        return 104;
    } else {
        int stocks_present = 0;
        for (int i = INVENTORY_MIN_FOOD; i < INVENTORY_MAX_FOOD; i++) {
            stocks_present += b->data.house.inventory[i];
        }

        int stocks_per_pop = calc_percentage(stocks_present, b->house_population);
        if (stocks_per_pop <= 0) {
            return 4;
        } else if (stocks_per_pop < 100) {
            return 5;
        } else if (stocks_per_pop <= 200) {
            return 6;
        } else {
            return 7;
        }
    }
}

static int get_tooltip_tax_income(tooltip_context* c, const building* b) {
    int denarii = calc_adjust_with_percentage(b->tax_income_or_storage / 2, city_finance_tax_percentage());
    if (denarii > 0) {
        c->has_numeric_prefix = 1;
        c->numeric_prefix = denarii;
        return 45;
    } else if (b->house_tax_coverage > 0) {
        return 44;
    } else {
        return 43;
    }
}

static int get_tooltip_water(tooltip_context* c, int grid_offset) {
    if (map_terrain_is(grid_offset, TERRAIN_GROUNDWATER)) {
        if (map_terrain_is(grid_offset, TERRAIN_FOUNTAIN_RANGE)) {
            return 2;
        } else {
            return 1;
        }
    } else if (map_terrain_is(grid_offset, TERRAIN_FOUNTAIN_RANGE)) {
        return 3;
    }

    return 0;
}

const city_overlay* city_overlay_for_food_stocks(void) {
    static city_overlay overlay = {
        OVERLAY_FOOD_STOCKS,
        COLUMN_TYPE_RISK,
        show_building_food_stocks,
        show_figure_food_stocks,
        get_column_height_food_stocks,
        0,
        get_tooltip_food_stocks,
        0,
        0
    };
    return &overlay;
}
const city_overlay* city_overlay_for_tax_income(void) {
    static city_overlay overlay = {OVERLAY_TAX_INCOME,
                                   COLUMN_TYPE_WATER_ACCESS,
                                   show_building_tax_income,
                                   show_figure_tax_income,
                                   get_column_height_tax_income,
                                   0,
                                   get_tooltip_tax_income,
                                   0,
                                   0};
    return &overlay;
}

static int terrain_on_water_overlay(void) {
    return TERRAIN_TREE | TERRAIN_ROCK | TERRAIN_WATER | TERRAIN_SHRUB | TERRAIN_GARDEN | TERRAIN_ROAD
           | TERRAIN_AQUEDUCT | TERRAIN_ELEVATION | TERRAIN_ACCESS_RAMP | TERRAIN_RUBBLE | TERRAIN_DUNE
           | TERRAIN_MARSHLAND;
}
static void draw_footprint_water(vec2i pixel, map_point point) {
    int grid_offset = point.grid_offset();
    int x = pixel.x;
    int y = pixel.y;
    // roads, bushes, dunes, etc. are drawn normally
    if (map_terrain_is(grid_offset, terrain_on_water_overlay())) {
        // (except for roadblocks on roads, draw these as flattened tiles)
        if (building_at(grid_offset)->type == BUILDING_ROADBLOCK)
            city_with_overlay_draw_building_footprint(x, y, grid_offset, 0);
        else if (map_property_is_draw_tile(grid_offset))
            ImageDraw::isometric_from_drawtile(map_image_at(grid_offset), x, y, 0);
    } else {
        int terrain = map_terrain_get(grid_offset);
        building* b = building_at(grid_offset);
        // draw houses, wells and water supplies either fully or flattened
        if (terrain & TERRAIN_BUILDING && (building_is_house(b->type)) || b->type == BUILDING_WELL
            || b->type == BUILDING_WATER_SUPPLY) {
            if (map_property_is_draw_tile(grid_offset))
                city_with_overlay_draw_building_footprint(x, y, grid_offset, 0);
        } else {
            // draw groundwater levels
            int image_id = image_id_from_group(GROUP_TERRAIN_OVERLAY_WATER);
            switch (map_terrain_get(grid_offset) & (TERRAIN_GROUNDWATER | TERRAIN_FOUNTAIN_RANGE)) {
            case TERRAIN_GROUNDWATER | TERRAIN_FOUNTAIN_RANGE:
            case TERRAIN_FOUNTAIN_RANGE:
                image_id += 2;
                break;
            case TERRAIN_GROUNDWATER:
                image_id += 1;
                break;
            }
            ImageDraw::isometric(image_id, x, y);
        }
    }
}
static void draw_top_water(vec2i pixel, map_point point) {
    int grid_offset = point.grid_offset();
    int x = pixel.x;
    int y = pixel.y;
    if (!map_property_is_draw_tile(grid_offset))
        return;
    if (map_terrain_is(grid_offset, terrain_on_water_overlay())) {
        if (!map_terrain_is(grid_offset, TERRAIN_BUILDING)) {
            color color_mask = 0;
            if (map_property_is_deleted(grid_offset) && map_property_multi_tile_size(grid_offset) == 1)
                color_mask = COLOR_MASK_RED;
            //            ImageDraw::isometric_top_from_drawtile(map_image_at(grid_offset), x, y, color_mask,
            //            city_view_get_scale_float());
        }
    } else if (map_building_at(grid_offset))
        city_with_overlay_draw_building_top(pixel, point);
}

const city_overlay* city_overlay_for_water(void) {
    static city_overlay overlay = {OVERLAY_WATER,
                                   COLUMN_TYPE_WATER_ACCESS,
                                   show_building_water,
                                   show_figure_water,
                                   get_column_height_water,
                                   get_tooltip_water,
                                   0,
                                   draw_footprint_water,
                                   draw_top_water};
    return &overlay;
}