//
//  UI_Event.hpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/14/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef UI_Event_hpp
#define UI_Event_hpp
#include <iostream>
#include <vector>
using namespace std;
class UI_Event{
private:
    string Event_name;
    static vector<string> Event_list;//all possible input event
    long Event_index;//event index
public:
    int mouse_status[2];
    UI_Event();
    UI_Event(string in);
    bool set_event(string in);//set event
    string get_event();//get event name
    long get_event_index(){return Event_index;}//get event index from Event_list
    ~UI_Event();
};
#endif /* UI_Event_hpp */
