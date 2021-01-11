
//model_loader shaders are used for loading models that have PBR materials.

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in vec3 a_Bitangent;  

out vec2 v_TexCoords;
out vec3 v_NormalCoord;
out vec3 v_Position;   //worldPos
out mat3 m_TBN;


/// for geometry shader. If geometry shader is not used then we need to send the data to the fragment shader, that's why the out list here has duplicates////
out VS_OUT {
    vec2 v_TexCoords;
    vec3 v_NormalCoord;
    vec3 v_Position;
    mat3 m_TBN;
    vec3 v_Tangent;
    vec3 v_Bitangent;
    vec3 a_Position;
    mat4 u_ViewProjection;
    mat4 u_Model;
} vs_out;


uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_Normal; //tranpose(M^-1) used for keeping the scaling of the normals coherent.


//TBN matrix for normal mapping
mat3 getTBN() {
    vec3 Tangent = normalize(vec3(u_Model * vec4(a_Tangent, 0.0)));
    vec3 Normal = normalize(vec3(u_Model * vec4(a_Normal, 0.0)));
    // re-orthogonalize T with respect to N
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 Bitangent = cross(Normal, Tangent);

    return mat3(Tangent, Bitangent, Normal);

}


void main(){
    m_TBN = getTBN(); // (Tangent, Bitangent, Normal) matrix

    v_TexCoords = a_TexCoords;
    vs_out.v_TexCoords = a_TexCoords;
    vs_out.m_TBN = m_TBN;
    vs_out.v_Tangent = a_Tangent;
    vs_out.v_Bitangent = a_Bitangent;


    v_NormalCoord = mat3(u_Normal) * a_Normal;
    v_Position = vec3(u_Model * vec4(a_Position, 1.0f));

    vs_out.v_NormalCoord = v_NormalCoord;
    vs_out.v_Position = v_Position;
    vs_out.a_Position = a_Position;
    vs_out.u_Model = u_Model;
    vs_out.u_ViewProjection = u_ViewProjection;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}