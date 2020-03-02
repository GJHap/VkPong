#include <exception>
#include <iostream>

#include <GLFW/glfw3.h>
#include <vulkan\vulkan.h>

#include "VulkanInitialization.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    try
    {
        VulkanInstanceInfo instanceInfo = initializeVulkan(window);
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
        throw e;
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}