#include <iostream>
#include <cmath>
#include <tuple>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "texture.hpp"
#include "camera.hpp"

#define STRING(x) #x
#define XSTRING(x) STRING(x)    // macro expansion CMAKE definition SOURCE_ROOT

using std::cout;
using std::endl;
using std::sqrt;

// Normal vecotrs (Unit vectors/length 1) - help calculate how light acts on certain object
// either perpendicular to surface of triangle (face normals (flat shaded))
// or other ways (vertex normals (smooth shaded)- perpendicular to plane created by all adjacent vertices)
GLfloat vertices[] =
{
    //    Coordinates    /      Colors         /    Texture Coords   /      Normals
    -1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

GLuint nonNormalIndices[] =
{   // vertex order
    0, 1, 2,
    0, 2, 3,
};

GLuint indices[] =
{
    0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

GLfloat lightVertices[] =
{   // coords
    -0.1f, -0.1f,  0.1f,    // front left bottom
    -0.1f, -0.1f, -0.1f,    // back left bottom
     0.1f, -0.1f, -0.1f,    // back right bottom
     0.1f, -0.1f,  0.1f,    // front right bottom
    -0.1f,  0.1f,  0.1f,    // front left top
    -0.1f,  0.1f, -0.1f,    // back left top
     0.1f,  0.1f, -0.1f,    // back right top
     0.1f,  0.1f,  0.1f     // front right top
};

GLuint lightIndices[] =
{
    0, 1, 2,    // bottom back left
    0, 2, 3,    // bottom front right
    0, 4, 7,    // front side top left
    0, 7, 3,    // front side bottom right
    3, 7, 6,    // right side top left
    3, 6, 2,    // right side bottom right
    2, 6, 5,    // back side top
    2, 5, 1,    // back side bottom
    1, 5, 4,    // left side top left
    1, 4, 0,    // left side bottom right
    4, 5, 6,    // top back
    4, 6, 7     // top front
};

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core library rather than compatibility (legacy)
    
    const unsigned int width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    const unsigned int height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // glad loading needs config for openGL
    gladLoadGL();

    glViewport(0, 0, width, height); // area of drawing on window
    std::string vertexShaderPath = "/resources/shaders/default.vert";
    std::string fragmentShaderPath = "/resources/shaders/default.frag";
    Shader shaderProgram((XSTRING(SOURCE_ROOT) + vertexShaderPath).c_str(), (XSTRING(SOURCE_ROOT) + fragmentShaderPath).c_str());

    // Generate VAo and bind
    VAO VAO1;
    VAO1.Bind();

    // Generate VBO and EBO with vertices and indices
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // Vertex attrribute, values per vertex, value type, coordinates as ints, stride of vertices, offset
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

    // Unbind to prevent modifying with other functions
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    std::string lightVertexShaderPath = "/resources/shaders/light.vert";
    std::string lightFragmentShaderPath = "/resources/shaders/light.frag";
    Shader lightShader((XSTRING(SOURCE_ROOT) + lightVertexShaderPath).c_str(), (XSTRING(SOURCE_ROOT) + lightFragmentShaderPath).c_str());

    VAO lightVAO;
    lightVAO.Bind();

    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));

    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);

    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


    std::string imgPath1 = "/resources/textures/planks.png";
    std::string imgPath2 = "/resources/textures/planksSpec.png";
    // image, texture type, texture slot, texture format, pixel type
    Texture plank((XSTRING(SOURCE_ROOT) + imgPath1).c_str(), GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture plankSpec((XSTRING(SOURCE_ROOT) + imgPath2).c_str(), GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
    // shader, uniform, unit
    plank.texUnit(shaderProgram, "tex0", 0);
    plankSpec.texUnit(shaderProgram, "tex1", 1);
    
    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    while (!glfwWindowShouldClose(window)) {

        // Sky blue
        glClearColor(0.24f, 0.63f, 0.95f, 1.0f);
        // Using command on color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        double time = glfwGetTime();

        lightPos = glm::vec3(std::cos(time), 0.5f, std::sin(time));
        lightModel = glm::translate(glm::mat4(1.0f), lightPos);

        // Tell OpenGL which shader program to use
        shaderProgram.Activate();

        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));

        camera.Matrix(shaderProgram, "camMatrix");
      
        plank.Bind();
        plankSpec.Bind();
        
        // Bind VAO for OpenGL to use
        VAO1.Bind();

        // primitive, how many indices to draw, data type, index of indicies
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        

        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix");
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices)/sizeof(int), GL_UNSIGNED_INT, 0);


        // Update image each frame
        glfwSwapBuffers(window);

        // Take care of all GLFW events
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    plank.Delete();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}