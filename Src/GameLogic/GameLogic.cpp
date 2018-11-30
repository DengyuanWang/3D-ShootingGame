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
            //index,objname,model,texture,size,position;
                for(int i=0,j = 0;j<tmp.length();j++)
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
                            cout<<tmp.substr(k,j-k)<<endl;
                            if(tmp.substr(k,j-k)=="player")
                                player_tag = true;
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
                    if(player_tag)//recoard player index
                    {
                        player_index =(int) G_objs.size()-1;
                        player_tag = false;
                    }
                }
        }
    }
//reset view matrix
    glm::mat4 tmpmat = G_objs[player_index].get_Model();
    //get pose, camera is a little higher than player
    eye_pos = glm::vec3(tmpmat[3])+glm::vec3(0,0.05f,0);
    //get viewat_vector, whose length is 0.2f;
    ViewAt_vec = glm::vec3(0,-1,5);
    ViewAt_vec = glm::normalize(ViewAt_vec)*0.2f;
    //calculate view_matrix;
    ui.View_matrix = glm::lookAt(eye_pos-ViewAt_vec, eye_pos, glm::vec3{0,1,0});
}
void GameLogic::Add_Game_obj()
{
    G_objs.push_back(Game_Obj());
}
bool GameLogic::Update(UI_Event uievent)
{
//Player controller:
    //change view angle
    float xy[2] ={-uievent.mouse_status[0]/500.0f,uievent.mouse_status[1]/500.0f};
    ViewAt_vec = glm::rotateY(ViewAt_vec,xy[0]);
    ViewAt_vec = glm::rotateX(ViewAt_vec,xy[1]);
    glm::mat4 tmpmat = G_objs[player_index].get_Model();
    //get pose, camera is a little higher than player
    eye_pos = glm::vec3(tmpmat[3])+glm::vec3(0.01f,0.05f,-0.1f);
    ui.View_matrix = glm::lookAt(eye_pos-ViewAt_vec, eye_pos, glm::vec3{0,1,0});
    //move player
    if(uievent.check_event("Up"))
    {
        
    }
    if(uievent.check_event("Down"))
    {
        
    }
    if(uievent.check_event("Left"))
    {
        
    }
    if(uievent.check_event("Right"))
    {
        
    }
    
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
