#include "MouseHandler.h"
#include "LoggingMacros.h"

glm::vec2 MouseHandler::DeltaMousePosition;
glm::vec2 MouseHandler::LastMousePosition;
glm::vec2 MouseHandler::MousePosition;

void MouseHandler::MouseCallback(GLFWwindow* Window, double PositionX, double PositionY)
{
    MousePosition = glm::vec2(PositionX, PositionY);
}

glm::vec2 MouseHandler::GetDeltaMousePosition()
{
    return DeltaMousePosition;
}

void MouseHandler::UpdateMouse()
{
    DeltaMousePosition = MousePosition - LastMousePosition;
    LastMousePosition = MousePosition;
}


