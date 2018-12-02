//
//  main.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/14/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//
//libs included
#include <iostream>
//files included
#include "GameLogic/GameLogic.hpp"
using namespace std;
std::unordered_map<ThreeDPOS, std::vector<void*>> Hashmap;//mapping between 3D pos and GameObjs
void *Gobj_list = NULL;
void *Gevent_list = NULL;
int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
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
        UI_Event ui_event = G_logic.ui.get_input_event();
        if(ui_event.check_event("Esc"))
                loop_tag = false;
        else if(ui_event.check_event("FullScreen_switch"))
                G_logic.ui.full_screen_switch();
        else loop_tag = G_logic.Update(ui_event);

    }

    
    G_logic.ui.close_window();
    return 0;
}
