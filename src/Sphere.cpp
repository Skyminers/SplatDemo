//
// Created by 刘一辰 on 2020/10/25.
//

#include "Sphere.h"

Sphere::Sphere(float radius, int xFragment, int yFragment) {
    const double PI = acos(-1);
    for(int y = 0; y <= yFragment; ++ y){
        for(int x = 0; x <= xFragment; ++ x){
            float yFrac = (float ) y / yFragment;
            float xFrac = (float ) x / xFragment;
            float xPos = radius * std::cos(xFrac * 2.f * PI) * std::sin(yFrac * PI);
            float yPos = radius * std::cos(yFrac * PI);
            float zPos = radius * std::sin(xFrac * 2.f * PI) * std::sin(yFrac * PI);
            vertex.push_back(xPos);
            vertex.push_back(yPos);
            vertex.push_back(zPos);
//            vertex.push_back( (float )(10*x % 255) / 255);
//            vertex.push_back( (float )(10*y % 255) / 255);
//            vertex.push_back( (float )(10*(x+y) % 255) / 255);
            vertex.push_back( xFrac);
            vertex.push_back( yFrac);
        }
    }

    for(int y = 0; y < yFragment; ++ y){
        for(int x = 0;x < xFragment; ++ x){
            indices.push_back(y*(xFragment + 1) + x);
            indices.push_back((y+1)*(xFragment + 1) + x);
            indices.push_back((y+1)*(xFragment + 1) + x + 1);

            indices.push_back(y*(xFragment + 1) + x);
            indices.push_back(y*(xFragment + 1) + x + 1);
            indices.push_back((y+1)*(xFragment + 1) + x + 1);
        }
    }

}

unsigned int Sphere::getIndicesSize() {
    return indices.size();
}

unsigned int Sphere::getVertexSize() {
    return vertex.size();
}

int* Sphere::getIndicesAddress() {
    return &indices[0];
}

float* Sphere::getVertexAddress() {
    return &vertex[0];
}