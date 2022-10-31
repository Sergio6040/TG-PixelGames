#include "Bullet.h"

FBullet::FBullet(const float InX, const float InY, const olc::vf2d& InAim)
{
	X = InX;
	Y = InY;
	Aim = InAim;
	VelX = Speed * Aim.x;
	VelY = Speed * Aim.y;
}

void FBullet::UpdatePosition(const float fElapsedTime)
{
	X += VelX * fElapsedTime;
	Y += VelY * fElapsedTime;
}
