#include "Map.h"
#include "Switch_Manage.h"
#include <sstream>
#define CHIP_S 54
#define SIZE (VECTOR2{54, 54})
Map Collision;
Map SpriteTexture;
Map BG;
extern Fan* pushedFan;
extern std::vector<Fan*> slip_fan;
WindMap WindM;
WindMap Fan1;
WindMap Fan2;
GameLib::Sprite* sprMain;
std::vector<Fan>fans;
std::vector<Fan>wind;
std::vector<Fan>dist;
#define FAN_UP 37
extern Player player;
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
            int tx = (chip[y][x] - 1) % 14;
            int ty = chip[y][x] / 14;

            if(chip[y][x] == 140 || chip[y][x] == 154 || chip[y][x] == 168) ty--;

            DrawChip({ x * size.x, y * size.y }, tx * size.x, ty * size.y);
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
    if (map->getChip({ obj->pos.x + 1 + obj->pivot.x + 27, obj->pos.y - obj->pivot.y + 54 }) != 0 || map->getChip({ obj->pos.x - 1 - obj->pivot.x + 27, obj->pos.y - obj->pivot.y + 54 }) != 0)
        return true;
    if (map->getChip({ obj->pos.x + 1 + obj->pivot.x + 27, obj->pos.y - obj->pivot.y / 2 }) != 0 || map->getChip({ obj->pos.x - 1 - obj->pivot.x + 27, obj->pos.y - obj->pivot.y / 2 }) != 0)
        return true;
    return false;
}
bool VertiChipCheck(Object* obj, Map *map)
{   
    if (map->getChip({ std::roundf((obj->pos.x - obj->pivot.x + 1) / 54) * 54, obj->pos.y + 1 + 54 / 2 }) != 0 || map->getChip({ obj->pos.x + obj->pivot.x - 1, obj->pos.y + 1 + 54 / 2 }) != 0)
        return true;
    
    return false;
}
bool TopChipCheck(Object* obj, Map* map)
{
    if (map->getChip({ obj->pos.x + 1, obj->pos.y - obj->pivot.y + 18}))
        return true;
    if (map->getChip({ obj->pos.x + obj->pivot.x - 1, obj->pos.y - obj->pivot.y + 18 }))
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
        VECTOR2 p_tl{ player.pos.x - player.pivot.x - 5, player.pos.y - player.pivot.y };
        VECTOR2 p_br{ player.pos.x + player.pivot.x + 5, player.pos.y };
        VECTOR2 f_tl{ fans[alpha].x * 54.0f - 27.0f, fans[alpha].y * 54.0f - 27.0f };
        VECTOR2 f_br{ fans[alpha].x * 54.0f + 27.0f, fans[alpha].y * 54.0f + 27.0f };
        if (HitCheck(p_tl, p_br, f_tl, f_br))
        {
            //player.PushFan(&fans[alpha]);
            f = &fans[alpha];
            //break;

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
            if (wM->chip[alpha][beta] == Fan::Direction::BLOCK)
                fans.push_back({ beta, alpha, Fan::Direction::BLOCK, true, true });
            if (wM->StartOn)
            {
                if (wM->chip[alpha][beta] == Fan::Direction::RIGHT)
                    fans.push_back({ beta, alpha, Fan::Direction::RIGHT, true, false});
                if (wM->chip[alpha][beta] == Fan::Direction::LEFT)
                    fans.push_back({ beta, alpha, Fan::Direction::LEFT, true, false });
                if (wM->chip[alpha][beta] == Fan::Direction::UP)
                    fans.push_back({ beta, alpha, Fan::Direction::UP , true, false});
            }
            else if (wM->AlwaysOn)
            {
                if (wM->chip[alpha][beta] == Fan::Direction::RIGHT)
                    fans.push_back({ beta, alpha, Fan::Direction::RIGHT, true, true });
                if (wM->chip[alpha][beta] == Fan::Direction::LEFT)
                    fans.push_back({ beta, alpha, Fan::Direction::LEFT, true, true });
                if (wM->chip[alpha][beta] == Fan::Direction::UP)
                    fans.push_back({ beta, alpha, Fan::Direction::UP , true, true });
            }
            else
            {
                if (wM->chip[alpha][beta] == Fan::Direction::RIGHT)
                    fans.push_back({ beta, alpha, Fan::Direction::RIGHT, false, false });
                if (wM->chip[alpha][beta] == Fan::Direction::LEFT)
                    fans.push_back({ beta, alpha, Fan::Direction::LEFT, false, false });
                if (wM->chip[alpha][beta] == Fan::Direction::UP)
                    fans.push_back({ beta, alpha, Fan::Direction::UP , false, false });
            }
        }
    }

   /* for (auto& a : fans)
        a.On = true;*/
}
void wind_update()
{
    VECTOR2 temp{0, 0};
    VECTOR2 pos;
    wind.clear();
    dist.clear();
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        if (!fans[alpha].On)
        {
            if (!fans[alpha].AlwaysOn) {
                continue;
            }
            //continue;
        }
        if (fans[alpha].dir != Fan::Direction::BLOCK)
            fans[alpha].timer++;
        
        int distance{};
        for (int beta = 0; beta < fans.size(); ++beta)
        {
            if (!fans[beta].On)
            {
                if (!fans[beta].AlwaysOn)
                    continue;
                //continue;
            }
            if (alpha == beta)
                continue;
            if (fans[alpha].dir == Fan::Direction::UP || fans[alpha].dir == Fan::Direction::UP)
                continue;
            if (fans[alpha].y == fans[beta].y)
            {
                if (fans[alpha].x - fans[beta].x > 0)
                    continue;
                distance = abs(fans[alpha].x - fans[beta].x);
                
                int center = fans[alpha].x + distance / 2;
                if (center && distance < 10)
                {
                    dist.push_back({ center, fans[alpha].y, Fan::Direction::UP });
                    ++alpha;
                    ++beta;
                    break;
                }
            }
        }
    }
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        if (!fans[alpha].On)
        {
            if (!fans[alpha].AlwaysOn)
                continue;
            //continue;
        }
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
                    int a{ Collision.getChip({ pos.x * 54, pos.y * 54 + 55 }) };
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
                            
                            t = true;
                        }
                    }
                    /*if (t)
                        break;*/
                    //if (!a && !b && !t)
                    //{
                    //    temp = { 0, 0 };
                    //}

                    if (dist[gamma].dir == Fan::Direction::UP || dist[gamma].dir == Fan::Direction::RISE)
                        temp = { 0, -1 };
                    break;
                }
            }
            for (auto& a : fans)
            {
                if (pos == VECTOR2{ (float)a.x, (float)a.y })
                    temp = { 0, 0 };

            }
            int a{ Collision.getChip((pos + temp)* 54.0f) };
            if (a)
            {
                //int b{ Collision.getChip((pos + temp) * 54.0f) };
                temp = { 0, 0 };
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
            //if (Collision.getChip(pos * 54))
            //    continue;
            wind.push_back({ (int)pos.x, (int)pos.y, dir });
        }
    }
    WindM.FanCollision();
}
void map_init(std::string map_name)
{
    sprMain = GameLib::sprite_load(L"./Data/Images/OBJ_4.png");
    fans.clear();
    wind.clear();
    dist.clear();

    std::shared_ptr<GameLib::Sprite> share_sprFan(sprMain);


    /*Shutter.Init(share_sprFan, "./Data/Map/Map4/Map4Shutter.txt", SIZE);
    SwitchShutter.Init(share_sprFan, "./Data/Map/Map4/Map4SwitchShutter.txt", SIZE);
    SwitchFan.Init(share_sprFan, "./Data/Map/Map4/Map4SwitchFan.txt", SIZE);*/

    ShutterManage::GetInstance()->Init("./Data/Map/" + map_name + "/Shutter.txt");
    
    ShutterSwitch_Manage::GetInstance()->Init("./Data/Map/" + map_name + "/SwitchShutter.txt");
    FanSwitch_Manage::GetInstance()->Init("./Data/Map/" + map_name + "/SwitchFan.txt");


    Collision.Init(share_sprFan, "./Data/Map/" + map_name + "/Col.txt", SIZE);
    SpriteTexture.Init(share_sprFan, "./Data/Map/" + map_name + "/Tex.txt", SIZE);
    BG.Init(share_sprFan, "./Data/Map/" + map_name + "/BG.txt", SIZE);
    WindM.Init(share_sprFan, "./Data/Map/" + map_name + "/Fan.txt", SIZE);
    WindM.AlwaysOn = true;
    Fan1.Init(share_sprFan, "./Data/Map/" + map_name + "/Fan1.txt", SIZE);
    Fan1.StartOn = true;
    Fan2.Init(share_sprFan, "./Data/Map/" + map_name + "/Fan2.txt", SIZE);
    Fan2.StartOn = false;
    wind_init(&WindM);
    wind_init(&Fan1);
    wind_init(&Fan2);

    for (int alpha = 0; alpha < fans.size(); ++alpha)
        fans[alpha].spr = share_sprFan;
}
void map_update()
{
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        if (fans[alpha].y == 17)
            continue;
        bool Bottom{ false };
        for (auto& a : fans)
        {
            if (&a == &fans[alpha])
                continue;
            if (a.y == fans[alpha].y + 1 && a.x == fans[alpha].x)
            {
                Bottom = true;
                break;
            }
        }
        if (Bottom)
            continue;
        if (!Collision.getChip(fans[alpha].pos + VECTOR2(0, 54)))
            fans[alpha].pos.y += 5.0f;
    }
    WindM.Update();
    wind_update();
    //for (int alpha = 0; alpha < fans.size(); ++alpha)
    //{
    //    for (int beta = 0; beta < fans.size(); ++beta)
    //    {
    //        if (alpha == beta)
    //            continue;
    //        if (fans[alpha].x != fans[beta].x)
    //            continue;
    //        if (fans[alpha].y == fans[beta].y + 1 || fans[alpha].y == fans[beta].y - 1)
    //            fans[alpha].pos.x = fans[beta].pos.x;
    //    }
    //}
    if (pushedFan && pushedFan->dir != Fan::Direction::BLOCK)
    {
        for (auto& a : fans)
        {
            if (a.x == pushedFan->x && (a.y == pushedFan->y - 1 || a.y == pushedFan->y + 1))
            {
                a.pos.x = pushedFan->pos.x;
                break;
            }
        }
    } else {
        if (slip_fan.size() == 1 && slip_fan[0]->dir != Fan::Direction::BLOCK) {
            for (auto& a : fans)
            {
                if (abs(a.pos.x - slip_fan[0]->pos.x) <= 27 && abs(a.pos.y - slip_fan[0]->pos.y) <= 200)
                {
                    a.pos.x = slip_fan[0]->pos.x;
                    break;
                }
            }
        }
    }
    for (int alpha = 0; alpha < fans.size(); ++alpha)
    {
        fans[alpha].x = fans[alpha].pos.x / 54;
        fans[alpha].y = fans[alpha].pos.y / 54;
    }

    ShutterManage::GetInstance()->Active(ShutterSwitch_Manage::GetInstance()->isActive);
}
void map_render()
{
    //Collision.Draw();
    //WindM.Draw();
    ShutterManage::GetInstance()->Render();
    BG.Draw();
    SpriteTexture.Draw();


    for (auto& a : fans)
        a.Draw();
    /*Shutter.Draw();
    SwitchShutter.Draw();
    SwitchFan.Draw();*/

    //for (int alpha = 0; alpha < wind.size(); ++alpha)
    //    circle(
    //        VECTOR2((wind[alpha].x) * 54, (wind[alpha].y) * 54),
    //        5.0f,
    //        {1.0f, 1.0f},
    //        0.0f,
    //        { 1, 0, 0, 1 }
    //);
    //for (int alpha = 0; alpha < fans.size(); ++alpha)
    //{
    //    circle(VECTOR2(fans[alpha].x * 54.0f, fans[alpha].y * 54.0f), 5.0f, VECTOR2(1.0f, 1.0f), 0.0f, { 0, 1, 1, 1 });
    //}

    //wind.clear();


    ShutterSwitch_Manage::GetInstance()->Render();
    FanSwitch_Manage::GetInstance()->Render();
}


void map_deinit()
{
    Collision.Clear();
    WindM.Clear();
    Fan1.Clear();
    Fan2.Clear();
    BG.Clear();
    wind.clear();
    fans.clear();
    dist.clear();

}

bool HitGoal(Object* obj, Map* map)
{
    int right{ map->getChip(obj->pos + VECTOR2(54.0f, 0)) };
    int left{ map->getChip(obj->pos - VECTOR2(54.0f, 0)) };
    int down{ map->getChip(obj->pos + VECTOR2(0, 54.0f)) };
    int up{ map->getChip(obj->pos - VECTOR2(0, 54.0f)) };
    if (right == 3 || left == 3 || down == 3 || up == 3)
        return true;
    return false;

}
