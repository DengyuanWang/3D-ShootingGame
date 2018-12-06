//
//  Player.cpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Player.hpp"
#include "../GameLogic/Game_Obj.hpp"
Player::Player(void *ptr_in){
    Death_tag = false;//default as false
    Health = 1;//0~100
    Speed = 10;//default as 1
    Abilities.push_back("Walk");//default as "walk"
    ViewAt_vector = glm::vec4(0,0,0.2f,0);
    eye_pos_offset = glm::vec4(0.01f,0.02f,-0.25f,0);
    Component_name = "Player";
    Game_Obj* ptr;
    ptr = (Game_Obj*)ptr_in;
    ptr->attach_component("weapon");
    ptr->attach_component("physics_simulator");
    ((Physics_simulator*)ptr->get_component("Physics_simulator"))->enable_gravity = true;
}
void Player::Update(UI_Event &UIEvent,void* ptr_in)
{
    update_view(UIEvent,ptr_in);
}
void Player::update_view(UI_Event &UIEvent,void* ptr_in)
{
    Game_Events* Gptr =(Game_Events*)Gevent_list;//get game event handler
    Game_Obj* ptr;
    ptr = (Game_Obj*)ptr_in;
    vector<void*> collision_list;
    collision_list = ((Physics_simulator*)ptr->get_component("Physics_simulator"))->collision_list;
    
    for(int i=0;i<collision_list.size();i++)
    {
        Game_Obj* ptr2 = (Game_Obj*)collision_list[i];
        if(ptr2->get_type()=="floor") continue;
        if(ptr2->get_type()=="gate")
        {
            Gptr->set_event("ReatchGate", true);
            cout<<"you win"<<endl;
        }
        if(ptr2->get_type()=="bullet")
        {
            cout<<"detected hit"<<endl;
            Bullet *bptr = (Bullet*)ptr2->get_component("Bullet");
            if(bptr->ownership!="player")
            {
                Health--;
                if(Health<=0)
                    Gptr->set_event("Player_dead", true);
            }
        }
    }
    //Player controller:
    //jump
    
    if(UIEvent.check_event("Jump")){
        ptr->attach_component("physics_simulator");
        ((Physics_simulator*)ptr->get_component("Physics_simulator"))->velocity = glm::vec4{0,1,0,0};
        UIEvent.reset_event("Jump");
    }
    //move player
    float step = Speed*(Gptr->currentTime-Gptr->lastTime)/1000.0;
    glm::vec3 move_vec{0,0,0};
    if(UIEvent.check_event("Up")){
        move_vec +=glm::vec3{0,0,1};
        UIEvent.reset_event("Up");
    }
    if(UIEvent.check_event("Down")){
        move_vec +=glm::vec3{0,0,-1};
        UIEvent.reset_event("Down");
    }
    if(UIEvent.check_event("Left")){
        move_vec +=glm::vec3{1,0,0};
        UIEvent.reset_event("Left");
    }
    if(UIEvent.check_event("Right")){
        move_vec +=glm::vec3{-1,0,0};
        UIEvent.reset_event("Right");
    }
    if(abs(move_vec.x)+abs(move_vec.y)+abs(move_vec.z)>0.01)
    {
        bool move_allow = true;
        glm::mat4 tmp_mat = ptr->get_Model();
        ptr->local_translate(glm::normalize(move_vec)*step);
        //check collision
        vector<Game_Obj> *Gobj_list_ptr;
        Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
        for(int i=0;i<Gobj_list_ptr->size();i++)
        {
            //check_collision(ptr->collider_center,(*Gobj_list_ptr)[i].collider_center,
            //ptr->collider_size,(*Gobj_list_ptr)[i].collider_size)
            if(((*Gobj_list_ptr)[i].Index)!=ptr->Index//not self
               &&(*Gobj_list_ptr)[i].check_collision(ptr))//collide with others
            {
                if((*Gobj_list_ptr)[i].get_type()=="gate")
                {
                    
                    Gptr->set_event("ReatchGate", true);
                    cout<<"you win"<<endl;
                }
                if((*Gobj_list_ptr)[i].get_type()=="bullet")
                {
                    Bullet *bptr = (Bullet*)(*Gobj_list_ptr)[i].get_component("Bullet");
                    if(bptr->ownership!="player")
                    {
                        Health--;
                        if(Health<=0)
                            Gptr->set_event("Player_dead", true);
                    }
                }
                move_allow = false;
            }
        }
        if(move_allow != true)
            ptr->set_mat(tmp_mat);
    }
    //change view angle
    float xy[2] ={-UIEvent.mouse_status[0]/500.0f,UIEvent.mouse_status[1]/500.0f};
    ptr->local_rotation(glm::vec3(0,xy[0],0));
    ViewAt_vector = glm::rotateX(ViewAt_vector,xy[1]);//rotate in model's coordinates
    glm::vec3 D = glm::normalize(glm::vec3(ViewAt_vector.x,ViewAt_vector.y,ViewAt_vector.z));
    angle = -2*glm::acos(glm::dot(D, glm::vec3(0,0,1)));
    if(D.y<0) angle=-angle;
    if(D.y==0) angle = 0;
    /*
     1. the camera pos is at a const offset: (0.01f,0.05f,-0.1f) of the player's model
     2. the view direction is a vector in yz plane of player's local coordinate:
     notice that the up vector is always constant: [0 1 0];
     */
    glm::mat4 tmpmat = ptr->get_Model();
    //get pose, camera is a little higher than player
    float len =sqrt(pow(tmpmat[0].x,2)+pow(tmpmat[0].y,2)+pow(tmpmat[0].z,2));
    //cout<<"x"<<tmpmat[3].x<<"y"<<tmpmat[3].y<<"z"<<tmpmat[3].z<<endl;
    glm::vec4 offset = tmpmat*eye_pos_offset/len;
    glm::vec3 eye_pos = glm::vec3(tmpmat[3])+glm::vec3(offset.x,offset.y,offset.z);
    //get viewat_vector, whose length is 0.2f;
    glm::vec4 View_vec = tmpmat*ViewAt_vector;
    //calculate view_matrix;
    view_matrix = glm::lookAt(eye_pos-glm::vec3(View_vec.x,View_vec.y,View_vec.z), eye_pos, glm::vec3{0,1,0});
    if(UIEvent.check_event("Shoot"))
    {
        Gptr->set_event("Player_shoot", true);
        UIEvent.reset_event("Shoot");
    }
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
