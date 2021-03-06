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
    glm::vec4 eye_pos_offset;//in player's coordination
    glm::vec4 ViewAt_vector;//in player's coordination
//functions
    void Add_Game_obj();//add new game obj
    void set_view();//calculate view matrix based on player
    void decode_cmd(string in);
    void set_indices();
    void erase_objs();
    void add_bullets();
public:
    UI ui;
    GameLogic();
    ~GameLogic();
    void restart();//reset game
    bool Update();//ture for win, false for continue
};
#endif /* GameLogic_hpp */
