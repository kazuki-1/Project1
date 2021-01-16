#include "Map.h"
#include <sstream>
#define CHIP_S 54
Map test;
WindMap WindM;
GameLib::Sprite* sprTest;
GameLib::Sprite* sprWind;
GameLib::Sprite* sprFan;
std::vector<Fan>fans;
std::vector<Fan>wind;
std::vector<Fan>dist;
#define FAN_UP 37
extern Player player;
void wind_init(WindMap *wM);
void wind_update();
void LoadCSV(std::string file_path, int arr[MAP_Y][MAP_X]) 
{
    std::ifstream data(file_path);
    if (!data)
        assert(!"File Doesn't exist");
    std::string str{};
    int col = 0, row = 0;
    while (std::getline(data, str)) 
    {
        std::stringstream ss(str);
        std::string cell;
        col = 0;
        while (std::getline(ss, cell, ',')) 
        {
            arr[row][col] = atoi(cell.c_str());
            col++;
        }
        row++;
    }
}
void Map::Draw()
{
    for (int y = 0; y < MAP_Y; ++y)
    {
        for (int x = 0; x < MAP_X; ++x)
        {
            if (chip[y][x] == 0)
                continue;
            DrawChip({ x * size.x, y * size.y },  (chip[y][x] - 1)% 8 * size.x, chip[y][x] / 8 * size.y );
        }
    }
}
int Map::getChip(VECTOR2 pos)
{
    int a = (int)pos.x / 54;
    int b = (int)pos.y / 54;
    return chip[b][a];
}
bool HoriChipCheck(Object* obj, Map *map)
{
    if (map->getChip({ obj->pos.x + 1 + obj->pivot.x + 27, obj->pos.y }) != 0 || map->getChip({ obj->pos.x - 1 - obj->pivot.x + 27, obj->pos.y }) != 0)
        return true;
    if (map->getChip({ obj->pos.x + 1 + obj->pivot.x + 27, obj->pos.y - obj->pivot.y + 1 }) != 0 || map->getChip({ obj->pos.x - 1 - obj->pivot.x + 27, obj->pos.y - obj->pivot.y + 1}) != 0)
        return true;
    if (map->getChip({ obj->pos.x + 1 + obj->pivot.x + 27, obj->pos.y - obj->pivot.y / 2 }) != 0 || map->getChip({ obj->pos.x - 1 - obj->pivot.x + 27, obj->pos.y - obj->pivot.y / 2 }) != 0)
        return true;
    return false;
}
bool VertiChipCheck(Object* obj, Map *map)
{
    if (map->getChip({ obj->pos.x - obj->pivot.x - 1, obj->pos.y + 1 + 54 / 2 }) != 0 || map->getChip({ obj->pos.x + obj->pivot.x + 1, obj->pos.y + 1 + 54 / 2 }) != 0)
        return true;
    
    return false;
}
bool TopChipCheck(Object* obj, Map* map)
{
    if (map->getChip({ obj->pos.x, obj->pos.y - obj->pivot.y + 1 }))
        return true;
    if (map->getChip({ obj->pos.x + obj->pivot.x, obj->pos.y - obj->pivot.y + 1 }))
        return true;
    if (map->getChip({ obj->pos.x - obj->pivot.x, obj->pos.y - obj->pivot.y + 1 }))
        return true;
    return false;
}
void WindMap::Update()
{
}
using namespace GameLib::primitive;
bool HitCheck(VECTOR2 a_tl, VECTOR2 a_br, VECTOR2 b_tl, VECTOR2 b_br)
{
    if (a_tl.x > b_br.x)
        return false;
    if (a_tl.y > b_br.y)
        return false;
    if (a_br.x < b_tl.x)
        return false;
    if (a_br.y < b_tl.y)
        return false;
    return true;
}
bool WindMap::WindHit(Player* a)
{
    for (int y = 0; y < MAP_Y; ++y)
    {
        for (int x = 0; x < MAP_X; ++x)
        {
            VECTOR2 p_tl = a->pos - a->pivot;
            VECTOR2 p_br = a->pos + VECTOR2(a->pivot.x, 0);
            VECTOR2 c_tl = { x * 54 - 27.0f, y * 54 - 27.0f };
            VECTOR2 c_br = { x * 54 + 27.0f, y * 54 - 27.0f };
            if (HitCheck(p_tl, p_br, c_tl, c_br))
                return true;
        }
    }
    return false;
}
void WindMap::FanCollision()
{
    Fan* f{ nullptr };
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        VECTOR2 p_tl{ player.pos.x - player.pivot.x, player.pos.y - player.pivot.y };
        VECTOR2 p_br{ player.pos.x + player.pivot.x, player.pos.y };
        VECTOR2 f_tl{ fans[alpha].x * 54.0f - 27.0f, fans[alpha].y * 54.0f - 27.0f };
        VECTOR2 f_br{ fans[alpha].x * 54.0f + 27.0f, fans[alpha].y * 54.0f + 27.0f };
        if (HitCheck(p_tl, p_br, f_tl, f_br))
        {
            //player.PushFan(&fans[alpha]);
            f = &fans[alpha];
        }

    }
    if (f)
        player.PushFan(f);
}
void wind_init(WindMap *wM)
{
    for (int alpha = 0; alpha < MAP_Y; ++alpha)
    {
        for (int beta = 0; beta < MAP_X; ++beta)
        {
            if (wM->chip[alpha][beta] == 2)
                fans.push_back({ beta, alpha, Fan::Direction::RIGHT });
            if (wM->chip[alpha][beta] == 1)
                fans.push_back({ beta, alpha, Fan::Direction::LEFT });
            if (wM->chip[alpha][beta] == FAN_UP)
                fans.push_back({ beta, alpha, Fan::Direction::UP });
        }
    }
}
void wind_update()
{
    VECTOR2 temp{0, 0};
    VECTOR2 pos;
    wind.clear();
    dist.clear();
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        int distance{};
        for (int beta = 0; beta < fans.size(); ++beta)
        {
            if (alpha == beta)
                continue;
            if (fans[alpha].y == fans[beta].y)
            {
                distance = abs(fans[alpha].x - fans[beta].x);
                int center = fans[alpha].x + distance / 2;
                if (center && distance < 10)
                {
                    dist.push_back({ center, fans[alpha].y, Fan::Direction::RISE });
                    ++alpha;
                    ++beta;
                    break;
                }
            }
        }
    }
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        pos = VECTOR2((float)fans[alpha].x, (float)fans[alpha].y);
        
        if (fans[alpha].dir == Fan::Direction::LEFT)
            temp = { -1, 0 };
        if (fans[alpha].dir == Fan::Direction::RIGHT)
            temp = { 1, 0 };
        if (fans[alpha].dir == Fan::Direction::UP)
            temp = { 0, -1 };
        for (int beta = 0; beta < 5; ++beta)
        {
            VECTOR2 tP{ pos };
            if (tP + temp == pos)
                break;
            pos += temp;
            for (int gamma = 0; gamma < dist.size(); ++gamma)
            {
                if (pos == VECTOR2{ (float)dist[gamma].x, (float)dist[gamma].y })
                {
                    int a{ test.getChip({ pos.x * 54, pos.y * 54 + 55 }) };
                    int b{ WindM.getChip({pos.x * 54, pos.y * 54 + 55}) };
                    bool t{};
                    for (int alpha = 0; alpha < dist.size(); ++alpha)
                    {
                        if (pos + VECTOR2{0, 1} == VECTOR2{ (float)dist[alpha].x, (float)dist[alpha].y })
                            t = true;
                    }
                    for (int alpha = 0; alpha < dist.size(); ++alpha)
                    {
                        if (pos + VECTOR2{ 0, -1 } == VECTOR2{ (float)dist[alpha].x, (float)dist[alpha].y })
                        {
                            temp = { 0, 0 };
                            t = true;
                        }
                    }
                    /*if (t)
                        break;*/
                    if (!a && !b && !t)
                    {
                        temp = { 0, 0 };
                    }

                    else if (dist[gamma].dir == Fan::Direction::UP || dist[gamma].dir == Fan::Direction::RISE)
                        temp = { 0, -1 };
                    break;
                }
            }
            for (auto& a : fans)
            {
                if (pos == VECTOR2{ (float)a.x, (float)a.y })
                    temp = { 0, 0 };

            }
            for (int y = 0; y < MAP_Y; ++y)
            {
                for (int x = 0; x < MAP_X; ++x)
                {
                    if (test.getChip(pos * 54.0f))
                        temp = { 0, 0 };
                }
            }
            Fan::Direction dir = Fan::Direction::UP;
            if (temp == VECTOR2{ -1, 0 })
                dir = Fan::Direction::LEFT;
            else if (temp == VECTOR2{ 1, 0 })
                dir = Fan::Direction::RIGHT;
            else if (temp == VECTOR2{ 0, -1 })
                dir = Fan::Direction::UP;
            else if (temp == VECTOR2{ 0, 0 })
                dir = Fan::Direction::NONE;
            if (test.getChip(pos * 54))
                continue;
            wind.push_back({ (int)pos.x, (int)pos.y, dir });
        }
    }
    WindM.FanCollision();
}
void map_init()
{
    sprTest = GameLib::sprite_load(L"./Data/Map/back_chip.png");
    sprWind = GameLib::sprite_load(L"./Data/Map/back_chip.png");
    sprFan = GameLib::sprite_load(L"./Data/Map/back_chip.png");

    std::shared_ptr<GameLib::Sprite> share_sprFan (sprFan);

    WindM.mapSpr.reset(sprWind);
    WindM.fileN = "./Data/Map/test4_1.txt";
    WindM.Set();
    WindM.size = { 54, 54 };

    test.mapSpr.reset(sprTest);
    test.fileN = "./Data/Map/test4.txt";
    test.Set();
    test.size.x = 54;
    test.size.y = 54;
    wind_init(&WindM);
    for (int alpha = 0; alpha < fans.size(); ++alpha)
        fans[alpha].spr = share_sprFan;
}
void map_update()
{
    WindM.Update();
    wind_update();
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        for (int beta = 0; beta < fans.size(); ++beta)
        {
            if (alpha == beta)
                continue;
            if (fans[alpha].x != fans[beta].x)
                continue;
            if (fans[alpha].y == fans[beta].y + 1 || fans[alpha].y == fans[beta].y - 1)
                fans[alpha].pos.x = fans[beta].pos.x;
        }
    }
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        fans[alpha].x = fans[alpha].pos.x / 54;
        fans[alpha].y = fans[alpha].pos.y / 54;
    }
}
void map_render()
{
    test.Draw();
    //WindM.Draw();
    for (auto& a : fans)
        a.Draw();
    for (int alpha = 0; alpha < wind.size(); ++alpha)
        circle(
            VECTOR2((wind[alpha].x) * 54, (wind[alpha].y) * 54),
            5.0f,
            {1.0f, 1.0f},
            0.0f,
            { 1, 0, 0, 1 }
    );
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        circle(VECTOR2(fans[alpha].x * 54.0f, fans[alpha].y * 54.0f), 5.0f, VECTOR2(1.0f, 1.0f), 0.0f, { 0, 1, 1, 1 });
    }

    //wind.clear();
}
//bool HoriFanCheck(Player* p, Fan* f)
//{
//    VECTOR2 p_tl{ p->pos.x -  p->pivot.x,  p->pos.y - p->pivot.y };
//    VECTOR2 p_br{ p->pos.x +  p->pivot.x,  p->pos.y };
//    VECTOR2 f_tl{ f->x * 54.0f - 27.0f, f->y * 54.0f - 27.0f };
//    VECTOR2 f_br{ f->x * 54.0f + 27.0f, f->y * 54.0f + 27.0f };
//    if (p_tl.x > f_br.x)
//        return false;
//    if (p_tl.y > f_br.y)
//        return false;
//    if (p_br.x < f_tl.x)
//        return false;
//    if (p_br.y < f_tl.y)
//        return false;
//    return true;
//
//}