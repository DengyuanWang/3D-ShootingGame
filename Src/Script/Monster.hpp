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
    virtual void Update(UI_Event &UIEvent,void* ptr_in){
        cout<<"Monsters"<<endl;};
};
class Static_M: public Monster{
public:
    Static_M(void *ptr_in);
    void Update(UI_Event &UIEvent, void *ptr_in);
};
class Moving_M: public Monster{
public:
    Moving_M(void *ptr_in);
    void Update(UI_Event &UIEvent, void *ptr_in);
};
#endif /* Monster_hpp */
