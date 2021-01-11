#version 330 core
out vec4 FragColor;
in vec2 v_TexCoords;
// material parameters
uniform sampler2D texture_albedo1;


void main() {		
      FragColor = texture(texture_albedo1, v_TexCoords);
     // FragColor = vec4(v_TexCoords, 0.0f, 1.0f);
} 