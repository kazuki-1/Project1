#pragma once
#include "../GameLib/game_lib.h"
#include <iomanip>
#include <string>
#include <fstream>
#include "Object.h"
#include "Player.h"
#include "common.h"
#include <ostream>

#define MAP_X 38
#define MAP_Y 18
class Player;

void LoadCSV(std::string file_path, int arr[MAP_Y][MAP_X]);
class Map
{
public:
    Map() {}
    std::shared_ptr<GameLib::Sprite>mapSpr;
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

    virtual void Init(std::shared_ptr<GameLib::Sprite> sp, std::string fN, VECTOR2 s)
    {
        for (int y = 0; y < MAP_Y; y++) {
            for (int x = 0; x < MAP_X; x++) {
                chip[y][x] = 0;
            }
        }

        if (!std::ifstream(fN.c_str())) return;

        mapSpr = sp;
        fileN = fN;
        size = s;
        Set();
    }
    void Clear()
    {
        for (int y = 0; y < MAP_Y; ++y)
        {
            for (int x = 0; x < MAP_X; ++x)
            {
                chip[y][x] = 0;
            }
        }
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
    void Init(std::shared_ptr<GameLib::Sprite> sp, std::string fN, VECTOR2 s) override
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
        DOWN = 134, UP = 135, RIGHT = 136, LEFT = 137, RISE, NONE
    }dir;
    int id;
    bool On{};
    bool AlwaysOn{};
    Fan(int a, int b, Direction c, bool on, bool alwayson) : x(a), y(b), dir(c) , On(on), AlwaysOn(alwayson){}
    Fan(int a, int b, Direction c) : x(a), y(b), dir(c) {}
    void Draw()
    {
        int _dir = dir;
        if(_dir == DOWN) _dir = 133;
        else if(_dir == UP) _dir = 134;
        else if (_dir == RIGHT) _dir = 135;
        else if (_dir == LEFT) _dir = 136;

        GameLib::sprite_render(spr.get(), pos.x, pos.y, 1, 1, (_dir % 14) * 54, _dir / 14 * 54, 54, 54, 27, 27, 0.0f, 1, 1, 1, 1);
    }
};
void map_init(std::string map_name);
void map_update();
void map_render();
void map_deinit();
bool HoriChipCheck(Object* obj, Map* map);
bool VertiChipCheck(Object* obj, Map* map);
bool TopChipCheck(Object* obj, Map* map);
bool HitGoal(Object* obj, Map* map);
bool HitCheck(VECTOR2 a_tl, VECTOR2 a_br, VECTOR2 b_tl, VECTOR2 b_br);
bool HoriFanCheck(Player* p, Fan* f);
void wind_init(WindMap* wM);
void wind_update();
