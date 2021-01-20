#include "Map.h"
#include "Player.h"
#include "UI.h"
#include "Wind_Effect.h"
#include "TransitionEffect.h"
extern Player player;
extern Map Collision;
Transition* transition;
GameLib::Sprite* sprTrans;
int t_state, t_timer;

extern int curScene;

void test_init()
{
    t_state = 0;
    t_timer = 0;
    //sprTrans = sprite_load(L"./Data/Images/transition.png");
    //transition->Initialize(sprTrans, { 0, 0 }, { 1.5, 1.5 }, { 0, 0 }, { 1238, 2129 });
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

        map_init(GETFOLDERNAME((curScene - 1)));
        player_init(GETFOLDERNAME((curScene - 1)));


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
    GameLib::clear(0, 0, 0);
    //test.Draw();


    map_render();
    player_render();

    UI_GP_Manage::GetInstance()->Render();
    Wind_Effect::GetInstance()->Render();
}

void test_deinit()
{

}