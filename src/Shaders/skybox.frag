#version 450

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColour;

// We use Set 1 Binding 0 for the texture, consistent with other textured materials
layout(set = 1, binding = 0) uniform sampler2D texSampler;

void main(){
    // Sample texture directly. No lighting/shadows.
    vec4 color = texture(texSampler, fragTexCoord);
    
    // Using raw color for vibrancy in skybox.
    outColour = color;
}
