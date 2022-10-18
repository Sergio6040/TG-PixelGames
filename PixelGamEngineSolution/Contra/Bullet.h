#pragma once
#include "GameObject.h"

class FBullet : public FGameObject
{
    float Speed = 5.0f;
public:

    FBullet(const float InX, const float InY, const olc::vi2d& InAim)
    {
        X = InX;
        Y = InY;
        Aim = InAim;
        VelX = Speed * Aim.x;
        VelY = Speed * Aim.y;
    }

    void UpdatePosition()
    {
        X += VelX;
        Y += VelY;
    }
    
};