#include "Title_Scene.h"
int state_t, timer_t;

void title_init()
{
    state_t = 0;
    timer_t = 0;
}

void title_update()
{
    switch (state_t)
    {
    case 0:
        ++state_t;
    case 1:

        ++state_t;
    case 2:
        ++timer_t;
        break;
    }
}

void title_render()
{

}

void title_deinit()
{

}