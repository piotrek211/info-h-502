//Reflection on the ceiling

#version 330 core
out vec4 v_FragColor;

in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoords;

uniform vec3 u_ViewPos;
uniform samplerCube u_Room;    //color cube map of the environment
uniform sampler2D texture_albedo1;  //original color

void main() {
    
    float alpha = 0.25; //how much of the impact has the room color on the final color.
    vec3 Incident = normalize(v_Position - u_ViewPos);
    vec3 Reflection = reflect(Incident, normalize(v_Normal));
    vec3 baseColor = texture(texture_albedo1, v_TexCoords).rgb;
    vec3 roomColor = texture(u_Room, Reflection).rgb;
    vec3 color = (1 - alpha) * baseColor + alpha * roomColor;
    v_FragColor = vec4(color, 1.0);
}