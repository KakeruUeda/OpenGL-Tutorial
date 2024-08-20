#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
  GLFWwindow *const window;

  GLfloat size[2];

  GLfloat scale;

public:
  Window(int width = 640, int height = 480, const char *title = "Hello!")
    : window(glfwCreateWindow(width, height, title, NULL, NULL))
    , scale(100.0f)
  {
    if (window == NULL)
    {
      std::cerr << "Cant't create GLFW window." << std::endl;
      exit(1);
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
      std::cerr << "Can't initialize GLEW" << std::endl;
      exit(1);
    }

    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window, this);

    glfwSetWindowSizeCallback(window, resize);

    resize(window, width, height);
  }

  virtual ~Window()
  {
    glfwDestroyWindow(window);
  }

  explicit operator bool()
  {
    glfwWaitEvents();

    return !glfwWindowShouldClose(window);
  }

  void swapBuffers() const
  {
    glfwSwapBuffers(window);
  }

  static void resize(GLFWwindow *window, int width, int height)
  {
    int fbwidth, fbheight;
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

    glViewport(0, 0, fbwidth, fbheight);

    Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

    if (instance != NULL)
    {
      instance->size[0] = static_cast<GLfloat>(width);
      instance->size[1] = static_cast<GLfloat>(height);
    }
  }

  const GLfloat *getSize() const
  {
    return size;
  }

  GLfloat getScale() const 
  {
    return scale;
  }
};