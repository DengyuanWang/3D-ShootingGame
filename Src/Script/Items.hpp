//
//  Items.hpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Items_hpp
#define Items_hpp

#include <iostream>
#include <vector>
#include "Component.hpp"
using namespace std;
class Items: public Component{
public:
    string Items_name;
    Items();
    virtual void Update(UI_Event &UIEvent,void* ptr_in){
        cout<<"items"<<endl;};
};
class Final_gate: public Items{
    Final_gate();
    void Update(UI_Event &UIEvent,void* ptr_in);
};
class Weapon: public Items{//weapon system
private:
    float Cooldown;//second
    unsigned int LastshotTime;
public:
    Weapon();
    void Update(UI_Event &UIEvent,void* ptr_in);
};
class Bullet: public Items{
private:
    float speed;//fly speed
    glm::vec4 dir_vec;
    unsigned int born_time;
    float life;//exist time limit;
public:
    Bullet();
    void Update(UI_Event &UIEvent,void* ptr_in);
    bool DieBeauseOld();
};
#endif /* Items_hpp */
