#include "Map.h"
#include "Player.h"
#include "UI.h"
#include "Wind_Effect.h"
#include "TransitionEffect.h"
extern Player player;
extern Map Collision;
//std::unique_ptr<Transition>transition(new Transition());
int t_state, t_timer;
bool Clear{};
extern int curScene;

void test_init()
{

    //sprTrans = ;
    t_state = 0;
    t_timer = 0;
    Transition::Instance()->Initialize(sprite_load(L"./Data/Images/transition.png"), { 0, 0 }, { 1.5, 1.5 }, { 0, 0 }, { 1238, 2129 });
}

void test_update()
{
    switch (t_state)
    {
    case 0:
        if (Transition::Instance()->state) {
            Transition::Instance()->Update();
            return;
        }
        ++t_state;
        // nothing
    case 1:
        UI_GP_Manage::GetInstance()->Init();
        map_init(GETFOLDERNAME((curScene - 1)));
        player_init(GETFOLDERNAME((curScene - 1)));


        Wind_Effect::GetInstance()->Init();
        ++t_state;
    case 2:
        if(Clear)
            UI_GP_Manage::GetInstance()->Update();

        map_update();
        player_update();
        if (Transition::Instance()->Transitioning())
        {
            Transition::Instance()->Update();
        }
        
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
    Transition::Instance()->Draw();
}

void test_deinit()
{
}