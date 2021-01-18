#include "Map.h"
#include "Player.h"
#include "UI.h"
#include "Wind_Effect.h"

extern Player player;
extern Map Collision;
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
        UI_GP_Manage::GetInstance()->Init();

        map_init();
        player_init();


        Wind_Effect::GetInstance()->Init();
        ++t_state;
    case 2:
        UI_GP_Manage::GetInstance()->Update();

        map_update();
        player_update();


        Wind_Effect::GetInstance()->Update();
        break;
    }
}

void test_render()
{
    GameLib::clear(1, 0, 0);
    //test.Draw();


    map_render();
    player_render();

    UI_GP_Manage::GetInstance()->Render();
    Wind_Effect::GetInstance()->Render();
}

void test_deinit()
{

}