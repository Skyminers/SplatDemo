
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb/stb_image.h"
#include "Shaders.h"
#include "Camera.h"
#include "skyBox.h"
#include "floor.h"
#include <iostream>
#include <vector>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* windows, double xOffset, double yOffset);
bool getTextureID(unsigned int &ID, char *s);
void renderSkybox();
void renderFloor();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Shaders *floorShader, *skyBoxShader;

glm::mat4 projection, view, model;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    floorShader = new Shaders("../src/shaders/floorVS.glsl", "../src/shaders/floorFS.glsl");
    skyBoxShader = new Shaders("../src/shaders/skyBoxVS.glsl", "../src/shaders/skyBoxFS.glsl");

    unsigned int floorTexture;

    if(!getTextureID(floorTexture, "../img/floor.jpg")) return 0;


    skyBoxInit("../img/skybox");
    floorInit();
    // render loop
    // -----------

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.getViewMat();
        model = glm::mat4();

        renderFloor();
        renderSkybox();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void renderFloor(){
    floorShader->useProgram();
    floorShader->setMat4("view", view);
    floorShader->setMat4("projection", projection);
    floorShader->setMat4("model", model);
    floorShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    floorShader->setVec3("lightPos", .0, .100, .0);
    floorShader->setVec3("cameraPos", camera.getPosition());
    floorShader->setFloat("roughness", 1.8);
    floorShader->setFloat("fresnel", .1);
    drawFloor();
}

void renderSkybox(){
    skyBoxShader->useProgram();
    view = glm::mat4(glm::mat3(camera.getViewMat()));
    skyBoxShader->setMat4("view", view);
    skyBoxShader->setMat4("projection", projection);
    drawSkyBox(projection, camera);
}



// Read img file and generate ID of texture.
// -----------------------------------------
bool getTextureID(unsigned int &ID, char *s){
    int width, height, nrChannels;
    unsigned char *data = stbi_load(s, &width, &height, &nrChannels, 0);
    if(!data){
        std::cerr << "Failed to open img." << std::endl;
        return false;
    }

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKey(cameraMovement::FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKey(cameraMovement::BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKey(cameraMovement::LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKey(cameraMovement::RIGHT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKey(cameraMovement::UP, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKey(cameraMovement::DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    static bool firstFlag = true;
    static float lastX, lastY;
    if(firstFlag){
        lastX = xpos;
        lastY = ypos;
        firstFlag = false;
    }
    float xOffset = (xpos - lastX);
    float yOffset = (lastY - ypos);
    lastX = xpos;
    lastY = ypos;

    camera.processMouse(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* windows, double xOffset, double yOffset){
    camera.processScroll(yOffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
