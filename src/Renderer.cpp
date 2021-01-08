#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>

#include <Camera.h>
#include <Renderer.h>
#include <Shaders.h>
#include <skybox.h>
#include <floor.h>

extern Camera camera;
glm::mat4 projection, view, model;

Shaders *floorShader, *skyBoxShader;

void ShadersInit() {
    floorShader = new Shaders(  "../src/shaders/floorVS.glsl", 
                                "../src/shaders/floorFS.glsl");
    skyBoxShader = new Shaders( "../src/shaders/skyBoxVS.glsl", 
                                "../src/shaders/skyBoxFS.glsl");
    //    unsigned int floorTexture;
    //    if(!getTextureID(floorTexture, "../img/floor.jpg")) return 0;
    skyBoxInit("../resource/texture/skybox");
    floorInit();
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