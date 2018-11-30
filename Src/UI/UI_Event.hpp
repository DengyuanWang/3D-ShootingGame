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
#include <unordered_map>
using namespace std;
class UI_Event{
private:
    string Event_name;
    static vector<string> Event_list;//all possible input event
    unordered_map<string, bool> Event_table;
    long Event_index;//event index
public:
    int mouse_status[2];
    UI_Event();
    UI_Event(string in);
    bool set_event(string in);//set event
    bool reset_event(string in);
    void reset_table();
    bool check_event(string in);//get event state
    bool check_valiable(string in);
    ~UI_Event();
};
#endif /* UI_Event_hpp */
