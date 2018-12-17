//
//  Monster.cpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Monster.hpp"
#include "../GameLogic/Game_Obj.hpp"
Monster::Monster(){
    Death_tag = false;//default as false
    Health = 1;//0~100
    Speed = 0;//default as 1
    Component_name = "Monster";
    //vector<string> Abilities;//default as "walk"
}
Static_M::Static_M(void *ptr_in){
    Component_name = "Static_monster";
    Game_Obj* ptr =(Game_Obj*)ptr_in;
    ptr->attach_component("weapon");
}
void Monster::Update(UI_Event &UIEvent, void *ptr_in)
{
    Game_Events* Gptr =(Game_Events*)Gevent_list;//get game event handler
    Game_Obj* ptr =(Game_Obj*)ptr_in;
    vector<Game_Obj> *Gobj_list_ptr;
    Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
    for(int k=0;k<ptr->collision_indices.size();k++)
    {
        int i =ptr->collision_indices[k];
        string collide_name = (*Gobj_list_ptr)[i].get_type();
        if((*Gobj_list_ptr)[i].get_type()=="bullet")//collide with bullet
        {
            Bullet *tmpptr = (Bullet*)(*Gobj_list_ptr)[i].get_component("Bullet");
            string b=tmpptr->ownership;
            if(b=="player")//not self bullet
            {
                Health--;
                if(Health<=0){
                    Gptr->set_event("Erase", true);
                    Gptr->Erase_index.push_back(ptr->Index);//kill self
                    return;
                }
            }
        }
    }
}
void Static_M::Update(UI_Event &UIEvent, void *ptr_in){
    Monster::Update(UIEvent, ptr_in);
    return;
}

Moving_M::Moving_M(void *ptr_in){
    Component_name = "Moving_monster";
    Game_Obj* ptr=(Game_Obj*)ptr_in;
    ptr->attach_component("physics_simulator");
    ((Physics_simulator*)ptr->get_component("Physics_simulator"))->enable_gravity = true;
}
void Moving_M::Update(UI_Event &UIEvent, void *ptr_in)
{
    Monster::Update(UIEvent, ptr_in);
    int t = rand()%7;
    Speed = 0.1f;
    Game_Obj* Gobj_ptr;
    Gobj_ptr = (Game_Obj*)ptr_in;
    Physics_simulator *Pptr = (Physics_simulator*)Gobj_ptr->get_component("Physics_simulator");
    glm::vec4 dir_vec;
    glm::vec3 V0;
    float x=Pptr->velocity.x,y=Pptr->velocity.y,z=Pptr->velocity.z;
    if(sqrt(x*x+y*y+z*z)>0)
        return;
    x=rand()%100-50;y=rand()%100-50;z=rand()%100-50;
    Pptr->velocity = glm::vec4{x,y,z,0}/100.0f;
    return;
    switch (t) {
        case 0://forward
            dir_vec= Gobj_ptr->get_Model()*glm::vec4{0,0,1,0};//direction equals to parent's face direction
            V0= glm::normalize(glm::vec3{dir_vec})*Speed;
            Pptr->velocity = glm::vec4{V0.x,V0.y,V0.z,0};
            break;
        case 1://backward
            dir_vec= Gobj_ptr->get_Model()*glm::vec4{0,0,-1,0};//direction equals to parent's face direction
            V0= glm::normalize(glm::vec3{dir_vec})*Speed;
            Pptr->velocity = glm::vec4{V0.x,V0.y,V0.z,0};
            break;
        case 2://left
            dir_vec= Gobj_ptr->get_Model()*glm::vec4{1,0,0,0};//direction equals to parent's face direction
            V0= glm::normalize(glm::vec3{dir_vec})*Speed;
            Pptr->velocity = glm::vec4{V0.x,V0.y,V0.z,0};
            break;
        case 3://right
            dir_vec= Gobj_ptr->get_Model()*glm::vec4{-1,0,0,0};//direction equals to parent's face direction
            V0= glm::normalize(glm::vec3{dir_vec})*Speed;
            Pptr->velocity = glm::vec4{V0.x,V0.y,V0.z,0};
            break;
        case 4://up
            dir_vec= Gobj_ptr->get_Model()*glm::vec4{0,1,0,0};//direction equals to parent's face direction
            V0= glm::normalize(glm::vec3{dir_vec})*Speed;
            Pptr->velocity = glm::vec4{V0.x,V0.y,V0.z,0};
            break;
        default://none
            //Pptr->velocity = glm::vec4{0,0,0,0};
            break;
    }
}
