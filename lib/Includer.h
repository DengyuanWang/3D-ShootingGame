//
//  Includer.h
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Includer_h
#define Includer_h
#include <vector>
#include <unordered_map>
#include "glad/glad.h"
#include <SDL2/SDL_opengl.h>
// Include GLM
#define GLM_FORCE_RADIANS 1
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/quaternion.hpp"
struct ThreeDPOS{
    int x,y,z;
    ThreeDPOS(glm::vec3 in_vec): x(in_vec.x),y(in_vec.y),z(in_vec.z){};
    bool operator==(const ThreeDPOS& a) const
    {
        return (x == a.x && y == a.y&& z==a.z);
    }
};
namespace std {
    template <>
    struct hash<ThreeDPOS>
    {
        std::size_t operator()(const ThreeDPOS& k) const
        {
            using std::size_t;
            using std::hash;
            using std::string;
            
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:
            
            return ((hash<int>()(k.x)
                     ^ (hash<int>()(k.y) << 1)) >> 1)
            ^ (hash<int>()(k.z) << 1);
        }
    };
    
}
extern std::unordered_map<ThreeDPOS, std::vector<void*>> Hashmap;//mapping between 3D pos and GameObjs
extern void* Gobj_list;
extern void* Gevent_list;
#endif /* Includer_h */
