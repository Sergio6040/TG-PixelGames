#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


class FFlappyBird : public olc::PixelGameEngine
{
private:
	float BirdPosition_y = 0.0f;
	float BirdVelocity_y = 0.0f;
	float BirdAcceleration_y = 0.0f;

	float Gravity = 100.0f;

	float SectionWidth;
	std::list<int> ListSection;
	float LevelPosition = 0.0f;

	bool bHasCollided = false;

	bool bResetGame = false;

	int MaxFlapCount = 0;

	int FlapsCount = 0;
	int AttemptsCount = 0;

public:
	FFlappyBird()
	{
		sAppName = "Example";
	}

protected:
	bool OnUserCreate() override
	{
		ListSection = { 0, 0, 0, 0 };
		SectionWidth = (float)ScreenWidth() / (float)(ListSection.size() - 1);
		return true;
	}

	bool OnUserUpdate(float ElapsedTime) override
	{
		// called once per frame

		if (bResetGame)
		{
			bHasCollided = false;
			bResetGame = false;
			ListSection = { 0, 0, 0, 0 };
			BirdAcceleration_y = 0;
			BirdVelocity_y = 0;
			BirdPosition_y = ScreenHeight() / 2.0f;
			FlapsCount = 0;
			AttemptsCount++;
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

			if (GetKey(olc::SPACE).bPressed && BirdVelocity_y >= Gravity / 2.0f)
			{
				BirdAcceleration_y = 0.0f;
				BirdVelocity_y = -Gravity / 2.0f;
				FlapsCount++;
				if (FlapsCount > MaxFlapCount)
				{
					MaxFlapCount = FlapsCount;
				}
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

			LevelPosition += 14.0f * ElapsedTime;

			if (LevelPosition > SectionWidth)
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
			for (auto s : ListSection)
			{
				if (s != 0)
				{
					DrawRect(SectionIndex * SectionWidth + 10 - LevelPosition, ScreenHeight() - s,
						SectionWidth / 3, ScreenHeight(),
						olc::GREEN);

					DrawRect(SectionIndex * SectionWidth + 10 - LevelPosition, 0,
						SectionWidth / 3, ScreenHeight() - s - 15,
						olc::GREEN);
				}
				SectionIndex++;
			}


			int Bird_X = (int)(ScreenWidth() / 4.0f);//Sets the x position of the bird, this never changes

			//Draw bird
			if (BirdVelocity_y > 0)
			{
				DrawString(Bird_X, BirdPosition_y + 0, " ////");
				DrawString(Bird_X, BirdPosition_y + 8, "</////O>");
			}
			else
			{
				DrawString(Bird_X, BirdPosition_y + 0, "</////O>");
				DrawString(Bird_X, BirdPosition_y + 8, " ////");
			}

			DrawString(1, 1, "| Attemps: " + std::to_string(AttemptsCount) + "| Flaps: " + std::to_string(FlapsCount) + "| Max Score:" + std::to_string(MaxFlapCount));
		}

		return true;
	}
};

int main()
{
	FFlappyBird Game;
	Game.Construct(600, 400, 2, 2);
	Game.Start();

}
