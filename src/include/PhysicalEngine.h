#ifndef SPLATDEMO_PHYSICALENGINE_H
#define SPLATDEMO_PHYSICALENGINE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class object {
public:
    glm::vec3 getPos();
    glm::vec3 getColor();
    bool isAlive();

protected:
    glm::vec3 position, color;
    bool alive = true;
    float speed, g;
};

#endif //SPLATDEMO_PHYSICALENGINE_H
