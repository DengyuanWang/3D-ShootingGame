//
//  GUI_OpenGL.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/14/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//


#include "GUI_OpenGL.hpp"

#if defined(__APPLE__)
const string shaderPath = "/lib/GLshader/";
const string modelPath = "/lib/models/";
const string woodPath = "lib/textures/wood.bmp";
const string brickPath = "lib/textures/brick.bmp";
const string playerPath = "lib/models/ninja.dae";
#elif defined(__linux__)
const string shaderPath = "../lib/GLshader/";
const string modelPath = "../lib/models/";
const string woodPath = "../lib/textures/wood.bmp";
const string brickPath = "../lib/textures/brick.bmp";
const string playerPath = "../lib/models/ninja.dae";
#endif


vector<string>
        GUI_OpenGL::Model_names = {"knot", "sphere", "cube", "teapot", "teapotLowPoly", "player"};

GUI_OpenGL::GUI_OpenGL() {
    Model_num = 0;
    init_tag = false;
}

bool GUI_OpenGL::init_Opengl() {
    //Load OpenGL to SDL
    if (gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        cout << "OpenGL loaded" << endl;
        cout << "Vendor:  " << glGetString(GL_VENDOR) << endl;
        cout << "Renderer:" << glGetString(GL_RENDERER) << endl;
        cout << "Version: " << glGetString(GL_VERSION) << endl;
    } else {
        printf("ERROR: Failed to initialize OpenGL context.\n");
        exit(0);
    }
    //init vertexSource and fragmentSource
    init_source();
    //load shader
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    loadShader(VertexShader, VertexSource);
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    loadShader(FragmentShader, FragmentSource);
    //Join the vertex and fragment shaders together into one program
    linkshader();

    //glGenVertexArrays(1, &Vao); //Create a VAO
    //glBindVertexArray(Vao); //Bind the above created VAO to the current context
    init_tag = true;



    //// Allocate Texture 0 (Wood) ///////
    SDL_Surface *surface = SDL_LoadBMP(woodPath.c_str());
    if (surface == NULL) { //If it failed, print the error
        printf("Error: \"%s\"\n", SDL_GetError());
        return 1;
    }
    GLuint tex0;
    glGenTextures(1, &tex0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex0);

    //What to do outside 0-1 range
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Load the texture into memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture
    SDL_FreeSurface(surface);
    //// End Allocate Texture ///////

    //// Allocate Texture 1 (Brick) ///////
    SDL_Surface *surface1 = SDL_LoadBMP(brickPath.c_str());
    if (surface == NULL) { //If it failed, print the error
        printf("Error: \"%s\"\n", SDL_GetError());
        return 1;
    }
    GLuint tex1;
    glGenTextures(1, &tex1);

    //Load the texture into memory
    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, tex1);
    //What to do outside 0-1 range
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface1->w, surface1->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface1->pixels);
    glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture

    SDL_FreeSurface(surface1);
    //// End Allocate Texture ///////
    Texs.push_back(tex0);
    Texs.push_back(tex1);

    return true;
}

void GUI_OpenGL::init_source() {
    VertexSource = readShaderSource("textured-Vertex");
    FragmentSource = readShaderSource("textured-Fragment");
}

void GUI_OpenGL::loadShader(GLuint shaderID, const GLchar *shaderSource) {
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    //Let's double check the shader compiled
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status); //Check for errors
    if (!status) {
        char buffer[512];
        glGetShaderInfoLog(shaderID, 512, NULL, buffer);
        printf("Shader Compile Failed. Info:\n\n%s\n", buffer);
    }
}

void GUI_OpenGL::linkshader() {
    //Join the vertex and fragment shaders together into one program
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glBindFragDataLocation(ShaderProgram, 0, "outColor"); // set output
    glLinkProgram(ShaderProgram); //run the linker
}

bool GUI_OpenGL::load_models() {
    for (int i = 0; i < Model_names.size(); i++)//load all models
    {
        ifstream modelFile;

        if (Model_names[i] == "player") {
            Model_loader *playerLoader = new Model_loader();
            playerVao = playerLoader->LoadModel(playerPath, ShaderProgram);

        } else {
            string full_filename = modelPath + Model_names[i] + ".txt";
            modelFile.open(full_filename);
            int numLines = 0;
            modelFile >> numLines;
            vector<float> Model_data;
            for (int i = 0; i < numLines; i++) {
                float k;
                modelFile >> k;
                Model_data.push_back(k);
            }
            printf("Mode line count: %d\n", numLines);

            float *modelData = Model_data.data();
            NumVerts.push_back(numLines / 8);

            GLuint vao;
            glGenVertexArrays(1, &vao); //Create a VAO
            Vaos.push_back(vao);
            glBindVertexArray(Vaos[Vaos.size() - 1]); //Bind the above created VAO to the current context

            GLuint vbo;
            glGenBuffers(1, &vbo);
            Vbos.push_back(vbo);
            glBindBuffer(GL_ARRAY_BUFFER, Vbos[Vbos.size() - 1]);
            glBufferData(GL_ARRAY_BUFFER, numLines * sizeof(float), modelData, GL_STATIC_DRAW);

            GLint posAttrib = glGetAttribLocation(ShaderProgram, "position");
            glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
            //Attribute, vals/attrib., type, isNormalized, stride, offset
            glEnableVertexAttribArray(posAttrib);

            GLint normAttrib = glGetAttribLocation(ShaderProgram, "inNormal");
            glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
            glEnableVertexAttribArray(normAttrib);

            GLint texAttrib = glGetAttribLocation(ShaderProgram, "inTexcoord");
            glEnableVertexAttribArray(texAttrib);
            glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));

            glEnable(GL_DEPTH_TEST);
        }
        Model_num++;
    }
    return true;
}

