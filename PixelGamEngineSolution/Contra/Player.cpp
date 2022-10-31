#include "Player.h"

FPlayer::FPlayer()
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

FPlayer::FPlayer(const float InX, const float InY)
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

void FPlayer::UpdatePosition(const float fElapsedTime, const float InOffsetX, const float InTileWidth, const float InTileHeight)
{
	//Gravity
	VelY += Gravity * fElapsedTime;

	//Position
	X += VelX * fElapsedTime;
	Y += VelY * fElapsedTime;

	//Stop Movement
	if (bOnGround)
	{
		VelX = 0.0f;
	}

	ClampVelocities();

	AbsolutePosition = { (X - InOffsetX) * InTileWidth, (Y)*InTileHeight };

	HitPosition = { AbsolutePosition.x + Width / 2 , (AbsolutePosition.y + Height / 2) + HitOffsetY };

	Crosshair = { (Aim.x * 10) + HitPosition.x, (Aim.y * 10) + HitPosition.y };
}

//---------------------------------------------------------------------------------------------

void FPlayer::Jump()
{
	if (VelY == 0.0f)
	{
		AddToVelocity_Y(-JumpForce);
	}
}

//---------------------------------------------------------------------------------------------

void FPlayer::Respawn()
{
	bIsDead = false;
	bIsGhost = true;
	GhostCounter = 1.f;
	X = 1;
	Y = 0;
}

//---------------------------------------------------------------------------------------------

void FPlayer::ClampVelocities()
{
	if (VelX > 10.0f)
	{
		VelX = 10.0f;
	}
	if (VelX < -10.0f)
	{
		VelX = -10.0f;
	}

	if (VelY > 10.0f)
	{
		VelY = 10.0f;
	}
	if (VelY < -10.0f)
	{
		VelY = -10.0f;
	}
}

//---------------------------------------------------------------------------------------------

bool FPlayer::GetOnGround() const
{
	return bOnGround;
}

void FPlayer::SetOnGround(bool InState)
{
	bOnGround = InState;
}

//---------------------------------------------------------------------------------------------

bool FPlayer::GetIsGhost() const
{
	return bIsGhost;
}

void FPlayer::SetIsGhost(bool InState)
{
	bIsGhost = InState;
}

//---------------------------------------------------------------------------------------------

float FPlayer::GetGhostCounter() const
{
	return GhostCounter;
}

void FPlayer::SetGhostCounter(const float InValue)
{
	GhostCounter = InValue;
}

void FPlayer::AddToGhostCounter(const float InValue)
{
	GhostCounter += InValue;
}

//---------------------------------------------------------------------------------------------

bool FPlayer::GetCollidesGround() const
{
	return bCollidesGround;
}

void FPlayer::SetCollidesGround(const bool InState)
{
	bCollidesGround = InState;
}

//---------------------------------------------------------------------------------------------

float FPlayer::GetHitRadius() const
{
	return HitRadius;
}

void FPlayer::SetRadius(const float InRadius)
{
	HitRadius = InRadius;
}

//---------------------------------------------------------------------------------------------

olc::vf2d FPlayer::GetHitPosition() const
{
	return HitPosition;
}

void FPlayer::SetHitPosition(const float InX, const float InY)
{
	HitPosition = { InX, InY };
}

//---------------------------------------------------------------------------------------------

float FPlayer::GetHitOffsetY() const
{
	return HitOffsetY;
}

void FPlayer::SetHitOffsetY(const float InOffset)
{
	HitOffsetY = InOffset;
}

//---------------------------------------------------------------------------------------------

bool FPlayer::GetIsCrouched() const
{
	return bIsCrouched;
}

void FPlayer::SetIsCrouched(bool InState)
{
	bIsCrouched = InState;
}

//---------------------------------------------------------------------------------------------

bool FPlayer::GetIsShooting() const
{
	return bIsShooting;
}

void FPlayer::SetIsShooting(const bool InState)
{
	bIsShooting = InState;
}

//---------------------------------------------------------------------------------------------

bool FPlayer::GetIsMoving() const
{
	return bIsMoving;
}

void FPlayer::SetIsMoving(const bool InState)
{
	bIsMoving = InState;
}
