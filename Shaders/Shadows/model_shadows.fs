#version 330 core
in vec4 v_FragPos;

uniform vec3 v_LightPos;
uniform float f_FarPlane;

void main() {
    float lightDistance = length(v_FragPos.xyz - v_LightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / f_FarPlane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}