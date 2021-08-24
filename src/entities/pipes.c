#include <gb/gb.h>
#include "pipes.h"

void create_column_of_pipes(struct PipeCol pipes[],  UINT8 last_sprite_id,  UINT8 type_height_col_pipe, UINT8 x) {
    // Define the height based on the type of column we want to build 

    const UINT8 v_sepration = 32;
    const UINT8 v_screen_offset = 16;
    const UINT8 h_screen_offset = 8;

    UINT8 upper_pipe_limit;
    UINT8 lower_pipe_begining;
    UINT8 lower_pipe_limit;
    UINT8 height;
    switch (type_height_col_pipe)
    {
    case 1:
        height = 4;
        break;
    case 2:
        height = 5;
        break;
    case 3:
        height = 6;
        break; 
    default:
        height = 5;
        break;
    }

    /// UPPER PIPE SETUP
    
    upper_pipe_limit = (height * 8) + v_screen_offset;

    for (UINT8 y = 16+ 24; y < upper_pipe_limit; y+=8) {
        last_sprite_id++;
        set_sprite_tile(last_sprite_id, 2);
        move_sprite(last_sprite_id, x, y);

    }
    last_sprite_id++;
    set_sprite_tile(last_sprite_id, 3);
    move_sprite(last_sprite_id, x, upper_pipe_limit);

    lower_pipe_begining = upper_pipe_limit + v_sepration;

    /// LOWER PIPE SETUP
    last_sprite_id++;
    set_sprite_tile(last_sprite_id, 1);
    move_sprite(last_sprite_id, x, lower_pipe_begining );
    
     lower_pipe_limit = (16 + 144) - (8*8);

   // UINT8 remaining = ((256 - (16 + (8 * height) + v_sepration)) /8) - 12;

    for (UINT8 j = lower_pipe_begining + 8; j < lower_pipe_limit; j+=8) {
        last_sprite_id++;
        set_sprite_tile(last_sprite_id, 2);
        move_sprite(last_sprite_id, x, j);
    }
  

    UINT8 pipes_index =0;
    pipes[pipes_index].upper_y = upper_pipe_limit + 8;
    pipes[pipes_index].upper_x = x;
    pipes[pipes_index].upper_width = 8;
    pipes[pipes_index].upper_height = upper_pipe_limit + 8;


    pipes[pipes_index].lower_y = lower_pipe_begining;
    pipes[pipes_index].lower_x = x;
    pipes[pipes_index].lower_width = 8;
    pipes[pipes_index].lower_height = 16 + 144 -32;
    pipes_index++;

}

void setup_pipes(struct PipeCol pipes[], UINT8 last_sprite_id){
    for (UINT8 i = last_sprite_id; i <= (last_sprite_id + 2); i++) { 

        create_column_of_pipes(pipes,  i,  1, (24 * i) + (8 * i) );

    }
}
