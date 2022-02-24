#ifndef WINDOW_POSITION
#define WINDOW_POSITION

#include "files.hpp"
#include <fstream>
#include <filesystem>
#include <GLFW/glfw3.h>

namespace windowPosition
{
    bool load(GLFWwindow *window);
    void save(GLFWwindow *window);
};

#endif