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
	int Id;

	bool bIsDead;

public:

	FGameObject() : X(0), Y(0), VelX(0), VelY(0), Width(0), Height(0), Lives(0), Direction(0), Id(0), bIsDead(false) {}

	void UpdatePosition(const float InX, const float InY);

	void UpdatePosition(const float InX);

	//---------------------------------------------------------------------------------------------

	int GetID() const;

	//---------------------------------------------------------------------------------------------

	float GetX() const;

	void SetX(const float InX);

	//---------------------------------------------------------------------------------------------

	float GetY() const;

	void SetY(const float InY);

	//---------------------------------------------------------------------------------------------

	int GetLives() const;

	void SetLives(const int InValue);

	void AddToLives(const int InValue);

	//---------------------------------------------------------------------------------------------

	int GetDirection() const;

	void SetDirection(const int InDirection);

	//---------------------------------------------------------------------------------------------

	float GetVelocity_X() const;

	void SetVelocity_X(const float InVelocity);

	void AddToVelocity_X(const float InVelocity);

	//---------------------------------------------------------------------------------------------

	float GetVelocity_Y() const;

	void SetVelocity_Y(const float InVelocity);

	void AddToVelocity_Y(const float InVelocity);

	//---------------------------------------------------------------------------------------------

	olc::vf2d GetAim() const;

	void SetAim(const int AimX, const int AimY);

	void SetAimX(const int AimX);

	void SetAimY(const int AimY);

	//---------------------------------------------------------------------------------------------

	float GetWidth() const;

	void SetWidth(const float InWidth);

	//---------------------------------------------------------------------------------------------

	float GetHeight() const;

	void SetHeight(const float InHeight);

	//---------------------------------------------------------------------------------------------

	olc::vf2d GetAbsolutePosition() const;

	void SetAbsolutePosition(const float InX, const float InY);

	//---------------------------------------------------------------------------------------------

	olc::vf2d GetCrosshair() const;

	void SetCrosshair(const float InX, const float InY);

	//---------------------------------------------------------------------------------------------

	bool GetIsDead() const;

	void SetIsDead(const bool InState);
};
