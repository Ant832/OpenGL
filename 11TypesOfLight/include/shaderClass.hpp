#ifndef SHADER_CLASS_HPP
#define SHADER_CLASS_HPP

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

using std::cout;
using std::endl;

std::string get_file_contents(const char* filename);

class Shader {
    void compileErrors(unsigned int shader, const char* type);
public:
    GLuint ID;  // new shader program
    Shader(const char* vertexFile, const char* fragmentFile);

    void Activate();
    void Delete();
};

#endif