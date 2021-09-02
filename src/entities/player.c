#include <gb/gb.h>

const struct Player {
    UBYTE sprite_id;
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
};

struct Player player;


void setup_player() {
    sprite_ids[0] = true;
    player.sprite_id = 0;
    player.x = 32;
    player.y = 60;
    player.width  = 8;
    player.height = 8;
    set_sprite_tile(player.sprite_id, player.sprite_id);
    move_sprite(player.sprite_id, player.x, player.y);
}

void move_game_object(UINT8 x, UINT8 y) {
    player.x += x;
    player.y += y;
    move_sprite(player.sprite_id, player.x + x, player.y + y);
}
