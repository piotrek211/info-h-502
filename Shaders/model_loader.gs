
//geometry shader for placing 3 runways by only having the position of the most left one in the scene.

#version 330 core
layout(triangles) in;
layout (triangle_strip, max_vertices=9) out;

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

       for (float j = 0.0; j <= 88.0; j = j + 44.0) {
            for(int i = 0; i < gl_in.length(); i++) {
                vec4 interVec4 = gs_in[i].u_Model * (vec4(gs_in[i].a_Position, 1.0) + vec4(j, 0.0, 0.0, 0.0));
                gl_Position = gs_in[i].u_ViewProjection * interVec4;
        
                v_NormalCoord = gs_in[i].v_NormalCoord;
                v_Position = vec3(interVec4);
                v_TexCoords = gs_in[i].v_TexCoords;
                m_TBN = gs_in[i].m_TBN;

                EmitVertex();
            }
            EndPrimitive();  //runway ended
       }
}