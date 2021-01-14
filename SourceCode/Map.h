#pragma once
#include "../GameLib/game_lib.h"
#include <iomanip>
#include <string>
#include <fstream>
#include "Object.h"
#include "Player.h"

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
};
bool HoriChipCheck(Object* obj, Map *map);
bool VertiChipCheck(Object* obj, Map *map);
bool TopChipCheck(Object* obj, Map* map);
bool HitCheck(VECTOR2 a_tl, VECTOR2 a_br, VECTOR2 b_tl, VECTOR2 b_br);
//void test_init();
enum CHIP_TYPE
{
    // TODO : PLACE HOLDER FOR CHIP TYPES
};


class WindMap : public Map
{
public:
    void Update();
    bool WindHit(Player* a);
    void setWind(int chip[MAP_Y][MAP_X], int x, int y);
};

class Fan
{
public:
    int x;
    int y;
    enum Direction
    {
        UP, DOWN, LEFT, RIGHT, NONE
    }dir;
    int id;
    bool On{};
    Fan(int a, int b, Direction c) : x(a), y(b), dir(c) {}
};
void map_init();
void map_update();
void map_render();