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
    void Activate()
    {
        state = true;
    }
    bool Transitioning()
    {
        return state;
    }
    void Initialize(GameLib::Sprite* sp, VECTOR2 p, VECTOR2 s, VECTOR2 tp, VECTOR2 ts)
    {
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
            pos = { -3000 - pivot.x, 0 };
            angle = DirectX::XMConvertToRadians(90.0f);
        }
    }
    void Update()
    {
        if (state)
        {
            if (Horizontol)
            {
                pos.x += 60;
            }
            else if (Vertical)
            {
                pos.y += 30;
            }
        }
    }
    void Draw()
    {
        GameLib::sprite_render(spr.get(), pos.x, pos.y, size.x, size.y, tPos.x, tPos.y, tSize.x, tSize.y, pivot.x, pivot.y, angle, 1, 1, 1, 1);
    }

};

