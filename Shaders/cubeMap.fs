//We want to output color into the cube map so we need to take it from the texture.
//To simplify the things, for the reflection, only the albedo is used.

#version 330 core
out vec4 FragColor;
in vec2 v_TexCoords;
uniform sampler2D texture_albedo1;


void main() {		
      FragColor = texture(texture_albedo1, v_TexCoords);

} 