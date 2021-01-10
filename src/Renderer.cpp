#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC

#include <Renderer.h>
#include "defineList.h"

extern Camera camera;
glm::mat4 projection, view, model;

Shaders *floorShader, *skyBoxShader, *playerShader, *bulletShader;

void ShadersInit() {
    floorShader = new Shaders(  "../src/shaders/floorVS.glsl", 
                                "../src/shaders/floorFS.glsl");
    skyBoxShader = new Shaders( "../src/shaders/skyBoxVS.glsl", 
                                "../src/shaders/skyBoxFS.glsl");
    playerShader = new Shaders("../src/shaders/playerVS.glsl",
                                "../src/shaders/playerFS.glsl");
    bulletShader = new Shaders("../src/shaders/playerVS.glsl",
                                "../src/shaders/playerFS.glsl");
    //    unsigned int floorTexture;
    //    if(!getTextureID(floorTexture, "../img/floor.jpg")) return 0;
    skyBoxInit("../resource/texture/skybox");
    floorInit();
    bulletRenderInit();
    playerRenderInit();
}

void renderFloor() {
    floorShader->useProgram();
    floorShader->setMat4("view", view);
    floorShader->setMat4("projection", projection);
    floorShader->setMat4("model", model);
    floorShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    floorShader->setVec3("lightPos", 2*FLOOR_MAX_POSITION, 100.0f, 0.5*FLOOR_MAX_POSITION);
    floorShader->setVec3("cameraPos", (camera.bind())?camera.getPlayerPos():camera.getPosition());
    drawFloor();
}

void renderSkybox() {
    skyBoxShader->useProgram();
    view = glm::mat4(glm::mat3(camera.getViewMat()));
    skyBoxShader->setMat4("view", view);
    skyBoxShader->setMat4("projection", projection);
    drawSkyBox(projection, camera);
}

void renderBullet(Bullet* bullet, float time) {
    bulletShader->useProgram();
    glm::mat4 model;
    model = glm::translate(model, bullet->getPos());
    float ratio = bullet->getRadius();
    model = glm::scale(model, glm::vec3(ratio, ratio, ratio));
    bulletShader->setMat4("projection", projection);
    bulletShader->setMat4("view", view);
    bulletShader->setMat4("model", model);
    bulletShader->setVec3("color", bullet->getColor());
    drawBullet();
}

void renderPlayer(Player* player, float time) {
    playerShader->useProgram();
    glm::mat4 model;
    float ratio = player->getHalfLength();
    model = glm::translate(model, player->getPos() + glm::vec3(0, ratio, 0));
    model = glm::scale(model, glm::vec3(ratio, ratio, ratio));
    model = glm::rotate(model, glm::radians(player->rotateAngle()), glm::vec3(0, 1, 0));
    playerShader->setMat4("projection", projection);
    playerShader->setMat4("view", view);
    playerShader->setMat4("model", model);
    playerShader->setVec3("color", player->getColor());
    drawPlayer();
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