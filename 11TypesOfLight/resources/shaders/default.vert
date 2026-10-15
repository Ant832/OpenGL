#version 330 core
// Pos/Coords
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// texture vertices
layout (location = 2) in vec2 aTex;

layout (location = 3) in vec3 aNormal;

// Outputs color to Fragment Shader
out vec3 color;

out vec2 texCoord;

out vec3 Normal;
out vec3 crntPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    crntPos = vec3(model * vec4(aPos, 1.0f));

    // Outputs pos/coords of all vertices
    gl_Position = camMatrix * vec4(crntPos, 1.0);
    // Assigns colors from vertex data to "color"
    color = aColor;
    texCoord = aTex;
    Normal = mat3(model) * aNormal;
}