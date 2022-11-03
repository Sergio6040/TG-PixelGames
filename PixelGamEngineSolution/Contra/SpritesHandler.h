#pragma once
#include "olcPixelGameEngine.h"

struct FSpritesHandler
{
	olc::Sprite* BackgroundSprite = new olc::Sprite("./Assets/Level.png");

	olc::Sprite* PlayerIdleSprite = new olc::Sprite("./Assets/Player_Idle.png");
	olc::Sprite* PlayerRunSprite = new olc::Sprite("./Assets/Player_Run.png");
	olc::Sprite* PlayerShootRunSprite = new olc::Sprite("./Assets/Player_Shoot_Run.png");
	olc::Sprite* PlayerJumpSprite = new olc::Sprite("./Assets/Player_Jump.png");
	olc::Sprite* PlayerCrouchSprite = new olc::Sprite("./Assets/Player_Crouch.png");
	olc::Sprite* PlayerAimDownSprite = new olc::Sprite("./Assets/Player_AimDown.png");
	olc::Sprite* PlayerAimUpSprite = new olc::Sprite("./Assets/Player_Aim_up.png");
	olc::Sprite* PlayerRunAimUpSprite = new olc::Sprite("./Assets/Player_Run_AimUp.png");
	olc::Sprite* PlayerDyingSprite = new olc::Sprite("./Assets/Player_Dying.png");
	olc::Sprite* PlayerDeadSprite = new olc::Sprite("./Assets/Player_Dead.png");

	olc::Sprite* MedalSprite = new olc::Sprite("./Assets/Medal.png");

	olc::Sprite* BulletSprite = new olc::Sprite("./Assets/Bullet.png");
	olc::Sprite* Bullet2Sprite = new olc::Sprite("./Assets/Bullet2.png");

	olc::Sprite* EnemyIdleSprite = new olc::Sprite("./Assets/Clown_1.png");
	olc::Sprite* EnemyDownSprite = new olc::Sprite("./Assets/Clown_1_Down.png");
	olc::Sprite* EnemyUpSprite = new olc::Sprite("./Assets/Clown_1_Up.png");

	olc::Sprite* EndGameSprite = new olc::Sprite("./Assets/EndGame.png");

	~FSpritesHandler()
	{
		delete BackgroundSprite;
		delete PlayerIdleSprite;
		delete PlayerRunSprite;
		delete PlayerShootRunSprite;
		delete PlayerJumpSprite;
		delete PlayerCrouchSprite;
		delete PlayerAimDownSprite;
		delete PlayerAimUpSprite;
		delete PlayerRunAimUpSprite;
		delete PlayerDyingSprite;
		delete PlayerDeadSprite;
		delete MedalSprite;
		delete BulletSprite;
		delete Bullet2Sprite;
		delete EnemyIdleSprite;
		delete EnemyDownSprite;
		delete EnemyUpSprite;
	}

};

