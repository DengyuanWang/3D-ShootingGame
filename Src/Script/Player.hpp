//
//  Player.hpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <vector>
#include "Game_Events.h"
using namespace std;
class Player{
public:
    bool Death_tag;//default as false
    float Health;//0~100
    float Speed;//default as 1
    vector<string> Abilities;//default as "walk"
    Player();
    void Update(Game_Events input_event);
};
#endif /* Player_hpp */
