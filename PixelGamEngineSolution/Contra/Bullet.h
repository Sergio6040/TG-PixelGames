#pragma once
#include "GameObject.h"

class FBullet : public FGameObject
{
    float Speed = 200.f;
public:

    FBullet(const float InX, const float InY, const olc::vf2d& InAim)
    {
        X = InX;
        Y = InY;
        Aim = InAim;
        VelX = Speed * Aim.x;
        VelY = Speed * Aim.y;
    }

    void UpdatePosition(const float fElapsedTime)
    {
        X += VelX * fElapsedTime;
        Y += VelY * fElapsedTime;
    }
    
};
