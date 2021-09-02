#include <gb/gb.h>
#include <stdbool.h>

const UINT8 v_sepration = 32;
const UINT8 v_screen_offset = 16;
const UINT8 h_screen_offset = 8;

const UINT8 num_pipe_cols = 4;
struct Pipe
{
    UINT8 sprite_id;
    UINT8 x;
    UINT8 y;
};

struct ColumnOfPipes
{
    bool is_created;
    bool is_scored;
    struct Pipe pipes[6];
};

struct ColumnOfPipes columns[4];
UINT8 index_col_pipes = 0;

void setup_pipes()
{
    FOR_I(0, num_pipe_cols)
    {
        FOR_J(0, 6)
        {
            columns[i].pipes[j].x = 168;
            columns[i].pipes[j].y = 0;
        }
    }

    FOR_FROM_ZERO_TO(num_pipe_cols)
    {
        columns[i].is_created = false;
        columns[i].is_scored = false;
    }
}


void create_pipe(UINT8 index, UINT8 x)
{
    UINT8 upper_blank = 0;
    UINT8 upper_count = random_range(1, 6);
    UINT8 space_count = 4;
    UINT8 lower_count = 10 - upper_count - space_count - upper_blank;

    UINT8 starting_y_coor = (upper_blank * 8) + v_screen_offset;

    UINT8 upper_pipe_limit = (upper_count * 8) + starting_y_coor;
    UINT8 pipe_count = 0;
    for (UINT8 y = starting_y_coor; y < upper_pipe_limit; y += 8)
    {
        INT8 id = get_next_available();

        sprite_ids[id] = true;
        set_sprite_tile(id, (y + 8) == upper_pipe_limit ? 3 : 2);
        move_sprite(id, x, y);
        columns[index].pipes[pipe_count].sprite_id = id;
        columns[index].pipes[pipe_count].x = x;
        columns[index].pipes[pipe_count].y = y;
        pipe_count++;
    }

    UINT8 lower_pipe_begining = upper_pipe_limit + (space_count * 8);
    UINT8 lower_pipe_limit = lower_pipe_begining + (lower_count * 8);

    for (UINT8 j = lower_pipe_begining; j < lower_pipe_limit; j += 8)
    {
        UINT8 id2 = get_next_available();

        sprite_ids[id2] = true;
        set_sprite_tile(id2, j == lower_pipe_begining ? 1 : 2);
        move_sprite(id2, x, j);
        columns[index].pipes[pipe_count].sprite_id = id2;
        columns[index].pipes[pipe_count].x = x;
        columns[index].pipes[pipe_count].y = j;
        pipe_count++;
    }
    columns[index].is_created = true;
    columns[index].is_scored = false;
    index_col_pipes++;
}

INT8 get_next_slot_col_pipe_available()
{
    INT8 next_available = -1;
    FOR_FROM_ZERO_TO(num_pipe_cols)
    {
        if (columns[i].is_created == false)
        {
            next_available = i;
            break;
        }
    }
    return next_available;
}
