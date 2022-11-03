#pragma once

#include "olcPixelGameEngine.h"
#include "Player.h"

class FInputHandler
{
	olc::PixelGameEngine* PixelEngine;
public:

	FInputHandler(olc::PixelGameEngine* InEngine)
	{
		PixelEngine = InEngine;
	}

	void PlayerInput(FPlayer& Player) const;

	void MovementInputs(FPlayer& Player) const;

	void TwoInputs(FPlayer& Player) const;

	void StopAiming(FPlayer& Player) const;

	void DownInputs(FPlayer& Player) const;

};
