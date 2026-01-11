#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColour;

layout(push_constant) uniform PushConsts {
    mat4 mvp;
    vec4 lightDir;     // Direction (world space)
    vec4 lightColor;   // rgb = color, w = intensity
    vec4 ambientLight; // rgb = color, w = intensity
    vec4 objectColor;
} pushConsts;

void main(){
    // Ambient
    float ambientIntensity = pushConsts.ambientLight.w;
    vec3 ambient = ambientIntensity * pushConsts.ambientLight.rgb; 
    
    // Diffuse
    vec3 norm = normalize(fragNormal);
    // pushConsts.lightDir is the direction the light is pointing.
    // We need the direction FROM the fragment TO the light source.
    // For directional light, this is -lightDir.
    vec3 lightDir = normalize(-pushConsts.lightDir.xyz);
    
    float diff = max(dot(norm, lightDir), 0.0);
    float lightIntensity = pushConsts.lightColor.w;
    vec3 diffuse = diff * pushConsts.lightColor.rgb * lightIntensity;

    // Combine
    vec3 resColor = (ambient + diffuse) * pushConsts.objectColor.rgb;

    outColour = vec4(resColor, 1.0);    
}