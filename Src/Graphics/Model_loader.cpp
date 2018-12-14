//
//  Model_loader.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/17/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Model_loader.hpp"

using namespace glm;

Model_loader::Model_loader() {}
Model_loader::~Model_loader() {}

GLuint Model_loader::LoadModel(const std::string &modelfn, GLuint s) {
    shader = s;

    bool success = false;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &posvb);
    glGenBuffers(1, &normvb);
    glGenBuffers(1, &texvb);
    glGenBuffers(1, &bonevb);
    glGenBuffers(1, &indexvb);

    scene = importer.ReadFile(modelfn.c_str(),
                              aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                              aiProcess_JoinIdenticalVertices);

    if (scene) {
        inverseTransform = scene->mRootNode->mTransformation.Inverse();
        success = InitFromScene(scene);
    } else {
        std::cerr << "Error parsing collada file " << modelfn << importer.GetErrorString() << std::endl;
    }

    if(!success) {
        std::cerr << "Error Initializing scene " << std::endl;
    }

    cout << "Player Model Loaded " << numIndices << " indices " << numBones << " bones" << endl;

    glBindVertexArray(0);

    return vao;
}

bool Model_loader::InitFromScene(const aiScene *scene) {
    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> texcoords;
    vector<VertexBoneData> bones;
    vector<uint> indices;

    uint nVertices = 0;
    uint nIndicies = 0;

    numIndices = scene->mMeshes[0]->mNumFaces * 3;

    nVertices = scene->mMeshes[0]->mNumVertices;
    nIndicies = numIndices;

    positions.reserve(nVertices);
    normals.reserve(nVertices);
    texcoords.reserve(nVertices);
    bones.resize(nVertices);
    indices.reserve(nIndicies);

    const aiMesh *pMesh = scene->mMeshes[0];
    InitMesh(pMesh, positions, normals, texcoords, bones, indices);

    glUseProgram(shader);

    //TODO Get proper shader IN names

    // POSITION
    GLint posAttrib = glGetAttribLocation(shader, "position");
    glBindBuffer(GL_ARRAY_BUFFER, posvb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0])*positions.size(), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // TEXTURES
    GLint texAttrib = glGetAttribLocation(shader, "inTexcoord");
    glBindBuffer(GL_ARRAY_BUFFER, texvb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0])*texcoords.size(), texcoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // NORMALS
    GLint normAttrib = glGetAttribLocation(shader, "inNormal");
    glBindBuffer(GL_ARRAY_BUFFER, normvb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0])*normals.size(), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // BONESglm rotate with quaternion
    GLint boneIDAttrib = glGetAttribLocation(shader, "in_boneIDs");
    GLint boneWeightAttrib = glGetAttribLocation(shader, "in_boneWeights");
    glBindBuffer(GL_ARRAY_BUFFER, bonevb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0])*bones.size(), bones.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(boneIDAttrib, 4, GL_INT, sizeof(VertexBoneData), (void*)0);
    glVertexAttribPointer(boneWeightAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)(sizeof(int)*4));

    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(texAttrib);
    glEnableVertexAttribArray(normAttrib);
    glEnableVertexAttribArray(boneIDAttrib);
    glEnableVertexAttribArray(boneWeightAttrib);

    // Generate Index Array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexvb);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*indices.size(), indices.data(), GL_STATIC_DRAW);

    for(unsigned int i = 0; i < (sizeof(boneLocations) / sizeof(boneLocations[0])); i++){
        char name[128];
        memset(name, 0, sizeof(name));
        sprintf(name, "global_bones[%d]", i);
        boneLocations[i] = glGetUniformLocation(shader, name);

        if(boneLocations[i] < 0){
            std::cerr << "BoneLocation error: " << i << " " << boneLocations[i] << std::endl;
        }
    }

    return glGetError() != GL_NO_ERROR;

}

bool Model_loader::InitMesh(const aiMesh *inMesh, vector<glm::vec3> &positions, vector<glm::vec3> &normals,
                            vector<glm::vec2> &texCoords, vector<Model_loader::VertexBoneData> &bones,
                            vector<unsigned int> &indices) {
    const aiVector3D zeroVec(0.f, 0.f, 0.f);

    for (int i = 0; i < inMesh->mNumVertices; i++) {
        const aiVector3D *pPos = &(inMesh->mVertices[i]);
        const aiVector3D *pNormal = &(inMesh->mNormals[i]);
        const aiVector3D *pTex = inMesh->HasTextureCoords(0) ? &(inMesh->mTextureCoords[0][i]) : &zeroVec;

        positions.push_back(vec3(pPos->x, pPos->y, pPos->z));
        normals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
        texCoords.push_back(vec2(pTex->x, pTex->y));
    }

    LoadBones(inMesh, bones);

    for(int i = 0; i < inMesh->mNumFaces; i++){
        const aiFace &face = inMesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
}

void Model_loader::LoadBones(const aiMesh *inMesh, vector<Model_loader::VertexBoneData> &bones) {
    numBones = 0;
    for(uint i = 0; i < inMesh->mNumBones; i++){
        uint bIndex = 0;
        aiBone *pBone = inMesh->mBones[i];
        string bName(pBone->mName.data);

        if(boneMapping.find(bName) == boneMapping.end()){
            bIndex = numBones;
            numBones++;
            BoneInfo info;
            boneInfo.push_back(info);
            boneInfo[bIndex].boneOffset = pBone->mOffsetMatrix;
            boneMapping[bName] = bIndex;
        } else{
            bIndex = boneMapping[bName];
        }

        for(uint j = 0; j < pBone->mNumWeights; j++){
            uint vID = pBone->mWeights[j].mVertexId;
            float weight = pBone->mWeights[j].mWeight;
            bones[vID].AddBoneData(bIndex, weight);
        }
    }
}

void Model_loader::VertexBoneData::AddBoneData(uint boneID, float weight) {
    for(uint i = 0; i < sizeof(IDs) / sizeof(IDs[0]); i++){
        if(weights[i] == 0.0){
            IDs[i] = boneID;
            weights[i] = weight;
            return;
        }
    }
    // Should never happen...I hope.
    assert(0);
}
