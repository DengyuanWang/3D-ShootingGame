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
        Type_index = std::distance( Type_list.begin(), index );;//event none
        Model_name = Type_list[Type_index];
        return true;
    }else return false;
}
void Game_Obj::translate(glm::vec3 T_vec)
{
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), T_vec);
    Model =  trans *Model;
}
void Game_Obj::scale(glm::vec3 S_vec)
{
    glm::mat4 modelMatrix = glm::mat4();
    modelMatrix = glm::scale(modelMatrix, S_vec);
    Model = Model*modelMatrix;
}
void Game_Obj::rotation(glm::vec3 angles)//rotate angles for each axises
{
    Model = glm::rotate(Model, angles.x, glm::vec3(1.0f,0.0f,0.0f));
    Model = glm::rotate(Model, angles.y, glm::vec3(0.0f,1.0f,0.0f));
    Model = glm::rotate(Model, angles.z, glm::vec3(0.0f,0.0f,1.0f));
}
Game_Obj::~Game_Obj()
{

}
