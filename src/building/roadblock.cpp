#include "building/roadblock.h"

#include "building/building.h"

void building_roadblock_set_permission(int p, building* b) {
    if (b->type == BUILDING_ROADBLOCK) {
        int permission_bit = 1 << p;
        b->subtype.roadblock_exceptions ^= permission_bit;
    }
}

int building_roadblock_get_permission(int p, building* b) {
    if (b->type != BUILDING_ROADBLOCK)
        return 0;

    int permission_bit = 1 << p;
    return (b->subtype.roadblock_exceptions & permission_bit);
}
