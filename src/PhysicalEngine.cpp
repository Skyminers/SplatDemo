#include "PhysicalEngine.h"

const float eps = 1e-6;

glm::vec3 Object::getPos() {
    return position;
}

glm::vec3 Object::getColor() {
    return color;
}

bool Object::isAlive() const {
    return alive;
}

unsigned int Object::getID() {
    return id;
}

float Cube::rotateAngle() const {
    return -yaw;
}

float Cube::getHalfLength() const {
    return halfLength;
}

float Cube::getFateY() const {
    return fateY;
}

float Sphere::getRadius() const {
    return radius;
}

glm::vec3 PhysicalEngine::rotate(glm::vec3 pos, glm::vec3 rotatePoint, float angle) {
    glm::mat4 rotMat;
    rotMat = glm::translate(rotMat, glm::vec3(rotatePoint.x, 0, rotatePoint.z));
    rotMat = glm::rotate(rotMat, glm::radians(-angle), glm::vec3(0, 1, 0));
    rotMat = glm::translate(rotMat, glm::vec3(-rotatePoint.x, 0, -rotatePoint.z));
    return glm::vec3(rotMat * glm::vec4(pos, 1.0f));
}

bool PhysicalEngine::inBox(glm::vec3 pos, glm::vec3 boxPoint, float halfLength, float fateY, float angle, float r) {
    glm::vec3 newPos = rotate(pos, boxPoint, angle);
    float max_x = boxPoint.x + halfLength;
    float min_x = boxPoint.x - halfLength;
    float max_y = boxPoint.y + halfLength * fateY;
    float min_y = boxPoint.y - halfLength * fateY;
    float max_z = boxPoint.z + halfLength;
    float min_z = boxPoint.z - halfLength;
    if (newPos.x < min_x - r || newPos.x > max_x + r) return false;
    if (newPos.y < min_y - r || newPos.y > max_y + r) return false;
    return !(newPos.z < min_z - r || newPos.z > max_z + r);
}

