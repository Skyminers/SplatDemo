#ifndef SPLATDEMO_PHYSICALENGINE_H
#define SPLATDEMO_PHYSICALENGINE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
using namespace std;

class Object {
public:
    glm::vec3 getPos();
    glm::vec3 getColor();
    bool isAlive();
    unsigned int getID();

protected:
    glm::vec3 position, color;
    bool alive = true;
    float speed, g;
    unsigned int id, teamid;

};

class Cube : public Object{
public:
    float rotateAngle();
    float getHalfLength();

protected:
    float yaw = 0.0f, pitch = 30.0f, zoom = 50.0f, halfLength = 1.0f;
    glm::vec3 V = glm::vec3(0.0f, 0.0f, 0.0f);

};

class Sphere : public Object {
public:
    float getRadius();

protected:
    float radius = 0.2f;

};

class PhysicalEngine {
public:
    static glm::vec3 rotate(glm::vec3 pos, glm::vec3 rotatePoint, float angle);
    static bool inBox(glm::vec3 pos, glm::vec3 boxPoint, float halfLength, float angle, float r);
    static bool intersect(Cube a, Cube b);
    static bool intersect(Cube a, Sphere b);

};

#endif //SPLATDEMO_PHYSICALENGINE_H
