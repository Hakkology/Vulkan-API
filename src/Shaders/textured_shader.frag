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
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-pushConsts.lightDir.xyz);
    
    // Wrapped Diffuse
    float wrappedDiff = dot(N, L) * 0.5 + 0.5;
    float diff = wrappedDiff * wrappedDiff;
    
    vec3 diffuse = diff * pushConsts.lightColor.rgb * pushConsts.lightColor.w;
    vec3 ambient = pushConsts.ambientLight.rgb * pushConsts.ambientLight.w;

    vec4 texColor = texture(texSampler, fragTexCoord);
    vec3 baseColor = texColor.rgb * pushConsts.objectColor.rgb;

    vec3 result = (ambient + diffuse) * baseColor;
    result = pow(result, vec3(1.0/2.2));

    outColour = vec4(result, 1.0);    
}