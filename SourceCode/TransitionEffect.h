#pragma once
#include "Object.h"

class Transition : public Object
{
public:
    bool Vertical;
    bool Horizontol;
    bool state;
    void Activate()
    {
        state = true;
    }
    bool State()
    {
        return state;
    }
    void Initialize(GameLib::Sprite* sp, VECTOR2 p, VECTOR2 s, VECTOR2 tp, VECTOR2 ts)
    {
        Object::Initialize(sp, p, s, tp, ts);
    }
    void Reset()
    {
        state = false;
        if (Vertical)
            pos = { 0, -1500 };
        else if (Horizontol)
            pos = { -3000, 0 };

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

};