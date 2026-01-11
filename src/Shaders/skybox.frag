#version 450

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColour;

// We use Set 1 Binding 0 for the texture, consistent with other textured materials
layout(set = 1, binding = 0) uniform sampler2D texSampler;

void main(){
    // Sample texture directly. No lighting, no shadows.
    vec4 color = texture(texSampler, fragTexCoord);
    
    // Apply simple Gamma Correction (Linear -> sRGB) if needed, 
    // consistent with other shaders that do pow(result, 1.0/2.2).
    // Assuming the input texture is sRGB, but if we want to be safe:
    // vec3 result = pow(color.rgb, vec3(1.0/2.2));
    // Actually, usually we want the raw color for skyboxes if they are images.
    // Let's stick to raw color for vibrancy.
    
    outColour = color;
}
