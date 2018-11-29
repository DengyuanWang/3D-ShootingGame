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
    int Status=0;//{unstart,start,end}={0,1,2}
    string Event;                  //1  2  3  4  5  6  7  8  9  10  11
    vector<vector<int>> Origin_Map={{1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1},//1
                                    {1, 8, 0, 1, 1, 1, 1, 1, 1,  2,  1},//2
                                    {1, 1, 0, 4, 0, 0, 0, 0, 1,  0,  1},//3
                                    {1, 1, 0, 1, 1, 1, 1, 0, 1,  0,  1},//4
                                    {1, 1, 0, 1, 1, 1, 1, 12, 0,  0,  1},//5
                                    {1, 1, 5, 0, 0, 0, 1, 7, 1,  1,  1},//6
                                    {1, 6, 0, 0, 1, 0, 0, 0, 1,  0,  1},//7
                                    {1, 0, 1, 1, 1, 0, 1, 0, 1,  0,  1},//8
                                    {1, 0, 1, 1, 1, 0, 1, 0, 0,  0,  1},//9
                                    {1, 9, 1, 1, 1, 3, 1, 1, 1,  10,  1},//10
                                    {1, 1, 1, 1, 1, 11, 1, 1, 1,  1,  1},//11
                                    };
    int player_pos[2];
    glm::mat4 View_matrix;
    glm::mat4 Origin_ViewMatrix;
    glm::vec3 Camera_pos;
    glm::vec3 Lookat;
    glm::vec3 Up_vec;
    vector<vector<int>> hash_forOBJ;
    float Step = 0.2;
    int Player_index=-1;
    int key_index=-1;
    int monster_index = -1;
    bool switchKey;
    /*{0,1,2,3}={none,wall,Gate,player,door[1~4],key[1~4],monster} 0,1,2,3,[4,5,6,7],[8,9,10,11],12
      5*5: 0000G
           WW0W0
           0W0W0
           0W0WW
           S0000
    */
    bool forward();//move forward
    bool backward();//move backward
    bool left();//move left
    bool right();//move right
    bool turnL(float in);//move view direction left
    //bool turnR(float in);//move view direction right
    bool turnUp(float in);//move view direction up
   // bool turnDown(float in);//move view direction down
    bool check_forUpdata(float x,float y);
    bool check_forMonsterUpdata(float x,float y);
    bool update_monster();
    long long timer = 0;
    float last_move[2]={-1,-1};
    bool freeze = false;
public:
    UI ui;
    GameLogic();
    ~GameLogic();
    void restart();//reset game
    bool update_UntilWin(UI_Event input_event);//ture for win, false for continue
    void update_view(UI_Event input_event);
};
#endif /* GameLogic_hpp */
