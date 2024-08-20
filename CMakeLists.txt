cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the directory where your custom Find*.cmake scripts are located
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find GLEW using the provided FindGLEW.cmake script
find_package(GLEW REQUIRED)

# Find GLFW3 using the provided FindGLFW3.cmake script
find_package(GLFW3 REQUIRED)

# Find GLM using the provided FindGLM.cmake script
find_package(GLM REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include directories (add the include directory)
include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR} ${GLM_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

# Add the executable
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(OpenGLTutorial ${SOURCES})

# Link the GLEW, GLFW3, and OpenGL libraries
target_link_libraries(OpenGLTutorial ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY})

# Display a message if GLEW, GLFW3, and GLM are found
if(GLEW_FOUND)
    message(STATUS "GLEW found: ${GLEW_LIBRARIES}")
else()
    message(FATAL_ERROR "GLEW not found. Please install GLEW and try again.")
endif()

if(GLFW_FOUND)
    message(STATUS "GLFW3 found: ${GLFW_LIBRARIES}")
else()
    message(FATAL_ERROR "GLFW3 not found. Please install GLFW3 and try again.")
endif()

if(GLM_FOUND)
    message(STATUS "GLM found: ${GLM_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "GLM not found. Please install GLM and try again.")
endif()

if(OPENGL_FOUND)
    message(STATUS "OpenGL found: ${OPENGL_gl_LIBRARY}")
else()
    message(FATAL_ERROR "OpenGL not found. Please install OpenGL and try again.")
endif()
