#pragma once
#include "Object.h"

class Transition : public Object
{
public:
    static Transition* Instance()
    {
        static Transition instance;
        return &instance;
    }
    bool Vertical;
    bool Horizontol;
    bool state;
    void Activate(bool isVertical)
    {
        if (!isVertical) {
            pos = { SCREEN_W + pivot.y , 0};
            angle = DirectX::XMConvertToRadians(90.0f);

            Horizontol = true;
        } else {
            pos = { SCREEN_W / 2, -pivot.y };
            Vertical = true;
        }
        state = true;
    }
    bool Transitioning()
    {
        return Horizontol || Vertical;
    }
    void Initialize(GameLib::Sprite* sp, VECTOR2 p, VECTOR2 s, VECTOR2 tp, VECTOR2 ts)
    {
        if (spr) return;
        Object::Initialize(sp, p, s, tp, ts);
        pivot = { ts / 2 };
        
    }
    void Reset()
    {
        state = false;
        if (Vertical)
        {
            pos = { 0, -1500 - pivot.y };
        }
        else if (Horizontol)
        {
            pos = { 3000 + pivot.x, SCREEN_H / 2 };
            angle = DirectX::XMConvertToRadians(90.0f);
        }
    }
    void Update()
    {
        if (Horizontol)
        {
            pos.x -= 60;
            if (pos.x <= SCREEN_W / 2) {
                state = false;
            } else if(pos.x <= -2000) {
                Horizontol = false;
            }
        }
        else if (Vertical)
        {
            pos.y += 30;
            if (pos.y > SCREEN_H / 2) {
                state = false;
            } else if (pos.y > pivot.y) {
                Vertical = false;
            }
        }
    }
    void Draw() {
        if(Vertical || Horizontol)
            GameLib::sprite_render(spr.get(), pos.x, pos.y, size.x, size.y, tPos.x, tPos.y, tSize.x, tSize.y, pivot.x, pivot.y, angle, 1, 1, 1, 1);
    }

};

