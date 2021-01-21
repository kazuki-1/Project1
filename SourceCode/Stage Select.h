#pragma once
#include "../GameLib/game_lib.h"
#include "common.h"

#include <iostream>
class SEL_ELEM
{
public:
    std::shared_ptr<GameLib::Sprite> spr;
    VECTOR2 pos;
    VECTOR2 scale;
    VECTOR2 tPos;
    VECTOR2 tSize;
    VECTOR2 pivot{ tSize / 2 };
    void Draw()
    {
        GameLib::sprite_render(spr.get(),
            pos.x, pos.y, 
            scale.x, scale.y, 
            tPos.x, tPos.y, 
            tSize.x, tSize.y, 
            pivot.x, pivot.y, 
            0.0f, 
            1, 1, 1, alpha);
    }
    SEL_ELEM() {}
    SEL_ELEM(VECTOR2 p, GameLib::Sprite* sp) : pos(p), spr(sp)
    {
    }
    float alpha{ 0.5f };
    bool Cursor;
    void Update();
};

void stSel_Init();
void stSel_Update();
void stSel_Render();