//
//  Game_Events.h
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Game_Events_hpp
#define Game_Events_hpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "../UI/UI_Event.hpp"
using namespace std;
class Game_Events{
public:
    vector<int> Erase_index;
    vector<int*> new_bullet_list;//save parent index
    static vector<string> Game_Event_list;
    vector<bool> Events;
    unordered_map<string, string>HashMap;
    Game_Events();
    void set_event(string eventname,bool value);
    bool check_event(string eventname);
    unsigned int currentTime,lastTime;
    void Update();
    void Update_Collision();
private:
    int find_event(string name);
};
#endif /* Game_Events_h */
