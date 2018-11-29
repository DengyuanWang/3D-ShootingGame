//
//  Game_Obj.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/15/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Game_Obj.hpp"
//all possible input model
vector<string> Game_Obj::Type_list={"floor","gate","wall","door","key","player","monster"};
Game_Obj::Game_Obj(string model_name)
{
    if(load_model(model_name))//if model name exists
    {
        ifstream modelFile;
        string modelFile_name;
        if(Type_list[Type_index]=="floor")
        {
            texture_index = 0;
            modelFile_name = "cube";
        }
        else if(Type_list[Type_index]=="gate")
        {
            texture_index = 1;
            modelFile_name = "teapot";
        }
        else if(Type_list[Type_index]=="wall")
        {
            texture_index = 1;
            modelFile_name = "cube";
        }
        else if(Type_list[Type_index]=="monster")
        {
            texture_index = -1;
            modelFile_name = "teapot";
        }
        else if(Type_list[Type_index]=="door")
        {
            texture_index = -1;
             modelFile_name = "knot";
        }
        else if(Type_list[Type_index]=="key")
        {
            texture_index = -1;
            modelFile_name = "sphere";
        }
        else if(Type_list[Type_index]=="player")
        {
            texture_index = -1;
            modelFile_name = "teapot";
        }
        string full_filename ="lib/models/"+modelFile_name+".txt";
        modelFile.open(full_filename);
        int numLines = 0;
        modelFile >> numLines;
        for (int i = 0; i < numLines; i++){
            float k;
            modelFile >> k;
            Model_data.push_back(k);
        }
        printf("Mode line count: %d\n",numLines);
        NumVerts = numLines/8;
        Loaded_tag = false;//loaded to OpenGL or not
        Display_tag = false;//Needed to Display or not
    }else{

    }
}
bool Game_Obj::load_model(string name)
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
void Game_Obj::uniform_scale(float factor)
{
    factor = factor>30?30:factor;
    if(factor>0&&factor<=31)//assume scale less than ten times
    {
        glm::vec3 scale = glm::vec3(factor, factor, factor);
        glm::mat4 modelMatrix = glm::mat4();
        modelMatrix = glm::scale(modelMatrix, scale);
        Model = Model*modelMatrix;
    }
}
void Game_Obj::rotation(glm::vec3 angles)//rotate angles for each axises
{
    Model = glm::rotate(Model, angles.x, glm::vec3(1.0f,0.0f,0.0f));
    Model = glm::rotate(Model, angles.y, glm::vec3(0.0f,1.0f,0.0f));
    Model = glm::rotate(Model, angles.z, glm::vec3(0.0f,0.0f,1.0f));
}
float* Game_Obj::get_Model_data()
{
    if(NumVerts==0) return NULL;
    else{
        return Model_data.data();
    }
}
Game_Obj::~Game_Obj()
{

}
