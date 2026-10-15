#version 330 core

// Output colors (rgba)
out vec4 FragColor;

// Inputs from vertex shader (color and texture)
in vec3 color;
in vec2 texCoord;

in vec3 Normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec4 lightColor;

uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight() {
    vec3 lightVector = lightPos - crntPos;
    float distance = length(lightVector);
    float a = 0.41f; // intensity
    float b = 0.015f; // reach
    // cg intensity equation
    float intensity = 1.0f / (a * distance * distance + b * distance + 1.0f);

    float ambient = 0.20f;

    vec3 normal = normalize(Normal);
    // angle of position from pixel to light
    vec3 lightDirection = normalize(lightVector);
    // cosine of normal vector of pixel and lightDirection (percent of brightness)
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // max spec light intensity
    float specularLight = 0.50f;
    // same as angle for light, but to camera
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(tex0, texCoord) * (diffuse * intensity + ambient) + texture(tex1, texCoord).r * specular * intensity) * lightColor;
}

vec4 directLight() {
    float ambient = 0.20f;

    vec3 normal = normalize(Normal);
    // angle of position from pixel to light
    vec3 lightDirection = normalize(vec3(0.0f, 1.0f, 1.0f));
    // cosine of normal vector of pixel and lightDirection (percent of brightness)
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // max spec light intensity
    float specularLight = 0.50f;
    // same as angle for light, but to camera
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(tex0, texCoord) * (diffuse + ambient) + texture(tex1, texCoord).r * specular) * lightColor;
}

vec4 spotLight() {
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    float ambient = 0.20f;

    vec3 normal = normalize(Normal);
    // angle of position from pixel to light
    vec3 lightDirection = normalize(lightPos - crntPos);
    // cosine of normal vector of pixel and lightDirection (percent of brightness)
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // max spec light intensity
    float specularLight = 0.50f;
    // same as angle for light, but to camera
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    return (texture(tex0, texCoord) * (diffuse * intensity + ambient) + texture(tex1, texCoord).r * specular * intensity) * lightColor;
}

void main() {
    FragColor = pointLight();
}