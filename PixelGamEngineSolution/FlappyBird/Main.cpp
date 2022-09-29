#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "Bird.h"


class FFlappyBird : public olc::PixelGameEngine
{
private:

	float Gravity = 100.0f;

	float SectionWidth;
	std::list<int> ListSection;
	float LevelPosition = 0.0f;

	bool bHasCollided = false;
	bool bResetGame = true;

	int MaxFlapCount = 0;
	int FlapsCount = 0;
	int AttemptsCount = 0;

	FBird RedBird;

	olc::Sprite* BgSprite = nullptr;
	olc::Decal* BgDecal = nullptr;

	olc::Sprite* UpSprite = nullptr;
	olc::Decal* UpDecal = nullptr;

	olc::Sprite* DownSprite = nullptr;
	olc::Decal* DownDecal = nullptr;

	olc::Sprite* PipeSprite = nullptr;
	olc::Decal* PipeDecal = nullptr;

public:
	FFlappyBird()
	{
		sAppName = "Flappy Bird";
	}

protected:
	bool OnUserCreate() override
	{
		ListSection = { 0, 0, 0 };
		SectionWidth = (float)ScreenWidth() / (float)(ListSection.size() - 1);

		BgSprite = new olc::Sprite("./sprites/background-day.png");
		BgDecal = new olc::Decal(BgSprite);

		UpSprite = new olc::Sprite("./sprites/redbird-upflap.png");
		UpDecal = new olc::Decal(UpSprite);

		DownSprite = new olc::Sprite("./sprites/redbird-downflap.png");
		DownDecal = new olc::Decal(DownSprite);

		PipeSprite = new olc::Sprite("./sprites/pipe-green-down.png");
		PipeDecal = new olc::Decal(PipeSprite);


		return true;
	}

	bool OnUserUpdate(float ElapsedTime) override
	{
		if (bResetGame)
		{
			ResetGame();
		}

		if (bHasCollided)
		{
			if (GetKey(olc::SPACE).bReleased)
			{
				bResetGame = true;
			}
		}
		else
		{
			Clear(olc::BLACK);
			//DrawDecal(olc::vf2d(0, 0), BgDecal, {0.15f, 0.15f});//Print BackGround

			if (GetKey(olc::SPACE).bPressed && RedBird.GetVelocityY() >= Gravity / 3.0f)
			{
				RedBird.Jump(Gravity);

				FlapsCount++;
				if (FlapsCount > MaxFlapCount)
				{
					MaxFlapCount = FlapsCount;
				}
			}
			else
			{
				RedBird.Fall(Gravity * ElapsedTime);
				//BirdAcceleration_y += Gravity * ElapsedTime;
			}

			if (RedBird.GetAccelerationY() >= Gravity)
			{
				RedBird.SetAccelerationY(Gravity);
			}

			//physics
			RedBird.UpdatePosition(ElapsedTime);

			LevelPosition += 14.0f * ElapsedTime;

			SectionThing();

			DrawBird();

			//RedBird.SetPositionY(BirdPosition_y);


			DrawString(1, 1, "| Attemps: " + std::to_string(AttemptsCount) + "| Flaps: " + std::to_string(FlapsCount) + "| Max Score:" + std::to_string(MaxFlapCount));
		}

		return true;
	}

	void DrawBird()

	{
		if (RedBird.GetVelocityY() > 0)
		{
			/*DrawString(Bird_X, BirdPosition_y + 0, " ////");
			DrawString(Bird_X, BirdPosition_y + 8, "</////O>");*/
			DrawDecal(RedBird.GetPosition(), UpDecal, { 0.12f, 0.12f });

		}
		else
		{
			/*DrawString(Bird_X, BirdPosition_y + 0, "</////O>");
			DrawString(Bird_X, BirdPosition_y + 8, " ////");*/
			DrawDecal(RedBird.GetPosition(), DownDecal, { 0.12f, 0.12f });

		}
	}

	void SectionThing()
	{
		if (LevelPosition + 10 > SectionWidth)
		{
			LevelPosition -= SectionWidth;
			ListSection.pop_front();
			int i = rand() % (ScreenHeight() - 20);

			if (i <= 10)
			{
				i = 0;
			}
			ListSection.push_back(i);
		}

		int SectionIndex = 0;
		for (int PipeSize : ListSection)
		{
			if (PipeSize != 0)
			{
				olc::vf2d PipePositionUp = { (float)(SectionIndex * SectionWidth - LevelPosition), (float)(ScreenHeight() + PipeSize) };
				//olc::vf2d PipePositionDown = { (float)(SectionIndex * SectionWidth + 20 - LevelPosition), (float)(ScreenHeight() - PipeSize) };

				DrawDecal(PipePositionUp, PipeDecal, { 0.15f, -0.15f });
				//DrawDecal(PipePositionDown, PipeDecal, { 0.15f, 0.15f });


				DrawRect(SectionIndex * SectionWidth + 10 - LevelPosition, ScreenHeight() - PipeSize,
					SectionWidth / 3, ScreenHeight(),
					olc::GREEN);

				DrawRect(SectionIndex * SectionWidth + 10 - LevelPosition, 0,
					SectionWidth / 3, ScreenHeight() - PipeSize - 15,
					olc::GREEN);
			}
			SectionIndex++;
		}
	}

	void ResetGame()
	{
		bHasCollided = false;
		bResetGame = false;

		ListSection = { 0, 0, 0 };

		FlapsCount = 0;
		AttemptsCount++;

		RedBird.SetAccelerationY(0);
		RedBird.SetVelocityY(0);
		RedBird.SetPosition((int)(ScreenWidth() / 5.0f), ScreenHeight() / 3.0f);
	}
};

int main()
{
	FFlappyBird Game;
	Game.Construct(40, 60, 16, 16);
	Game.Start();

}
