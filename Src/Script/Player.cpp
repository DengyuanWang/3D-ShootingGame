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
void Player::Update(UI_Event &UIEvent, Game_Events &G_event,void* parent)
{
    update_view(UIEvent,G_event, parent);
}
void Player::update_view(UI_Event &UIEvent, Game_Events &G_event,void* parent)
{
    Game_Obj* ptr;
    ptr = (Game_Obj*)parent;
    //Player controller:
    //move player
    float step = 0.1;glm::vec3 move_vec{0,0,0};
    if(UIEvent.check_event("Up"))
        move_vec +=glm::vec3{0,0,1};
    if(UIEvent.check_event("Down"))
        move_vec +=glm::vec3{0,0,-1};
    if(UIEvent.check_event("Left"))
        move_vec +=glm::vec3{1,0,0};
    if(UIEvent.check_event("Right"))
        move_vec +=glm::vec3{-1,0,0};
    if(abs(move_vec.x)+abs(move_vec.y)+abs(move_vec.z)>0.01)
        ptr->local_translate(glm::normalize(move_vec)*step);
    //change view angle
    float xy[2] ={-UIEvent.mouse_status[0]/500.0f,UIEvent.mouse_status[1]/500.0f};
    ptr->local_rotation(glm::vec3(0,xy[0],0));
    ViewAt_vector = glm::rotateX(ViewAt_vector,xy[1]);//rotate in model's coordinates
}
