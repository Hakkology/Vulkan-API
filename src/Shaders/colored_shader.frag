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

void main(){
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-pushConsts.lightDir.xyz);
    
    // --- Wrapped Diffuse (Half-Lambert) ---
    // Instead of max(dot, 0), we map [-1, 1] to [0, 1]
    // bu sayede ışık almayan (arka) yüzeylerde bile bir gradyan oluşur.
    float wrappedDiff = dot(N, L) * 0.5 + 0.5;
    
    // Wrapped lighting'in karesini alarak kontrastı koruyalım (standard Half-Lambert)
    float diff = wrappedDiff * wrappedDiff;
    
    vec3 diffuse = diff * pushConsts.lightColor.rgb * pushConsts.lightColor.w;

    // --- Ambient ---
    // Ambient'ı biraz daha düşük tutup diffuse'un arkaya sarkan kısmına güveneceğiz.
    vec3 ambient = pushConsts.ambientLight.rgb * pushConsts.ambientLight.w;

    // Final
    vec3 result = (ambient + diffuse) * pushConsts.objectColor.rgb;

    // Gamma correction
    result = pow(result, vec3(1.0/2.2));

    outColour = vec4(result, 1.0);    
}