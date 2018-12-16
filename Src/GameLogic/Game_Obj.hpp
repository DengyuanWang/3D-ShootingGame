//
//  Game_Obj.hpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/15/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Game_Obj_hpp
#define Game_Obj_hpp
#include "../Graphics/Animated_Model.hpp"
#include "../Script/Includer.h"
using namespace std;
class Game_Obj{
private:

    static vector<string> Type_list;
    int Type_index;//index in Type_list
    //homogeneous matrix saved Obj info
    glm::mat4 Model;//model position rotation etc

public:
    int Index;//index of current obj in all obj list
    //we assume all object use box collider;
    glm::vec3 collider_size;
    glm::vec4 collider_offset;
    glm::vec4 collider_center;//relative coordinates
    vector<int> collision_indices;
    int key[3]={0,0,0};
    string Model_name;
    string Texture_name;
    //bool Display_tag;//Needed to Display or not
    vector<Component*> Comp_list;//list of component
    glm::vec3 FaceAt,Pos,Scales;
//basic functionalities
    Game_Obj();
    void translate(glm::vec3 T_vec);
    void local_rotation(glm::vec3 angles);//rotate angles for each axises
    void local_translate(glm::vec3 T_vec);
    void scale(glm::vec3 S_vec);//local scale
    glm::mat4 get_Model(){return Model;}
    void apply(glm::mat4 M4){Model = Model*M4;}//apply a mat4 to model matrix
    void set_mat(glm::mat4 M4){Model = M4;}//set model mat
    bool Specify_type(string name);//specifygameobject type
    void set_index(int index){Index = index;}//index in objects list
    ~Game_Obj();
//Specific functionalities to involve game component
    bool attach_component(string component_name);
    Component* get_component(string component_name);
    void Update(UI_Event &input_event);
    bool check_collision(Game_Obj *ptr);//check collision with other Game obj
// If model has animation

    string get_type(){return Type_list[Type_index];};
};



#endif /* Game_Obj_hpp */
