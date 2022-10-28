#pragma once
#include "GameObject.h"
#include "Bullet.h"

class FPlayer : public FGameObject
{
private:
    
    float JumpForce = 6.0f;
    float Gravity = 9.0f;
    float HitRadius = 7.0f;
    float GhostCounter = 0.0f;

    olc::vf2d HitPosition;
    float HitOffsetY = 0.0f;

    bool bOnGround = false;
    bool bIsGhost = false;
    bool bCollidesGround = true;
    bool bIsCrouched = false;
    bool bIsShooting = false;
    bool bIsMoving = false;

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
        Aim = { 1, 0 };
    }
    
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
        Aim = { 1, 0 };
    }

    //---------------------------------------------------------------------------------------------
    
    void UpdatePosition(const float fElapsedTime, const float InOffsetX, const float InTileWidth, const float InTileHeight)
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

        AbsolutePosition = { (X - InOffsetX) * InTileWidth, (Y) * InTileHeight };

        HitPosition = { AbsolutePosition.x + Width / 2 , (AbsolutePosition.y + Height / 2) + HitOffsetY };

        Crosshair = { (Aim.x * 10) + HitPosition.x, (Aim.y * 10) + HitPosition.y };
    }

    //---------------------------------------------------------------------------------------------

    void Jump()
    {
        if(VelY == 0.0f)
        {
            AddToVelocity_Y(-JumpForce);
        }
    }

    //---------------------------------------------------------------------------------------------

    void Respawn()
    {
        bIsDead = false;
        X = 1;
        Y = 0;
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

    //---------------------------------------------------------------------------------------------

    float GetHitRadius() const
    {
        return HitRadius;
    }

    void SetRadius(const float InRadius)
    {
        HitRadius = InRadius;
    }

    //---------------------------------------------------------------------------------------------

    olc::vf2d GetHitPosition() const
    {
        return HitPosition;
    }

    void SetHitPosition(const float InX, const float InY)
    {
        HitPosition = {InX, InY};
    }

    //---------------------------------------------------------------------------------------------

    float GetHitOffsetY() const
    {
        return HitOffsetY;
    }

    void SetHitOffsetY(const float InOffset)
    {
        HitOffsetY = InOffset;
    }

    //---------------------------------------------------------------------------------------------

    bool GetIsCrouched() const
    {
        return bIsCrouched;
    }

    void SetIsCrouched(bool InState)
    {
        bIsCrouched = InState;
    }

    //---------------------------------------------------------------------------------------------

    bool GetIsShooting() const
    {
        return bIsShooting;
    }

    void SetIsShooting(const bool InState)
    {
        bIsShooting = InState;
    }

    //---------------------------------------------------------------------------------------------

    bool GetIsMoving() const
    {
        return bIsMoving;
    }

    void SetIsMoving(const bool InState)
    {
        bIsMoving = InState;
    }

};
