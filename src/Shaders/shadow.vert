#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(push_constant) uniform PushConsts {
    mat4 mvp; 
    vec4 objectColor;
} pushConsts;

void main() {
    gl_Position = pushConsts.mvp * vec4(inPosition, 1.0);
}
