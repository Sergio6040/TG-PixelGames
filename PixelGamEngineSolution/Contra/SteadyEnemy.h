#pragma once
#include "GameObject.h"
#include "Player.h"

class FSteadyEnemy : public FGameObject
{
    olc::vf2d VectorToPlayer;
    float DistanceToPlayer = 0.0f;

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
        bIsDead = false;
    }
    
    void AimToPlayer(FPlayer& InPlayer)
    {
        VectorToPlayer.x = InPlayer.GetAbsolutePosition().x - X;
        VectorToPlayer.y = InPlayer.GetAbsolutePosition().y - Y;

        DistanceToPlayer = sqrt(pow(VectorToPlayer.x, 2) + pow(VectorToPlayer.y, 2));

        Aim.x = VectorToPlayer.x / DistanceToPlayer;
        Aim.y = VectorToPlayer.y / DistanceToPlayer;

        Crosshair = { (Aim.x * 10) + X + Width / 2 , (Aim.y * 10) + Y + Height / 2};
    }
    
};

