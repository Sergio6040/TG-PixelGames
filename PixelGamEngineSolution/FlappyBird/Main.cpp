#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


class FFlappyBird : public olc::PixelGameEngine
{
private:
	float BirdPosition_y = 0.0f;
	float BirdVelocity_y = 0.0f;
	float BirdAcceleration_y = 0.0f;

	float Gravity = 100.0f;


public:
	FFlappyBird()
	{
		sAppName = "Example";
	}

protected:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float ElapsedTime) override
	{
		// called once per frame


		if (GetKey(olc::SPACE).bPressed)
		{
			BirdAcceleration_y = 0.0f;
			BirdVelocity_y = -Gravity / 4.0f;
		}
		else
		{
			BirdAcceleration_y += Gravity * ElapsedTime;
		}

		if (BirdAcceleration_y >= Gravity)
		{
			BirdAcceleration_y = Gravity;
		}

		//physics
		BirdVelocity_y += BirdAcceleration_y * ElapsedTime;
		BirdPosition_y += BirdVelocity_y * ElapsedTime;

		int Bird_X = (int)(ScreenWidth() / 3.0f);//Sets the x position of the bird, this never changes

		//Draw bird
		if (BirdVelocity_y > 0)
		{
			Draw(Bird_X, BirdPosition_y + 0, olc::WHITE);
			Draw(Bird_X, BirdPosition_y + 1, olc::BLUE);
		}
		else
		{
			Draw(Bird_X, BirdPosition_y + 0, olc::BLUE);
			Draw(Bird_X, BirdPosition_y + 1, olc::WHITE);
		}

		return true;
	}
};

int main()
{
	FFlappyBird Game;
	Game.Construct(80, 48, 16, 16);
	Game.Start();

}
