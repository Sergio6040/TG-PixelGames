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

	olc::Sprite* OverSprite = nullptr;
	olc::Decal* OverDecal = nullptr;

	std::vector<olc::Sprite*> NumbersSprites;
	std::vector<olc::Decal*> NumbersDecal;

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

		OverSprite = new olc::Sprite("./sprites/gameover.png");
		OverDecal = new olc::Decal(OverSprite);

		NumbersSprites = {
			new olc::Sprite("./sprites/0.png"),
			new olc::Sprite("./sprites/1.png"),
			new olc::Sprite("./sprites/2.png"),
			new olc::Sprite("./sprites/3.png"),
			new olc::Sprite("./sprites/4.png"),
			new olc::Sprite("./sprites/5.png"),
			new olc::Sprite("./sprites/6.png"),
			new olc::Sprite("./sprites/7.png"),
			new olc::Sprite("./sprites/8.png"),
			new olc::Sprite("./sprites/9.png"),
		};

		for (olc::Sprite* InSprite : NumbersSprites)
		{
			NumbersDecal.push_back(new olc::Decal(InSprite));
		}

		return true;
	}

	// --------------------------------------------------------------------------------

	bool OnUserUpdate(float ElapsedTime) override
	{
		if (bResetGame)
		{
			ResetGame();
		}

		Clear(olc::BLACK);
		DrawDecal(olc::vf2d(0, 0), BgDecal, {0.15f, 0.15f});//Print BackGround
		SectionThing();
		DrawBird();

		if (bHasCollided)
		{
			DrawDecal(olc::vf2d(ScreenWidth() / 6, ScreenHeight() / 4), OverDecal, { 0.15f, 0.15f });
			if (GetKey(olc::SPACE).bReleased)
			{
				bResetGame = true;
			}
		}
		else
		{
			BirdMechanics(ElapsedTime);
			//screen colittions
			if (RedBird.GetPosition().y > ScreenHeight() || RedBird.GetPosition().y < -1)
			{
				bHasCollided = true;
			}

			//ui
			PrintNumber(FlapsCount);
		}
		
		return true;
	}
	// --------------------------------------------------------------------------------

	void DrawBird()
	{
		if (RedBird.GetVelocityY() > 0)
		{
			DrawDecal(RedBird.GetPosition(), UpDecal, { 0.12f, 0.12f });
		}
		else
		{
			DrawDecal(RedBird.GetPosition(), DownDecal, { 0.12f, 0.12f });
		}
	}
	
	// --------------------------------------------------------------------------------

	void BirdMechanics(const float InElapsedTime)
	{
		if (GetKey(olc::SPACE).bPressed && RedBird.GetVelocityY() >= Gravity / 6.0f)
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
			RedBird.Fall(Gravity * InElapsedTime);
		}

		if (RedBird.GetAccelerationY() >= Gravity)
		{
			RedBird.SetAccelerationY(Gravity);
		}

		RedBird.UpdatePosition(InElapsedTime);

		LevelPosition += 14.0f * InElapsedTime;
	}
	
	// --------------------------------------------------------------------------------

	void SectionThing()
	{
		if (LevelPosition > SectionWidth + 10)
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
				olc::vf2d PipePositionUp = { (float)(SectionIndex * SectionWidth + 20 - LevelPosition), (float)(ScreenHeight() - PipeSize - 20) };
				olc::vf2d PipePositionDown = { (float)(SectionIndex * SectionWidth + 20 - LevelPosition), (float)(ScreenHeight() - PipeSize) };

				DrawDecal(PipePositionUp, PipeDecal, { 0.15f, -0.15f });
				DrawDecal(PipePositionDown, PipeDecal, { 0.15f, 0.15f });

				olc::vf2d BirdPosition = RedBird.GetPosition();
				//pipes collitions
				if (BirdPosition.x > PipePositionUp.x && BirdPosition.x < PipePositionUp.x + 8 && BirdPosition.y < PipePositionUp.y)
				{//upper pipe
					bHasCollided = true;
				}
				if (BirdPosition.x > PipePositionDown.x && BirdPosition.x < PipePositionDown.x + 8 && BirdPosition.y > PipePositionDown.y)
				{//Down pipe
					bHasCollided = true;
				}
			}
			SectionIndex++;
		}
	}
	
	// --------------------------------------------------------------------------------

	void PrintNumber(const int InValue)
	{
		olc::vf2d Position = { ((float)ScreenWidth() / 2) - 2, 5 };
		if (InValue < 10)
		{
			DrawDecal(Position, NumbersDecal[InValue], { 0.15f, 0.15f });
		}
		else
		{
			int ScoreSave = InValue;
			std::vector<int> Score;
			while (ScoreSave != 0)
			{
				//converts the number into an array
				Score.insert(Score.begin(), ScoreSave % 10);
				ScoreSave /= 10;
			}

			for (auto s : Score)
			{
				DrawDecal(Position, NumbersDecal[s], { 0.15f, 0.15f });
				Position.x += 4;
			}

		}


	}

	// --------------------------------------------------------------------------------

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

// O------------------------------------------------------------------------------O
// | MAIN                                     |
// O------------------------------------------------------------------------------O

int main()
{
	FFlappyBird Game;
	Game.Construct(40, 60, 16, 16);
	Game.Start();

}
