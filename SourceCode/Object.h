#ifndef OBJECT_H
#define OBJECT_H
#include "../GameLib/game_lib.h"

class Object
{
public:
    std::unique_ptr<GameLib::Sprite>spr;
    std::string name;
    VECTOR2 pos;
    VECTOR2 size;
    VECTOR2 tPos;
    VECTOR2 tSize;
    VECTOR2 pivot{};
    float angle{};
    virtual void Initialize(GameLib::Sprite* sp, VECTOR2 p, VECTOR2 s, VECTOR2 tp, VECTOR2 ts)
    {
        spr.reset(sp);
        pos = p;
        size = s;
        tPos = tp;
        tSize = ts;
    }
    void Draw()
    {
        GameLib::sprite_render(spr.get(), pos.x, pos.y, size.x, size.y, tPos.x, tPos.y, tSize.x, tSize.y, pivot.x, pivot.y, angle,  1, 1, 1, 1 );
    }
    virtual void Update() {}
};

#endif