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
vector<string> UI_Event::Event_list={"None","Esc","FullScreen_switch","Up","Down","Left","Right","Switch_key","Switch_freeze"};
UI_Event::UI_Event()
{
    Event_index = 0;//event none
    Event_name = Event_list[0];
}
UI_Event::UI_Event(string in)
{
    set_event(in);
}
bool UI_Event::set_event(string in)//set event
{
    vector<string>::iterator index = std::find(Event_list.begin(), Event_list.end(), in);
    if (index != Event_list.end())
    {
        Event_index = std::distance( Event_list.begin(), index );;//event none
        Event_name = Event_list[Event_index];
        return true;
    }else return false;
}
string UI_Event::get_event()//get event name
{
    return Event_name;
}
UI_Event::~UI_Event()
{
    
}
