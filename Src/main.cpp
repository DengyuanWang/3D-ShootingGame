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
int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    
    GameLogic G_logic;//init game environment and map

    bool loop_tag = true;
    //Try to fix the problem of no display at beginning
        G_logic.ui.full_screen_switch();
        G_logic.ui.full_screen_switch();
    while(loop_tag)
    {
        UI_Event ui_event = G_logic.ui.get_input_event();
        loop_tag = G_logic.Update(ui_event);
        switch(ui_event.get_event_index())
        {
            case 1://"Esc"
                loop_tag = false;
                break;
            case 2://"Fullscreen_switch"
                G_logic.ui.full_screen_switch();
                break;
            default:
                break;
        }

    }

    
    G_logic.ui.close_window();
    return 0;
}
