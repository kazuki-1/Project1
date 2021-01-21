int state_ss;
int timer_ss;
#include "Stage Select.h"
std::shared_ptr<GameLib::Sprite>sprTest;
std::shared_ptr<GameLib::Sprite>sprLock;
extern int cur_stage_level;
bool CursorSelect(VECTOR2 cPos, SEL_ELEM* obj)
{
    //VECTOR2 temp{};
    if (cPos.x > obj->pos.x + obj->pivot.x) return false;
    if (cPos.x < obj->pos.x - obj->pivot.x) return false;
    if (cPos.y < obj->pos.y - obj->pivot.y) return false;
    if (cPos.y > obj->pos.y + obj->pivot.y) return false;
    return true;
}

SEL_ELEM test[8];
void SEL_ELEM::Update()
{
    static float sin_val;
    if (Cursor) {
        sin_val += 0.07f;
        scale = { 1 + (abs(sinf(sin_val)) * 0.2f), 1 + (abs(sinf(sin_val)) * 0.2f) };
    } else {
        scale += VECTOR2{ 1, 1 } - scale;
        alpha = std::max(alpha, 0.3f);
    }
}
void stSel_Init()
{
    state_ss = 0;
    timer_ss = 0;
    int a{};
    sprTest.reset(GameLib::sprite_load(L"./Data/Images/StageImg.png"));
    sprLock.reset(GameLib::sprite_load(L"./Data/Images/Lock.png"));

    test[0].pos = { 100 + 150, 300 + 100 };
    test[1].pos = { 450 + 150, 300 + 100 };
    test[2].pos = { 800 + 150, 300 + 100 };
    test[3].pos = { 1150 + 150,300 + 100 };
    test[4].pos = { 1500 + 150,300 + 100 };
    test[5].pos = { 100 + 150, 600 + 100 };
    test[6].pos = { 450 + 150, 600 + 100 };
    test[7].pos = { 800 + 150, 600 + 100 };
    int i = 0;
    for (auto& a : test) {
        a.spr = sprTest;
        a.scale = { 1, 1 };
        a.tSize = { 300, 144 };
        a.tPos.x = i++ * 300;
        a.pivot = a.tSize / 2;
        a.alpha = 0.3f;
    }
}

void stSel_Update()
{
    switch (state_ss)
    {
    case 0:
    case 1:
    case 2:

        for (int i = 0; i < SIZEOF_ARRAY(test); i++) {
            if (i > cur_stage_level) break;
            if (CursorSelect(VECTOR2{ (float)GameLib::input::getCursorPosX(), (float)GameLib::input::getCursorPosY() }, &test[i]))
            {
                test[i].Cursor = true;
                break;
            }
            else
                test[i].Cursor = false;
        }
        for (auto& a : test)
            a.Update();
        ++timer_ss;
        for (int i = 0; i < SIZEOF_ARRAY(test); i++) {
            if (i > cur_stage_level) break;
            if (CursorSelect(VECTOR2{ (float)GameLib::input::getCursorPosX(), (float)GameLib::input::getCursorPosY() }, &test[i]) && (GameLib::input::TRG_RELEASE(0) & GameLib::input::PAD_START)) {
                nextScene = (i + 2);
                break;
            }
        }
    }
}

void stSel_Render() {
    GameLib::clear({ 1, 1, 1, 1 });
    for (int i = 0; i < 8; i++) {
        test[i].alpha = 1.0f;
        if (i > cur_stage_level) {
            test[i].alpha = 0.5f;
        }
        test[i].Draw();
        
        if (i > cur_stage_level) {
            GameLib::sprite_render(sprLock.get(), test[i].pos.x, test[i].pos.y, test[i].scale.x, test[i].scale.y, 0, 0, test[i].tSize.x, test[i].tSize.y, test[i].pivot.x, test[i].pivot.y, 0, 1, 1, 1, 1);
        }
    }

}


