#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Matrix.h"
#include "Shape.h"

GLboolean printShaderInfoLog(GLuint shader, const char *str)
{
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
      std::cerr << "Compile error in " << str << std::endl;

  GLsizei bufSize;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
  if (bufSize > 1)
  {
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
  if (status == GL_FALSE)
    std::cerr << "Link error." << std::endl;

  GLsizei bufSize;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
  if (bufSize > 1)
  {
    std::vector<GLchar> infoLog(bufSize);
    GLsizei length;
    glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
    std::cerr << &infoLog[0] << std::endl;
  }

  return static_cast<GLboolean>(status);
}

// vsrc : vertex shader source
// fsrc : fragment shader source
GLuint createProgram(const char *vsrc, const char *fsrc) 
{
  const GLuint program(glCreateProgram());

  if (vsrc != NULL)
  {
    const GLuint vobj(glCreateShader(GL_VERTEX_SHADER)); 
    glShaderSource(vobj, 1, &vsrc, NULL); 
    glCompileShader(vobj);

    if (printShaderInfoLog(vobj, "vertex shader"))
      glAttachShader(program, vobj); 
    glDeleteShader(vobj);
  }

  if (fsrc != NULL)
  {
    const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER)); 
    glShaderSource(fobj, 1, &fsrc, NULL); 
    glCompileShader(fobj);

    if (printShaderInfoLog(fobj, "fragment shader"))
      glAttachShader(program, fobj); 
    glDeleteShader(fobj);
  }
  
  glBindAttribLocation(program, 0, "position"); 
  glBindFragDataLocation(program, 0, "fragment"); 
  glLinkProgram(program);
  
  if (printProgramInfoLog(program))
    return program;

  glDeleteProgram(program);
    return 0;
}

bool readShaderSource(const char *name, std::vector<GLchar> &buffer)
{
  if (name == NULL) return false;

  std::ifstream file(name, std::ios::binary);
  if (file.fail())
  {
    std::cerr << "Can't open the file." << name << std::endl;
    return false;
  }

  file.seekg(0L, std::ios::end);
  GLsizei length = static_cast<GLsizei>(file.tellg());

  buffer.resize(length + 1);

  file.seekg(0L, std::ios::beg);
  file.read(buffer.data(), length);
  buffer[length] = '\0';

  if (file.fail())
  {
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

constexpr Object::Vertex rectangleVertex[] =
{
  { -0.5f, -0.5f }, 
  { 0.5f, -0.5f },
  { 0.5f, 0.5f },
  { -0.5f, 0.5f }
};

int main()
{
  if (glfwInit() == GL_FALSE)
  {
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

  const GLuint program(loadProgram("../shaders/point.vert", "../shaders/point.frag"));

  const GLint modelviewLoc(glGetUniformLocation(program, "modelview"));
  const GLint projectionLoc(glGetUniformLocation(program, "projection"));

  std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));
  
  while (window)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    const GLfloat *const size(window.getSize());
    const GLfloat scale(window.getScale() * 2.0f);
    const GLfloat w(size[0] / scale), h(size[1] / scale);
    const Matrix projection(Matrix::frustum(-w, w, -h, h, 1.0f, 10.0f));
    
    const GLfloat *const location(window.getLocation());
    const Matrix model(Matrix::translate(location[0], location[1], 0.0f));

    const Matrix view(Matrix::lookat(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

    const Matrix modelview(view * model);

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());

    shape->draw();

    window.swapBuffers();
  }
}