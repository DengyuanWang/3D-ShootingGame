//
//  Monster.hpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Monster_hpp
#define Monster_hpp

#include <iostream>
#include <vector>
#include "Component.hpp"
using namespace std;
class Monster: public Component{
public:
    bool Death_tag;//default as false
    float Health;//0~100
    float Speed;//default as 1
    vector<string> Abilities;//default as "walk"
    Monster();
    void Update(Game_Events input_event);
};
class Static_M: public Monster{
    Static_M();
    void Update(Game_Events input_event);
};
class Moving_M: public Monster{
    Moving_M();
    void Update(Game_Events input_event);
};
#endif /* Monster_hpp */
