//
// Created by 刘一辰 on 2021/1/9.
//

#ifndef SPLATDEMO_DEFINELIST_H
#define SPLATDEMO_DEFINELIST_H

// Lighting
#define LIGHT_POS 2*FLOOR_MAX_POSITION,100.0f,0.5*FLOOR_MAX_POSITION

// Floor and world position limitations
#define FLOOR_SIZE 300
#define FLOOR_MAX_POSITION 50
#define FLOOR_UNIT (2.0*FLOOR_MAX_POSITION/FLOOR_SIZE)
#define FLOOR_COLOR 0.6
#define GET_POS(x,y) ((x)*(FLOOR_SIZE+1)+(y))
#define OUT_RADIUS 6
#define IN_RADIUS 2
#define FLOOR_ELEMENT_COUNT 9

// Shadow
#define SHADOW_WIDTH 2048
#define SHADOW_HEIGHT 2048
#define SHADOW_LIGHT_POS FLOOR_MAX_POSITION-5,30.0f,0
#define SHADOW_WATCH_POS 0,0,0

// Window
#define SCR_WIDTH 800
#define SCR_HEIGHT 600

#endif //SPLATDEMO_DEFINELIST_H
