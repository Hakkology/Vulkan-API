#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColour;

layout(push_constant) uniform PushConsts {
    mat4 mvp;
    vec4 lightDir;
    vec4 lightColor;
    vec4 ambientLight;
    vec4 objectColor;
} pushConsts;

layout(binding = 1) uniform sampler2D texSampler;

void main(){
    // Ambient
    float ambientStrength = pushConsts.ambientLight.w;
    vec3 ambient = ambientStrength * pushConsts.ambientLight.rgb; 
    
    // Diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(-pushConsts.lightDir.xyz);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pushConsts.lightColor.rgb;

    // Combine
    vec4 texColor = texture(texSampler, fragTexCoord);
    vec3 objectColor = texColor.rgb * pushConsts.objectColor.rgb;

    vec3 result = (ambient + diffuse) * objectColor;

    outColour = vec4(result, 1.0);    
}