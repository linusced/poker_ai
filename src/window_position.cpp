#include "../header/window_position.hpp"

bool windowPosition::load(GLFWwindow *window)
{
    std::ifstream file(applicationSupportPath("window_position.dat"));
    if (!file.is_open())
        return false;

    int windowX = -1, windowY = -1, windowW = -1, windowH = -1;

    std::string key, value;
    while (!file.eof())
    {
        file >> key >> value;

        bool invalidNum = false;
        for (auto &c : value)
            if (c < '0' || c > '9')
            {
                invalidNum = true;
                break;
            }

        if (!invalidNum)
        {
            if (key == "x")
                windowX = std::stoi(value);
            else if (key == "y")
                windowY = std::stoi(value);
            else if (key == "w")
                windowW = std::stoi(value);
            else if (key == "h")
                windowH = std::stoi(value);
        }
    }
    file.close();

    if (windowX == -1 || windowY == -1 || windowW == -1 || windowH == -1)
        return false;

    glfwSetWindowSize(window, windowW, windowH);
    glfwSetWindowPos(window, windowX, windowY);
    return true;
}

void windowPosition::save(GLFWwindow *window)
{
    std::ofstream file(applicationSupportPath("window_position.dat"));
    if (!file.is_open())
        return;

    int windowX, windowY, windowW, windowH;
    glfwGetWindowPos(window, &windowX, &windowY);
    glfwGetWindowSize(window, &windowW, &windowH);

    file << "x " << windowX
         << "\ny " << windowY
         << "\nw " << windowW
         << "\nh " << windowH;

    file.close();
}