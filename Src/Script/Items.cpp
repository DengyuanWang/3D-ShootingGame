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
}
Weapon::Weapon(){
    Cooldown = 1;//1 second
    Items_name = "Weapon";
    LastshotTime = 0;
}
void Weapon::Update(UI_Event &uievent)
{
    //get handlers
    Game_Events *Gptr = (Game_Events*)Gevent_list;
    Game_Obj* Gobj_ptr = (Game_Obj*)parent;
    vector<Game_Obj> *Gobj_list_ptr;
    Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
    if(uievent.check_event("Shoot")
       && Gptr->currentTime>LastshotTime+Cooldown*1000)
    {
        LastshotTime = Gptr->currentTime;
        cout<<"shoot"<<endl;
        //create bullet obj
        (*Gobj_list_ptr).push_back(Game_Obj());
        (*Gobj_list_ptr)[Gobj_list_ptr->size()-1].Index = (*Gobj_list_ptr)[Gobj_list_ptr->size()-2].Index+1;
        (*Gobj_list_ptr)[Gobj_list_ptr->size()-1].Specify_type("bullet");
        (*Gobj_list_ptr)[Gobj_list_ptr->size()-1].Model_name = "sphere";
        (*Gobj_list_ptr)[Gobj_list_ptr->size()-1].Texture_name = "color";
        (*Gobj_list_ptr)[Gobj_list_ptr->size()-1].scale(glm::vec3{0.1,0.1,0.2});
        (*Gobj_list_ptr)[Gobj_list_ptr->size()-1].set_mat(Gobj_ptr->get_Model());
        (*Gobj_list_ptr)[Gobj_list_ptr->size()-1].collider_size = glm::vec3{0.1f,0.1f,0.2f};
        (*Gobj_list_ptr)[Gobj_list_ptr->size()-1].collider_offset = glm::vec4{0,0,0,1};
    }
}
Bullet::Bullet(){
    Game_Events *Gptr = (Game_Events*)Gevent_list;
    speed = .2;
    born_time = Gptr->currentTime;
    life = 2;
}
void Bullet::Update(UI_Event &uievent)
{
    Game_Events *Gptr;Gptr = (Game_Events*)Gevent_list;
    Game_Obj* Gobj_ptr;
    Gobj_ptr = (Game_Obj*)parent;
    if(DieBeauseOld())
    {
        Gptr->set_event("Erase", true);
        Gptr->Erase_index = Gobj_ptr->Index;
        return;
    }
    dir_vec= Gobj_ptr->get_Model()*glm::vec4{0,0,1,0};//direction equals to parent's face direction
    glm::vec3 t1,t2;
    t1 = glm::vec3(Gobj_ptr->get_Model()[3]);
    Gobj_ptr->translate(glm::normalize(glm::vec3{dir_vec})*speed);
    t2 = glm::vec3(Gobj_ptr->get_Model()[3]);
    cout<<"move"<<endl;
}
bool Bullet::DieBeauseOld(){
    Game_Events *Gptr = (Game_Events*)Gevent_list;
    if(Gptr->currentTime > born_time + 1000*life)
        return true;
    else return false;
}
