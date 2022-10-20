#pragma once
#include "olcPixelGameEngine.h"

class FGameObject
{
protected:
    float X;
    float Y;
    float VelX;
    float VelY;
    float Width;
    float Height;

    olc::vf2d Aim;
    olc::vf2d AbsolutePosition;
    olc::vf2d Crosshair;

    int Lives;
    int Direction;
    
public:
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

    float GetWidth() const
    {
        return Width;
    }

    void SetWidth(const float InWidth) 
    {
        Width = InWidth;
    }

    //---------------------------------------------------------------------------------------------

    float GetHeight() const
    {
        return Height;
    }

    void SetHeight(const float InHeight)
    {
        Height = InHeight;
    }

    //---------------------------------------------------------------------------------------------

    olc::vf2d GetAbsolutePosition() const
    {
        return AbsolutePosition;
    }

    void SetAbsolutePosition(const float InX, const float InY)
    {
        AbsolutePosition = { InX, InY };
    }

    //---------------------------------------------------------------------------------------------

    olc::vf2d GetCrosshair() const
    {
        return Crosshair;
    }

    void SetCrosshair(const float InX, const float InY)
    {
        Crosshair = { InX, InY };
    }
};
