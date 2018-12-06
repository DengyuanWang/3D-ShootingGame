//
//  Game_Obj.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/15/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Game_Obj.hpp"
//all possible input model
vector<string> Game_Obj::Type_list=
            { "floor","gate","wall","door","key","player","monster",
                //new items in shooting game
              "teleporter", "elements","weapon","treasureBox","destructibleWall","bullet",
                //new monsters
              "static_monster","moving_monster"
            };
Game_Obj::Game_Obj()
{
    
}
bool Game_Obj::Specify_type(string name)
{
    vector<string>::iterator index = std::find(Type_list.begin(), Type_list.end(), name);
    if (index != Type_list.end())
    {
        Type_index = (int)std::distance( Type_list.begin(), index );;//event none
        Model_name = Type_list[Type_index];
        attach_component(name);
        //if(name=="player"||name=="static_monster")
        //{//add one more component for player and static_monster
        //    attach_component("weapon");
        //}
        
        return true;
    }else return false;
}
void Game_Obj::translate(glm::vec3 T_vec)//translate in world coordinate
{
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), T_vec);
    Model =  trans *Model;
}
void Game_Obj::local_translate(glm::vec3 T_vec)//translate in world coordinate
{
    glm::mat4 stash_model = Model;
    glm::vec4 x_local{1.0f,0.0f,0.0f,0},y_local{0.0f,1.0f,0.0f,0},z_local{0.0f,0.0f,1.0f,0};
    glm::vec4 x_world,y_world,z_world;
    x_world = Model*x_local;y_world = Model*y_local;z_world = Model*z_local;
    glm::vec4 vec = T_vec.x*x_world+T_vec.y*y_world+T_vec.z*z_world;
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(vec.x,vec.y,vec.z));
    Model =  trans *Model;
}
void Game_Obj::scale(glm::vec3 S_vec)
{
    glm::mat4 modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, S_vec);
    Model = Model*modelMatrix;
}
void Game_Obj::local_rotation(glm::vec3 angles)//rotate angles for each axises in local coordinates
{
    glm::vec4 x_local{1.0f,0.0f,0.0f,0},y_local{0.0f,1.0f,0.0f,0},z_local{0.0f,0.0f,1.0f,0};
    glm::vec4 x_world,y_world,z_world;
    x_world = Model*x_local;y_world = Model*y_local;z_world = Model*z_local;
    
    Model = glm::rotate(Model, angles.x, glm::vec3(x_world.x,x_world.y,x_world.z));
    Model = glm::rotate(Model, angles.y, glm::vec3(y_world.x,y_world.y,y_world.z));
    Model = glm::rotate(Model, angles.z, glm::vec3(z_world.z,z_world.y,z_world.z));
}

bool Game_Obj::attach_component(string component_name)
{
    if(component_name=="player")
    {
        Comp_list.push_back(new Player(this));
        cout<<"ptr:"<<this<<endl;
    }
    if(component_name=="weapon"){
        Comp_list.push_back(new Weapon(this));
    }
    if(component_name=="bullet"){
        Comp_list.push_back(new Bullet(this));
    }
    if(component_name=="static_monster")
    {
        Comp_list.push_back(new Static_M(this));
    }
    if(component_name=="physics_simulator")
    {
        Comp_list.push_back(new Physics_simulator(this));
    }
    return true;
}
Component* Game_Obj::get_component(string component_name)
{
    for(int i=0;i<Comp_list.size();i++)
    {
        Component* cpt;
        cpt=Comp_list[i];
        if(cpt->Component_name==component_name)
            return cpt;
    }
    return NULL;
}
void Game_Obj::Update(UI_Event &input_event)
{
    //cout<<Type_list[Type_index]<<endl;
    collider_center = Model*collider_offset;
    Game_Events G_events;
    for(int i=0;i<Comp_list.size();i++)
    {
        Component* cpt;
        cpt=Comp_list[i];
        cpt->Update(input_event,this);
    }
}
bool Game_Obj::check_collision(Game_Obj *ptr)//check collision with other Game obj
{
    glm::mat4 M4 = ptr->get_Model();//get model;
    glm::vec4 center= ptr->collider_offset;
    glm::vec3 size = ptr->collider_size;
    vector<glm::vec4> points;
    points.push_back(center+glm::vec4(size.x,size.y,size.z,0));//+++
    points.push_back(center+glm::vec4(size.x,size.y,-size.z,0));//++-
    points.push_back(center+glm::vec4(size.x,-size.y,size.z,0));//+-+
    points.push_back(center+glm::vec4(size.x,-size.y,-size.z,0));//+--
    points.push_back(center+glm::vec4(-size.x,size.y,size.z,0));//-++
    points.push_back(center+glm::vec4(-size.x,size.y,-size.z,0));//-+-
    points.push_back(center+glm::vec4(-size.x,-size.y,size.z,0));//--+
    points.push_back(center+glm::vec4(-size.x,-size.y,-size.z,0));//---
    for(int i=0;i<points.size();i++)
    {

        //convert to world coordinates
        points[i] = M4*points[i];
        //convert to current obj's local coordinates
        points[i] = glm::inverse(Model)*points[i];
        //check if collide
        points[i]  = points[i] - collider_offset;
        
        if(pow(points[i].x,2)+pow(points[i].y,2)+pow(points[i].z,2)<.5)
        {
            
        }
        if(points[i].x>-collider_size.x&&points[i].x<collider_size.x&&
           points[i].y>-collider_size.y&&points[i].y<collider_size.y&&
           points[i].z>-collider_size.z&&points[i].z<collider_size.z)
            return true;
    }
    
    return false;
}
Game_Obj::~Game_Obj()
{

}
