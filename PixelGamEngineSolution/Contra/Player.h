#pragma once
#include "GameObject.h"
#include "Bullet.h"

class FPlayer : public FGameObject
{
private:
    
    float JumpForce = 5.0f;
    float Gravity = 5.0f;

    bool bOnGround = false;
    bool bIsGhost = false;
    bool bCollidesGround = true;

public:
    FPlayer()
    {
        X = 0.0f;
        Y = 0.0f;
        VelX = 0.0f;
        VelY = 0.0f;
        Lives = 3;
        Direction = 1;
        bIsGhost = false;
        Width = 32;
        Height = 42;
    };
    
    FPlayer(const float InX, const float InY)
    {
        X = InX;
        Y = InY;
        VelX = 0.0f;
        VelY = 0.0f;
        Lives = 3;
        Direction = 1;
        bIsGhost = false;
        Width = 32;
        Height = 42;
    }

    //---------------------------------------------------------------------------------------------
    
    void UpdatePosition(const float fElapsedTime, const float InOffsetX, const float InOffsetY, const float InTileWidth, const float InTileHeight)
    {
        //Gravity
        VelY += Gravity * fElapsedTime;

        //Position
        X += VelX * fElapsedTime;
        Y += VelY * fElapsedTime;

        //Stop Movement
        if(bOnGround)
        {
            VelX = 0.0f;
        }

        ClampVelocities();

        AbsolutePosition = { (X - InOffsetX) * InTileWidth, (Y - InOffsetY) * InTileHeight };

        Crosshair = { (Aim.x * 10) + AbsolutePosition.x + Width / 2, (Aim.y * 10) + AbsolutePosition.y + Height / 2 };
    }

    //---------------------------------------------------------------------------------------------

    void Jump()
    {
        // VelY += JumpForce
        if(VelY == 0.0f)
        {
            AddToVelocity_Y(-JumpForce);
        }
    }

    //---------------------------------------------------------------------------------------------

    // void Shoot(std::vector<FBullet>& InBulletStorage)
    // {
    //     InBulletStorage.push_back(FBullet());
    // }

    
    //---------------------------------------------------------------------------------------------
    
    void ClampVelocities()
    {
        if(VelX > 10.0f)
        {
            VelX = 10.0f;
        }
        if(VelX < -10.0f)
        {
            VelX = -10.0f;
        }

        if(VelY > 10.0f)
        {
            VelY = 10.0f;
        }
        if(VelY < -10.0f)
        {
            VelY = -10.0f;
        }
    }

    //---------------------------------------------------------------------------------------------
    
    bool GetOnGround() const
    {
        return bOnGround;
    }

    void SetOnGround(bool InState)
    {
        bOnGround = InState;
    }

    //---------------------------------------------------------------------------------------------

    bool GetCollidesGround() const
    {
        return bCollidesGround;
    }

    void SetCollidesGround(const bool InState)
    {
        bCollidesGround = InState;
    }
};
