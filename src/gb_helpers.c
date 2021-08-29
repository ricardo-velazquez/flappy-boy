#include <gb/gb.h>



void perform_delay(UINT8 times)
{
    for (UINT8 i = 0; i < times; i++)
    {
        wait_vbl_done();
    }
}