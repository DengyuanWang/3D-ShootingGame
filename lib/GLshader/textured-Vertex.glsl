#version 150 core

in vec3 position;
//in vec3 inColor;
//const vec3 inColor = vec3(0.f,0.7f,0.f);
const vec3 inLightDir = normalize(vec3(-1,1,-1));
in vec3 inNormal;
in vec2 inTexcoord;
in ivec4 in_boneIDs;
in vec4 in_boneWeights;

out vec3 Color;
out vec3 vertNormal;
out vec3 pos;
out vec3 lightDir;
out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 inColor;

uniform bool animated;

const int MAX_BONES = 100;
uniform mat4 global_bones[MAX_BONES];

void animateModel(){
}

void main() {
    vec4 iPos = vec4(position, 1.0);
    vec4 iNorm = vec4(inNormal, 0.0);

    if(animated) {
        mat4 bonetransform = global_bones[in_boneIDs[0]] * in_boneWeights[0];
        bonetransform     += global_bones[in_boneIDs[1]] * in_boneWeights[1];
        bonetransform     += global_bones[in_boneIDs[2]] * in_boneWeights[2];
        bonetransform     += global_bones[in_boneIDs[3]] * in_boneWeights[3];

        iPos = bonetransform * vec4(position, 1.0);
        iNorm = (bonetransform * vec4(inNormal, 0.0));
    }

    Color = inColor;
    gl_Position = proj * view * model * iPos;
    pos = (view * model * iPos).xyz;
    lightDir = (view * vec4(inLightDir,0.0)).xyz; //It's a vector!
    vec4 norm4 = transpose(inverse(view*model)) * iNorm;
    vertNormal = normalize(norm4.xyz);
    texcoord = inTexcoord;
}
