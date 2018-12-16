//
//  main.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/14/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//
//libs included
#include <iostream>
#include <set>
//files included
#include "GameLogic/GameLogic.hpp"
using namespace std;
std::unordered_map<ThreeDPOS, std::vector<void*>> Hashmap;//mapping between 3D pos and GameObjs
void *Gobj_list = NULL;
void *Gevent_list = NULL;
void generate_map();
string numberToString(float* arrayIn,int len);
int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    generate_map();
    Game_Events G_events;
    Gevent_list = &G_events;
    GameLogic G_logic;//init game environment and map
    G_events.currentTime = SDL_GetTicks();//init timer;
    bool loop_tag = true;
    //Try to fix the problem of no display at beginning
        G_logic.ui.full_screen_switch();
        G_logic.ui.full_screen_switch();
    
    while(loop_tag)
    {
        G_logic.ui.catch_input_event();
       // if(G_logic.ui.rst.check_event("Shoot"))
         //   cout<<"outcheck"<<G_logic.ui.rst.check_event("Shoot")<<endl;
        if(G_logic.ui.rst.check_event("Esc"))
                loop_tag = false;
        else if(G_logic.ui.rst.check_event("FullScreen_switch"))
                G_logic.ui.full_screen_switch();
        else loop_tag = G_logic.Update();

    }

    
    G_logic.ui.close_window();
    return 0;
}
void generate_map()
{
    int mapsize[2]={15,15};
    string filename = "./lib/maps/map1.txt";
    ofstream file;
    file.open (filename);
    file << "#index Objname model texture size[fx,fy,fz] position[x y z] boxcollider[x y z] colliderpos_offset[x y z]\n";
    
    int map_tag[50][50];
    //0 for nothing,1 for wall,2 for monster, 3 for final_gate,4 for player,5 moving_monster
    
    for(int i=0;i<mapsize[0];i++)
    for(int j=0;j<mapsize[1];j++)
    {
        if(i==0||i==mapsize[0]-1||j==0||j==mapsize[1]-1)
            map_tag[i][j] = 1;
        else{
            int t =rand()%20;
            if(t<18)
                map_tag[i][j] = 0;
            else if(t<19)
                map_tag[i][j] = 5;
            else
                map_tag[i][j] = 1;

        }
    }
    std::vector<int> posX;
    std::vector<int> posY;
    for(int i=0;i<30;i++)
    {
        posX.push_back(int(rand()%mapsize[0]));
        posY.push_back(int(rand()%mapsize[1]));
    }
    std::set<int> posSetX(posX.begin(),posX.end());
    std::set<int> posSetY(posY.begin(),posY.end());
    std::vector<int> posX1(posSetX.begin(),posSetX.end());
    std::vector<int> posY1(posSetY.begin(),posSetY.end());
    map_tag[posX1[0]][posY1[0]]=3;//player
    map_tag[posX1[1]][posY1[1]]=4;//final_gate
    for(int i=2;i<3&&i<posX1.size()&&i<posY1.size();i++)
    {
       map_tag[posX1[i]][posY1[i]]=2;//set monster;
    }
    int index = 0;
    string space=" ",brackets="[]";
    string objname="floor",model="cube",texture="wood",size="[]";
    string position="[]",boxcollider="[]",colliderpos_offset="[]\n";
    float sizeV[3]={0,0,0},positionV[3]={0,0,0};
    float boxcolliderV[3]={0,0,0},colliderpos_offsetV[3]={0,0,0};
    //generate floor
    {
        //set value
        sizeV[0]=50;sizeV[1]=1;sizeV[2]=50;
        positionV[0]=0;positionV[1]=0;positionV[2]=0;
        boxcolliderV[0]=0;boxcolliderV[1]=0;boxcolliderV[2]=0;
        colliderpos_offsetV[0]=0;colliderpos_offsetV[1]=-1.0f;colliderpos_offsetV[2]=0;
        //convert to string
        size = space+brackets[0]+ numberToString(sizeV,3) +brackets[1]+space;
        position = space+brackets[0]+numberToString(positionV,3) +brackets[1]+space;
        boxcollider = space+brackets[0]+numberToString(boxcolliderV,3)+brackets[1]+space;
        colliderpos_offset = space+brackets[0]+numberToString(colliderpos_offsetV,3)+brackets[1]+space;
        //write
        file<<index++<<space<<objname+space+model+space+texture
                +space+size+position+boxcollider+colliderpos_offset+"\n";
    }
    //generate wall around scene
        //set value

    for(int i=0;i<mapsize[0];i++)
    for(int j=0;j<mapsize[1];j++)
    {
        positionV[0]=i;positionV[1]=1;positionV[2]=j;
        if(map_tag[i][j]==1)//wall
        {
            sizeV[0]=1;sizeV[1]=1;sizeV[2]=1;
            boxcolliderV[0]=.7;boxcolliderV[1]=.7;boxcolliderV[2]=.7;
            colliderpos_offsetV[0]=0;colliderpos_offsetV[1]=0;colliderpos_offsetV[2]=0;
            objname="wall";model="cube";texture="brick";
        }
        else if(map_tag[i][j]==2)//monster
        {
            
            sizeV[0]=.5;sizeV[1]=.5;sizeV[2]=.5;
            boxcolliderV[0]=.5;boxcolliderV[1]=.5;boxcolliderV[2]=.5;
            colliderpos_offsetV[0]=0;colliderpos_offsetV[1]=0.2;colliderpos_offsetV[2]=0;
            objname="static_monster";model="knot";texture="wood";
        }
        else if(map_tag[i][j]==3)//final_gate
        {
            sizeV[0]=1;sizeV[1]=1;sizeV[2]=1;
            boxcolliderV[0]=.5;boxcolliderV[1]=.5;boxcolliderV[2]=.2;
            colliderpos_offsetV[0]=0;colliderpos_offsetV[1]=0;colliderpos_offsetV[2]=0;
            objname="gate";model="teapot";texture="color";
        }
        else if(map_tag[i][j]==4)//player
        {
            sizeV[0]=.1;sizeV[1]=.1;sizeV[2]=.1;
            boxcolliderV[0]=1;boxcolliderV[1]=1;boxcolliderV[2]=.5;
            colliderpos_offsetV[0]=0;colliderpos_offsetV[1]=0;colliderpos_offsetV[2]=0;
            objname="player";model="player";texture="color";
        }
        else if(map_tag[i][j]==5)//Moving monster
        {
            positionV[1]=2;
            sizeV[0]=.5;sizeV[1]=.5;sizeV[2]=.5;
            boxcolliderV[0]=.1;boxcolliderV[1]=.1;boxcolliderV[2]=.1;
            colliderpos_offsetV[0]=0;colliderpos_offsetV[1]=0;colliderpos_offsetV[2]=0;
            objname="moving_monster";model="sphere";texture="wood";
        }
        else{ continue;}
        //convert to string
        size = space+brackets[0]+ numberToString(sizeV,3) +brackets[1]+space;
        position = space+brackets[0]+numberToString(positionV,3) +brackets[1]+space;
        boxcollider = space+brackets[0]+numberToString(boxcolliderV,3)+brackets[1]+space;
        colliderpos_offset = space+brackets[0]+numberToString(colliderpos_offsetV,3)+brackets[1]+space;
        file<<index<<space<<objname+space+model+space+texture+space+size+position+boxcollider+colliderpos_offset+"\n";
    }
    //generate FinalGate
    
    //generate avatar pos
    
    //generate static monster
    
    //generate move monster
    
    file.close();
}
string numberToString(float* arrayIn,int len)
{
    string result="";
    for(int i=0;i<len;i++)
    {
        ostringstream convert2;
        convert2 << arrayIn[i];
        string Text = convert2.str(); //int to string, Text="456"
        if(i==len-1)
            result = result+Text;
        else
            result = result+Text+" ";
    }
    return result;
}
