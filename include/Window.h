#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
    GLFWwindow *const window;

    GLfloat size[2];

    GLfloat scale;

    GLfloat location[2];

    int keyStatus;

public:
    Window(int width = 640, int height = 480, const char *title = "Hello!")
        : window(glfwCreateWindow(width, height, title, NULL, NULL)), scale(100.0f), location{0.0f, 0.0f},
          keyStatus(GLFW_RELEASE)
    {
        if(window == NULL) {
            std::cerr << "Cant't create GLFW window." << std::endl;
            exit(1);
        }

        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;
        if(glewInit() != GLEW_OK) {
            std::cerr << "Can't initialize GLEW" << std::endl;
            exit(1);
        }

        glfwSwapInterval(1);

        glfwSetWindowSizeCallback(window, resize);

        glfwSetScrollCallback(window, wheel);

        glfwSetKeyCallback(window, keyboard);

        glfwSetWindowUserPointer(window, this);

        resize(window, width, height);
    }

    virtual ~Window()
    {
        glfwDestroyWindow(window);
    }

    explicit operator bool()
    {
        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE) {
            location[0] -= 2.0f / size[0];
        } else if(glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE) {
            location[0] += 2.0f / size[0];
        } if(glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE) {
            location[1] -= 2.0f / size[1];
        } else if(glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE) {
          location[1] += 2.0f / size[1];
        }

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
            location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
        }

        return !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE);
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

        if(instance != NULL) {
            instance->size[0] = static_cast<GLfloat>(width);
            instance->size[1] = static_cast<GLfloat>(height);
        }
    }

    static void wheel(GLFWwindow *window, double x, double y)
    {
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

        if(instance != NULL) {
            instance->scale += static_cast<GLfloat>(y);
        }
    }

    static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

        if(instance != NULL) {
            instance->keyStatus = action;
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

    const GLfloat *getLocation() const
    {
        return location;
    }
};