#include "VBO.hpp"

VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
    glGenBuffers(1, &ID);

    glBindBuffer(GL_ARRAY_BUFFER, ID); // Binding - make certain obj the current obj
    // functions that modify certain types of objects modify the binded obj
    // Store vertices in VBO
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
    glDeleteBuffers(1, &ID);
}