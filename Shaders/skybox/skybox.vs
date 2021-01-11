
//Shader for drawing the skybox, by using the cube map 

#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 u_ViewProjection;

void main() {
    v_TexCoords = a_Position;
    vec4 pos = u_ViewProjection * vec4(a_Position, 1.0);
    gl_Position = pos.xyww; //setting z component to w so the depth is always 1.0
}  