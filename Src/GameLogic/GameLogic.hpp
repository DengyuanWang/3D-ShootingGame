//
//  GameLogic.hpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/15/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef GameLogic_hpp
#define GameLogic_hpp
#include "../UI/UI.hpp"
class GameLogic{
private:
//Variables
    int Status=0;//{unstart,start,end}={0,1,2}
    vector<Game_Obj> G_objs;//save all GameObjects
    double Timer = 0;//game timer
    int player_index;
//functions
    void Add_Game_obj();
    
    
    
    
public:
    UI ui;
    GameLogic();
    ~GameLogic();
    void restart();//reset game
    bool Update(UI_Event input_event);//ture for win, false for continue
};
#endif /* GameLogic_hpp */
