//
//  Physics_simulator.cpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 12/3/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Physics_simulator.hpp"
#include "../GameLogic/Game_Obj.hpp"
Physics_simulator::Physics_simulator(void* ptr_in){
    velocity=glm::vec4(0,0,0,0);//save current velocity of obj
    gravity=glm::vec4{0,-9.8,0,0};//save gravity affects the obj
    acceleration=glm::vec4(0,0,0,0);//save acceleration of obj
    enable_gravity = false;//true for enable
    Component_name = "Physics_simulator";
    Game_Obj* ptr;
    ptr = (Game_Obj*)ptr_in;

}
void Physics_simulator::Update(UI_Event &UIEvent,void* ptr_in)
{
    //F = M*A
    //dV = dT*A
    //dX = dT*(1/2*dV + V0)
    //dH = 1/2 * gravity * dT^2
    //Vnew = V+dV+ gravity*dT
    Game_Events* Gptr =(Game_Events*)Gevent_list;//get game event handler
    Game_Obj* ptr;
    ptr = (Game_Obj*)ptr_in;
    float dT = (Gptr->currentTime-Gptr->lastTime)/1000.0;
    glm::vec4 dV = acceleration*dT;
    glm::vec4 dX = (dV*0.5f+velocity)*dT;
    glm::vec4 dH{0,0,0,0};
    if(enable_gravity){
        dH = gravity*dT*dT*0.5f;
        velocity = velocity+dV+gravity*dT;
    }else{
        velocity = velocity+dV;
    }
    glm::vec4 movement = dX+dH;
    ptr->translate(glm::vec3{movement.x,movement.y,movement.z});
}
