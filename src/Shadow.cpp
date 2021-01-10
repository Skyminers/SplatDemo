//
// Created by 刘一辰 on 2021/1/10.
//

#include "Shadow.h"
#include "defineList.h"

// * * * * * * *
// I find that I must declare depthTexture in 'floor.cpp', so I move all code below to 'floor.cpp'
// * * * * * * *

//unsigned int depthFBO, depthTexture;
//
//void shadowInit(){
//    glGenFramebuffers(1, &depthFBO);
//
//    //Generate a empty texture
//    glGenTextures(1, &depthTexture);
//    glBindTexture(GL_TEXTURE_2D, depthTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//    // Bind frame buffer
//    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//void shadowBind(){
//    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
//    glClear(GL_DEPTH_BUFFER_BIT);
//}
//
//void shadowUnBind(){
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glViewport(0, 0, SCR_WIDTH*2, SCR_HEIGHT*2); // TODO: 为什么 SCR_WIDTH 需要设置为 2 倍 ？
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}