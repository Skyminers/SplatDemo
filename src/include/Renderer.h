#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>

#include <Camera.h>
#include <Shaders.h>
#include <SkyBox.h>
#include <Floor.h>
#include <PlayerView.h>
#include <GameLogic.h>

void ShadersInit();
bool getTextureID(unsigned int &ID, char *s);
void renderSkybox();
void renderFloor();
void renderBullet(Bullet* bullet, float time);
void renderPlayer(Player* player, float time);

#endif