#include "PhysicalEngine.h"

const float eps = 1e-6;

glm::vec3 Object::getPos() {
    return position;
}

glm::vec3 Object::getColor() {
    return color;
}

bool Object::isAlive() {
    return alive;
}

unsigned int Object::getID() {
    return id;
}

float Cube::rotateAngle() {
    return -yaw;
}

float Cube::getHalfLength() {
    return halfLength;
}

float Sphere::getRadius() {
    return radius;
}

glm::vec3 PhysicalEngine::rotate(glm::vec3 pos, glm::vec3 rotatePoint, float angle) {
    glm::mat4 rotMat;
    rotMat = glm::translate(rotMat, glm::vec3(rotatePoint.x, 0, rotatePoint.z));
    rotMat = glm::rotate(rotMat, glm::radians(-angle), glm::vec3(0, 1, 0));
    rotMat = glm::translate(rotMat, glm::vec3(-rotatePoint.x, 0, -rotatePoint.z));
    return glm::vec3(rotMat * glm::vec4(pos, 1.0f));
}

bool PhysicalEngine::inBox(glm::vec3 pos, glm::vec3 boxPoint, float halfLength, float angle, float r) {
    glm::vec3 newPos = rotate(pos, boxPoint, angle);
    float max_x = boxPoint.x + halfLength;
    float min_x = boxPoint.x - halfLength;
    float max_y = boxPoint.y + halfLength;
    float min_y = boxPoint.y - halfLength;
    float max_z = boxPoint.z + halfLength;
    float min_z = boxPoint.z - halfLength;
    if (newPos.x < min_x - r || newPos.x > max_x + r) return false;
    if (newPos.y < min_y - r || newPos.y > max_y + r) return false;
    return !(newPos.z < min_z - r || newPos.z > max_z + r);
}

bool PhysicalEngine::intersect(Cube a, Cube b) {
    float ahl = a.getHalfLength();
    float bhl = b.getHalfLength();
    glm::vec3 bpos = b.getPos() + glm::vec3(0, bhl, 0);
    glm::vec3 apos = a.getPos() + glm::vec3(0, ahl, 0);
    float angle_a = a.rotateAngle();
    float angle_b = b.rotateAngle();
    float cosA = cos(glm::radians(angle_a));
    float sinA = sin(glm::radians(angle_a));
    float cosB = cos(glm::radians(angle_b));
    float sinB = sin(glm::radians(angle_b));
    vector<glm::vec3> p(9);
    p[0] = bpos;
    p[1] = bpos + glm::vec3((sinB + cosB) * bhl, bhl, (cosB - sinB) * bhl);
    p[2] = bpos + glm::vec3((-sinB + cosB) * bhl, bhl, (-cosB - sinB) * bhl);
    p[3] = bpos + glm::vec3((sinB + cosB) * bhl, -bhl, (cosB - sinB) * bhl);
    p[4] = bpos + glm::vec3((-sinB + cosB) * bhl, -bhl, (-cosB - sinB) * bhl);
    p[5] = bpos + glm::vec3((sinB - cosB) * bhl, bhl, (cosB + sinB) * bhl);
    p[6] = bpos + glm::vec3((-sinB - cosB) * bhl, bhl, (-cosB + sinB) * bhl);
    p[7] = bpos + glm::vec3((sinB - cosB) * bhl, -bhl, (cosB + sinB) * bhl);
    p[8] = bpos + glm::vec3((-sinB - cosB) * bhl, -bhl, (-cosB + sinB) * bhl);
    for (auto it : p)
        if (inBox(it, apos, ahl, angle_a, eps)) return true;
    p[0] = apos;
    p[1] = apos + glm::vec3((sinA + cosA) * ahl, ahl, (cosA - sinA) * ahl);
    p[2] = apos + glm::vec3((-sinA + cosA) * ahl, ahl, (-cosA - sinA) * ahl);
    p[3] = apos + glm::vec3((sinA + cosA) * ahl, -ahl, (cosA - sinA) * ahl);
    p[4] = apos + glm::vec3((-sinA + cosA) * ahl, -ahl, (-cosA - sinA) * ahl);
    p[5] = apos + glm::vec3((sinA - cosA) * ahl, ahl, (cosA + sinA) * ahl);
    p[6] = apos + glm::vec3((-sinA - cosA) * ahl, ahl, (-cosA + sinA) * ahl);
    p[7] = apos + glm::vec3((sinA - cosA) * ahl, -ahl, (cosA + sinA) * ahl);
    p[8] = apos + glm::vec3((-sinA - cosA) * ahl, -ahl, (-cosA + sinA) * ahl);
    for (auto it : p)
        if (inBox(it, bpos, bhl, angle_b, eps)) return true;
    return false;
}

bool PhysicalEngine::intersect(Cube a, Sphere b) {
    float ahl = a.getHalfLength();
    glm::vec3 bpos = b.getPos();
    glm::vec3 apos = a.getPos() + glm::vec3(0, ahl, 0);
    float angle_a = a.rotateAngle();
    return inBox(bpos, apos, ahl, angle_a, b.getRadius());
}