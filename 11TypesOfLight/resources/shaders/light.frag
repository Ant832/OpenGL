#version 330 core

// Output colors (rgba)
out vec4 FragColor;

uniform vec4 lightColor;

void main() {
    FragColor = lightColor;
}