#pragma once
#include "GameObject.h"

class FBullet : public FGameObject
{
private:
    float Speed = 200.f;
    
public:

    FBullet(const float InX, const float InY, const olc::vf2d& InAim);

    void UpdatePosition(const float fElapsedTime);
    
};
