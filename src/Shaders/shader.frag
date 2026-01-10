#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;

layout(location = 0) out vec4 outColour;

layout(push_constant) uniform PushConsts {
    mat4 mvp;
    vec4 lightDir; // Direction of the light
    vec4 lightColor; // Color of the light
} pushConsts;

void main(){
    // Ambient
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0); 
    
    // Diffuse
    // Assuming lightDir is vector TO the light.
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(pushConsts.lightDir.xyz); 
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pushConsts.lightColor.rgb;

    // Combine
    vec3 objectColor = vec3(1.0, 0.5, 0.31); 
    vec3 result = (ambient + diffuse) * objectColor;

    outColour = vec4(result, 1.0);    
}