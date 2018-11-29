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
public:
    float Aspect;
    GUI_OpenGL();
    ~GUI_OpenGL();
    bool init_Opengl();
    void init_source();
    void loadShader(GLuint shaderID, const GLchar* shaderSource);
    void linkshader();
    bool load_model(Game_Obj &obj_in);//load and append
    bool draw_model(Game_Obj &obj_in,glm::mat4 view,int tag);//draw all model
    bool switch_model(int src,int dst);
    bool clear_screen();
    char* readShaderSource(const char* shaderFile);
};

#endif /* GUI_OpenGL_hpp */
