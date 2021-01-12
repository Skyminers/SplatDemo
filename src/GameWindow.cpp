#include <GameWindow.h>
#include <Camera.h>
#include <Renderer.h>
#include "defineList.h"


Camera camera(glm::vec3(0.0f, 15.0f, 0.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;
extern GameLogic game;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    //camera.check();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKey(cameraMovement::FORWARD, deltaTime, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKey(cameraMovement::BACKWARD, deltaTime, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKey(cameraMovement::LEFT, deltaTime, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKey(cameraMovement::RIGHT, deltaTime, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.processKey(cameraMovement::CAMERA_PLAYER, deltaTime, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.processKey(cameraMovement::LAST_PLAYER, deltaTime, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.processKey(cameraMovement::NEXT_PLAYER, deltaTime, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKey(cameraMovement::UP, deltaTime, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKey(cameraMovement::DOWN, deltaTime, currentFrame);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        camera.processKey(cameraMovement::SHOOT, deltaTime, currentFrame);
    }
    if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT) {
        camera.shooting = false;
    }
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

    camera.processMouse(xOffset, yOffset, currentFrame);
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

GameWindow::GameWindow():window(NULL) {
    init();
}

void GameWindow::init() {
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SplatDemo", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GameWindow::run() {
    extern glm::mat4 projection, view, model;
    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        GameLogic::checkPlayer(currentFrame);
        for (auto bullet = Bullet::bulletQueue.begin(); bullet != Bullet::bulletQueue.end();) {
            GameLogic::checkBullet(*bullet, currentFrame);
            if (!(*bullet)->isAlive()) {
                Bullet::bulletQueue.erase(bullet);
                continue;
            }
            bullet++;
        }
        for (auto player = Player::playerQueue.begin(); player != Player::playerQueue.end();) {
            if (!(*player)->isAlive()) {
                (*player)->deadBomb(currentFrame, (*player)->getDefeater());
                Player::playerQueue.erase(player);
                continue;
            }
            player++;
        }
        for (auto particle = Particle::particleQueue.begin(); particle != Particle::particleQueue.end();) {
            //GameLogic::checkParticle(*bullet, currentFrame);
            (*particle)->update(deltaTime);
            if (!(*particle)->isAlive()) {
                Particle::particleQueue.erase(particle);
                continue;
            }
            particle++;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Depth test
        renderShadowDepth(Player::playerQueue, Bullet::bulletQueue);

        // render
        // ------

        projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.getViewMat();
        model = glm::mat4();

        for (auto bullet : Bullet::bulletQueue) {
            renderBullet(bullet, currentFrame);
        }
        GameLogic::plantBullet();
        for (auto player : Player::playerQueue) {
            renderPlayer(player, currentFrame);
        }
        for (auto particle : Particle::particleQueue) {
            renderParticle(particle, currentFrame);
        }
        
        renderFloor();
        renderSkybox();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        camera.jitter += deltaTime;
        camera.shootJitter += deltaTime;
        if (camera.shooting) camera.processKey(cameraMovement::SHOOT, deltaTime, currentFrame);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}