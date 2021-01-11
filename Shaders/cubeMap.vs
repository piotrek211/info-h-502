
//cubeMap shaders are for rendering the scene into the cube maps which will be used later on for calculation the reflected value on the ceiling.

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;  

out VS_OUT {
    vec2 v_TexCoords;
} vs_out;


uniform mat4 u_Model;

void main(){
    vs_out.v_TexCoords = a_TexCoords;
    gl_Position = u_Model * vec4(a_Position, 1.0);
}