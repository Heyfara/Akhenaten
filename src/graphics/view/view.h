#pragma once

#include "core/buffer.h"
#include "grid/point.h"
#include "zoom.h"

extern int SCROLL_MIN_SCREENTILE_X;
extern int SCROLL_MIN_SCREENTILE_Y;
extern int SCROLL_MAX_SCREENTILE_X;
extern int SCROLL_MAX_SCREENTILE_Y;
void camera_calc_scroll_limits();

typedef vec2i screen_tile;
typedef vec2i camera_coordinate;

struct SDL_Renderer;

struct view_data_t {
    int screen_width;
    int screen_height;
    bool sidebar_collapsed;
    int orientation;
    struct {
        screen_tile tile_internal;
        camera_coordinate position;
    } camera;
    struct {
        vec2i offset;
        vec2i size_pixels;
        int width_tiles;
        int height_tiles;
    } viewport;
    screen_tile selected_tile;
};

struct figure_draw_cache_data_t;

struct view_context {
    figure_draw_cache_data_t *figure_cache;
    view_data_t *view;
    SDL_Renderer *renderer;
    float global_render_scale;
};

view_context view_context_main();

using tile_draw_callback = void(vec2i pixel, tile2i tile, view_context &payload);
using minimap_draw_callback = void(vec2i pixel, tile2i point);

view_data_t& city_view_data_unsafe();

void city_view_camera_position_refresh();

void city_view_init();
void city_settings_init();

int city_view_orientation(void);
int city_view_relative_orientation(int orientation);
int city_view_absolute_orientation(int orientation_relative);
void city_view_reset_orientation(void);

screen_tile city_view_get_camera_screentile();
map_point city_view_get_camera_mappoint();
vec2i camera_get_position();
vec2i camera_get_pixel_offset_internal(view_context &ctx);
void city_view_get_camera_max_tile(int* x, int* y);
void city_view_get_camera_max_pixel_offset(int* x, int* y);
vec2i city_view_get_camera_in_pixels();
void city_view_get_camera_scrollable_pixel_limits(view_data_t& view, vec2i &min_pos, vec2i &max_pos);
void city_view_get_camera_scrollable_viewspace_clip(vec2i &clip);

void camera_go_to_pixel(view_context &ctx, vec2i pixel, bool validate);
void camera_go_to_corner_tile(screen_tile screen, bool validate);
void camera_go_to_screen_tile(screen_tile screen, bool validate);
void camera_go_to_mappoint(map_point point);
void camera_scroll(int x, int y);

screen_tile camera_get_selected_screen_tile();
void city_view_set_selected_view_tile(const screen_tile* tile);

void city_view_rotate_left(void);
void city_view_rotate_right(void);

void city_view_refresh_viewport();

void city_view_set_viewport(int screen_width, int screen_height);
void city_view_get_viewport(vec2i &pos, vec2i &size);
void city_view_get_viewport_size_tiles(int* width, int* height);

bool pixel_is_inside_viewport(vec2i pixel);
bool city_view_is_sidebar_collapsed(void);
void city_view_start_sidebar_toggle(void);
void city_view_toggle_sidebar(void);

void city_view_foreach_valid_map_tile(view_context &ctx,
                                      tile_draw_callback* callback1,
                                      tile_draw_callback* callback2 = nullptr,
                                      tile_draw_callback* callback3 = nullptr,
                                      tile_draw_callback* callback4 = nullptr,
                                      tile_draw_callback* callback5 = nullptr,
                                      tile_draw_callback* callback6 = nullptr);

void city_view_foreach_tile_in_range(view_context &ctx, int grid_offset, int size, int radius, tile_draw_callback* callback);
void city_view_foreach_minimap_tile(int x_offset, int y_offset, int absolute_x, int absolute_y, int width_tiles, int height_tiles, minimap_draw_callback callback);