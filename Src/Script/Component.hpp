//
//  Component.hpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/29/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Component_hpp
#define Component_hpp

#include <iostream>
#include <vector>
#include "Game_Events.hpp"
#include "../../lib/Includer.h"
using namespace std;
class Component{
private:
    
public:
    string Component_name;
    Component();
    virtual void Update(UI_Event &UIEvent,void* ptr_in){
        cout<<"child"<<endl;
    }
    virtual void test(){
        cout<<"test virtual"<<endl;
    }
    virtual bool check_collision(glm::vec4 pos1,glm::vec4 pos2, glm::vec3 size1,glm::vec3 size2){
        return false;
    }
};
#endif /* Component_hpp */
