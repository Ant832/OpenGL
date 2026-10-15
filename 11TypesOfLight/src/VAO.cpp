#include "VAO.hpp"

VAO::VAO() {
    glGenVertexArrays(1, &ID); // generate and bind VAO BEFORE VBO
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    VBO.Bind();

    // Configure VAO
    // Vertex attrribute, values per vertex, value type,  coordinates as ints, stride of vertices, offset
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout); // pos of vertex attrib
    VBO.Unbind();
}

void VAO::Bind() {
    glBindVertexArray(ID);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}