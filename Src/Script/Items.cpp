//
//  Items.cpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Items.hpp"
#include "../GameLogic/Game_Obj.hpp"
Items::Items(){
    Items_name="";
    ownership="";
}
Weapon::Weapon(){
    Cooldown = .2;//1 second
    Items_name = "Weapon";
    LastshotTime = 0;
}
void Weapon::Update(UI_Event &uievent,void* ptr_in)
{
    //get handlers
    Game_Events *Gptr = (Game_Events*)Gevent_list;
    Game_Obj* Gobj_ptr = (Game_Obj*)ptr_in;
    vector<Game_Obj> *Gobj_list_ptr;
    Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
    if(Gobj_ptr->get_type()=="player")
        if(Gptr->check_event("Player_shoot")
           && Gptr->currentTime>LastshotTime+Cooldown*1000)
        {
            LastshotTime = Gptr->currentTime;
            cout<<Gobj_ptr->get_type()<<"shoot"<<endl;
            //create bullet obj
            Gptr->set_event("new_bullet", true);
            Gptr->new_bullet_list.push_back(&Gobj_ptr->Index);
            Gptr->set_event("Player_shoot",false);
        }
    else if(Gobj_ptr->get_type()=="static_monster")
    {
        
    }
}
Bullet::Bullet(){
    Game_Events *Gptr = (Game_Events*)Gevent_list;
    speed = .04;
    born_time = Gptr->currentTime;
    life = 2;
}
void Bullet::Update(UI_Event &uievent,void* ptr_in)
{
    Game_Events *Gptr;Gptr = (Game_Events*)Gevent_list;
    Game_Obj* Gobj_ptr;
    Gobj_ptr = (Game_Obj*)ptr_in;
    if(DieBeauseOld())
    {
        Gptr->set_event("Erase", true);
        Gptr->Erase_index.push_back(Gobj_ptr->Index);
        return;
    }    
    dir_vec= Gobj_ptr->get_Model()*glm::vec4{0,0,1,0};//direction equals to parent's face direction
    Gobj_ptr->translate(glm::normalize(glm::vec3{dir_vec})*speed);
   // cout<<"move"<<endl;
}
bool Bullet::DieBeauseOld(){
    Game_Events *Gptr = (Game_Events*)Gevent_list;
    if(Gptr->currentTime > born_time + 1000*life)
        return true;
    else return false;
}
