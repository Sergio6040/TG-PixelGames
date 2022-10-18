#pragma once
#include "GameObject.h"

class FSteadyEnemy : public FGameObject
{
    
public:
    FSteadyEnemy(const float InX, const float InY)
    {
        X = InX;
        Y = InY;
        Direction = -1;
        Aim = {-1.0f, 0.0f};
        Lives = 1; 
    }
    
};
