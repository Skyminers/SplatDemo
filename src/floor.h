//
// Created by 刘一辰 on 2021/1/6.
//

#ifndef SPLATDEMO_FLOOR_H
#define SPLATDEMO_FLOOR_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>

float floorVertices[] = {
        -100.0f, 0.0f, -100.0f, 1.0f, 1.0f, 1.0f,
        -100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f,
         100.0f, 0.0f, -100.0f, 1.0f, 1.0f, 1.0f,
         100.0f, 0.0f,  100.0f, 1.0f, 1.0f, 1.0f
};

unsigned int floorElement[] = {
        0, 1, 2,
        1, 2, 3
};

unsigned int floorVAO, floorVBO, floorEBO;
unsigned int floorTexture; // unused

void floorInit(){
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof floorVertices, &floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof floorElement, &floorElement, GL_STATIC_DRAW);
}

void drawFloor(){
    glBindVertexArray(floorVAO);
    glDrawElements(GL_TRIANGLES, sizeof floorElement, GL_UNSIGNED_INT, 0);
}

#endif //SPLATDEMO_FLOOR_H
