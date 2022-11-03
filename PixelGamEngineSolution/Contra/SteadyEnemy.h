#pragma once
#include "GameObject.h"
#include "Player.h"

class FSteadyEnemy : public FGameObject
{
private:
	int ShootCount;

	olc::vf2d VectorToPlayer;
	float DistanceToPlayer;
	float ShootCoolDown;

public:

	FSteadyEnemy(const float InX, const float InY, const int InId);

	void AimToPlayer(const FPlayer& InPlayer);

	//-----------------------------------------------------------------------------------------------------

	float GetShootCoolDown() const;

	void SetShootCoolDown(const float InCoolDown);

	void AddToShootCoolDown(const float InValue);

	//-----------------------------------------------------------------------------------------------------

	int GetShootCount() const;

	void SetShootCount(const int InCount);

	void AddToShootCount(const int InCount);

};

