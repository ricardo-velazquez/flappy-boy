#include <gb/gb.h>

extern struct Player {
    UBYTE sprite_id;
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
};

void setup_player_draft(struct Player* player);