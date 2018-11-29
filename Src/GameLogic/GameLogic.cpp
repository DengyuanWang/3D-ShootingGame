//
//  GameLogic.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/15/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "GameLogic.hpp"
GameLogic::GameLogic()
{
    if(!ui.open_window())//open new window
    {
        cout<< "Open window failed\n";
    }
    //init openGL
    if(!ui.Init_openGL())
    {
        cout<<"load and init openGL fail"<<endl;
    }
    Status = 0;
    restart();
}
void GameLogic::restart()
{
    Status = 1;
//load map
    //Formate:
    //    index Objname model texture size[fx,fy,fz] position[x y z]
    string mapname = "testmap";
    string full_filename ="lib/maps/"+mapname+".txt";
    ifstream mapFile(full_filename);
    string tmp;
    while(getline(mapFile,tmp))
    {
        if(tmp[0]=='#') continue;
        else{
            bool player_tag = false;
            for(int i=0,j=0;i<6;i++)//index,objname,model,texture,size,position;
                for(j = i+1;j<tmp.length();j++)
                {
                    int k;stringstream ss;float d1,d2,d3;
                    if(tmp[j]==' ') continue;
                    switch (i) {
                        case 0://index
                            Add_Game_obj();
                            j++;//skip it
                            break;
                        case 1://objname
                            k = j;
                            while(tmp[++j]!=' ');
                            //set type
                            G_objs[G_objs.size()-1].Specify_type(tmp.substr(k,j-k));
                            if(tmp.substr(k,j-k)=="player") player_tag = true;
                            break;
                        case 2://model
                            k = j;
                            while(tmp[++j]!=' ');
                            //set model
                            G_objs[G_objs.size()-1].Model_name = tmp.substr(k,j-k);
                            break;
                        case 3://texture
                            k = j;
                            while(tmp[++j]!=' ');
                            //set texture
                            G_objs[G_objs.size()-1].Texture_name = tmp.substr(k,j-k);
                            break;
                        case 4://size
                            k = j;
                            while(tmp[++j]!=']');
                            ss<<tmp.substr(k+1,j-k-1);
                            ss>>d1>>d2>>d3;
                            G_objs[G_objs.size()-1].scale(glm::vec3{d1,d2,d3});
                            break;
                        case 5://position
                            k = j;
                            while(tmp[++j]!=']');
                            ss<<tmp.substr(k+1,j-k-1);
                            ss>>d1>>d2>>d3;
                            G_objs[G_objs.size()-1].translate(glm::vec3{d1,d2,d3});
                            break;
                        default:
                            cout<<"error map"<<endl;
                            exit(0);
                            break;
                    }
                    i++;
                }
            if(player_tag)//recoard player index
                player_index =(int) G_objs.size()-1;
        }
    }
    //reset view matrix
    ui.View_matrix = glm::inverse(G_objs[player_index].get_Model());
}
void GameLogic::Add_Game_obj()
{
    G_objs.push_back(Game_Obj());
}
bool GameLogic::Update(UI_Event uievent)
{
    
    
//Draw models
    vector<string> model_names,textures;
    vector<glm::mat4> model_matrixs;
    for(int i=0;i<G_objs.size();i++)
    {
        model_names.push_back(G_objs[i].Model_name);
        model_matrixs.push_back(G_objs[i].get_Model());
        textures.push_back(G_objs[i].Texture_name);
    }
    if(!ui.Draw(model_names, model_matrixs, textures))
        return false;
    return true;
}

GameLogic::~GameLogic(){
    
}
