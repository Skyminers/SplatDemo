#ifndef SKYBOX_H
#define SKYBOX_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Camera.h>

void skyBoxInit(string imgPath);
void drawSkyBox(glm::mat4 projection, Camera camera);
bool getSkyBoxID(unsigned int &ID, string path);

#endif
