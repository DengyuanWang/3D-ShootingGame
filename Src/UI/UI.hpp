//
//  UI.hpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/14/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef UI_hpp
#define UI_hpp
//lib included
#include <iostream>
#include <SDL2/SDL.h>
//files included
#include "UI_Event.hpp"
#include "../Graphics/GUI_OpenGL.hpp"
using namespace std;

class UI{
private:
    GUI_OpenGL gui_gl;
    bool Fullscreen;
    int ScreenWidth;
    int ScreenHeight;
    float Aspect;
    vector<Game_Obj> G_objs;
    SDL_Window *Window;//window handler
    SDL_GLContext GL_context;//window OpenGL context handler
    SDL_Event WindowsEvent;//window event
    glm::mat4 View_matrix;
public:
    bool Active_tag;//true for windo alive, false for no window alive
    UI();
    ~UI();
    bool open_window();//init SDL and open a window
    bool close_window();//close window
    bool full_screen_switch();//switch between full screen or not
    UI_Event get_input_event();//get event for keyboard input or click
    bool Draw();
    bool Init_openGL();
    bool Add_Game_obj(string model_file);
    bool Load_modelsToOpenGL();
    bool set_visible(int index);
    bool set_Invisible(int index);
    bool rotate_obj(int index,glm::vec3 angles);
    bool translate_obj(int index,glm::vec3 vec);
    bool uniformScale_obj(int index,float factor);
    int get_objnum(){return G_objs.size();}
    string get_objtype(int index)
        {if(index>=G_objs.size()||!G_objs[index].Display_tag) return ""; return G_objs[index].get_model_name();}
    void set_view(glm::mat4 in){View_matrix = in;}
    bool switch_model(int src,int dst);
    bool apply_mat4(int index,glm::mat4 M4);
    glm::mat4 get_md(int index){return G_objs[index].get_Model();}
    bool set_key(int index,int key[3]){memcpy(G_objs[index].key,key,3*sizeof(int));return true;}
    bool cmp_key(int key_index,int door_index){     return G_objs[key_index].key[0]==G_objs[door_index].key[0]&&
                                                        G_objs[key_index].key[1]==G_objs[door_index].key[1]&&
                                                        G_objs[key_index].key[2]==G_objs[door_index].key[2];}
    bool getkey(int index,int key[3]){ memcpy(key,G_objs[index].key,3*sizeof(int)); return true;}
    
};


#endif /* UI_hpp */
