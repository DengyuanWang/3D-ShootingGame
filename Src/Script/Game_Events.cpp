//
//  Game_Events.cpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/29/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Game_Events.hpp"

vector<string> Game_Events::Game_Event_list={
    //player's event
    "Player_forward","Player_backward","Player_left","Player_right",
    "View_up","View_down","View_left","View_right",
    "ReatchGate",
    //other event
    "Create_enemies","Pumpt_textbox","Hide_textbox","Block_retreat_way"
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
