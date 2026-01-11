#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

layout (location = 0) out vec3 fragNormal;
layout (location = 1) out vec3 fragPos;
layout (location = 2) out vec2 fragTexCoord;

layout(push_constant) uniform PushConsts {
    mat4 mvp;
    vec4 lightDir;
    vec4 lightColor;
    vec4 ambientLight;
    vec4 objectColor;
} pushConsts;

void main(){
    gl_Position = pushConsts.mvp * vec4(pos, 1.0);
    fragNormal = inNormal;
    fragPos = pos;
    fragTexCoord = inTexCoord;
}