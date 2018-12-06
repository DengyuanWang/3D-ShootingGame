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
    gravity=glm::vec4{0,-2,0,0};//save gravity affects the obj
    acceleration=glm::vec4(0,0,0,0);//save acceleration of obj
    enable_gravity = false;//true for enable
    Component_name = "Physics_simulator";
    Game_Obj* ptr;
    ptr = (Game_Obj*)ptr_in;
    collision_list = vector<void*>{};
}
void Physics_simulator::Update(UI_Event &UIEvent,void* ptr_in)
{
    //reset collisionlist
    collision_list = vector<void*>{};
    
    Game_Events* Gptr =(Game_Events*)Gevent_list;//get game event handler
    Game_Obj* ptr;
    ptr = (Game_Obj*)ptr_in;
    float max_T =(Gptr->currentTime-Gptr->lastTime)/1000.0;
    int loop_num=5;
    for(int i=1;i<loop_num;i++)
    {
        float dT =  max_T*(i/float(loop_num));
        glm::vec4 V_stash = velocity;
        glm::mat4 M_stash = ptr->get_Model();
        
        glm::vec4 movement = calcu_movement(dT);
        ptr->translate(glm::vec3{movement.x,movement.y,movement.z});
        
        vector<Game_Obj> *Gobj_list_ptr;
        Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
        //check if collide with others
        
        for(int i=0;i<Gobj_list_ptr->size();i++)
        {
            if(((*Gobj_list_ptr)[i].Index)!=ptr->Index//not self
               &&(*Gobj_list_ptr)[i].check_collision(ptr))//collide with others
            {
                collision_list.push_back(&(*Gobj_list_ptr)[i]);
                if(ptr->get_type()!="bullet"&&(*Gobj_list_ptr)[i].get_type()!="bullet")//only allow bullet to overlap with others
                {
                    ptr->set_mat(M_stash);
                    velocity = glm::vec4{0,0,0,0};
                    return;
                }else{
                    velocity = glm::vec4{0,0,0,0};
                    return;
                }
            }
        }
    }
}
glm::vec4 Physics_simulator::calcu_movement(float dT)
{
    //F = M*A
    //dV = dT*A
    //dX = dT*(1/2*dV + V0)
    //dH = 1/2 * gravity * dT^2
    //Vnew = V+dV+ gravity*dT
    glm::vec4 dV = acceleration*dT;
    glm::vec4 dX = (dV*0.5f+velocity)*dT;
    glm::vec4 dH{0,0,0,0};
    if(enable_gravity){
        dH = gravity*dT*dT*0.5f;
        velocity = velocity+dV+gravity*dT;
    }else{
        velocity = velocity+dV;
    }
    return dX+dH;
}
