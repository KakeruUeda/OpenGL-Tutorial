#include "Matrix.h"
#include "Shape.h"
#include "ShapeIndex.h"
#include "SolidShape.h"
#include "SolidShapeIndex.h"
#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

GLboolean printShaderInfoLog(GLuint shader, const char *str)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        std::cerr << "Compile error in " << str << std::endl;
    }

    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
    if(bufSize > 1) {
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }

    return static_cast<GLboolean>(status);
}

GLboolean printProgramInfoLog(GLuint program)
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
        std::cerr << "Link error." << std::endl;

    GLsizei bufSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
    if(bufSize > 1) {
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }

    return static_cast<GLboolean>(status);
}

/**
 *  @param vsrc vertex shader source
 *  @param fsrc fragment shader source
 */
GLuint createProgram(const char *vsrc, const char *fsrc)
{
    const GLuint program(glCreateProgram());

    if(vsrc != NULL) {
        const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
        glShaderSource(vobj, 1, &vsrc, NULL);
        glCompileShader(vobj);

        if(printShaderInfoLog(vobj, "vertex shader"))
            glAttachShader(program, vobj);
        glDeleteShader(vobj);
    }

    if(fsrc != NULL) {
        const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
        glShaderSource(fobj, 1, &fsrc, NULL);
        glCompileShader(fobj);

        if(printShaderInfoLog(fobj, "fragment shader"))
            glAttachShader(program, fobj);
        glDeleteShader(fobj);
    }

    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "normal");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);

    if(printProgramInfoLog(program))
        return program;

    glDeleteProgram(program);
    return 0;
}

bool readShaderSource(const char *name, std::vector<GLchar> &buffer)
{
    if(name == NULL)
        return false;

    std::ifstream file(name, std::ios::binary);
    if(file.fail()) {
        std::cerr << "Can't open the file." << name << std::endl;
        return false;
    }

    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());

    buffer.resize(length + 1);

    file.seekg(0L, std::ios::beg);
    file.read(buffer.data(), length);
    buffer[length] = '\0';

    if(file.fail()) {
        std::cerr << "Can't read the file." << name << std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}

GLuint loadProgram(const char *vert, const char *frag)
{
    std::vector<GLchar> vsrc;
    const bool vstat(readShaderSource(vert, vsrc));

    std::vector<GLchar> fsrc;
    const bool fstat(readShaderSource(frag, fsrc));

    return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

constexpr Object::Vertex rectangleVertex[] = {
    {-0.5f, -0.5f},
    {0.5f,  -0.5f},
    {0.5f,  0.5f },
    {-0.5f, 0.5f }
};

constexpr Object::Vertex octahedronVertex[] = {
    {0.0f,  1.0f,  0.0f },
    {-1.0f, 0.0f,  0.0f },
    {0.0f,  -1.0f, 0.0f },
    {1.0f,  0.0f,  0.0f },
    {0.0f,  1.0f,  0.0f },
    {0.0f,  0.0f,  1.0f },
    {0.0f,  -1.0f, 0.0f },
    {0.0f,  0.0f,  -1.0f},
    {-1.0f, 0.0f,  0.0f },
    {0.0f,  0.0f,  1.0f },
    {1.0f,  0.0f,  0.0f },
    {0.0f,  0.0f,  -1.0f}
};

constexpr Object::Vertex cubeVertex[] = {
    {-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f}, // (0)
    {-1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 0.8f}, // (1)
    {-1.0f, 1.0f,  1.0f,  0.0f, 0.8f, 0.0f}, // (2)
    {-1.0f, 1.0f,  -1.0f, 0.0f, 0.8f, 0.8f}, // (3)
    {1.0f,  1.0f,  -1.0f, 0.8f, 0.0f, 0.0f}, // (4)
    {1.0f,  -1.0f, -1.0f, 0.8f, 0.0f, 0.8f}, // (5)
    {1.0f,  -1.0f, 1.0f,  0.8f, 0.8f, 0.0f}, // (6)
    {1.0f,  1.0f,  1.0f,  0.8f, 0.8f, 0.8f}  // (7)
};

constexpr GLuint wireCubeIndex[] = {
    1, 0,  // (a)
    2, 7,  // (b)
    3, 0,  // (c)
    4, 7,  // (d)
    5, 0,  // (e)
    6, 7,  // (f)
    1, 2,  // (g)
    2, 3,  // (h)
    3, 4,  // (i)
    4, 5,  // (j)
    5, 6,  // (k)
    6, 1   // (l)
};

/// @brief Each face has a different color.
constexpr Object::Vertex solidCubeVertex[] = {
    // left
    { -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f }, 
    { -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f }, 
    { -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
    { -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f }, 
    { -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
    { -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
    // back
    { 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f  }, 
    { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f  }, 
    { -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f  },
    { 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f  }, 
    { -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f  },
    { 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f  },
    // bottom
    { -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
    { 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f }, 
    { 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
    { -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f }, 
    { 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
    { -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
    // right
    { 1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f  }, 
    { 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f  }, 
    { 1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 0.0f  },
    { 1.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f  }, 
    { 1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 0.0f  },
    { 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f  },
    // upper
    { -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f }, 
    { -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
    { -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
    // front
    { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f }, 
    { 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f }, 
    { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
    { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f }, 
    { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
    { -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f } 
};

constexpr GLuint solidCubeIndex[] = {
    0, 1, 2, 3, 4, 5, // left
    6, 7, 8, 9,10,11, // back 
    12, 13, 14, 15, 16, 17, // bottom
    18, 19, 20, 21, 22, 23, // right
    24, 25, 26, 27, 28, 29, // upper
    30,31,32,33,34,35 // front
};

int main()
{
    if(glfwInit() == GL_FALSE) {
        std::cerr << "Can't initialize GLFW" << std::endl;
        return 1;
    }

    atexit(glfwTerminate);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // depth buffer
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    const GLuint program(loadProgram("../shaders/point.vert", "../shaders/point.frag"));

    const GLint modelviewLoc(glGetUniformLocation(program, "modelview"));
    const GLint projectionLoc(glGetUniformLocation(program, "projection"));
    const GLint normalMatrixLoc(glGetUniformLocation(program, "normalMatrix"));

    std::unique_ptr<const Shape> shape(new SolidShapeIndex(3, 36, solidCubeVertex, 36, solidCubeIndex));

    glfwSetTime(0.0);

    while(window) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        const GLfloat *const size(window.getSize());
        const GLfloat fovy(window.getScale() * 0.01f);
        const GLfloat aspect(size[0] / size[1]);
        const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));

        const GLfloat *const location(window.getLocation());
        const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f));
        const Matrix model(Matrix::translate(location[0], location[1], 0.0f) * r);

        const Matrix view(Matrix::lookat(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

        GLfloat normalMatrix[9];

        const Matrix modelview(view * model);

        modelview.getNormalMatrix(normalMatrix);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix);

        shape->draw();

        const Matrix modelview1(modelview * Matrix::translate(0.0f, 0.0f, 3.0f));

        modelview1.getNormalMatrix(normalMatrix);

        glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview1.data());
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix);

        shape->draw();

        window.swapBuffers();
    }
}

