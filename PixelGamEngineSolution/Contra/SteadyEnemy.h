#pragma once
#include "GameObject.h"
#include "Player.h"

class FSteadyEnemy : public FGameObject
{
    int ShootCount;
    
    olc::vf2d VectorToPlayer;
    float DistanceToPlayer;
    float ShootCoolDown;

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
        ShootCoolDown = 1.5f;
        DistanceToPlayer = 0.0f;
        ShootCount = 0;
    }
    
    void AimToPlayer(const FPlayer& InPlayer)
    {
        if(InPlayer.GetX() >= X)
        {
            Direction = 1;
        }
        else
        {
            Direction = -1;
        }

        VectorToPlayer.x = InPlayer.GetAbsolutePosition().x - X;
        VectorToPlayer.y = InPlayer.GetAbsolutePosition().y - Y;
        
        DistanceToPlayer = (float)sqrt(pow(VectorToPlayer.x, 2) + pow(VectorToPlayer.y, 2));

        Aim.x = VectorToPlayer.x / DistanceToPlayer;
        Aim.y = VectorToPlayer.y / DistanceToPlayer;

        Crosshair = { (Aim.x * 10) + X + Width / 2 , (Aim.y * 10) + Y + Height / 2};
    }

    //-----------------------------------------------------------------------------------------------------

    float GetShootCoolDown() const
    {
        return ShootCoolDown;
    }

    void SetShootCoolDown(const float InCoolDown)
    {
        ShootCoolDown = InCoolDown;
    }

    void AddToShootCoolDown(const float InValue)
    {
        ShootCoolDown += InValue;
    }

    //-----------------------------------------------------------------------------------------------------

    int GetShootCount() const
    {
        return ShootCount;
    }

    void SetShootCount(const int InCount)
    {
        ShootCount = InCount;
    }

    void AddToShootCount(const int InCount)
    {
        ShootCount += InCount;
    }

    
};

