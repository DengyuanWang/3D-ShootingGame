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

public:
    UI_Event rst;
    glm::mat4 View_matrix;
    bool Active_tag;//true for window alive, false for no window alive
//Window and OS
    UI();
    ~UI();
    bool open_window();//init SDL and open a window
    bool close_window();//close window
    bool full_screen_switch();//switch between full screen or not
    void catch_input_event();//get event for keyboard input or click
//GUI and OpenGL
    bool Draw(vector<string> model_names,vector<glm::mat4> model_matrixs,vector<string> textures);
    bool Init_openGL();
};


#endif /* UI_hpp */
