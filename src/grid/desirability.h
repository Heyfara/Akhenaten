#ifndef MAP_DESIRABILITY_H
#define MAP_DESIRABILITY_H

#include "core/buffer.h"

void map_desirability_clear(void);
void map_desirability_update(void);

int map_desirability_get(int grid_offset);
int map_desirability_get_max(int x, int y, int size);

#endif // MAP_DESIRABILITY_H
