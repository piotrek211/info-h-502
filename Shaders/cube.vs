
//cube.vs and .fs are the shaders used for calculating the reflected values on the ceiling marble.

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out vec3 v_Normal;
out vec3 v_Position;
out vec2 v_TexCoords;


uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_Normal;

void main(){
    v_TexCoords = a_TexCoords;
    v_Normal = mat3(u_Normal) * a_Normal;
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}