#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class window
{
  GLFWwindow *const window;

public:
  window(int width = 640, int height = 480, const char *title = "Hello!")
    : window(glfwCreateWindow(width, height, title, NULL, NULL))
  {
    if (window == NULL)
    {
      std::cerr << "Cant't create GLFW window." << std::endl;
      exit(1);
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TURE;
    if (glewInit() != GLEW_OK)
    {
      std::cerr << "Can't initialize GLEW" << std::endl;
      exit(1);
    }

    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, resize);

    resize(window, width, height);
  }

  virtual ~window()
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
  }

};