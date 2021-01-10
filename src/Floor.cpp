#include <Floor.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <queue>
#include <set>
#include <iostream>
#include "defineList.h"

std::vector<float> floorVertices;
std::vector<unsigned int> floorElement;

unsigned int floorVAO, floorVBO, floorEBO;
unsigned int floorTexture; // unused

void genFloorVertices(){
    for(int i = 0; i <= FLOOR_SIZE; ++ i){
        for(int j = 0; j <= FLOOR_SIZE; ++ j){
            float x = 1.0*(i - (FLOOR_SIZE/2)) / (FLOOR_SIZE/2) * FLOOR_MAX_POSITION;
            float y = 0;
            float z = 1.0*(j - (FLOOR_SIZE/2)) / (FLOOR_SIZE/2) * FLOOR_MAX_POSITION;
            floorVertices.push_back(x);
            floorVertices.push_back(y);
            floorVertices.push_back(z);
            floorVertices.push_back(FLOOR_COLOR);
            floorVertices.push_back(FLOOR_COLOR);
            floorVertices.push_back(FLOOR_COLOR);
            floorVertices.push_back(0);
        }
    }
    for(int i = 0; i < FLOOR_SIZE; ++i) {
        for (int j = 0; j < FLOOR_SIZE; ++j) {
            int idx = i * (FLOOR_SIZE + 1) + j;
            int idy = idx + 1;
            int idu = idx + (FLOOR_SIZE + 1);
            int idv = idy + (FLOOR_SIZE + 1);

            // a triangle
            floorElement.push_back(idx);
            floorElement.push_back(idy);
            floorElement.push_back(idu);

            // a triangle
//            floorElement.push_back(idu);
//            floorElement.push_back(idv);
//            floorElement.push_back(idy);

            // an amazing effect !!!
            floorElement.push_back(idx);
            floorElement.push_back(idy);
            floorElement.push_back(idv);
        }
    }
}

void floorInit(){
    genFloorVertices();
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float )*floorVertices.size(), &floorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*floorElement.size(), &floorElement[0], GL_STATIC_DRAW);
}

void drawFloor(){
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float )*floorVertices.size(), &floorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glDrawElements(GL_TRIANGLES, sizeof(unsigned int)*floorElement.size(), GL_UNSIGNED_INT, 0);
}

void plant(float x, float z, glm::vec3 color){
    using namespace std;
    static const char dx[] = {0,  0, -1,  1};
    static const char dy[] = {1, -1,  0,  0};
    static queue<BulletPos > q;
    static set<BulletPos> s;

    if(!checkPos(x, z)){
        //std::cerr << "Error BulletPos " << std::endl;
        return ;
    }

    s.clear();
    BulletPos src(int(x/FLOOR_UNIT)  + (FLOOR_SIZE/2),int(z/FLOOR_UNIT) + (FLOOR_SIZE/2));
    //cerr << "Going to plant (" << src.x << ", " << src.z << ") : " << src.mapToId() << endl;
    q.push(src);

    while(!q.empty()){
        BulletPos nowPos = q.front();
        q.pop();

        if(!nowPos.checkPos()) continue;

        // distance between this position and bullet position
        float dis = BulletPos::distance(nowPos, src);

        // check whether need render
        if(dis > OUT_RADIUS*OUT_RADIUS) continue;

        int id = nowPos.mapToId();



        if(dis <= IN_RADIUS*IN_RADIUS){
            floorVertices[id*7 + 6] = 1.0f;
        }else{ // calc difference
            float tFactor = 1.0f/ (OUT_RADIUS - IN_RADIUS);
            float d = sqrt(dis);
            float t = 1.0f - (d - IN_RADIUS)*tFactor;
//            glm::vec3 ori(floorVertices[id*7 + 3],
//                      floorVertices[id*7 + 4],
//                      floorVertices[id*7 + 5]);
//            if(ori != color){
//                floorVertices[id*7 + 6] = t;
//            }else

            floorVertices[id*7 + 6] += t;

            if(floorVertices[id*7 + 6] > 1){
                floorVertices[id*7 + 6] = 1.0f;
            }
        }

        floorVertices[id*7 + 3] = color[0];
        floorVertices[id*7 + 4] = color[1];
        floorVertices[id*7 + 5] = color[2];

        // find adjacent
        for(int i = 0;i < 4; ++ i){
            BulletPos nxt(nowPos + BulletPos(dx[i], dy[i]));
            if(s.find(nxt) == s.end()){
                s.insert(nxt);
                q.push(nxt);
            }
        }
    }

}

BulletPos::BulletPos(const int &a, const int &b): x(a), z(b) {}

float BulletPos::distance(const BulletPos &a, const BulletPos &b) {
    return (a.x - b.x)*(a.x - b.x) + (a.z - b.z)*(a.z - b.z);
}

int BulletPos::mapToId() {
    return GET_POS(x, z);
}

bool BulletPos::checkPos() {
    return x >= 0 && x < FLOOR_SIZE && z >= 0 && z < FLOOR_SIZE;
}

bool checkPos(float x, float z){
    return x >= -FLOOR_MAX_POSITION && x <= FLOOR_MAX_POSITION && z >= -FLOOR_MAX_POSITION && z <= FLOOR_MAX_POSITION;
}