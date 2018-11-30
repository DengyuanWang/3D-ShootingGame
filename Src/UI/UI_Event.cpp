//
//  UI_Event.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/14/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.cout
//

#include "UI_Event.hpp"
#include <algorithm>
//all possible input event
vector<string> UI_Event::Event_list={"None","Esc","FullScreen_switch",
    "Up","Down","Left","Right",
    "UpLeft","UpRight","DownLeft","DownRight"
};
UI_Event::UI_Event()
{
    reset_table();
}
UI_Event::UI_Event(string in)
{
    reset_table();
    set_event(in);
}
bool UI_Event::check_valiable(string in)
{
    vector<string>::iterator index = std::find(Event_list.begin(), Event_list.end(), in);
        if (index != Event_list.end())
        {
            return true;
        }else return false;
}
bool UI_Event::set_event(string in)//set event
{
    if(check_valiable(in)){
        Event_table[in] = true;
        return true;
    }
    else
    return false;
}
bool UI_Event::reset_event(string in)
{
    if(check_valiable(in)){
        Event_table[in] = false;
        return true;
    }
    else
        return false;
}
void UI_Event::reset_table()
{
    for(int i=0;i<Event_list.size();i++)
        Event_table[Event_list[i]]=false;
}
bool UI_Event::check_event(string in)//get event name
{
    if(check_valiable(in)){
        return Event_table[in];
    }
    else
    {
        cout<<"error event name"<<endl;
        exit(0);
    }
}
UI_Event::~UI_Event()
{
    
}
