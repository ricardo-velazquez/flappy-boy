#include <gb/gb.h>

extern struct Player {
    UBYTE sprite_id;
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
};

void setup_player(struct Player* player);

void move_game_object(struct Player* obj, UINT8 x, UINT8 y);