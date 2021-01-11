#version 330 core
layout(triangles) in;
layout (triangle_strip, max_vertices=18) out;

in VS_OUT {
    vec2 v_TexCoords;
    vec3 v_Position;
    vec3 aPos;
    mat4 u_ViewProjection;
    mat4 u_model;
} gs_in[];

out vec2 v_TexCoords;
 
 void main() {

  for(int i = 0; i < gl_in.length(); i++) {
    gl_Position = gl_in[i].gl_Position;
    v_TexCoords = gs_in[i].v_TexCoords;

    EmitVertex();
  }
  EndPrimitive();

   for(int i = 0; i < gl_in.length(); i++) {
     gl_Position = gs_in[i].u_ViewProjection* gs_in[i].u_model * (vec4(gs_in[i].aPos, 1.0) + vec4(50.0, 0.0, 0.0, 0.0));
     v_TexCoords = gs_in[i].v_TexCoords;
    EmitVertex();
  }
  EndPrimitive();


   for(int i = 0; i < gl_in.length(); i++) {
     gl_Position = gs_in[i].u_ViewProjection* gs_in[i].u_model * (vec4(gs_in[i].aPos, 1.0) + vec4(100.0, 0.0, 0.0, 0.0));
     v_TexCoords = gs_in[i].v_TexCoords;
    EmitVertex();
  }
  EndPrimitive();



  for(int i = 0; i < gl_in.length(); i++) {
     gl_Position = gs_in[i].u_ViewProjection* gs_in[i].u_model * (vec4(gs_in[i].aPos, 1.0) + vec4(0.0, 0.0, -160.0, 0.0));
     v_TexCoords = gs_in[i].v_TexCoords;
    EmitVertex();
  }
  EndPrimitive();


  for(int i = 0; i < gl_in.length(); i++) {
     gl_Position = gs_in[i].u_ViewProjection* gs_in[i].u_model * (vec4(gs_in[i].aPos, 1.0) + vec4(50.0, 0.0, -160.0, 0.0));
     v_TexCoords = gs_in[i].v_TexCoords;
    EmitVertex();
  }
  EndPrimitive();


  for(int i = 0; i < gl_in.length(); i++) {
     gl_Position = gs_in[i].u_ViewProjection* gs_in[i].u_model * (vec4(gs_in[i].aPos, 1.0) + vec4(100.0, 0.0, -160.0, 0.0));
     v_TexCoords = gs_in[i].v_TexCoords;
    EmitVertex();
  }
  EndPrimitive();






}