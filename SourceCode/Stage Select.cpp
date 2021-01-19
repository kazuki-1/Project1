int state_ss;
int timer_ss;
#include "Stage Select.h"
std::unique_ptr<GameLib::Sprite>sprTest;
bool CursorSelect(VECTOR2 cPos, SEL_ELEM* obj)
{
    //VECTOR2 temp{};
    if (cPos.x > obj->pos.x + obj->pivot.x) return false;
    if (cPos.x < obj->pos.x - obj->pivot.x) return false;
    if (cPos.y < obj->pos.y - obj->pivot.y) return false;
    if (cPos.y > obj->pos.y + obj->pivot.y) return false;
    return true;
}

SEL_ELEM test[10];
void SEL_ELEM::Update()
{
    if (Cursor)
    {
        alpha += 0.01f;
        alpha = std::min(alpha, 1.0f);
    }
    else
    {
        alpha -= 0.01f;
        alpha = std::max(alpha, 0.3f);
    }
}
void stSel_Init()
{
    state_ss = 0;
    timer_ss = 0;
    int a{};
    sprTest.reset(GameLib::sprite_load(L"./Data/Images/test.png"));
    test[0].pos = { 100 + 150, 300 + 100 };
    test[1].pos = { 450 + 150, 300 + 100 };
    test[2].pos = { 800 + 150, 300 + 100 };
    test[3].pos = { 1150 + 150,300 + 100 };
    test[4].pos = { 1500 + 150,300 + 100 };
    test[5].pos = { 100 + 150, 600 + 100 };
    test[6].pos = { 450 + 150, 600 + 100 };
    test[7].pos = { 800 + 150, 600 + 100 };
    test[8].pos = { 1150 + 150, 600 + 100 };
    test[9].pos = { 1500 + 150, 600 + 100 };
    for (auto& a : test)
    {
        a.spr = sprTest.get();
        a.scale = { 1, 1 };
        a.tSize = { 300, 200 };
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
        for (auto& a : test)
        {
            if (CursorSelect(VECTOR2{ (float)GameLib::input::getCursorPosX(), (float)GameLib::input::getCursorPosY() }, &a))
            {
                a.Cursor = true;
                break;
            }
            else
                a.Cursor = false;
        }
        for (auto& a : test)
            a.Update();
        ++timer_ss;
        for (int i = 0; i < SIZEOF_ARRAY(test); i++) {
            if (CursorSelect(VECTOR2{ (float)GameLib::input::getCursorPosX(), (float)GameLib::input::getCursorPosY() }, &test[i]) && (GameLib::input::TRG_RELEASE(0) & GameLib::input::PAD_START))
                nextScene = (i + 2);
                break;
        }
    }
}

void stSel_Render()
{
    GameLib::clear({ 1, 1, 1, 1 });
    for (auto& a : test)
        a.Draw();

}