#include <gb/gb.h>
#include <stdio.h>
#include "pipes.h"

struct ColumnOfPipe create_column_of_col(UBYTE sprite_ids[], UINT8 x)
{
    // Define the height based on the type of column we want to build
    struct Pipe pipes [6];

    const UINT8 v_sepration = 32;
    const UINT8 v_screen_offset = 16;
    const UINT8 h_screen_offset = 8;

    UINT8 pipe_index = 0;

    UINT8 upper_blank = 3;
    UINT8 upper_count = 3;
    UINT8 space_count = 3;
    UINT8 lower_count = 3;

    UINT8 starting_y_coor = (upper_blank * 8) + v_screen_offset;

    UINT8 upper_pipe_limit = (upper_count * 8) + starting_y_coor;

    for (UINT8 y = starting_y_coor; y < upper_pipe_limit; y += 8)
    {
        INT8 id = get_next_available(sprite_ids);
        sprite_ids[id] = 1;
        set_sprite_tile(id, (y + 8) == upper_pipe_limit ? 3 : 2);
        move_sprite(id, x, y);
        pipes[pipe_index].sprite_id = id;
        pipes[pipe_index].x = x;
        pipes[pipe_index].y = y;
        pipes[pipe_index].width = 8;
        pipes[pipe_index].height = 8;
        pipe_index++;
    }

    UINT8 lower_pipe_begining = upper_pipe_limit + (space_count * 8);
    UINT8 lower_pipe_limit = lower_pipe_begining + (lower_count * 8);

    for (UINT8 j = lower_pipe_begining; j < lower_pipe_limit; j += 8)
    {
        INT8 id2 = get_next_available(sprite_ids);
        sprite_ids[id2] = 1;
        set_sprite_tile(id2, j == lower_pipe_begining ? 1 : 2);
        move_sprite(id2, x, j);
        pipes[pipe_index].sprite_id = id2;
        pipes[pipe_index].x = x;
        pipes[pipe_index].y = j;
        pipes[pipe_index].width = 8;
        pipes[pipe_index].height = 8;
        pipe_index++;
    }
    struct ColumnOfPipe col;
    
    for (INT8 r =0; r!= 5 ; r++ ){
        col.pipes[r] = pipes[r];
    }

    return col;
}

INT8 get_next_available(UBYTE sprite_ids[])
{
    INT8 next_available = -1;
    INT8 i;
    for (i = 0; i != 40; i++)
    {
        if (sprite_ids[i] == 0)
        {
            next_available = i;
            break;
        }
    }
    return next_available;
}

UINT8 setup_col(UBYTE sprite_ids[])
{
    for (UINT8 i = 1; i <= 5; i++)
    {
        columnsOfCol[i-1] = create_column_of_col( sprite_ids, (24 * i) + (12 * i));
    }
    return 0;
}
