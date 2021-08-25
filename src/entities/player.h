#include <gb/gb.h>

extern const struct Player {
    UBYTE col_pipe_id;
    UBYTE sprite_id;
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
};

void setup_player(struct Player* player, UBYTE sprite_ids[]);

void move_game_object(struct Player* obj, UINT8 x, UINT8 y);