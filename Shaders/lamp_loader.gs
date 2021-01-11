
//Geometry shader that place 6 lamps in one draw call.
#version 330 core
layout(triangles) in;
layout (triangle_strip, max_vertices=18) out;

in VS_OUT {
    vec2 v_TexCoords;
    vec3 v_NormalCoord;
    vec3 v_Position;
    mat3 m_TBN;
    vec3 v_Tangent;
    vec3 v_Bitangent;
    vec3 a_Position;
    mat4 u_ViewProjection;
    mat4 u_Model;
} gs_in[];

out vec2 v_TexCoords;
out vec3 v_NormalCoord;
out vec3 v_Position;   //worldPos
out mat3 m_TBN;

 
 void main() {

     for (float j = 0.0; j >= -160.0; j = j - 160.0){
         for (float k = 0.0; k <= 100.0; k = k + 50.0) {
            for(int i = 0; i < gl_in.length(); i++) {
                gl_Position = gs_in[i].u_ViewProjection* gs_in[i].u_Model * (vec4(gs_in[i].a_Position, 1.0) + vec4(k, 0.0, j, 0.0));
                v_NormalCoord = gs_in[i].v_NormalCoord;
                v_Position = gs_in[i].v_Position;
                v_TexCoords = gs_in[i].v_TexCoords;
                m_TBN = gs_in[i].m_TBN;
                EmitVertex();
            }
             EndPrimitive(); //one lamp ended
        }
    }

}