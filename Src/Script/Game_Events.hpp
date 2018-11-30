//
//  Game_Events.h
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Game_Events_hpp
#define Game_Events_hpp
#include <iostream>
#include <vector>
#include "../UI/UI_Event.hpp"
using namespace std;
class Game_Events{
public:
    static vector<string> Game_Event_list;
    int current_event;
    string eventname;
};
#endif /* Game_Events_h */
