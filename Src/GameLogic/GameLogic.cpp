//
//  GameLogic.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/15/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//
#include "GameLogic.hpp"

#if defined(__APPLE__)
const string mapPath = "lib/maps/";
#elif defined(__linux__)
const string mapPath = "../lib/maps/";
#endif

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
    string mapname = "map1";
    string full_filename =mapPath+mapname+".txt";
    ifstream mapFile(full_filename);
    string tmp;
    while(getline(mapFile,tmp))
    {
        decode_cmd(tmp);
    }
//reset view matrix
    //set_view();
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
    float len =sqrt((pow(tmpmat[0].x,2)+pow(tmpmat[0].y,2)+pow(tmpmat[0].z,2)));
    //cout<<"x"<<tmpmat[3].x<<"y"<<tmpmat[3].y<<"z"<<tmpmat[3].z<<endl;
    glm::vec4 offset = tmpmat*eye_pos_offset/len;
    glm::vec3 eye_pos = glm::vec3(tmpmat[3])+glm::vec3(offset.x,offset.y,offset.z);
    //get viewat_vector, whose length is 0.2f;
    glm::vec4 View_vec = tmpmat*ViewAt_vector;
    //calculate view_matrix;
    ui.View_matrix = glm::lookAt(eye_pos-glm::vec3(View_vec.x,View_vec.y,View_vec.z), eye_pos, glm::vec3{0,1,0});
}
void GameLogic::Add_Game_obj()//add at head
{
    //G_objs.push_back();
    G_objs.insert(G_objs.begin(), Game_Obj());
}
bool GameLogic::Update()
{
    Game_Events* Gptr=(Game_Events*)Gevent_list;
    Gptr->lastTime = Gptr->currentTime;
    Gptr->currentTime = SDL_GetTicks();
    if(Gptr->check_event("ReatchGate")||Gptr->check_event("Player_dead"))
        return false;
    if(Gptr->check_event("Erase"))
    {
        erase_objs();
        Gptr->set_event("Erase", false);

    }
    
    if(Gptr->check_event("new_bullet"))
    {
        add_bullets();
        Gptr->set_event("new_bullet", false);
        set_indices();
    }
//Update all Game objs
    set_indices();
    Gptr->Update();//update all collider informations
    for(int i=0;i<G_objs.size();i++)
        G_objs[i].Update(ui.rst);
//Get camera pos
    
    Player *tmp = (Player *)(G_objs[player_index]).get_component("Player");
    glm::mat4 vmat =tmp->view_matrix;
    ui.View_matrix =vmat;
    /*
    float angle =  M_PI/2*(Gptr->currentTime-Gptr->lastTime)/1000.0;
    G_objs[player_index].local_rotation(glm::vec3(0,angle,0));
    glm::mat4 r = G_objs[player_index].get_Model();
    r = glm::translate(r, glm::vec3(0,0,2));
    glm::mat4 vmat2 = glm::inverse(r);
    ui.View_matrix = glm::inverse(r);
    */
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
void GameLogic::set_indices()//reset all index
{
    for(int i=0;i<G_objs.size();i++)
    {
        G_objs[i].Index = i;
        if(G_objs[i].get_type()=="player")
            player_index = i;
    }
}
void GameLogic::erase_objs()
{
    Game_Events* Gptr=(Game_Events*)Gevent_list;
    sort(Gptr->Erase_index.begin(),Gptr->Erase_index.end());//[0 2 1]->[0 1 2]
    while(Gptr->Erase_index.size()!=0)
    {
        int index =  Gptr->Erase_index[Gptr->Erase_index.size()-1];
        G_objs.erase(G_objs.begin()+index);
        Gptr->Erase_index.pop_back();
    }
    set_indices();
}
void GameLogic::add_bullets()
{
    Game_Events* Gptr=(Game_Events*)Gevent_list;
    int added_count = 0;
    while(Gptr->new_bullet_list.size()!=0)
    {
        int index = *Gptr->new_bullet_list[Gptr->new_bullet_list.size()-1];
        added_count+=1;
        index += added_count;
        stringstream ss;
//index Objname model texture size[xyz] position[xyz] boxcollider[xyz] colliderpos_offset[xyz]
        ss<<"0 "<<"bullet "<<"sphere "<<"color "<<"[.2 .2 .2] "
        <<"[0 0 0] "<<"[.6 .6 .6] "<<"[0 0 0]";
        decode_cmd(ss.str());
        Weapon* Wptr = ((Weapon*)G_objs[index].get_component("Weapon"));
        if(Wptr!=NULL)//To fix the bug in erase, when inlegal index happened, just ignore it
        {
            //cout<<"shoot from:"<<G_objs[index].get_type()<<endl;
            glm::mat4 M4 = ((Weapon*)G_objs[index].get_component("Weapon"))->Model;
            glm::vec3 tmp(M4[2]);
            G_objs[0].set_mat(M4);
            Bullet* bulletptr = (Bullet *)G_objs[0].get_component("Bullet");;
            bulletptr->ownership =G_objs[index].get_type();
            if(bulletptr->ownership=="")
            {
                
            }
            G_objs[0].scale(bulletptr->size);
            
        }
        Gptr->new_bullet_list[Gptr->new_bullet_list.size()-1]=NULL;
        Gptr->new_bullet_list.pop_back();
    }
    set_indices();
}
void GameLogic::decode_cmd(string in)
{
    bool player_tag = false,gate_tag=false;
    string tmp = in;
    if(tmp[0]=='#') return;
    else{
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
                    G_objs[0].set_index(k);
                    break;
                case 1://objname
                    k = j;
                    while(tmp[++j]!=' ');
                    //set type
                     G_objs[0].Specify_type(tmp.substr(k,j-k));
                    //cout<<tmp.substr(k,j-k)<<endl;
                    if(tmp.substr(k,j-k)=="player")
                        player_tag = true;
                    else if(tmp.substr(k,j-k)=="gate")
                        gate_tag = true;
                    break;
                case 2://model
                    k = j;
                    while(tmp[++j]!=' ');
                    //set model
                    G_objs[0].Model_name = tmp.substr(k,j-k);
                    break;
                case 3://texture
                    k = j;
                    while(tmp[++j]!=' ');
                    //set texture
                    G_objs[0].Texture_name = tmp.substr(k,j-k);
                    break;
                case 4://size
                    k = j;
                    while(tmp[++j]!=']');
                    ss<<tmp.substr(k+1,j-k-1);
                    ss>>d1>>d2>>d3;
                    G_objs[0].scale(glm::vec3{d1,d2,d3});
                    break;
                case 5://position
                    k = j;
                    while(tmp[++j]!=']');
                    ss<<tmp.substr(k+1,j-k-1);
                    ss>>d1>>d2>>d3;
                    G_objs[0].translate(glm::vec3{d1,d2,d3});
                    break;
                case 6://boxcollider size
                    k = j;
                    while(tmp[++j]!=']');
                    ss<<tmp.substr(k+1,j-k-1);
                    ss>>d1>>d2>>d3;
                    G_objs[0].collider_size = glm::vec3{d1,d2,d3};
                    break;
                case 7://box collider offset
                    k = j;
                    while(tmp[++j]!=']');
                    ss<<tmp.substr(k+1,j-k-1);
                    ss>>d1>>d2>>d3;
                    G_objs[0].collider_offset = glm::vec4{d1,d2,d3,1};
                    break;
                default:
                    cout<<"error map"<<endl;
                    exit(0);
                    break;
            }
            i++;
        }
    }
    if(gate_tag)
    {
        //G_objs[0].local_rotation(glm::vec3{0,M_PI/2,0});
        G_objs[0].local_rotation(glm::vec3{-M_PI/2,0,0});
         //G_objs[0].local_rotation(glm::vec3{0,0,M_PI/2});
    }
    if(player_tag){
        // G_objs[0].local_rotation(glm::vec3{-M_PI/2,0,0});
    }
}
