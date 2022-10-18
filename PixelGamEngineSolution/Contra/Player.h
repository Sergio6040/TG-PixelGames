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

    // const olc::vf2d Crosshair = {(Aim.x * 10) + PlayerPos.x + 32.f/2, (aux.y * 10) + PlayerPos.y + 42.f/2};


public:
    FPlayer() = default;
    
    FPlayer(const float InX, const float InY)
    {
        X = InX;
        Y = InY;
        Lives = 3;
        Direction = 1;
        bIsGhost = false;
    }

    //---------------------------------------------------------------------------------------------
    
    void PlayerMovement(const float fElapsedTime)
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
    
    float GetX() const
    {
        return X;
    }

    void SetX(const float InX)
    {
        X = InX;
    }

    //---------------------------------------------------------------------------------------------

    float GetY() const
    {
        return Y;
    }

    void SetY(const float InY)
    {
        Y = InY;
    }

    //---------------------------------------------------------------------------------------------

    int GetLives() const 
    {
        return Lives;
    }

    //---------------------------------------------------------------------------------------------

    int GetDirection() const
    {
        return Direction;
    }
    
    void SetDirection(const int InDirection)
    {
        Direction = InDirection;
    }

    //---------------------------------------------------------------------------------------------

    float GetVelocity_X() const
    {
        return VelX;
    }
    
    void SetVelocity_X(const float InVelocity)
    {
        VelX = InVelocity;
    }

    void AddToVelocity_X(const float InVelocity)
    {
        VelX += InVelocity;
    }

    //---------------------------------------------------------------------------------------------

    float GetVelocity_Y() const
    {
        return VelY;
    }
    
    void SetVelocity_Y(const float InVelocity)
    {
        VelY = InVelocity;
    }
    
    void AddToVelocity_Y(const float InVelocity)
    {
        VelY += InVelocity;
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

    olc::vf2d GetAim() const
    {
        return Aim;
    }

    void SetAim(const int AimX, const int AimY)
    {
        Aim.x = AimX;
        Aim.y = AimY;
    }

    void SetAimX(const int AimX)
    {
        Aim.x = AimX;
    }

    void SetAimY(const int AimY)
    {
        Aim.y = AimY;
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
