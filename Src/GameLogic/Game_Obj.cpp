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
              "teleporter", "elements","weapon","treasureBox","destructibleWall",
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
        Player *tmp = new Player;
        Comp_list.push_back(tmp);
    }
    return true;
}
void Game_Obj::Update(UI_Event &input_event)
{
    Game_Events G_events;
    for(int i=0;i<Comp_list.size();i++)
    {
        Component* cpt;
        cpt=Comp_list[i];
        cpt->Update(input_event,G_events,this);
    }
}
Game_Obj::~Game_Obj()
{

}
