
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb/stb_image.h"
#include "Shaders.h"
#include "Camera.h"
#include "Sphere.h"
#include "skyBox.h"
#include <iostream>
#include <vector>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* windows, double xOffset, double yOffset);
bool getTextureID(unsigned int &ID, char *s);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const float aroundTime1 = 10.0f;
const float aroundTime2 = 8.0f;
const float aroundTime3 = 6.0f;
const float selfAroundTime = 10.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

    Shaders normalShader("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");
    Shaders sunShader("./shaders/sunVertexShader.glsl","./shaders/sunFragmentShader.glsl");
    Shaders skyBoxShader("./shaders/skyBoxVS.glsl", "./shaders/skyBoxFS.glsl");
    Sphere sun(0.4f);

    unsigned int earthTexture, moonTexture, sunTexture;

    if(!getTextureID(earthTexture, "./img/earth.jpg")) return 0;
    if(!getTextureID(moonTexture,"./img/moon.jpg")) return 0;
    if(!getTextureID(sunTexture, "./img/sun.jpeg")) return 0;

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sun.getVertexSize()*sizeof(float ), sun.getVertexAddress(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sun.getIndicesSize()*sizeof(int ), sun.getIndicesAddress(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    unsigned int sunVAO;
    glGenVertexArrays(1, &sunVAO);
    glBindVertexArray(sunVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float ), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float ), (void*)(3*sizeof(float)) );
    glEnableVertexAttribArray(1);

    skyBoxInit("./img/skybox");
    // render loop
    // -----------

    glm::mat4 rotate1;
    glm::mat4 rotate2;
    glm::mat4 rotateSate;
    glm::mat4 selfRotation;

    glm::vec3 posSp2(-0.8f, -1.3f, -0.9f);
    glm::vec3 axisSp2 = glm::cross(posSp2, glm::vec3(0, 0, 1.0f));
    glm::vec3 posSate(0.6f, 0.7f, 0.9f);
    glm::vec3 axisSate = glm::cross(posSate, glm::vec3(0,0,1.0f));

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

        Shaders *shader;

        selfRotation = glm::rotate(selfRotation, glm::radians(deltaTime/selfAroundTime*360), glm::vec3(0.2f, 0.1f, 0.4f));

        glBindVertexArray(sunVAO);
        glBindTexture(GL_TEXTURE_2D, sunTexture);

        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMat();
        glm::mat4 model;
        model = model * selfRotation;
        shader = &sunShader;
        shader->useProgram();
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setMat4("model", model);

        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        shader = &normalShader;
        shader->useProgram();
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setMat4("model", model);
        shader->setVec3("lightColor",1.0f,1.0f,1.0f);
        shader->setVec3("lightPos",.0,.0,.0);
        shader->setVec3("cameraPos",camera.getPosition());
        shader->setFloat("roughness", 1.8);
        shader->setFloat("fresnel",.1);
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, earthTexture);

        rotate1 = glm::rotate(rotate1, glm::radians(deltaTime/aroundTime1*360), glm::vec3(0.0f, 1.0f, 0.0f));
        model = rotate1;
        model = glm::translate(model, glm::vec3(3.0f, 0, 0));
        model = model * selfRotation;
        shader->setMat4("model", model);

        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, moonTexture);
        model = rotate1;
        model = glm::translate(model, glm::vec3(3.0f,0,0));
        rotateSate = glm::rotate(rotateSate, glm::radians(deltaTime/aroundTime3*360), axisSate);
        model = model * rotateSate;
        model = glm::translate(model, posSate);
        model = model * selfRotation;
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        shader->setMat4("model", model);

        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        rotate2 = glm::rotate(rotate2, glm::radians(deltaTime/aroundTime2*360), axisSp2);
        model = rotate2;
        model = glm::translate(model, posSp2);
        model = model * selfRotation;
        shader->setMat4("model", model);

        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        shader = &skyBoxShader;
        shader->useProgram();
        view = glm::mat4(glm::mat3(camera.getViewMat()));
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        drawSkyBox();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
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
