//
//  Game_Events.h
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Game_Events_h
#define Game_Events_h
#include <iostream>
#include <vector>
using namespace std;
class Game_Events{
public:
    static vector<string> Game_Event_list ={
        "Player_forward","Player_backward","Player_left","Player_right",
        "View_up","View_down","View_left","View_right",
        "Create_enemies","Pumpt_textbox","Hide_textbox","Block_retreat_way"
    };
    int current_event;
};

#endif /* Game_Events_h */
