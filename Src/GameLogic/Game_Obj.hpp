//
//  Game_Obj.hpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/15/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Game_Obj_hpp
#define Game_Obj_hpp
#include "../Graphics/Model_loader.hpp"

using namespace std;
class Game_Obj{
private:
    int Index;//index of current obj in all obj list
    static vector<string> Type_list;
    int Type_index;//index in Type_list
    //homogeneous matrix saved Obj info
    glm::mat4 Model;//model position rotation etc
    int NumVerts;
    float* Data_result=NULL;
    vector<float> Model_data;
    bool load_model(string name);
    string Model_name="";
public:
    int key[3]={0,0,0};
    int texture_index;
    bool Loaded_tag;//loaded to OpenGL or not
    bool Display_tag;//Needed to Display or not
    Game_Obj(string model_name);
    void translate(glm::vec3 T_vec);
    void rotation(glm::vec3 angles);//rotate angles for each axises
    void uniform_scale(float factor);
    glm::mat4 get_Model(){return Model;}
    int get_NumVerts(){return NumVerts;}
    int get_index(){return Index;}
    void set_index(int index){Index = index;}
    void set_model_name(string type){Type_index = load_model(type);}
    string get_model_name(){return Model_name;}
    float* get_Model_data();
    void apply(glm::mat4 M4){Model = Model*M4;}
    ~Game_Obj();
};



#endif /* Game_Obj_hpp */
