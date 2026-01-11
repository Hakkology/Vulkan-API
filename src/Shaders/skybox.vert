#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

layout (location = 0) out vec2 fragTexCoord;

layout(push_constant) uniform PushConsts {
    mat4 mvp;
    mat4 model;
    vec4 objectColor;
} pushConsts;

void main(){
    // Use the MVP matrix as is. The CPU side should ensure the model is large enough
    // and centered on the camera to act as a skybox.
    gl_Position = pushConsts.mvp * vec4(pos, 1.0);
    
    // Pass texture coordinates directly to fragment shader
    fragTexCoord = inTexCoord;
}
