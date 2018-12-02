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
              "Static_monster","Moving_monster"
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
        if(name=="player")
        {//add one more component for player
            attach_component("weapon");
        }
        
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
/*
//check collision
    glm::vec3 pos((glm::vec3(Model[3])));
    vector<Game_Obj> *ptr;
    ptr =(vector<Game_Obj> *)Gobj_list;
    for(int i=0;i<ptr->size();i++)
    {
        if(((*ptr)[i].Index)!=this->Index//not self
           &&(*ptr)[i].check_collision(pos, collider_size))//collide with others
        {
            if(Type_list[ (*ptr)[i].Type_index]=="gate")
            {
                cout<<"you win"<<endl;
            }
            Model = stash_model;//undo the translate;
        }*/
}
void Game_Obj::scale(glm::vec3 S_vec)
{
    glm::vec4 x_local{1.0f,0.0f,0.0f,0},y_local{0.0f,1.0f,0.0f,0},z_local{0.0f,0.0f,1.0f,0};
    glm::vec4 x_world,y_world,z_world;
    x_world = Model*x_local;y_world = Model*y_local;z_world = Model*z_local;
    glm::vec3 local_scale = glm::vec3(S_vec.x*x_world+S_vec.y*y_world+S_vec.z*z_world);
    
    glm::mat4 modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, local_scale);
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
        Comp_list.push_back(new Player());
        Comp_list[Comp_list.size()-1]->parent_index = Index;
        cout<<"ptr:"<<this<<endl;
    }
    if(component_name=="weapon"){
        Comp_list.push_back(new Weapon());
        Comp_list[Comp_list.size()-1]->parent_index = Index;
    }
    if(component_name=="bullet"){
        Comp_list.push_back(new Bullet());
        Comp_list[Comp_list.size()-1]->parent_index = Index;
    }
    return true;
}
void Game_Obj::Update(UI_Event &input_event)
{
    cout<<Type_list[Type_index]<<endl;
    collider_center = Model*collider_offset;
    Game_Events G_events;
    for(int i=0;i<Comp_list.size();i++)
    {
        Component* cpt;
        cpt=Comp_list[i];
        cpt->Update(input_event,this);
    }
}
bool Game_Obj::check_collision(glm::vec3 pos,glm::vec3 size)
{
    glm::vec4 point = glm::vec4{pos.x,pos.y,pos.z,1};
    float a_minX=point.x-size.x,
            a_minY=point.y-size.y,
            a_minZ=point.z-size.z,
            a_maxX=point.x+size.x,
            a_maxY=point.y+size.y,
            a_maxZ=point.z+size.z;
    float b_minX=collider_center.x-collider_size.x,
            b_minY=collider_center.y-collider_size.y,
            b_minZ=collider_center.z-collider_size.z,
            b_maxX=collider_center.x+collider_size.x,
            b_maxY=collider_center.y+collider_size.y,
            b_maxZ=collider_center.z+collider_size.z;
   if ( (a_minX <= b_maxX && a_maxX >= b_minX) &&
        (a_minY <= b_maxY && a_maxY >= b_minY) &&
        (a_minZ <= b_maxZ && a_maxZ >= b_minZ))
       return true;
   else return false;
    
}
Game_Obj::~Game_Obj()
{

}
