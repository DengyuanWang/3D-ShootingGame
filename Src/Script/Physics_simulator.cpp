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
    gravity=glm::vec4{0,-1,0,0};//save gravity affects the obj
    acceleration=glm::vec4(0,0,0,0);//save acceleration of obj
    enable_gravity = false;//true for enable
    Component_name = "Physics_simulator";
    Game_Obj* ptr;
    ptr = (Game_Obj*)ptr_in;
    V_stash = velocity;
    M_stash = ptr->get_Model();
}
void Physics_simulator::Update(UI_Event &UIEvent,void* ptr_in)
{
    Game_Events* Gptr =(Game_Events*)Gevent_list;//get game event handler
    Game_Obj* ptr;
    ptr = (Game_Obj*)ptr_in;
    float max_T =(Gptr->currentTime-Gptr->lastTime)/1000.0;

    for(int i=0;i<10;i++)
    {
        float dT =  max_T*i/10;
        V_stash = velocity;
        M_stash = ptr->get_Model();
        
        glm::vec4 movement = calcu_movement(dT);
        ptr->translate(glm::vec3{movement.x,movement.y,movement.z});
        
        vector<Game_Obj> *Gobj_list_ptr;
        Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
        glm::vec3 pos(ptr->get_Model()[3]);
        
 /*       if( ptr->get_type()=="bullet")
        {
            Last_movement = movement;
            continue;
        }*/
        
        string Value="";
        int x = pos.x/3,z=pos.z/3;
        ostringstream convert1,convert2;
        convert1 << x<<"-"<<z;
        string Key = convert1.str();
        unordered_map<string,string>::iterator got = Gptr->HashMap.find (Key);
        if ( got != Gptr->HashMap.end() )
        {
            Value = Gptr->HashMap[Key];
            for(int i=0;i<Value.length();i++)
            {
                int j = i;
                while(i<Value.length()&&Value[i]!='|'){i++;}
                string idx = Value.substr(j,i-j);
                int index = std::stoi(idx);
                if(index!=ptr->Index&&
                   (*Gobj_list_ptr)[index].check_collision(ptr)==true)
                {
                    
                    if(ptr->get_type()!="bullet")//only allow bullet to overlap with others
                    {
                        ptr->set_mat(M_stash);
                        ptr->translate(glm::vec3{-Last_movement.x,-Last_movement.y,-Last_movement.z});
                        velocity = glm::vec4{0,0,0,0};
                        return;
                    }
                    else{
                        //velocity = glm::vec4{0,0,0,0};
                        //return;
                    }
                }
            }
        }
        
        //check if collide with others
        if(ptr->collision_indices.size()>0||(ptr->get_type()!="bullet"&&pos.y<0.0f))//collide with others or touch floor
        {
            if(ptr->get_type()!="bullet")//only allow bullet to overlap with others
            {
                ptr->set_mat(M_stash);
                ptr->translate(glm::vec3{-Last_movement.x,-Last_movement.y,-Last_movement.z});
                velocity = glm::vec4{0,0,0,0};
                return;
            }
            else{
                //velocity = glm::vec4{0,0,0,0};
                return;
            }
        }
        Last_movement = movement;
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
