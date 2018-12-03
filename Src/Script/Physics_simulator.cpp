//
//  Physics_simulator.cpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 12/3/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Physics_simulator.hpp"
Physics_simulator::Physics_simulator(void* ptr_in){
    glm::vec4 velocity=glm::vec4(0,0,0,1);//save current velocity of obj
    glm::vec4 gravity=glm::vec4{0,-1,0,1};//save gravity affects the obj
    glm::vec4 acceleration=glm::vec4(0,0,0,1);//save acceleration of obj
    enable_gravity = false;//true for enable
    Component_name = "Physics_simulator";
}
void Physics_simulator::Update(UI_Event &UIEvent,void* ptr_in)
{
    
}
