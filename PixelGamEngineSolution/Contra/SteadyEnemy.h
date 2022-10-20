#pragma once
#include "GameObject.h"

class FSteadyEnemy : public FGameObject
{
public:
    FSteadyEnemy(const float InX, const float InY, const int InId)
    {
        X = InX;
        Y = InY;
        VelX = 0;
        VelY = 0;
        Width = 32;
        Height = 42;
        Direction = -1;
        Aim = {-1.0f, 0.0f};
        Lives = 1; 
        Id = InId;
    }
    
    
};
