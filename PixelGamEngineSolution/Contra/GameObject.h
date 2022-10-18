#pragma once
#include "olcPixelGameEngine.h"

class FGameObject
{
public:
    float X = 0.0f;
    float Y = 0.0f;
    float VelX = 0.0f;
    float VelY = 0.0f;

    olc::vf2d Aim = {1.0f, 0.0f};

    int Lives;
    int Direction;
    
};
