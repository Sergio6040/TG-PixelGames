#include "GameObject.h"

void FGameObject::UpdatePosition(const float InX, const float InY)
{
	if (!bIsDead)
	{
		X = InX;
		Y = InY;
	}
}

void FGameObject::UpdatePosition(const float InX)
{
	X = InX;
}
//---------------------------------------------------------------------------------------------

int FGameObject::GetID() const
{
	return Id;
}

//---------------------------------------------------------------------------------------------

float FGameObject::GetX() const
{
	return X;
}

void FGameObject::SetX(const float InX)
{
	X = InX;
}

//---------------------------------------------------------------------------------------------

float FGameObject::GetY() const
{
	return Y;
}

void FGameObject::SetY(const float InY)
{
	Y = InY;
}

//---------------------------------------------------------------------------------------------

int FGameObject::GetLives() const
{
	return Lives;
}

void FGameObject::SetLives(const int InValue)
{
	Lives = InValue;
}

void FGameObject::AddToLives(const int InValue)
{
	Lives += InValue;
}

//---------------------------------------------------------------------------------------------

int FGameObject::GetDirection() const
{
	return Direction;
}

void FGameObject::SetDirection(const int InDirection)
{
	Direction = InDirection;
}

//---------------------------------------------------------------------------------------------

float FGameObject::GetVelocity_X() const
{
	return VelX;
}

void FGameObject::SetVelocity_X(const float InVelocity)
{
	VelX = InVelocity;
}

void FGameObject::AddToVelocity_X(const float InVelocity)
{
	VelX += InVelocity;
}

//---------------------------------------------------------------------------------------------

float FGameObject::GetVelocity_Y() const
{
	return VelY;
}

void FGameObject::SetVelocity_Y(const float InVelocity)
{
	VelY = InVelocity;
}

void FGameObject::AddToVelocity_Y(const float InVelocity)
{
	VelY += InVelocity;
}

//---------------------------------------------------------------------------------------------

olc::vf2d FGameObject::GetAim() const
{
	return Aim;
}

void FGameObject::SetAim(const int AimX, const int AimY)
{
	Aim.x = AimX;
	Aim.y = AimY;
}

void FGameObject::SetAimX(const int AimX)
{
	Aim.x = AimX;
}

void FGameObject::SetAimY(const int AimY)
{
	Aim.y = AimY;
}

//---------------------------------------------------------------------------------------------

float FGameObject::GetWidth() const
{
	return Width;
}

void FGameObject::SetWidth(const float InWidth)
{
	Width = InWidth;
}

//---------------------------------------------------------------------------------------------

float FGameObject::GetHeight() const
{
	return Height;
}

void FGameObject::SetHeight(const float InHeight)
{
	Height = InHeight;
}

//---------------------------------------------------------------------------------------------

olc::vf2d FGameObject::GetAbsolutePosition() const
{
	return AbsolutePosition;
}

void FGameObject::SetAbsolutePosition(const float InX, const float InY)
{
	AbsolutePosition = { InX, InY };
}

//---------------------------------------------------------------------------------------------

olc::vf2d FGameObject::GetCrosshair() const
{
	return Crosshair;
}

void FGameObject::SetCrosshair(const float InX, const float InY)
{
	Crosshair = { InX, InY };
}

//---------------------------------------------------------------------------------------------

bool FGameObject::GetIsDead() const
{
	return bIsDead;
}

void FGameObject::SetIsDead(const bool InState)
{
	bIsDead = InState;
}
