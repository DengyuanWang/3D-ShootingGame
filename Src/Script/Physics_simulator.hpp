//
//  Physics_simulator.hpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 12/3/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Physics_simulator_hpp
#define Physics_simulator_hpp

#include <iostream>
#include <vector>
#include "Component.hpp"
using namespace std;
class Physics_simulator: public Component{
public:
    glm::vec4 velocity;//save current velocity of obj
    glm::vec4 gravity;//save gravity affects the obj
    glm::vec4 acceleration;//save acceleration of obj
    glm::vec4 V_stash;
    glm::mat4 M_stash;
    glm::vec4 Last_movement;
    bool enable_gravity;//true for enable
    glm::vec4 calcu_movement(float dT);
public:
    Physics_simulator(void* ptr_in);
    void Update(UI_Event &UIEvent,void* ptr_in);
};

#endif /* Physics_simulator_hpp */
