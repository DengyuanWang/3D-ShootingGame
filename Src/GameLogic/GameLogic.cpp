//
//  GameLogic.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/15/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "GameLogic.hpp"
GameLogic::GameLogic()
{
    if(!ui.open_window())//open new window
    {
        cout<< "Open window failed\n";
    }
    //init openGL
    if(!ui.Init_openGL())
    {
        cout<<"load and init openGL fail"<<endl;
    }
    
    Status = 0;
    Event = "None";
    restart();
}
void GameLogic::restart()
{
    ui.Add_Game_obj("floor");
    ui.Load_modelsToOpenGL();
    ui.set_visible(ui.get_objnum()-1);
    ui.uniformScale_obj(ui.get_objnum()-1, 20.0f);
    ui.translate_obj(ui.get_objnum()-1, glm::vec3(5,5,-10.5));
    ui.Draw();
    hash_forOBJ = Origin_Map;
    for(int i=0;i<Origin_Map.size();i++)
        for(int j=0;j<Origin_Map[i].size();j++)
        {
            switch(Origin_Map[i][j])//3 means player
            {
                case 0://none
                    hash_forOBJ[i][j]=-1;//means nothing
                    break;
                case 1://wall
                    ui.Add_Game_obj("wall");
                    ui.Load_modelsToOpenGL();
                    ui.set_visible(ui.get_objnum()-1);
                    ui.translate_obj(ui.get_objnum()-1, glm::vec3(i,j,0));
                    hash_forOBJ[i][j] = ui.get_objnum()-1;
                    break;
                case 2://gate
                    ui.Add_Game_obj("gate");
                    ui.Load_modelsToOpenGL();
                    ui.set_visible(ui.get_objnum()-1);
                    ui.translate_obj(ui.get_objnum()-1, glm::vec3(i,j,0));
                    hash_forOBJ[i][j] = ui.get_objnum()-1;
                    break;
                case 3://player
                    player_pos[0]=i;player_pos[1]=j;
                    Camera_pos = glm::vec3(i,j,0);
                    Lookat = glm::vec3(i,1.0f+j,0);
                    Up_vec = glm::vec3(0.0f, 0.0f, 1.0f);
                    View_matrix = glm::lookAt(Camera_pos,Lookat,Up_vec);
                    Origin_ViewMatrix = View_matrix;
                    ui.Add_Game_obj("player");
                    ui.Load_modelsToOpenGL();
                    ui.set_visible(ui.get_objnum()-1);
                    ui.uniformScale_obj(ui.get_objnum()-1, 0.1f);
                    ui.translate_obj(ui.get_objnum()-1,glm::normalize(Lookat-Camera_pos)*Step*1.5f+ Camera_pos);
                    ui.set_key(ui.get_objnum()-1, new int[3] {0,0,0});//black means useless key
                    Player_index = ui.get_objnum()-1;
                    key_index = Player_index;
                    hash_forOBJ[i][j] = -1;
                    ui.set_view(View_matrix);
                    break;
                case 4://door
                case 5:
                case 6:
                case 7:
                    ui.Add_Game_obj("door");
                    ui.Load_modelsToOpenGL();
                    ui.set_visible(ui.get_objnum()-1);
                    ui.translate_obj(ui.get_objnum()-1, glm::vec3(i,j,0));
                    hash_forOBJ[i][j] = ui.get_objnum()-1;
                    if(Origin_Map[i][j]==4) ui.set_key(ui.get_objnum()-1, new int[3] {255,0,0});
                    if(Origin_Map[i][j]==5) ui.set_key(ui.get_objnum()-1, new int[3] {255,255,0});
                    if(Origin_Map[i][j]==6) ui.set_key(ui.get_objnum()-1, new int[3] {0,255,255});
                    if(Origin_Map[i][j]==7) ui.set_key(ui.get_objnum()-1, new int[3] {255,0,255});
                    break;
                case 8://key
                case 9://key
                case 10://key
                case 11://key
                    ui.Add_Game_obj("key");
                    ui.Load_modelsToOpenGL();
                    ui.set_visible(ui.get_objnum()-1);
                    ui.translate_obj(ui.get_objnum()-1, glm::vec3(i,j,0));
                    hash_forOBJ[i][j] = ui.get_objnum()-1;
                    if(Origin_Map[i][j]==8) ui.set_key(ui.get_objnum()-1, new int[3] {255,0,0});//red
                    if(Origin_Map[i][j]==9) ui.set_key(ui.get_objnum()-1, new int[3] {255,255,0});//yellow
                    if(Origin_Map[i][j]==10) ui.set_key(ui.get_objnum()-1, new int[3] {0,255,255});//cyan
                    if(Origin_Map[i][j]==11) ui.set_key(ui.get_objnum()-1, new int[3] {255,0,255});//purple
                    break;
                case 12://monster
                    ui.Add_Game_obj("monster");
                    ui.Load_modelsToOpenGL();
                    ui.set_visible(ui.get_objnum()-1);
                    ui.uniformScale_obj(ui.get_objnum()-1, 0.3f);
                    ui.translate_obj(ui.get_objnum()-1, glm::vec3(i,j,0));
                    hash_forOBJ[i][j] = ui.get_objnum()-1;
                    ui.set_key(ui.get_objnum()-1, new int[3] {0,0,0});//black monster
                    monster_index =ui.get_objnum()-1;
                    break;
                default:
                    
                    break;
            }
        }
    Status = 1;
}
void GameLogic::update_view(UI_Event input_event)
{
    float xy[2] ={input_event.mouse_status[0]/10.0f,input_event.mouse_status[1]/10.0f};
    cout<<input_event.mouse_status[0]<<endl;
    if(input_event.mouse_status[0]!=0)
        turnL(xy[0]);
    //if(input_event.mouse_status[1]!=0)
      // turnUp(xy[1]);
    ui.set_view(View_matrix);
}
bool GameLogic::update_UntilWin(UI_Event input_event)
{
    timer++;
    if(    Status == 0)
    {
        restart();return false;
    }

    
    switch (input_event.get_event_index()) {
        case 3://UP
            forward();
            break;
        case 4://Down
            backward();
            break;
        case 5://Left
            left();
            break;
        case 6://Right
            right();
            break;
        case 7://switch key
            switchKey = true;
            break;
        case 8://switch freeze
            freeze = !freeze;
            break;
        default:
            break;
    }
    if(monster_index!=-1&&timer%5==0)
    {
        update_monster();
    }
    ui.set_view(View_matrix);
    if (Status==2) return true;
    else return false;
}
bool GameLogic::update_monster(){
    if(freeze) return true;
    float x=last_move[0] ,y=last_move[1] ;
    glm::mat4 md = ui.get_md(monster_index);
    glm::vec3 pos(md[3]);
    while(true)
    {
        if(check_forMonsterUpdata(pos.x+x, pos.y+y))//could move
        {
            last_move[0] = x;last_move[1] = y;
            ui.translate_obj(monster_index,glm::vec3(x,y,0));
            glm::vec3 pos1(Camera_pos);
            if((pow(pos.x+x-pos1.x,2)+pow(pos.y+y-pos1.y,2))<1)
                Status = 2;
            return true;
        }
        switch(rand()%4)
        {
            case 0://Up
                x = Step;y = 0;break;
            case 1://down
                x = -Step;y = 0;break;
            case 2://left
                x = 0;y = -Step;break;
            case 3://right
                x = 0;y = Step;break;
            default:
                break;
        }
    }
    return false;
}
bool GameLogic::forward()//move forward in 2D plane
{
    glm::vec3 look_vec= Lookat - Camera_pos;
    look_vec = glm::vec3(look_vec.x,look_vec.y,0);//set z value to 0, to avoid fly
    look_vec = glm::normalize(look_vec)*Step;
    glm::mat4 matrix = glm::lookAt(Camera_pos+look_vec,Lookat+look_vec,Up_vec);
    if(check_forUpdata(Camera_pos.x+look_vec.x,Camera_pos.y+look_vec.y))
    {
        Camera_pos = Camera_pos+look_vec;
        Lookat = Lookat+look_vec;
        View_matrix = matrix;
        ui.translate_obj(Player_index,look_vec);
    }
    return true;
}
bool GameLogic::backward()//move backward
{
    glm::vec3 look_vec= Lookat - Camera_pos;
    look_vec = glm::vec3(look_vec.x,look_vec.y,0);//set z value to 0, to avoid fly
    look_vec = -glm::normalize(look_vec)*Step;
    glm::mat4 matrix = glm::lookAt(Camera_pos+look_vec,Lookat+look_vec,Up_vec);
    if(check_forUpdata(Camera_pos.x+look_vec.x,Camera_pos.y+look_vec.y))
    {
        Camera_pos = Camera_pos+look_vec;
        Lookat = Lookat+look_vec;
        View_matrix = matrix;
        ui.translate_obj(Player_index,look_vec);
    }
    return true;
}
bool GameLogic::left()//move left
{
    glm::vec3 left_vec = glm::cross(Up_vec,Lookat - Camera_pos);
    left_vec = glm::vec3(left_vec.x,left_vec.y,0);//set z value to 0, to avoid fly
    left_vec = glm::normalize(left_vec)*Step;
    glm::mat4 matrix = glm::lookAt(Camera_pos+left_vec,Lookat+left_vec,Up_vec);
    if(check_forUpdata(Camera_pos.x+left_vec.x,Camera_pos.y+left_vec.y))
    {
        Camera_pos = Camera_pos+left_vec;
        Lookat = Lookat+left_vec;
        View_matrix = matrix;
        ui.translate_obj(Player_index,left_vec);
    }
    return true;
}
bool GameLogic::right()//move right
{
    glm::vec3 right_vec = glm::cross(Lookat - Camera_pos,Up_vec);
    right_vec = glm::vec3(right_vec.x,right_vec.y,0);//set z value to 0, to avoid fly
    right_vec = glm::normalize(right_vec)*Step;
    glm::mat4 matrix = glm::lookAt(Camera_pos+right_vec,Lookat+right_vec,Up_vec);
    if(check_forUpdata(Camera_pos.x+right_vec.x,Camera_pos.y+right_vec.y))
    {
        Camera_pos = Camera_pos+right_vec;
        Lookat = Lookat+right_vec;
        View_matrix = matrix;
        ui.translate_obj(Player_index,right_vec);
    }
    return true;
}
bool GameLogic::turnL(float in)//move view direction left,in = [-1 1]
{
    float theta = in*M_PI/150;
    glm::vec3 look_vec= Lookat - Camera_pos;
    look_vec = glm::rotate(look_vec, theta, Up_vec);
    ui.translate_obj(Player_index,-(glm::normalize(Lookat-Camera_pos)*Step*1.5f+ Camera_pos));
    ui.rotate_obj(Player_index, glm::vec3(0.0f,0.0f,theta));
    Lookat = Camera_pos + look_vec;
    View_matrix = glm::lookAt(Camera_pos,Lookat,Up_vec);
    ui.translate_obj(Player_index,glm::normalize(Lookat-Camera_pos)*Step*1.5f+ Camera_pos);
    return true;
}
bool GameLogic::turnUp(float in)//move view direction up, in [-1 1]
{
    float theta = in*M_PI/150;
    glm::vec3 right_vec = glm::cross(Lookat - Camera_pos,Up_vec);
    glm::vec3 look_vec= Lookat - Camera_pos;
    look_vec = glm::rotate(look_vec, theta, right_vec);
    Up_vec = glm::rotate(Up_vec, theta, right_vec);
    ui.translate_obj(Player_index,-(glm::normalize(Lookat-Camera_pos)*Step*1.5f+ Camera_pos));
    ui.rotate_obj(Player_index, glm::vec3(theta,0.0f,0.0f));
    Lookat = Camera_pos + look_vec;
    View_matrix = glm::lookAt(Camera_pos,Lookat,Up_vec);
    ui.translate_obj(Player_index,glm::normalize(Lookat-Camera_pos)*Step*1.5f+ Camera_pos);
    return true;
}
bool GameLogic::check_forUpdata(float x,float y)//assume we use box collider for all obj,use sphere collider for user
{
    x = x+0.5;y = y+0.5;
    float radius = Step;
    float Xs[3] = {x,x+radius,x-radius};
    float Ys[3] = {y,y+radius,y-radius};
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
        {
            float x1 = Xs[i];float y1 = Ys[j];
            if((x1<0||x1>=Origin_Map.size())||(y1<0||y1>=Origin_Map[0].size()))//out of range
                continue;
            else if(ui.get_objtype(hash_forOBJ[floor(x1)][floor(y1)])=="wall")//detect wall
                return false;
            else if(ui.get_objtype(hash_forOBJ[floor(x1)][floor(y1)])=="gate")//detect gate
                Status = 2;
            else if(ui.get_objtype(hash_forOBJ[floor(x1)][floor(y1)])=="key")//detect key
            {
                if(hash_forOBJ[floor(x1)][floor(y1)]!=key_index&&switchKey)//another key
                {
                    switchKey = false;
                    ui.switch_model(key_index, hash_forOBJ[floor(x1)][floor(y1)] );
                    //swich model between new key and current one
                }
            }
            else if(ui.get_objtype(hash_forOBJ[floor(x1)][floor(y1)])=="door")//detect door
            {
                if(key_index!=-1)//open door
                {
                    if(!ui.cmp_key(key_index, hash_forOBJ[floor(x1)][floor(y1)]))
                        return false;
                    ui.set_Invisible(hash_forOBJ[floor(x1)][floor(y1)]);//open door
                    ui.set_Invisible(key_index);//destroy current key
                }
                else return false;
            }
        }
    return true;
}
bool GameLogic::check_forMonsterUpdata(float x,float y)//assume we use box collider for all obj,use sphere collider for user
{
    x = x+0.5;y = y+0.5;
    float radius = Step;
    float Xs[3] = {x,x+radius,x-radius};
    float Ys[3] = {y,y+radius,y-radius};
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
        {
            float x1 = Xs[i];float y1 = Ys[j];
            if((x1<0||x1>=Origin_Map.size())||(y1<0||y1>=Origin_Map[0].size()))//out of range
                return false;
            else if(ui.get_objtype(hash_forOBJ[floor(x1)][floor(y1)])=="wall")//detect wall
                return false;
            else if(ui.get_objtype(hash_forOBJ[floor(x1)][floor(y1)])=="gate")//detect gate
                return false;
            else if(ui.get_objtype(hash_forOBJ[floor(x1)][floor(y1)])=="key")//detect key
                return false;
            else if(ui.get_objtype(hash_forOBJ[floor(x1)][floor(y1)])=="door")//detect door
                return false;
        }
    return true;
}
GameLogic::~GameLogic(){
    
}
