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
Weapon::Weapon(void *ptr_in){
    Cooldown = .2;//1 second
    Items_name = "Weapon";
    LastshotTime = 0;
    Component_name = "Weapon";
}
void Weapon::set_model(void *ptr_in)
{
    Game_Obj* Gobj_ptr = (Game_Obj*)ptr_in;
    glm::vec3 X,Y,Z,UP,pos;
    if(Gobj_ptr->get_type()=="player"){
        Player *P_ptr =(Player*)Gobj_ptr->get_component("Player");
        glm::vec4 view_world = Gobj_ptr->get_Model()* P_ptr->ViewAt_vector;
        Z =glm::normalize(glm::vec3{view_world.x,view_world.y,view_world.z});
    }else {
        Model = Gobj_ptr->get_Model();
        Z =glm::normalize(glm::vec3(Model[2]));
    }
    UP = glm::vec3{0,1,0};
    X = glm::cross(UP, Z);
    Y = glm::cross(Z, X);
    pos = glm::vec3(Gobj_ptr->get_Model()[3])+Z*1.0f;
    float aaa[16] = {
        X.x, X.y, X.z, 0,
        Y.x, Y.y, Y.z, 0,
        Z.x, Z.y, Z.z, 0,
        pos.x, pos.y, pos.z, 1
    };
    memcpy( glm::value_ptr( Model ), aaa, sizeof( aaa ) );
}
void Weapon::Update(UI_Event &uievent,void* ptr_in)
{
    set_model(ptr_in);
    //get handlers
    Game_Events *Gptr = (Game_Events*)Gevent_list;
    Game_Obj* Gobj_ptr = (Game_Obj*)ptr_in;
    vector<Game_Obj> *Gobj_list_ptr;
    Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
    if(Gobj_ptr->get_type()=="player"){
        if(Gptr->check_event("Player_shoot")
           && Gptr->currentTime>LastshotTime+Cooldown*1000)
        {
            LastshotTime = Gptr->currentTime;
            //create bullet obj
            Gptr->set_event("new_bullet", true);
            Gptr->new_bullet_list.push_back(&Gobj_ptr->Index);
            Gptr->set_event("Player_shoot",false);
        }
    }
    else if(Gobj_ptr->get_type()=="static_monster")
    {
        if(Gptr->currentTime>LastshotTime+Cooldown*1000)
        {
            LastshotTime = Gptr->currentTime;
            //create bullet obj
            Gptr->set_event("new_bullet", true);
            Gptr->new_bullet_list.push_back(&Gobj_ptr->Index);
        }
    }
    else{
        //not defined
    }
}
Bullet::Bullet(void *ptr_in){
    Game_Events *Gptr = (Game_Events*)Gevent_list;
    speed = 5;
    born_time = Gptr->currentTime;
    life = 2;
    size = glm::vec3{.1,.1,.2};
    Component_name = "Bullet";
    Game_Obj* ptr;
    ptr = (Game_Obj*)ptr_in;
    ptr->attach_component("physics_simulator");
    init_phy = false;
}
void Bullet::Update(UI_Event &uievent,void* ptr_in)
{
    vector<Game_Obj> *Gobj_list_ptr;
    Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
    Game_Events *Gptr;Gptr = (Game_Events*)Gevent_list;
    Game_Obj* Gobj_ptr;
    Gobj_ptr = (Game_Obj*)ptr_in;
    if(DieBeauseOld())
    {
        Gptr->set_event("Erase", true);
        Gptr->Erase_index.push_back(Gobj_ptr->Index);
        return;
    }
    //check if need to destory when collide with others
    for(int i=0;i<Gobj_list_ptr->size();i++)
    {
        //cout<<"monster:"<<ptr->Index<<"others"<<(*Gobj_list_ptr)[i].Index<<endl;
        if(((*Gobj_list_ptr)[i].Index)!=Gobj_ptr->Index//not self
           &&(*Gobj_list_ptr)[i].check_collision(Gobj_ptr))//collide with others
        {
            string collide_name = (*Gobj_list_ptr)[i].get_type();
            bool erase_tag = false;
            if((*Gobj_list_ptr)[i].get_type()!="bullet")//collide with bullet
            {
                if(ownership=="player")
                {
                    if(collide_name!="player")
                        erase_tag=true;
                }else{
                    if(collide_name!="static_monster"&&collide_name!="moving_monster")
                        erase_tag=true;
                }
                if(erase_tag)//not ally's bullet
                {
                    Gptr->set_event("Erase", true);
                    Gptr->Erase_index.push_back(Gobj_ptr->Index);
                    return;
                }
            }
        }
    }
    
    
    
    if(init_phy==false)
    {
        Physics_simulator *Pptr = (Physics_simulator*)Gobj_ptr->get_component("Physics_simulator");
        dir_vec= Gobj_ptr->get_Model()*glm::vec4{0,0,1,0};//direction equals to parent's face direction
        glm::vec3 V0= glm::normalize(glm::vec3{dir_vec})*speed;
        Pptr->acceleration = glm::vec4{V0.x,V0.y,V0.z,0};
    }
   // dir_vec= Gobj_ptr->get_Model()*glm::vec4{0,0,1,0};//direction equals to parent's face direction
   // Gobj_ptr->translate(glm::normalize(glm::vec3{dir_vec})*speed);
   // cout<<"move"<<endl;    
}
bool Bullet::DieBeauseOld(){
    Game_Events *Gptr = (Game_Events*)Gevent_list;
    if(Gptr->currentTime > born_time + 1000*life)
        return true;
    else return false;
}
