#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColour;

layout(push_constant) uniform PushConsts {
    mat4 mvp;
    vec4 objectColor;
} pushConsts;

layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 lightSpaceMatrix;
    vec4 lightDir;
    vec4 lightColor;
    vec4 ambientLight;
} ubo;

layout(set = 0, binding = 1) uniform sampler2D shadowMap;

float calculateShadow(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5;
    
    if(projCoords.z > 1.0) return 0.0;

    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float bias = max(0.005 * (1.0 - dot(normalize(fragNormal), normalize(-ubo.lightDir.xyz))), 0.0005);
    
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main(){
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-ubo.lightDir.xyz);
    
    float wrappedDiff = dot(N, L) * 0.5 + 0.5;
    float diffStrength = wrappedDiff * wrappedDiff;
    
    vec4 fragPosLightSpace = ubo.lightSpaceMatrix * vec4(fragPos, 1.0);
    float shadow = calculateShadow(fragPosLightSpace);

    vec3 diffuse = (1.0 - shadow) * diffStrength * ubo.lightColor.rgb * ubo.lightColor.w;
    vec3 ambient = ubo.ambientLight.rgb * ubo.ambientLight.w;

    vec3 result = (ambient + diffuse) * pushConsts.objectColor.rgb;
    result = pow(result, vec3(1.0/2.2));

    outColour = vec4(result, 1.0);    
}