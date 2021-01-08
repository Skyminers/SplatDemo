#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* windows, double xOffset, double yOffset);

class GameWindow {
public:
    GameWindow();
    void init();
    void run();
private:
    GLFWwindow* window;
};

#endif