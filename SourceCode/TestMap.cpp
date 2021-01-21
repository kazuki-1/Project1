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
extern bool isPause;

int cur_stage_level = 0;
std::unique_ptr<GameLib::Sprite> gomispr;
std::unique_ptr<GameLib::Sprite>sprBG1;
std::unique_ptr<GameLib::Sprite>sprBG2;
std::unique_ptr<GameLib::Sprite>sprtext;
std::unique_ptr<GameLib::Sprite>sprtext2;
std::unique_ptr<GameLib::Sprite>sprTitle;

void BG_Render(SCENES s);
void test_init()
{

    //sprTrans = ;
    t_state = 0;
    t_timer = 0;
    Transition::Instance()->Initialize(sprite_load(L"./Data/Images/transition.png"), { 0, 0 }, { 1.75f, 1.75f }, { 0, 0 }, { 1238, 2129 });
    if (!gomispr)
        gomispr = std::unique_ptr<GameLib::Sprite>(sprite_load(L"./Data/Images/gomi_2.png"));
    if (!sprBG1)
        sprBG1 = std::unique_ptr<GameLib::Sprite>(sprite_load(L"./Data/Images/bg_1.png"));
    if (!sprBG2)
        sprBG2 = std::unique_ptr<GameLib::Sprite>(sprite_load(L"./Data/Images/bg_2.png"));
    if (!sprtext)
        sprtext = std::unique_ptr<GameLib::Sprite>(sprite_load(L"./Data/Images/text.png"));
    if (!sprtext2)
        sprtext2 = std::unique_ptr<GameLib::Sprite>(sprite_load(L"./Data/Images/text2.png"));
    if (!sprTitle)
        sprTitle = std::unique_ptr<GameLib::Sprite>(sprite_load(L"./Data/Images/title.png"));

}


float black_alpha = 0;
bool isOk = false;
void test_update()
{
    switch (t_state)
    {
    case 0:
        black_alpha = -0.8f;
        if (curScene - 1 > cur_stage_level)
            cur_stage_level = curScene - 1;
        isOk = false;
        if (Transition::Instance()->state) {
            Transition::Instance()->Update();
            return;
        }
        ++t_state;
        // nothing
    case 1:
        isOk = true;
        UI_GP_Manage::GetInstance()->Init();


        map_init(GETFOLDERNAME((curScene)));
        player_init(GETFOLDERNAME((curScene)));
            

        Wind_Effect::GetInstance()->Init();
        ++t_state;
    case 2:
        UI_GP_Manage::GetInstance()->Update();

        if (isPause) {
            music::pause(ROBOTWALK);
            break;
        } else {
            music::resume(ROBOTWALK);
        }

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
    BG_Render((SCENES)nextScene);
    //test.Draw();
    if ((curScene == 0 || curScene == 9) && isOk) {
        sprite_render(gomispr.get(), 540, (MAP_Y - 1) * 54, 0.65f, 0.65f, 0, 0, 1428, 559, 0, 559, 0, 1, 1, 1, 1);
    } 
    map_render();
    player_render();
    UI_GP_Manage::GetInstance()->Render();
    Wind_Effect::GetInstance()->Render();
    Transition::Instance()->Draw();

    if (curScene == SCENES::TITLE)
        sprite_render(sprTitle.get(), 0, 0, 0.7f, 0.7f, 0, 0, 1190, 564, 0, 0, 0, 1, 1, 1, 1);

    if (curScene == 9 && isOk) {
        static int timer = 0;
        sprite_render(sprtext2.get(), 0, 0, 1, 1, 0, 0, 1920, 1080, 0, 0, 0, 1, 1, 1, black_alpha);
        sprite_render(sprtext.get(), 0, 0, 1, 1, 0, 0, 1920, 1080, 0, 0, 0, 1, 1, 1, black_alpha - 0.45f);

        timer++;

        if (timer < 300) {
            black_alpha += 0.015f;
        }
        else
            nextScene = SCENES::TITLE;
    }
}

void test_deinit()
{
}

void BG_Render(SCENES s)
{
    switch (s)
    {
    case SCENES::TITLE:
    case SCENES::STAGE3:
    case SCENES::STAGE6:
    case SCENES::STAGE7:
    case SCENES::STAGE8:
    case 9:
        sprite_render(sprBG1.get(), 0, 0, 1, 1.2, 0, 0, 2056, 972, 0, 0, 0.0f, 1, 1, 1, 1);
        break;
    default:
        sprite_render(sprBG2.get(), 0, 0, 1, 1.2, 0, 0, 2056, 972, 0, 0, 0.0f, 1, 1, 1, 1);
    }
}
