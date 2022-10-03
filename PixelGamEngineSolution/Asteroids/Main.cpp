#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Ateroids : public olc::PixelGameEngine
{
public:
	Ateroids()
	{
		sAppName = "Asteroids";
	}

private:
	struct SSpaceObject
	{
		float X;
		float Y;
		float Dx;
		float Dy;
		int Size;
		float Angle;
	};

	std::vector<SSpaceObject> VectorAsteroids;
	SSpaceObject Player;


public:
	bool OnUserCreate() override
	{
		VectorAsteroids.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f });
		
		Player.X = ScreenWidth() / 2;
		Player.Y = ScreenHeight() / 2;
		Player.Dx = 0.0f;
		Player.Dy = 0.0f;
		Player.Angle = 0.0f;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);

		//player steer
		if (GetKey(olc::LEFT).bHeld)
		{
			Player.Angle -= 0.5f * fElapsedTime;
		}
		if (GetKey(olc::RIGHT).bHeld)
		{
			Player.Angle += 0.5f * fElapsedTime;
		}

		//Player Thrust
		if (GetKey(olc::UP).bHeld)
		{
			Player.Dx += sin(Player.Angle) * 20.0f * fElapsedTime;
			Player.Dy += -cos(Player.Angle) * 20.0f * fElapsedTime;
		}

		//VELOCITY changes position relative to time
		Player.X += Player.Dx * fElapsedTime;
		Player.Y += Player.Dy * fElapsedTime;

		WrapCoordinates(Player.X, Player.Y, Player.X, Player.Y);

		//update and draw asteroids
		for (auto& a : VectorAsteroids)
		{
			a.X += a.Dx * fElapsedTime;
			a.Y += a.Dy * fElapsedTime;

			WrapCoordinates(a.X, a.Y, a.X, a.Y);

			for (int i = 0; i < a.Size; i++)
			{
				for (int j = 0; j < a.Size; j++)
				{
					Draw(a.X + i, a.Y + j, olc::RED);
				}
			}
		}

		//Draw ship

		return true;
	}

	void WrapCoordinates(float InX, float InY, float& OutX, float& OutY)
	{
		OutX = InX;
		OutY = InY;

		if (InX < 0)
		{
			OutX = InX + (float)ScreenWidth();
		}
		if (InX >= (float)ScreenWidth())
		{
			OutX = InX - (float)ScreenWidth();
		}

		if (InY < 0)
		{
			OutY = InY + (float)ScreenHeight();
		}
		if (InY >= (float)ScreenHeight())
		{
			OutY = InY - (float)ScreenHeight();
		}
	}

	virtual bool Draw(int32_t X, int32_t Y, olc::Pixel Color)
	{
		float Fx, Fy;
		WrapCoordinates(X, Y, Fx, Fy);
		return PixelGameEngine::Draw(Fx, Fy, Color);
	}

};


int main()
{
	Ateroids Game;
	if (Game.Construct(160, 100, 8, 8))
	{
		Game.Start();
	}

	return 0;
}