bool PhysicalEngine::intersect(glm::vec3 p1, glm::vec3 p2, glm::vec3 boxPoint, float halfLength) {
    float max_x = boxPoint.x + halfLength;
    float min_x = boxPoint.x - halfLength;
    float max_y = 100.0f;//boxPoint.y + halfLength;
    float min_y = 0.0f;//boxPoint.y - halfLength;
    float max_z = boxPoint.z + halfLength;
    float min_z = boxPoint.z - halfLength;
    glm::vec3 dir = p2 - p1;
    float t0 = 0.0f, t1 = 1.0f;
    if (fabs(dir.x) < eps) {
        if (p1.x > max_x + eps || p1.x < min_x - eps) return false;
    } else {
        float inv_dir = 1.0f / dir.x;
        float t_near = (max_x - p1.x) * inv_dir;
        float t_far = (min_x - p1.x) * inv_dir;
        if (t_near > t_far) swap(t_near, t_far);
        t0 = max(t0, t_near);
        t1 = min(t1, t_far);
        if (t0 > t1 + eps) return false;
    }
    if (fabs(dir.y) < eps) {
        if (p1.y > max_y + eps || p1.y < min_y - eps) return false;
    } else {
        float inv_dir = 1.0f / dir.y;
        float t_near = (max_y - p1.y) * inv_dir;
        float t_far = (min_y - p1.y) * inv_dir;
        if (t_near > t_far) swap(t_near, t_far);
        t0 = max(t0, t_near);
        t1 = min(t1, t_far);
        if (t0 > t1 + eps) return false;
    }
    if (fabs(dir.z) < eps) {
        if (p1.z > max_z + eps || p1.z < min_z - eps) return false;
    } else {
        float inv_dir = 1.0f / dir.z;
        float t_near = (max_z - p1.z) * inv_dir;
        float t_far = (max_z - p1.z) * inv_dir;
        if (t_near > t_far) swap(t_near, t_far);
        t0 = max(t0, t_near);
        t1 = min(t1, t_far);
        if (t0 > t1 + eps) return false;
    }
    return true;
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
    vector<glm::vec3> p(8);
    p[0] = bpos + glm::vec3((sinB + cosB) * bhl, bhl, (cosB - sinB) * bhl);
    p[1] = bpos + glm::vec3((-sinB + cosB) * bhl, bhl, (-cosB - sinB) * bhl);
    p[2] = bpos + glm::vec3((sinB + cosB) * bhl, -bhl, (cosB - sinB) * bhl);
    p[3] = bpos + glm::vec3((-sinB + cosB) * bhl, -bhl, (-cosB - sinB) * bhl);
    p[4] = bpos + glm::vec3((sinB - cosB) * bhl, bhl, (cosB + sinB) * bhl);
    p[5] = bpos + glm::vec3((-sinB - cosB) * bhl, bhl, (-cosB + sinB) * bhl);
    p[6] = bpos + glm::vec3((sinB - cosB) * bhl, -bhl, (cosB + sinB) * bhl);
    p[7] = bpos + glm::vec3((-sinB - cosB) * bhl, -bhl, (-cosB + sinB) * bhl);
    for (auto it : p) it = rotate(it, apos, angle_a);
    if (intersect(p[0], p[1], apos, ahl)) return true;
    if (intersect(p[2], p[3], apos, ahl)) return true;
    if (intersect(p[4], p[5], apos, ahl)) return true;
    if (intersect(p[6], p[7], apos, ahl)) return true;
    if (intersect(p[0], p[2], apos, ahl)) return true;
    if (intersect(p[1], p[3], apos, ahl)) return true;
    if (intersect(p[4], p[6], apos, ahl)) return true;
    if (intersect(p[5], p[7], apos, ahl)) return true;
    if (intersect(p[0], p[4], apos, ahl)) return true;
    if (intersect(p[2], p[6], apos, ahl)) return true;
    if (intersect(p[1], p[5], apos, ahl)) return true;
    if (intersect(p[3], p[7], apos, ahl)) return true;

    p[0] = apos + glm::vec3((sinA + cosA) * ahl, ahl, (cosA - sinA) * ahl);
    p[1] = apos + glm::vec3((-sinA + cosA) * ahl, ahl, (-cosA - sinA) * ahl);
    p[2] = apos + glm::vec3((sinA + cosA) * ahl, -ahl, (cosA - sinA) * ahl);
    p[3] = apos + glm::vec3((-sinA + cosA) * ahl, -ahl, (-cosA - sinA) * ahl);
    p[4] = apos + glm::vec3((sinA - cosA) * ahl, ahl, (cosA + sinA) * ahl);
    p[5] = apos + glm::vec3((-sinA - cosA) * ahl, ahl, (-cosA + sinA) * ahl);
    p[6] = apos + glm::vec3((sinA - cosA) * ahl, -ahl, (cosA + sinA) * ahl);
    p[7] = apos + glm::vec3((-sinA - cosA) * ahl, -ahl, (-cosA + sinA) * ahl);

    for (auto it : p) it = rotate(it, bpos, angle_b);
    if (intersect(p[0], p[1], bpos, bhl)) return true;
    if (intersect(p[2], p[3], bpos, bhl)) return true;
    if (intersect(p[4], p[5], bpos, bhl)) return true;
    if (intersect(p[6], p[7], bpos, bhl)) return true;
    if (intersect(p[0], p[2], bpos, bhl)) return true;
    if (intersect(p[1], p[3], bpos, bhl)) return true;
    if (intersect(p[4], p[6], bpos, bhl)) return true;
    if (intersect(p[5], p[7], bpos, bhl)) return true;
    if (intersect(p[0], p[4], bpos, bhl)) return true;
    if (intersect(p[2], p[6], bpos, bhl)) return true;
    if (intersect(p[1], p[5], bpos, bhl)) return true;
    if (intersect(p[3], p[7], bpos, bhl)) return true;
    return false;
}

bool PhysicalEngine::intersect(Cube a, Sphere b) {
    float ahl = a.getHalfLength();
    float fateY = a.getFateY();
    glm::vec3 bpos = b.getPos();
    glm::vec3 apos = a.getPos() + glm::vec3(0, ahl * fateY, 0);
    float angle_a = a.rotateAngle();
    return inBox(bpos, apos, ahl, fateY, angle_a, b.getRadius());
}