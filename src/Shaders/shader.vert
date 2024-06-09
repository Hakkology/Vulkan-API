#version 450 // GLSL version 4.5

layout (location = 0) in vec3 pos; // Output colour for vertex (location is required)

// // Triangle vertex positions (will put in to vertex buffer later)
// vec3 positions[3] = vec3 [](
//     vec3(0.0, -0.4, 0.0),
//     vec3(0.4, 0.4, 0.0),
//     vec3(-.4, .4, 0)
// );

// // Triangle vertex colours
// vec3 colours[3] = vec3[](
//     vec3(1.0, 0.0, 0.0),
//     vec3(0.0, 1.0, 0.0),
//     vec3(0.0, 0.0, 1.0)
// );

void main(){
    gl_Position = vec4(pos, 1);
    //fragColour = colours[gl_VertexIndex];
}