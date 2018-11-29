//
//  UI.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/14/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "UI.hpp"
#define DEBUG true
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
    Fullscreen = !Fullscreen;
    SDL_SetWindowFullscreen(Window, Fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    return true;
}
UI_Event UI::get_input_event()//get event for keyboard input or click
{
    string result;
    int  X=0;
    int  Y=0;
    while(SDL_PollEvent(&WindowsEvent))//loop until get current event
    {
        SDL_GetRelativeMouseState(&X, &Y);
        if (  (WindowsEvent.type == SDL_QUIT)||
            (WindowsEvent.type == SDL_KEYUP && WindowsEvent.key.keysym.sym == SDLK_ESCAPE))
            result = "Esc"; //Exit Game Loop
        else if (WindowsEvent.type == SDL_KEYDOWN && WindowsEvent.key.keysym.sym == SDLK_f)
            result = "FullScreen_switch";
        else if (WindowsEvent.type == SDL_KEYDOWN && WindowsEvent.key.keysym.sym == SDLK_w)
            result = "Up";
        else if (WindowsEvent.type == SDL_KEYDOWN && WindowsEvent.key.keysym.sym == SDLK_s)
            result = "Down";
        else if (WindowsEvent.type == SDL_KEYDOWN && WindowsEvent.key.keysym.sym == SDLK_a)
            result = "Left";
        else if (WindowsEvent.type == SDL_KEYDOWN && WindowsEvent.key.keysym.sym == SDLK_d)
            result = "Right";
        else if (WindowsEvent.type == SDL_KEYDOWN && WindowsEvent.key.keysym.sym == SDLK_SPACE)
            result = "Switch_key";
        else if (WindowsEvent.type == SDL_KEYDOWN && WindowsEvent.key.keysym.sym == SDLK_c)
            result = "Switch_freeze";
        else result = "None";//all other treated as none
        
        if(result!="None"||X!=0||Y!=0) break;
    }
    UI_Event rst(result);rst.mouse_status[0] = X;rst.mouse_status[1] = Y;
    return rst;
}
bool UI::Init_openGL()
{
    gui_gl.Aspect = Aspect;
    if(!gui_gl.init_Opengl()) {cout<<"init openGL error"<<endl;return false;}
    return true;
}
bool UI::Load_modelsToOpenGL()
{
    for(int i=0;i<G_objs.size();i++)
    {
        if(G_objs[i].Loaded_tag==false)//To simplify this, we do not allow to unload a loaded Obj
        {
            if(!gui_gl.load_model(G_objs[i])){cout<<"load model error"<<endl;return false;}
            G_objs[i].Loaded_tag = true;
        }
    }
  return true;
}
bool UI::Add_Game_obj(string model_file)
{
    Game_Obj new_obj(model_file);
    if(new_obj.get_model_name()=="")
    {
        cout<<"load model"+model_file+"failed"<<endl;
        return false;
    }
    else{
        new_obj.set_index(G_objs.size());
        G_objs.push_back(new_obj);
        return true;
    }
}
bool UI::set_visible(int index)
{
    if(index>=G_objs.size())
        return false;
    G_objs[index].Display_tag = true;
    return true;
}
bool UI::set_Invisible(int index)
{
    if(index>=G_objs.size())
        return false;
    G_objs[index].Display_tag = false;
    return true;
}
bool UI::uniformScale_obj(int index,float factor)
{
    if(index>=G_objs.size()) return false;
    G_objs[index].uniform_scale(factor);
    return true;
}
bool UI::rotate_obj(int index,glm::vec3 angles)
{
    if(index>=G_objs.size()) return false;
    G_objs[index].rotation(angles);
    return true;
}
bool UI::translate_obj(int index,glm::vec3 vec)
{
    if(index>=G_objs.size()) return false;
    G_objs[index].translate(vec);
    return true;
}
bool UI::apply_mat4(int index,glm::mat4 M4)
{
    if(index>=G_objs.size()) return false;
    G_objs[index].apply(M4);
    return true;
}
bool UI::switch_model(int src, int dst)
{
    string tmp = G_objs[dst].get_model_name();
    G_objs[dst].set_model_name(G_objs[src].get_model_name());
    G_objs[src].set_model_name(tmp);
    int tmpk[3];
    memcpy(tmpk, G_objs[dst].key, 3*sizeof(int));
    memcpy(G_objs[dst].key, G_objs[src].key , 3*sizeof(int));
    memcpy(G_objs[src].key, tmpk , 3*sizeof(int));
    bool tag =G_objs[dst].Display_tag;
    G_objs[dst].Display_tag = G_objs[src].Display_tag;
    G_objs[src].Display_tag = tag;
    return gui_gl.switch_model(src, dst);
}
bool UI::Draw()
{
    gui_gl.clear_screen();
    for(int i=0;i<G_objs.size();i++)
    {
        if(G_objs[i].Display_tag==true)//if need display that obj
            gui_gl.draw_model(G_objs[i],View_matrix,G_objs[i].texture_index);
    }
    SDL_GL_SwapWindow(Window);
    return true;
}
