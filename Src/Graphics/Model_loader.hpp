//
//  Model_loader.hpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/17/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Model_loader_hpp
#define Model_loader_hpp
#include <iostream>
#include <SDL2/SDL.h>
//#include <SDL2_mixer/SDL_mixer.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include "../../lib/Includer.h"
using namespace std;
class Model_loader{
private:
    vector<float[3]> vec_list;//position
    vector<float[3]> color_list;//rgb
    vector<float[3]> norm_list;//norm
};

#endif /* Model_loader_hpp */
