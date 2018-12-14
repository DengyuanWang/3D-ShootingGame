//
//  UI.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/14/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "UI.hpp"
//bool Active_tag;//true for windo alive, false for no window alive
//SDL_Window *Window;//window handler
//SDL_GLContext Context;//window context handler
//SDL_Event WindowsEvent;//window event
UI::UI()
{
    Active_tag = false;
    Fullscreen = false;
    ScreenWidth = 800;
    ScreenHeight = 600;
    Aspect = ScreenWidth/(float)ScreenHeight;
}
UI::~UI(){
    if(Active_tag==true)//if user have not close window,close it
    {
        SDL_GL_DeleteContext(GL_context);
        SDL_DestroyWindow(Window);
        SDL_Quit();
        Active_tag = false;
    }
}
bool UI::open_window()//init SDL and open a window
{
    //Init SDL
    SDL_Init(SDL_INIT_VIDEO);  //Initialize Graphics (for OpenGL)
    
    //Print the version of SDL we are using
    SDL_version comp; SDL_version linked;
    SDL_VERSION(&comp); SDL_GetVersion(&linked);
    printf("\nCompiled against SDL version %d.%d.%d\n", comp.major, comp.minor, comp.patch);
    printf("Linked SDL version %d.%d.%d.\n", linked.major, linked.minor, linked.patch);
    
    //Ask SDL to get a recent version of OpenGL (3.2 or greater)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    //Create a window (offsetx, offsety, width, height, flags)
    Window = SDL_CreateWindow("My OpenGL Program", 100, 100,
                                          ScreenWidth, ScreenHeight, SDL_WINDOW_OPENGL);
    if (!Window){
        printf("Could not create window: %s\n", SDL_GetError());
        return EXIT_FAILURE; //Exit as SDL failed
    }

    GL_context = SDL_GL_CreateContext(Window); //Bind OpenGL to the window
    SDL_ShowCursor(0);
//  TODO
    SDL_SetRelativeMouseMode(SDL_TRUE);
    return true;
}
bool UI::close_window()//close window
{
    if(Active_tag==true)//if user have not close window,close it
    {
        SDL_GL_DeleteContext(GL_context);
        SDL_DestroyWindow(Window);
        SDL_Quit();
        Active_tag = false;
        return true;
    }
    return false;
}

bool UI::full_screen_switch()//switch between full screen or not
{
    Fullscreen = false;// !Fullscreen;
    SDL_SetWindowFullscreen(Window, Fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    return true;
}
void UI::catch_input_event()//get event for keyboard input or click
{
    string result;
    int  X=0;
    int  Y=0;
    const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
    while(SDL_PollEvent(&WindowsEvent))//loop until get current event
    {
        SDL_GetRelativeMouseState(&X, &Y);
        if(keyboard_state_array[SDL_SCANCODE_ESCAPE])
            rst.set_event("Esc");
        else if(keyboard_state_array[SDL_SCANCODE_F1])
            rst.set_event("FullScreen_switch");
        else if(keyboard_state_array[SDL_SCANCODE_W] && keyboard_state_array[SDL_SCANCODE_A])
        {rst.set_event("Up");rst.set_event("Left");}
        else if (keyboard_state_array[SDL_SCANCODE_W] && keyboard_state_array[SDL_SCANCODE_D])
        {rst.set_event("Up");rst.set_event("Right");}
        else if (keyboard_state_array[SDL_SCANCODE_S] && keyboard_state_array[SDL_SCANCODE_A])
        {rst.set_event("Down");rst.set_event("Left");}
        else if (keyboard_state_array[SDL_SCANCODE_S] && keyboard_state_array[SDL_SCANCODE_D])
        {rst.set_event("Down");rst.set_event("Right");}
        else if(keyboard_state_array[SDL_SCANCODE_W] && !keyboard_state_array[SDL_SCANCODE_S])
            rst.set_event("Up");
        else if(keyboard_state_array[SDL_SCANCODE_S] && !keyboard_state_array[SDL_SCANCODE_W])
            rst.set_event("Down");
        else if(keyboard_state_array[SDL_SCANCODE_A] && !keyboard_state_array[SDL_SCANCODE_D])
            rst.set_event("Left");
        else if(keyboard_state_array[SDL_SCANCODE_D] && !keyboard_state_array[SDL_SCANCODE_A])
            rst.set_event("Right");
        else if(keyboard_state_array[SDL_SCANCODE_SPACE])
            rst.set_event("Shoot");
        else if(keyboard_state_array[SDL_SCANCODE_C])
            rst.set_event("Jump");
        else rst.set_event("None");
       //if(!rst.check_event("None")||X!=0||Y!=0) break;
        break;
    }
    rst.mouse_status[0] = X;rst.mouse_status[1] = Y;
}
bool UI::Init_openGL()
{
    gui_gl.Aspect = Aspect;
    if(!gui_gl.init_Opengl()) {cout<<"init openGL error"<<endl;return false;}
    if(!gui_gl.load_models())//load all models in lib/models
    {
        cout<<"load model error"<<endl;
        return false;
    }
    return true;
}
bool UI::Draw(vector<string> model_names,vector<glm::mat4> model_matrixs,vector<string> textures)
{
    gui_gl.clear_screen();
    for(int i=0;i<model_names.size();i++)
    {
        
        if(!gui_gl.draw_model(View_matrix,model_matrixs[i],model_names[i],textures[i]))
        {
            cout<<"draw model fail"<<i<<endl;
            return false;
        }
    }
    
    SDL_GL_SwapWindow(Window);
    return true;
}
