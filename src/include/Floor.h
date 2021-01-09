#ifndef SPLATDEMO_FLOOR_H
#define SPLATDEMO_FLOOR_H

#include <glm/glm.hpp>

class BulletPos{
public:
    BulletPos(const int &a, const int &b);
    static float distance(const BulletPos &a, const BulletPos &b);
    int mapToId();
    bool checkPos();
    friend bool operator < (const BulletPos &a,const BulletPos &b){
        return a.x == b.x ? a.z < b.z : a.x < b.x;
    }
    friend BulletPos operator + (const BulletPos &a, const BulletPos &b){
        return BulletPos(a.x+b.x, a.z+b.z);
    }

private:
    int x, z;
};

void floorInit();
void drawFloor();
void genFloorVertices();
void plant(float x, float z, glm::vec3 color);
bool checkPos(float x, float z);

#endif //SPLATDEMO_FLOOR_H
