#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColour;

layout(push_constant) uniform PushConsts {
    mat4 mvp;
    vec4 lightDir; // Direction of the light
    vec4 lightColor; // Color of the light
    vec4 ambientLight; // Ambient light (rgb, intensity)
    vec4 objectColor; // Object color
} pushConsts;

void main(){
    // Ambient
    // Ambient
    float ambientStrength = pushConsts.ambientLight.w;
    vec3 ambient = ambientStrength * pushConsts.ambientLight.rgb; 
    
    // Diffuse
    // Assuming lightDir is vector TO the light.
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(-pushConsts.lightDir.xyz); // Negate to get vector TO light 
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pushConsts.lightColor.rgb;

    // Combine
    vec3 objectColor = pushConsts.objectColor.rgb; 
    vec3 result = (ambient + diffuse) * objectColor;

    outColour = vec4(result, 1.0);    
}