//
// Created by 刘一辰 on 2020/10/25.
//

#ifndef GLTEST_SPHERE_H
#define GLTEST_SPHERE_H

#include <glad/glad.h>
#include <vector>
#include <cmath>
#include <iostream>

class Sphere {
public:
    explicit Sphere(float radius = RADIUS, int xFragment = X_FRAGMENT, int yFragment = Y_FRAGMENT);
    unsigned int getVertexSize();
    unsigned int getIndicesSize();

    float* getVertexAddress();
    int* getIndicesAddress();

private:
    static constexpr float RADIUS = 1.0f;
    static constexpr int X_FRAGMENT = 50;
    static constexpr int Y_FRAGMENT = 50;
    std::vector<float> vertex;
    std::vector<int> indices;
    void updateVertex();
};


#endif //GLTEST_SPHERE_H
