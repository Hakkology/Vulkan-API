#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

layout (location = 0) out vec3 fragNormal;
layout (location = 1) out vec3 fragPos;
layout (location = 2) out vec2 fragTexCoord;

layout(push_constant) uniform PushConsts {
    mat4 mvp;
    mat4 model;
    vec4 objectColor;
} pushConsts;

layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 lightSpaceMatrix;
    vec4 lightDir;
    vec4 lightColor;
    vec4 ambientLight;
} ubo;

void main(){
    gl_Position = pushConsts.mvp * vec4(pos, 1.0);
    fragNormal = normalize(mat3(pushConsts.model) * inNormal);
    fragPos = vec3(pushConsts.model * vec4(pos, 1.0));
    fragTexCoord = inTexCoord;
}