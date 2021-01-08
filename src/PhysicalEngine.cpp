#include "PhysicalEngine.h"

glm::vec3 object::getPos() {
    return position;
}

glm::vec3 object::getColor() {
    return color;
}

bool object::isAlive() {
    return alive;
}