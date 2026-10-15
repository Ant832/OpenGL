#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <stb/stb_image.hpp>

#include "shaderClass.hpp"


class Texture {
public:
    GLuint ID;
    GLenum type;
    GLuint unit;
    Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

    // Assigns texture unit to a texture
    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();

};

#endif