#include <gb/gb.h>

extern const struct Pipe {
    UBYTE col_pipe_id;
    UBYTE sprite_id;
    UINT8 x;
    UINT8 y;
    //UINT8 width; The width and height are const to 8, saving memory by avoiding them
    //UINT8 height;
};

void setup_pipes(struct Pipe pipes[], UBYTE sprite_ids[], UINT8 length_of_pipes_to_create);

void create_pipe(struct Pipe pipes[], UBYTE sprite_ids[]);

//void create_column_of_pipes(struct ColumnOPipe column, UBYTE sprite_ids[], UINT8 x);

INT8 get_next_available(UBYTE sprite_ids[]);