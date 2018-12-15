//
//  Animated_Model.cpp
//  3D_MazeGame
//
//  Created by 王登远 on 11/17/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Animated_Model.hpp"

using namespace glm;

Animated_Model::Animated_Model() {}
Animated_Model::~Animated_Model() {}

bool Animated_Model::LoadModel(const std::string &modelfn, GLuint s) {
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
                              aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (scene) {
        inverseTransform = scene->mRootNode->mTransformation.Inverse();
        success = InitFromScene(scene);
    } else {
        std::cerr << "Error parsing collada file " << modelfn << importer.GetErrorString() << std::endl;
    }

    cout << "Player Model Loaded " << numIndices << " indices " << numBones << " bones" << endl;

    glBindVertexArray(0);

    return success;
}

bool Animated_Model::InitFromScene(const aiScene *scene) {
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

bool Animated_Model::InitMesh(const aiMesh *inMesh, vector<glm::vec3> &positions, vector<glm::vec3> &normals,
                            vector<glm::vec2> &texCoords, vector<Animated_Model::VertexBoneData> &bones,
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

void Animated_Model::LoadBones(const aiMesh *inMesh, vector<Animated_Model::VertexBoneData> &bones) {
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

void Animated_Model::CalcInterpScaling(aiVector3D &out, float animTime, const aiNodeAnim *nodeAnim) {
    if(nodeAnim->mNumScalingKeys == 1){
        out = nodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint scalingIndex = FindScaling(animTime, nodeAnim);
    uint nextIndex = scalingIndex + 1;
    assert(nextIndex < nodeAnim->mNumScalingKeys);
    float deltaT = (float)(nodeAnim->mScalingKeys[nextIndex].mTime - nodeAnim->mScalingKeys[scalingIndex].mTime);
    float factor = (animTime - (float)nodeAnim->mScalingKeys[scalingIndex].mTime) / deltaT;
//    printf("FACTOR: %f\n", factor);
//    assert(factor >= 0.0f && factor <= 1.0f);
    const aiVector3D start = nodeAnim->mScalingKeys[scalingIndex].mValue;
    const aiVector3D end = nodeAnim->mScalingKeys[nextIndex].mValue;
    aiVector3D delta = end - start;
    out = start + factor * delta;
}

void Animated_Model::CalcInterpRotating(aiQuaternion &out, float animTime, const aiNodeAnim *nodeAnim) {
    if(nodeAnim->mNumRotationKeys == 1){
        out = nodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint rotationIndex = FindRotation(animTime, nodeAnim);
    uint nextIndex = rotationIndex + 1;
    assert(nextIndex < nodeAnim->mNumRotationKeys);
    float deltaT = (float)(nodeAnim->mRotationKeys[nextIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime);
    float factor = (animTime - (float)nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaT;
//    assert(factor >= 0.0f && factor <= 1.0f);
    const aiQuaternion &start = nodeAnim->mRotationKeys[rotationIndex].mValue;
    const aiQuaternion &end = nodeAnim->mRotationKeys[nextIndex].mValue;
    aiQuaternion::Interpolate(out, start, end, factor);
    out = out.Normalize();
}

void Animated_Model::CalcInterpPosition(aiVector3D &out, float animTime, const aiNodeAnim *nodeAnim) {
    if(nodeAnim->mNumPositionKeys == 1){
        out = nodeAnim->mPositionKeys[0].mValue;
        return;
    }

    uint positionIndex = FindPosition(animTime, nodeAnim);
    uint nextIndex = positionIndex + 1;
    assert(nextIndex < nodeAnim->mNumPositionKeys);
    float deltaT = (float)(nodeAnim->mPositionKeys[nextIndex].mTime - nodeAnim->mPositionKeys[positionIndex].mTime);
    float factor = (animTime - (float)nodeAnim->mPositionKeys[positionIndex].mTime) / deltaT;
//    assert(factor >= 0.0f && factor <= 1.0f);
    const aiVector3D start = nodeAnim->mPositionKeys[positionIndex].mValue;
    const aiVector3D end = nodeAnim->mPositionKeys[nextIndex].mValue;
    aiVector3D delta = end - start;
    out = start + factor * delta;
}

uint Animated_Model::FindScaling(float animTime, const aiNodeAnim *nodeAnim) {
    assert(nodeAnim->mNumScalingKeys > 0);

    for(uint i = 0; i < nodeAnim->mNumScalingKeys - 1; i++){
        if(animTime < (float)nodeAnim->mScalingKeys[i+1].mTime) {
            return i;
        }
    }
    assert(0);
    return 0;
}

uint Animated_Model::FindRotation(float animTime, const aiNodeAnim *nodeAnim) {
    assert(nodeAnim->mNumRotationKeys > 0);

    for(uint i = 0; i < nodeAnim->mNumRotationKeys - 1; i++){
        if(animTime < (float)nodeAnim->mRotationKeys[i+1].mTime) {
            return i;
        }
    }
    assert(0);
    return 0;
}

uint Animated_Model::FindPosition(float animTime, const aiNodeAnim *nodeAnim) {
    for(uint i = 0; i < nodeAnim->mNumPositionKeys - 1; i++){
        if(animTime < (float)nodeAnim->mPositionKeys[i+1].mTime) {
            return i;
        }
    }
    assert(0);
    return 0;
}

const aiNodeAnim *Animated_Model::FindNodeAnim(const aiAnimation *animation, const string nodeName) {
    for(uint i = 0; i < animation->mNumChannels; i++){
        const aiNodeAnim *node = animation->mChannels[i];
        if(string(node->mNodeName.data) == nodeName){
            return node;
        }
    }
    return nullptr;
}

void Animated_Model::ReadNodeHierarchy(float animTime, const aiNode *node, const aiMatrix4x4 &parentTransform) {
    string nodeName(node->mName.data);
    const aiAnimation *animation = scene->mAnimations[0]; // Only one animation per model...
    aiMatrix4x4 nodeTransformation = node->mTransformation;

    const aiNodeAnim *nodeAnim = FindNodeAnim(animation, nodeName);

    if(nodeAnim) {
        aiMatrix4x4 scalingMat;
        aiMatrix4x4 rotateMat;
        aiMatrix4x4 translateMat;

        // Scaling transformation
        // Don't really need scaling, but it is included here for completeness.
        aiVector3D scaling;
        CalcInterpScaling(scaling, animTime, nodeAnim);
        aiMatrix4x4::Scaling(scaling, scalingMat);

        // Rotation Transform
        aiQuaternion quatRotation;
        CalcInterpRotating(quatRotation, animTime, nodeAnim);
        rotateMat = aiMatrix4x4(quatRotation.GetMatrix());


        // Translation Transform
        aiVector3D translation;
        CalcInterpPosition(translation, animTime, nodeAnim);
        aiMatrix4x4::Translation(translation, translateMat);

        nodeTransformation = translateMat * rotateMat * scalingMat;// scalingMat * rotateMat * translateMat;
    }

    aiMatrix4x4 globalTransformation = parentTransform * nodeTransformation;

    if(boneMapping.find(nodeName) != boneMapping.end()){
        uint boneIndex = boneMapping[nodeName];
        boneInfo[boneIndex].finalTransform =  inverseTransform * globalTransformation * boneInfo[boneIndex].boneOffset;
    }

    for(uint i = 0; i < node->mNumChildren; i++){
        ReadNodeHierarchy(animTime, node->mChildren[i], globalTransformation);
    }
}

glm::mat4 Animated_Model::convertMatrix(const aiMatrix4x4 &mat) {
    return (glm::make_mat4(&mat.a1));
}

void Animated_Model::BoneTransform(float seconds, vector<glm::mat4> &transforms) {
    aiMatrix4x4 identity = aiMatrix4x4(); // Sets to identity

    float ticksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float timeInTicks = seconds * ticksPerSecond;
    float animTime = fmod(timeInTicks, (float)scene->mAnimations[0]->mDuration);

    ReadNodeHierarchy(animTime, scene->mRootNode, identity);

    transforms.resize(numBones);

    for(uint i = 0; i < numBones; i++){
        transforms[i] = convertMatrix(boneInfo[i].finalTransform);
    }
}

void Animated_Model::Render(float currentFrame, glm::mat4 model, glm::mat4 view, glm::mat4 proj, bool running) {
    glBindVertexArray(vao);
    vector<mat4> boneTransforms;
    if(!running){currentFrame = 0.4f;}

    BoneTransform(currentFrame, boneTransforms);
    for(int i = 0; i < boneTransforms.size(); i++){
        assert(i < 100);
        glUniformMatrix4fv(boneLocations[i], 1, GL_TRUE, glm::value_ptr(boneTransforms[i]));
    }

    GLint uniAnimated = glGetUniformLocation(shader, "animated");
    glUniform1i(uniAnimated, 1);

    // FIXING WEIRD MODEL UPSIDE DOWN BUG
    model = glm::translate(model, vec3(0,-1,0));
    model = glm::rotate(model, 1 * glm::half_pi<float>(), vec3(1,0,0));
    model = glm::scale(model, vec3(2,2,2));
    GLint unicolor = glGetUniformLocation(shader, "inColor");
    glm::vec3 colVec(0.2f, 0.2f, 0.2f);
    glUniform3fv(unicolor, 1, glm::value_ptr(colVec));

    GLint uniModel = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

    GLint uniView = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    GLint uniProj = glGetUniformLocation(shader, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

    glUniform1i(uniAnimated, 0);
    glBindVertexArray(0);
}

void Animated_Model::VertexBoneData::AddBoneData(uint boneID, float weight) {
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
