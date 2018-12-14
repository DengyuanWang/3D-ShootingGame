//
//  GUI_OpenGL.hpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/14/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef GUI_OpenGL_hpp
#define GUI_OpenGL_hpp

#include <iostream>

#include "../GameLogic/Game_Obj.hpp"


using std::ifstream;
using namespace std;
class GUI_OpenGL{
private:
    static vector<string> Model_names;
    bool init_tag;
    GLchar* VertexSource;
    GLchar* FragmentSource;
    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint ShaderProgram;
    vector<GLuint> Vaos;
    vector<GLuint> Vbos;
    vector<GLuint> Texs;
    int Model_num;
    vector<int> NumVerts;//for model1
    Animated_Model *playerModel;
    float currentFrame;
public:
    float Aspect;
    GUI_OpenGL();
    ~GUI_OpenGL();
    bool init_Opengl();
    void init_source();
    void loadShader(GLuint shaderID, const GLchar* shaderSource);
    void linkshader();
    bool load_models();//load and append
    void set_current_frame(float frame);
    bool draw_model(glm::mat4 view,glm::mat4 model,string model_name,string texture);//draw all model
    bool draw_player(float currentFrame, glm::mat4 model, glm::mat4 view, glm::mat4 proj, bool running);//draw all model
    bool switch_model(int src,int dst);
    bool clear_screen();
    char* readShaderSource(const char* shaderFile);
};

#endif /* GUI_OpenGL_hpp */
