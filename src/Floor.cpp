#include <Floor.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <queue>
#include <set>
#include "defineList.h"
#include <iostream>

std::vector<float> floorVertices;
std::vector<unsigned int> floorElement;

unsigned int floorVAO, floorVBO, floorEBO;
unsigned int floorTexture; // unused
unsigned int noiseTexture;
unsigned int depthTexture, depthFBO;


inline float floorRelocation(int x) {
    return 1.0 * (x - (FLOOR_SIZE/2)) / (FLOOR_SIZE/2)
               * FLOOR_MAX_POSITION;
}
void genFloorVertices(){
    for(int i = 0; i <= FLOOR_SIZE; ++ i){
        for(int j = 0; j <= FLOOR_SIZE; ++ j){
            float x = floorRelocation(i);
            float y = 0;
            float z = floorRelocation(j);
            floorVertices.push_back(x);
            floorVertices.push_back(y);
            floorVertices.push_back(z);
            floorVertices.push_back(FLOOR_COLOR);
            floorVertices.push_back(FLOOR_COLOR);
            floorVertices.push_back(FLOOR_COLOR);
            floorVertices.push_back(0);
            //floorVertices.push_back(0);
            //floorVertices.push_back(0);
            floorVertices.push_back(float(i)/30);
            floorVertices.push_back(float(j)/30);
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
            floorElement.push_back(idu);
            floorElement.push_back(idv);
            floorElement.push_back(idy);

            // an amazing effect !!!
//            floorElement.push_back(idx);
//            floorElement.push_back(idy);
//            floorElement.push_back(idv);
        }
    }
}
bool getTextureID(unsigned int &ID, const char *s){
    int width, height, nrChannels;
    unsigned char *data = stbi_load(s, &width, &height, &nrChannels, 0);
    if(!data){
        //std::cerr << "Failed to open img." << std::endl;
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
void floorInit(std::string NoiseImgPath){
    genFloorVertices();
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);

    glBindVertexArray(floorVAO);

    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float )*floorVertices.size(), &floorVertices[0], GL_STATIC_DRAW);
    unsigned int floorElementSize = FLOOR_ELEMENT_COUNT*sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floorElementSize, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floorElementSize, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, floorElementSize, (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, floorElementSize, (void*)(7*sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*floorElement.size(), &floorElement[0], GL_STATIC_DRAW);

    if(!getTextureID(noiseTexture, NoiseImgPath.c_str())) {
//        std::cerr<<"get noise texture failed!"<<std::endl;
    }

    glBindVertexArray(0);
}

void drawFloor(){
    glBindVertexArray(floorVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*floorVertices.size(), &floorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, FLOOR_ELEMENT_COUNT*sizeof(float), (void*)(6*sizeof(float)));
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
        int idStartPos = id * FLOOR_ELEMENT_COUNT;

        float& colorAlpha = floorVertices[idStartPos + 6];
        if(dis <= IN_RADIUS*IN_RADIUS){
            colorAlpha = 1.0f;
        }else{ // calc difference
            float tFactor = 1.0f/ (OUT_RADIUS - IN_RADIUS);
            float d = sqrt(dis);
            float t = 1.0f - (d - IN_RADIUS)*tFactor;

            colorAlpha = std::min(colorAlpha + t, 1.0f);
        }

        floorVertices[idStartPos + 3] = color[0];
        floorVertices[idStartPos + 4] = color[1];
        floorVertices[idStartPos + 5] = color[2];
        // floorVertices[idStartPos + 3] = color[0]*colorAlpha + floorVertices[idStartPos + 3]*(1-colorAlpha);
        // floorVertices[idStartPos + 4] = color[1]*colorAlpha + floorVertices[idStartPos + 4]*(1-colorAlpha);
        // floorVertices[idStartPos + 5] = color[2]*colorAlpha + floorVertices[idStartPos + 5]*(1-colorAlpha);

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

glm::vec3 getColorAt(float x, float z){
    int id = BulletPos(int(x/FLOOR_UNIT)  + (FLOOR_SIZE/2),int(z/FLOOR_UNIT) + (FLOOR_SIZE/2)).mapToId();
    int idStartPos = id*FLOOR_ELEMENT_COUNT;
    return glm::vec3(floorVertices[idStartPos + 3], floorVertices[idStartPos + 4], floorVertices[idStartPos + 5]);
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

void shadowInit(){
    glGenFramebuffers(1, &depthFBO);

    //Generate a empty texture
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Bind frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void shadowBind(){
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void shadowUnBind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #ifdef __APPLE__
        glViewport(0, 0, SCR_WIDTH*2, SCR_HEIGHT*2); // TODO: 为什么 SCR_WIDTH 需要设置为 2 倍 ？
    #else
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    #endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}