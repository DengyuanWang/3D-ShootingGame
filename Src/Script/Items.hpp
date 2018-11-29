//
//  Items.hpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Items_hpp
#define Items_hpp

#include <iostream>
#include <vector>
#include "Component.hpp"
using namespace std;
class Items: public Component{
public:
    string Items_name;
    Items();
    void Update(Game_Events input_event);
};
class Final_gate: public Items{
    Final_gate();
    void Update(Game_Events input_event);
};

#endif /* Items_hpp */
