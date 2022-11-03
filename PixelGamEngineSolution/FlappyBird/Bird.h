#pragma once
#include "olcPixelGameEngine.h"


class FBird
{
private:
	olc::vf2d Position = {0, 0};

	float VelocityY = 0.0f;
	float AccelerationY = 0.0f;

	float JumpForce = 5.0f;

	olc::Sprite* UpImage = nullptr;
	olc::Decal* UpSprite = nullptr;

	olc::Sprite* DownImage = nullptr;
	olc::Decal* DownSprite = nullptr;

public:
	FBird()
	{
		UpImage = new olc::Sprite("./sprites/redbird-upflap.png");
		UpSprite = new olc::Decal(UpImage);

		DownImage = new olc::Sprite("./sprites/redbird-downflap.png");
		DownSprite = new olc::Decal(DownImage);
	}

	~FBird()
	{
		delete UpImage;
		delete UpSprite;

		delete DownImage;
		delete DownSprite;
	}

	void Jump(const float InGravity)
	{
		AccelerationY = 0.0f;
		VelocityY = -InGravity / JumpForce;
	}

	void Fall(const float InValue)
	{
		AccelerationY += InValue;
	}

	void UpdatePosition(const float InTime)
	{
		VelocityY += AccelerationY * InTime;
		Position.y += VelocityY * InTime;
	}

	olc::Decal* GetDecalUp() const
	{
		return UpSprite;
	}

	olc::Sprite* GetSprite()
	{
		return UpImage;
	}

	olc::vf2d GetPosition() const
	{
		return Position;
	}

	void SetPosition(const float InX, const float InY)
	{
		Position.x = InX;
		Position.y = InY;
	}

	void SetPositionY(const float InY)
	{
		Position.y = InY;
	}
	
	//-----------------------------------------------------------------------------------

	float GetVelocityY() const
	{
		return VelocityY;
	}

	void SetVelocityY(const float InVelocity)
	{
		VelocityY = InVelocity;
	}

	//-----------------------------------------------------------------------------------

	float GetAccelerationY() const
	{
		return AccelerationY;
	}

	void SetAccelerationY(const float InAcceleration)
	{
		AccelerationY = InAcceleration;
	}
};
