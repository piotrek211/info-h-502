

//particle shaders are for drawing the particles in the correct spot, by taking into account the ball placement

#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec4 position;

out float lifeTime;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform vec3 u_objectPos;
uniform vec3 u_velocity;


void main() {
    float scale = 0.05;
    gl_Position = u_ViewProjection * u_Model * vec4((vertex_position*scale) + u_objectPos + position.xyz, 1.0);
    lifeTime = position.w;
}