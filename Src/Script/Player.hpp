//
//  Player.hpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <vector>
#include "Component.hpp"
using namespace std;
class Player: public Component{
public:
    bool Death_tag;//default as false
    float Health;//0~100
    float Speed;//default as 1
    vector<string> Abilities;//default as "walk"
    glm::vec4 ViewAt_vector;//in player's coordination
    glm::vec4 eye_pos_offset;//in player's coordination
    glm::mat4 view_matrix;
    float angle;
    Player();
    void Update(UI_Event &UIEvent, void *ptr_in);
    void update_view(UI_Event &UIEvent, void*ptr_in);
    void test(){
        cout<<"test player virtual"<<endl;
    }
    bool check_collision(glm::vec4 pos1,glm::vec4 pos2, glm::vec3 size1,glm::vec3 size2);
};
#endif /* Player_hpp */
