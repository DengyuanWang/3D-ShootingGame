//
//  Model_loader.hpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/17/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#ifndef Model_loader_hpp
#define Model_loader_hpp
#include <map>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL2/SDL.h>
//#include <SDL2_mixer/SDL_mixer.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include "../../lib/Includer.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/quaternion.hpp"

using namespace std;

#define BONES_PER_VERTEX 4

class Model_loader{
private:
    struct BoneInfo
    {
        aiMatrix4x4 boneOffset;
        aiMatrix4x4 finalTransform;

        BoneInfo()
        {
            boneOffset = aiMatrix4x4();
            finalTransform = aiMatrix4x4();
        }
    };
    struct VertexBoneData{
        uint IDs[BONES_PER_VERTEX] {};
        float weights[BONES_PER_VERTEX]{};

        VertexBoneData()
        {
            memset(IDs, 0, sizeof(IDs));
            memset(weights, 0, sizeof(weights));
        }
        void AddBoneData(uint boneID, float weight);
    };

    bool InitFromScene(const aiScene *scene);

    bool InitMesh(const aiMesh *inMesh,
                  vector<glm::vec3> &positions,
                  vector<glm::vec3> &normals,
                  vector<glm::vec2> &texCoords,
                  vector<VertexBoneData> &bones,
                  vector<unsigned int> &indices);
    void LoadBones(const aiMesh *inMesh, vector<VertexBoneData> &bones);

    GLuint vao,
            posvb,
            normvb,
            texvb,
            bonevb,
            indexvb;


    GLint boneLocations[100];

    GLuint shader;

    std::map<std::string, uint> boneMapping;
    uint numBones;
    uint numIndices;
    vector<BoneInfo> boneInfo;
    aiMatrix4x4 inverseTransform;

    const aiScene *scene;
    Assimp::Importer importer;

public:
    Model_loader();
    ~Model_loader();

    GLuint LoadModel(const std::string &modelfn, GLuint shader);

};

#endif /* Model_loader_hpp */