bool GUI_OpenGL::clear_screen() {
    glClearColor(.2f, 0.4f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return true;
}

bool GUI_OpenGL::draw_model(glm::mat4 view, glm::mat4 model, string model_name, string texture) {
    int tag = 0;
    int index = 0;//need to decode from model_name;
    vector<string>::iterator Mindex = std::find(Model_names.begin(), Model_names.end(), model_name);
    if (Mindex != Model_names.end())
        index = (int) std::distance(Model_names.begin(), Mindex);//event none
    else return false;
    if (texture == "wood")
        tag = 0;
    else if (texture == "brick")
        tag = 1;
    else tag = -1;
    glUseProgram(ShaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texs[0]);//wood
    glUniform1i(glGetUniformLocation(ShaderProgram, "tex0"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Texs[1]);//brick
    glUniform1i(glGetUniformLocation(ShaderProgram, "tex1"), 1);


    GLint uniModel = glGetUniformLocation(ShaderProgram, "model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));


    GLint uniView = glGetUniformLocation(ShaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = glm::perspective(3.14f / 4, Aspect, .2f, 50.0f);
    //FOV, aspect ratio, near, far
    GLint uniProj = glGetUniformLocation(ShaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));


    GLint uniColor = glGetUniformLocation(ShaderProgram, "inColor");
    glm::vec3 colVec(0 / 255.0, 0 / 255.0, 0 / 255.0);
    glUniform3fv(uniColor, 1, glm::value_ptr(colVec));
    GLint uniTexID = glGetUniformLocation(ShaderProgram, "texID");
    glUniform1i(uniTexID, tag);


    glBindVertexArray(Vbos[index]);  //Bind the VAO for the shaders we are using
    glBindBuffer(GL_ARRAY_BUFFER, Vbos[index]);
    glDrawArrays(GL_TRIANGLES, 0, NumVerts[index]); //Number of vertices

    return true;
}

bool GUI_OpenGL::switch_model(int src, int dst) {
    //switch saved vertex data
    GLuint vao = Vaos[dst];
    GLuint vbo = Vbos[dst];
    int tmp = NumVerts[dst];
    NumVerts[dst] = NumVerts[src];
    Vaos[dst] = Vaos[src];
    Vbos[dst] = Vbos[src];
    Vaos[src] = vao;
    Vbos[src] = vbo;
    NumVerts[src] = tmp;
    return true;
}

GUI_OpenGL::~GUI_OpenGL() {
    if (init_tag) {
        glDeleteProgram(ShaderProgram);
        glDeleteShader(FragmentShader);
        glDeleteShader(VertexShader);
        for (int i = 0; i < (int) Vbos.size(); i++)
            glDeleteBuffers(1, &Vbos[i]);
        for (int i = 0; i < (int) Vaos.size(); i++)
            glDeleteVertexArrays(1, &Vaos[i]);
    }
}

char *GUI_OpenGL::readShaderSource(const char *shaderFile) {
    string name = shaderFile;
    name = shaderPath + name + ".glsl";
    FILE *fp;
    long length;
    char *buffer;

    // open the file containing the text of the shader code
    fp = fopen(name.c_str(), "r");

    // check for errors in opening the file
    if (fp == NULL) {
        printf("can't open shader source file %s\n", shaderFile);
        return NULL;
    }

    // determine the file size
    fseek(fp, 0, SEEK_END); // move position indicator to the end of the file;
    length = ftell(fp);  // return the value of the current position

    // allocate a buffer with the indicated number of bytes, plus one
    buffer = new char[length + 1];

    // read the appropriate number of bytes from the file
    fseek(fp, 0, SEEK_SET);  // move position indicator to the start of the file
    fread(buffer, 1, length, fp); // read all of the bytes

    // append a NULL character to indicate the end of the string
    buffer[length] = '\0';

    // close the file
    fclose(fp);

    // return the string
    return buffer;
}
