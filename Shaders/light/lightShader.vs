#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;


out VS_OUT {
    vec2 v_TexCoords;
    vec3 v_Position;
    vec3 aPos;
    mat4 u_ViewProjection;
    mat4 u_model;
} vs_out;


uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main(){
    vs_out.v_TexCoords = a_TexCoords;
    vs_out.v_Position = vec3(u_Model * vec4(a_Position, 1.0f));
    vs_out.aPos = a_Position;
    vs_out.u_model = u_Model;
    vs_out.u_ViewProjection = u_ViewProjection;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);


}