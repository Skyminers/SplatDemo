#include <Floor.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "defineList.h"

float floorVerticesDefault[] = {
        -100.0f, 0.0f, -100.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         100.0f, 0.0f, -100.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f, 0.0f,
};
unsigned int floorElementDefault[] = {
        0, 1, 2,
        1, 2, 3
};

std::vector<float> floorVertices;
std::vector<unsigned int> floorElement;

unsigned int floorVAO, floorVBO, floorEBO;
unsigned int floorTexture; // unused

void genFloorVertices(){
    for(int i = -FLOOR_SIZE/2; i < FLOOR_SIZE/2; ++ i){
        for(int j = -FLOOR_SIZE/2; j < FLOOR_SIZE/2; ++ j){
            float x = 1.0*i / (FLOOR_SIZE/2) * FLOOR_MAX_POSITION;
            float y = 0;
            float z = 1.0*j / (FLOOR_SIZE/2) * FLOOR_MAX_POSITION;
            floorVertices.push_back(x);
            floorVertices.push_back(y);
            floorVertices.push_back(z);
            floorVertices.push_back(FLOOR_COLOR);
            floorVertices.push_back(FLOOR_COLOR);
            floorVertices.push_back(FLOOR_COLOR);
            floorVertices.push_back(0);


        }
    }
}

void floorInit(){

    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof floorVerticesDefault, &floorVerticesDefault, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof floorElementDefault, &floorElementDefault, GL_STATIC_DRAW);
}

void drawFloor(){
    glBindVertexArray(floorVAO);
    glDrawElements(GL_TRIANGLES, sizeof floorElementDefault, GL_UNSIGNED_INT, 0);
}