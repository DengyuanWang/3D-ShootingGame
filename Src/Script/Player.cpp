//
//  Player.cpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Player.hpp"
#include "../GameLogic/Game_Obj.hpp"
Player::Player(){
    Death_tag = false;//default as false
    Health = 1;//0~100
    Speed = 1;//default as 1
    Abilities.push_back("Walk");//default as "walk"
    ViewAt_vector = glm::vec4(0,0,0.2f,0);
    eye_pos_offset = glm::vec4(0.01f,0.02f,-0.25f,0);
    Component_name = "Player";
}
void Player::Update(UI_Event &UIEvent)
{
    update_view(UIEvent);
}
void Player::update_view(UI_Event &UIEvent)
{
    Game_Events* Gptr =(Game_Events*)Gevent_list;//get game event handler
    Game_Obj* ptr;
    ptr = (Game_Obj*)parent;
    //Player controller:
    //move player
    float step = 0.05;glm::vec3 move_vec{0,0,0};
    if(UIEvent.check_event("Up"))
        move_vec +=glm::vec3{0,0,1};
    if(UIEvent.check_event("Down"))
        move_vec +=glm::vec3{0,0,-1};
    if(UIEvent.check_event("Left"))
        move_vec +=glm::vec3{1,0,0};
    if(UIEvent.check_event("Right"))
        move_vec +=glm::vec3{-1,0,0};
    if(abs(move_vec.x)+abs(move_vec.y)+abs(move_vec.z)>0.01)
    {
        ptr->local_translate(glm::normalize(move_vec)*step);
        //check collision
        vector<Game_Obj> *Gobj_list_ptr;
        Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
        for(int i=0;i<Gobj_list_ptr->size();i++)
        {
            if(((*Gobj_list_ptr)[i].Index)!=ptr->Index//not self
               &&check_collision(ptr->collider_center,(*Gobj_list_ptr)[i].collider_center,ptr->collider_size,(*Gobj_list_ptr)[i].collider_size))//collide with others
            {
                if((*Gobj_list_ptr)[i].get_type()=="gate")
                {
                    
                    Gptr->set_event("ReatchGate", true);
                    cout<<"you win"<<endl;
                }
                ptr->local_translate(-glm::normalize(move_vec)*step);
            }
        }
    }
    //change view angle
    float xy[2] ={-UIEvent.mouse_status[0]/500.0f,UIEvent.mouse_status[1]/500.0f};
    ptr->local_rotation(glm::vec3(0,xy[0],0));
    ViewAt_vector = glm::rotateX(ViewAt_vector,xy[1]);//rotate in model's coordinates

    
    
    
    
    
}
bool Player::check_collision(glm::vec4 pos1,glm::vec4 pos2, glm::vec3 size1,glm::vec3 size2){
    float a_minX=pos1.x-size1.x,
    a_minY=pos1.y-size1.y,
    a_minZ=pos1.z-size1.z,
    a_maxX=pos1.x+size1.x,
    a_maxY=pos1.y+size1.y,
    a_maxZ=pos1.z+size1.z;
    float b_minX=pos2.x-size2.x,
    b_minY=pos2.y-size2.y,
    b_minZ=pos2.z-size2.z,
    b_maxX=pos2.x+size2.x,
    b_maxY=pos2.y+size2.y,
    b_maxZ=pos2.z+size2.z;
    if ( (a_minX <= b_maxX && a_maxX >= b_minX) &&
        (a_minY <= b_maxY && a_maxY >= b_minY) &&
        (a_minZ <= b_maxZ && a_maxZ >= b_minZ))
        return true;
    else return false;
    return false;
}
