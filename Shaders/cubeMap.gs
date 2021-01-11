//geometry shader that takes one triangle and outputs 3x6= 18 triangles in total(one triangle per face)

#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_CubeMap[6];
out vec2 v_TexCoords;

in VS_OUT {
    vec2 v_TexCoords;
} gs_in[];



void main() {
    for(int face = 0; face < 6; ++face) {
        gl_Layer = face; 
        for(int i = 0; i < 3; ++i) {
            v_TexCoords = gs_in[i].v_TexCoords;;
            gl_Position = u_CubeMap[face] *  gl_in[i].gl_Position;
            EmitVertex();
        }    
        EndPrimitive();
    }
} 