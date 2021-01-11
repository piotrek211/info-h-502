#version 330 core
out vec4 FragColor;

in float lifeTime;
void main() {
    FragColor = vec4(lifeTime, 0.0, 0.0, 1.0);
}