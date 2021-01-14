#include "Map.h"
#include "Player.h"
extern Player player;
extern Map test;
int t_state, t_timer;

void test_init()
{
    t_state = 0;
    t_timer = 0;
}

void test_update()
{
    switch (t_state)
    {
    case 0:
        ++t_state;
        // nothing
    case 1:
        map_init();
        player_init();
        ++t_state;
    case 2:
        map_update();
        player_update();
        break;
    }
}

void test_render()
{
    GameLib::clear(0, 0, 0);
    //test.Draw();
    map_render();
    player_render();
}

void test_deinit()
{

}