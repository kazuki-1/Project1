#pragma once
#include "../GameLib/game_lib.h"
#include <iomanip>
#include <string>
#include <fstream>
#include "Object.h"
#include "Player.h"
#include "common.h"

#define MAP_X 38
#define MAP_Y 18
class Player;

void LoadCSV(std::string file_path, int arr[MAP_Y][MAP_X]);
class Map
{
public:
    Map() {}
    std::unique_ptr<GameLib::Sprite>mapSpr;
    std::string fileN;
    int chip[MAP_Y][MAP_X];
    VECTOR2 size;
    void Draw();
    void Set()
    {
        LoadCSV(fileN, chip);
    }
    void DrawChip(VECTOR2 p, int tx, int ty)
    {
        GameLib::sprite_render(mapSpr.get(), p.x, p.y, 1, 1, tx, ty, 54, 54, 27, 27, 0, 1, 1, 1, 1);
    }
    int getChip(VECTOR2 pos);
    virtual void Init(GameLib::Sprite* sp, std::string fN, VECTOR2 s)
    {
        mapSpr.reset(sp);
        fileN = fN;
        size = s;
        Set();
    }
};
//void test_init();


class WindMap : public Map
{
public:
    void Update();
    bool WindHit(Player* a);
    void FanCollision();
    bool StartOn;
    bool AlwaysOn;
    void Init(GameLib::Sprite* sp, std::string fN, VECTOR2 s) override
    {
        Map::Init(sp, fN, s);
        StartOn = false;
        AlwaysOn = false;
    }


};

class Fan
{
public:
    std::shared_ptr<GameLib::Sprite>spr;
    int x;
    int y;
    VECTOR2 pos{ x * 54.0f, y * 54.0f };
    
    enum Direction
    {
        DOWN = 134, UP, RIGHT, LEFT, RISE, NONE
    }dir;
    int id;
    bool On{};
    bool AlwaysOn{};
    Fan(int a, int b, Direction c, bool on, bool alwayson) : x(a), y(b), dir(c) , On(on), AlwaysOn(alwayson){}
    Fan(int a, int b, Direction c) : x(a), y(b), dir(c) {}
    void Draw()
    {
        GameLib::sprite_render(spr.get(), pos.x, pos.y, 1, 1, (dir / 14 - 1) * 54, dir / 14 * 54, 54, 54, 27, 27, 0.0f, 1, 1, 1, 1);
    }
};
void map_init();
void map_update();
void map_render();
bool HoriChipCheck(Object* obj, Map* map);
bool VertiChipCheck(Object* obj, Map* map);
bool TopChipCheck(Object* obj, Map* map);
bool HitCheck(VECTOR2 a_tl, VECTOR2 a_br, VECTOR2 b_tl, VECTOR2 b_br);
bool HoriFanCheck(Player* p, Fan* f);
void wind_init(WindMap* wM);
void wind_update();
