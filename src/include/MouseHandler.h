#pragma once

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

class MouseHandler
{
private:
    static glm::vec2 MousePosition;
    static glm::vec2 LastMousePosition;
    static glm::vec2 DeltaMousePosition;

    MouseHandler() = default;
public:
    static void MouseCallback(GLFWwindow* Window, double PositionX, double PositionY);
    static void UpdateMouse();

    static glm::vec2 GetDeltaMousePosition();
};
