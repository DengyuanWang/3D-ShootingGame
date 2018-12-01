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
    //    index Objname model texture size[fx,fy,fz] position[x y z] boxcollider[x y z] colliderpos_offset[x y z]
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
                            k = j;
                            while(tmp[++j]!=' ');
                            Add_Game_obj();
                            k =atoi(tmp.substr(k,j-k).c_str());
                            G_objs[G_objs.size()-1].set_index(k);
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
                        case 6://boxcollider size
                            k = j;
                            while(tmp[++j]!=']');
                            ss<<tmp.substr(k+1,j-k-1);
                            ss>>d1>>d2>>d3;
                            G_objs[G_objs.size()-1].collider_size = glm::vec3{d1,d2,d3};
                            break;
                        case 7://box collider offset
                            k = j;
                            while(tmp[++j]!=']');
                            ss<<tmp.substr(k+1,j-k-1);
                            ss>>d1>>d2>>d3;
                            G_objs[G_objs.size()-1].collider_offset = glm::vec4{d1,d2,d3,1};
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
    set_view();
    Gobj_list = &G_objs;
}
void GameLogic::set_view()
{
    /*
     1. the camera pos is at a const offset: (0.01f,0.05f,-0.1f) of the player's model
     2. the view direction is a vector in yz plane of player's local coordinate:
     notice that the up vector is always constant: [0 1 0];
     */
    glm::mat4 tmpmat = G_objs[player_index].get_Model();
    //get pose, camera is a little higher than player
    float len =sqrt(pow(tmpmat[0].x,2)+pow(tmpmat[0].y,2)+pow(tmpmat[0].z,2));
    //cout<<"x"<<tmpmat[3].x<<"y"<<tmpmat[3].y<<"z"<<tmpmat[3].z<<endl;
    glm::vec4 offset = tmpmat*eye_pos_offset/len;
    glm::vec3 eye_pos = glm::vec3(tmpmat[3])+glm::vec3(offset.x,offset.y,offset.z);
    //get viewat_vector, whose length is 0.2f;
    glm::vec4 View_vec = tmpmat*ViewAt_vector;
    //calculate view_matrix;
    ui.View_matrix = glm::lookAt(eye_pos-glm::vec3(View_vec.x,View_vec.y,View_vec.z), eye_pos, glm::vec3{0,1,0});
}
void GameLogic::Add_Game_obj()
{
    G_objs.push_back(Game_Obj());
}
bool GameLogic::Update(UI_Event uievent)
{
   
    Game_Events* Gptr=(Game_Events*)Gevent_list;
    Gptr->lastTime = Gptr->currentTime;
    Gptr->currentTime = SDL_GetTicks();
    
    if(Gptr->check_event("Erase"))
    {
        G_objs.erase(G_objs.begin()+Gptr->Erase_index);
        Gptr->set_event("Erase", false);
    }
    if(Gptr->check_event("ReatchGate"))
        return false;
//Update all Game objs
    for(int i=0;i<G_objs.size();i++)
    {
        G_objs[i].Index = i;
        G_objs[i].Update(uievent);
    }
    
    
//Get camera pos
    Player *tmp = (Player *)(G_objs[player_index].Comp_list[0]);
    ViewAt_vector = tmp->ViewAt_vector;
    eye_pos_offset= tmp->eye_pos_offset;
    set_view();
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
