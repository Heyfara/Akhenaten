#include "formation_layout.h"

static const int FORMATION_LAYOUT_POSITION_X[13][16] = {
        {0, 1, 0,  1, -1, -1, 0,  1,  -1, 2,  2,  2,  0,  1,  -1, 2},
        {0, 0, -1, 1, -1, 1,  -2, -2, 2,  2,  -3, -3, 3,  3,  -4, -4},
        {0, 0, 0,  1, 1,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1},
        {0, 2, -2, 1, -1, 3,  -3, 4,  -4, 5,  6,  -5, -6, 7,  8,  -7},
        {0, 0, 0,  1, 1,  1,  1,  0,  0,  1,  0,  1,  0,  1,  0,  1},
        {0, 0, 1,  0, -1, 1,  1,  -1, -1, 1,  1,  -1, -1, 1,  -1, 0},
        {0, 1, 0,  1, 2,  2,  1,  0,  2,  3,  3,  3,  1,  2,  0,  3},
        {0, 1, 0,  1, 2,  2,  1,  0,  2,  3,  3,  3,  1,  2,  0,  3},
        {0, 1, 0,  0, 1,  -1, 2,  -1, 1,  0,  1,  0,  1,  -1, 1,  -1},
        {0, 2, -1, 1, 1,  -1, 3,  -2, 0,  -4, -1, 0,  1,  4,  2,  -5}, // herd
        {0, 0, 0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        {0, 2, 0,  2, -2, -2, 0,  2,  -2, 4,  4,  4,  0,  2,  -2, 4},
        {0, 1, 0,  1, 2,  2,  1,  0,  2,  3,  3,  3,  1,  2,  0,  3}
};
static const int FORMATION_LAYOUT_POSITION_Y[13][16] = {
        {0, 0,  1,  1,  0,  1,  -1, -1, -1, -1, 0,  1,  2,  2,  2,  2},
        {0, 1,  0,  0,  1,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1},
        {0, -1, 1,  0,  -1, 1,  -2, -2, 2,  2,  -3, -3, 3,  3,  -4, -4},
        {0, 0,  0,  1,  1,  1,  1,  0,  0,  1,  0,  1,  0,  1,  0,  1},
        {0, -2, 2,  -1, 1,  -3, 3,  -4, 4,  -5, -6, 5,  6,  -7, -8, 7},
        {0, -1, 0,  1,  0,  -1, 1,  1,  -1, -1, 1,  1,  -1, 0,  0,  0},
        {0, 0,  1,  1,  0,  1,  2,  2,  2,  0,  1,  2,  3,  3,  3,  3},
        {0, 0,  1,  1,  0,  1,  2,  2,  2,  0,  1,  2,  3,  3,  3,  3},
        {0, -1, 1,  0,  0,  1,  1,  -1, -1, 1,  0,  2,  1,  1,  -2, 1},
        {0, 1,  -1, 1,  0,  1,  1,  -1, 2,  0,  3,  5,  4,  0,  3,  2}, // herd
        {0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        {0, 0,  2,  2,  0,  2,  -2, -2, -2, -2, 0,  2,  4,  4,  4,  4},
        {0, 0,  1,  1,  0,  1,  2,  2,  2,  0,  1,  2,  3,  3,  3,  3}
};

int formation_layout_position_x(int layout, int index) {
    return FORMATION_LAYOUT_POSITION_X[layout][index];
}

int formation_layout_position_y(int layout, int index) {
    return FORMATION_LAYOUT_POSITION_Y[layout][index];
}
