#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

layout (location = 0) out vec3 fragNormal;
layout (location = 1) out vec3 fragPos;
layout (location = 2) out vec2 fragTexCoord;

layout(push_constant) uniform PushConsts {
    mat4 mvp;
    mat4 model;
    vec4 objectColor;
} pushConsts;

layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 lightSpaceMatrix;
    vec4 lightDir;
    vec4 lightColor;
    vec4 ambientLight;
    float time;
    float padding[3];
} ubo;

void main(){
    vec3 animatedPos = pos;
    // Cape animation:
    // Assuming plane is originally XZ.
    // We displace Y (local normal direction) based on Z (local "length").
    // Frequency 5.0, Speed 5.0, Amplitude 0.1 (adjustable)
    // We dampen the effect at the top (z close to some value?)
    // drawPlane(0, size). Vertices are around center.
    // Let's just apply simple wave for now.
    
    // Simple wave: displace Y based on Z.
    // animatedPos.y += sin(pos.z * 2.0 + ubo.time * 5.0) * 0.1;

    // Modulate amplitude based on Z position to pin the "top" of the cape.
    // pos.z ranges from -0.4 (top) to +0.4 (bottom) in local space.
    // We map -0.4 -> 0.0 amplitude, +0.4 -> 1.0 amplitude.
    
    float amplitudeFactor = clamp((pos.z + 0.4) / 0.8, 0.0, 1.0);
    // Square it for smoother falloff? or just linear. Linear is fine.
    
    animatedPos.y += sin(pos.z * 10.0 + ubo.time * 8.0) * 0.1 * amplitudeFactor;

    gl_Position = pushConsts.mvp * vec4(animatedPos, 1.0);
    
    // Normal calculation (approximate or recalculated)
    // Since we deform geometry, normal changes. 
    // Ideally we compute analytical gradient: d(offset)/dz = ...
    // But for a simple effect, rotating the normal slightly or ignoring it (for ambient) might suffice.
    // Let's try to adjust normal simply.
    // dy/dz = cos(...) * 5.0 * 0.1 = 0.5 * cos(...)
    // Tangent is (0, 0.5*cos, 1). Normal is (-slope, 1).
    // Let's keep original normal for now (0,1,0) -> rotated by model.
    fragNormal = normalize(mat3(pushConsts.model) * inNormal);
    fragPos = vec3(pushConsts.model * vec4(animatedPos, 1.0));
    fragTexCoord = inTexCoord;
}