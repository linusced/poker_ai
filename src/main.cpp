#include "../header/gl/init.hpp"
#include "../header/app/app.hpp"
#include "../header/window_position.hpp"

int main(int argc, char const *argv[])
{
    int windowWidth = 1280, windowHeight = 720;
    GLFWwindow *window = nullptr;

    if (!init(window, windowWidth, windowHeight, "Poker AI"))
        return 1;

    std::vector<std::string> fileDirectories{applicationSupportPath("")};
    for (auto &dir : fileDirectories)
        if (!std::filesystem::is_directory(dir))
            std::filesystem::create_directory(dir);

    fileDirectories.clear();

    if (!windowPosition::load(window))
    {
        auto *videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowSize(window, videoMode->width, videoMode->height);
        glfwSetWindowPos(window, 0, 0);
    }
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    App appLoop{window, &windowWidth, &windowHeight, argc == 2 && strcmp(argv[1], "-dev") == 0};
    appLoop();

    windowPosition::save(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}