//
//  Game_Events.cpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/29/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Game_Events.hpp"
#include "../GameLogic/Game_Obj.hpp"

vector<string> Game_Events::Game_Event_list={
    //player's event
    "Player_forward","Player_backward","Player_left","Player_right",
    "View_up","View_down","View_left","View_right","Player_shoot",
    "ReatchGate","Player_dead",
    //other event
    "Create_enemies","Pumpt_textbox","Hide_textbox","Block_retreat_way",
    "Erase","new_bullet"
    };
Game_Events::Game_Events()
{
    for(int i=0;i<Game_Event_list.size();i++)
    {
        Events.push_back(bool{false});
    }
}
void Game_Events::set_event(string eventname,bool value)
{
    int index = find_event(eventname);
    if(index>=0){
        Events[index]=value;
    }
}
bool Game_Events::check_event(string eventname){
    int index = find_event(eventname);
    if(index>=0){
       return Events[index];
    }else {
        cout<<"wrong event name"<<endl;
        exit(0);
    }
}
int Game_Events::find_event(string name){
    vector<string>::iterator index = std::find(Game_Event_list.begin(), Game_Event_list.end(), name);
    if (index != Game_Event_list.end())
    {
        return (int)std::distance( Game_Event_list.begin(), index );;//event none
    }else return -1;
}
void Game_Events::Update()
{
    HashMap.clear();
    vector<Game_Obj> *Gobj_list_ptr;
    Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
    if(Gobj_list_ptr==nullptr)
        return;
    else{
        for(int i=0;i<Gobj_list_ptr->size();i++)
        {
            string Value="";
            glm::vec3 pos((*Gobj_list_ptr)[i].get_Model()[3]);
            int x = pos.x/3,z=pos.z/3;
            ostringstream convert1,convert2;
            convert1 << x<<"-"<<z;
            string Key = convert1.str();
            convert2 << i;
            unordered_map<string,string>::iterator got = HashMap.find (Key);
            if ( got == HashMap.end() )
                Value = convert2.str();
            else
                Value = HashMap[Key]+"|"+ convert2.str();
            HashMap[Key] = Value;
        }
        Update_Collision();
    }
}
void Game_Events::Update_Collision()
{
    vector<Game_Obj> *Gobj_list_ptr;
    Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
    if(Gobj_list_ptr==nullptr)
        return;
    else{
        for(int i=0;i<Gobj_list_ptr->size();i++)
            (*Gobj_list_ptr)[i].collision_indices.clear();
        for(int i=0;i<Gobj_list_ptr->size();i++)
            for(int j=i+1;j<Gobj_list_ptr->size();j++)
            {
                bool rst;
                if((*Gobj_list_ptr)[j].collider_size.x< (*Gobj_list_ptr)[i].collider_size.x)
                    rst =(*Gobj_list_ptr)[j].check_collision(&(*Gobj_list_ptr)[i]);
                else rst = (*Gobj_list_ptr)[i].check_collision(&(*Gobj_list_ptr)[j]);
                if(rst==true)
                {
                    (*Gobj_list_ptr)[i].collision_indices.push_back(j);
                    (*Gobj_list_ptr)[j].collision_indices.push_back(i);
                    //if((*Gobj_list_ptr)[j].get_type()=="bullet")
                      // cout<<(*Gobj_list_ptr)[i].get_type()<<"with"<<(*Gobj_list_ptr)[j].get_type()<<endl;;
                }
            }
    }
}
