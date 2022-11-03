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
	FPlayer();

	FPlayer(const float InX, const float InY);

	//---------------------------------------------------------------------------------------------

	void UpdatePosition(const float fElapsedTime, const float InOffsetX, const float InTileWidth, const float InTileHeight);

	//---------------------------------------------------------------------------------------------

	void Jump();

	//---------------------------------------------------------------------------------------------

	void Respawn();

	//---------------------------------------------------------------------------------------------

	void ClampVelocities();

	//---------------------------------------------------------------------------------------------

	bool GetOnGround() const;

	void SetOnGround(bool InState);

	//---------------------------------------------------------------------------------------------

	bool GetIsGhost() const;

	void SetIsGhost(bool InState);

	//---------------------------------------------------------------------------------------------

	float GetGhostCounter() const;

	void SetGhostCounter(const float InValue);

	void AddToGhostCounter(const float InValue);

	//---------------------------------------------------------------------------------------------

	bool GetCollidesGround() const;

	void SetCollidesGround(const bool InState);

	//---------------------------------------------------------------------------------------------

	float GetHitRadius() const;

	void SetRadius(const float InRadius);

	//---------------------------------------------------------------------------------------------

	olc::vf2d GetHitPosition() const;

	void SetHitPosition(const float InX, const float InY);

	//---------------------------------------------------------------------------------------------

	float GetHitOffsetY() const;

	void SetHitOffsetY(const float InOffset);

	//---------------------------------------------------------------------------------------------

	bool GetIsCrouched() const;

	void SetIsCrouched(bool InState);

	//---------------------------------------------------------------------------------------------

	bool GetIsShooting() const;

	void SetIsShooting(const bool InState);

	//---------------------------------------------------------------------------------------------

	bool GetIsMoving() const;

	void SetIsMoving(const bool InState);

};
