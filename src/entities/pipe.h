#include <gb/gb.h>

extern const struct Pipe {
    UBYTE sprite_id[4];
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
};

void setup_pipes();