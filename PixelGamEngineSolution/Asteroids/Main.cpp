#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <math.h>

class Ateroids : public olc::PixelGameEngine
{
public:
	Ateroids()
	{
		sAppName = "Asteroids";
	}

private:
	struct FSpaceObject
	{
		float X;
		float Y;
		float Dx;
		float Dy;
		int Size;
		float Angle;
	};

	std::vector<FSpaceObject> VectorAsteroids;
	std::vector<FSpaceObject> VectorBullets;
	FSpaceObject Player;

	std::vector<std::pair<float, float>> VectorModelShip;
	std::vector<std::pair<float, float>> VectorModelAsteroids;

	int Score = 0;
	bool bDead = false;
	float CoolDown = 0;

public:
	bool OnUserCreate() override
	{
		VectorModelShip =
		{
			{0.0f, -5.0f},
			{-2.5f, 2.5f},
			{2.5f, 2.5f}
		};// triangle ship

		int AsteroidVerts = 20;
		for (int i = 0; i < AsteroidVerts; i++)
		{
			float Radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
			float a = ((float)i / (float)AsteroidVerts) * 6.28318f;
			VectorModelAsteroids.push_back(std::make_pair(Radius * sinf(a), Radius * cosf(a)));
		}

		ResetGame();
		return true;
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	bool IsPointInsideCircle(float CircleX, float CircleY, float Radius, float PointX, float PointY)
	{
		return sqrt(pow((PointX - CircleX), 2) + pow((PointY - CircleY), 2)) <= Radius;
	}
	
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	void CreateAsteroid(int Index)
	{
		int Direction;
		if (Index % 2 == 0)
		{
			Direction = 1;
		}
		else
		{
			Direction = -1;
		}

		VectorAsteroids.push_back(
		{
			30.0f * sinf(Player.Angle + (Direction * 3.1415f) / 2.0f) + Index * 10.f,	//X
			30.0f * cosf(Player.Angle + (Direction * 3.1415f) / 2.0f) + Index * 10.f,	//Y
			Direction * 10.0f * sinf( Player.Angle),									//DX
			Direction * 10.0f * cosf( Player.Angle),									//DY
			(int)16,																	//Size
			0.0f																		//Rotation
		});
	}

	void ResetGame()
	{
		VectorAsteroids.clear();
		VectorBullets.clear();

		Player.X = ScreenWidth() / 2;
		Player.Y = ScreenHeight() / 2;
		Player.Dx = 0.0f;
		Player.Dy = 0.0f;
		Player.Angle = 0.0f;

		if (bDead)
		{
			Score = 0;
		}
		bDead = false;
		CoolDown = 0;

		int NumberOfAsteroids = 5;
		if (Score == 0)
		{
			NumberOfAsteroids = 2;
		}
		else
		{
			NumberOfAsteroids = Score / 500;
		}

		for (int i = 0; i < NumberOfAsteroids; i++)
		{
			CreateAsteroid(i);
		}
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	virtual bool Draw(int32_t X, int32_t Y, olc::Pixel Color)
	{
		float Fx, Fy;
		WrapCoordinates(X, Y, Fx, Fy);
		return PixelGameEngine::Draw(Fx, Fy, Color);
	}

	void PlayerControl(const float fElapsedTime)
	{
		//player steer
		if (GetKey(olc::LEFT).bHeld || GetKey(olc::A).bHeld)
		{
			Player.Angle -= 3.0f * fElapsedTime;
		}
		if (GetKey(olc::RIGHT).bHeld || GetKey(olc::D).bHeld)
		{
			Player.Angle += 3.0f * fElapsedTime;
		}

		//Player Thrust
		if (GetKey(olc::UP).bHeld || GetKey(olc::W).bHeld)
		{
			Player.Dx += sin(Player.Angle) * 20.0f * fElapsedTime;
			Player.Dy += -cos(Player.Angle) * 20.0f * fElapsedTime;
		}

		CoolDown -= fElapsedTime;
		//shoot!!
		if (GetKey(olc::SPACE).bReleased && CoolDown <= 0)
		{
			VectorBullets.push_back({ Player.X, Player.Y, 50.0f * sinf(Player.Angle), -50.0f * cosf(Player.Angle), 0, 0 });
			//shoot inertia
			Player.Dx -= sin(Player.Angle) * 1000.0f * fElapsedTime;
			Player.Dy -= -cos(Player.Angle) * 1000.0f * fElapsedTime;

			CoolDown = 0.3f;
		}

		//VELOCITY changes position relative to time
		Player.X += Player.Dx * fElapsedTime;
		Player.Y += Player.Dy * fElapsedTime;
		WrapCoordinates(Player.X, Player.Y, Player.X, Player.Y);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (bDead)
		{
			ResetGame();
		}

		Clear(olc::BLACK);

		PlayerControl(fElapsedTime);

		//player collisions
		for (auto& Asteroid : VectorAsteroids)
		{
			if (IsPointInsideCircle(Asteroid.X, Asteroid.Y, Asteroid.Size, Player.X, Player.Y))
			{
				bDead = true;
			}
		}

		//update and draw asteroids
		for (auto& Asteroid : VectorAsteroids)
		{
			Asteroid.X += Asteroid.Dx * fElapsedTime;
			Asteroid.Y += Asteroid.Dy * fElapsedTime;
			Asteroid.Angle += 0.5f * fElapsedTime;
			WrapCoordinates(Asteroid.X, Asteroid.Y, Asteroid.X, Asteroid.Y);
			DrawWireFrameModel(VectorModelAsteroids, Asteroid.X, Asteroid.Y, Asteroid.Angle, Asteroid.Size, olc::RED);
		}

		std::vector<FSpaceObject> NewAsteroids;
		//update and draw Bullets
		for (auto& Bullet : VectorBullets)
		{
			Bullet.X += Bullet.Dx * fElapsedTime;
			Bullet.Y += Bullet.Dy * fElapsedTime;
			WrapCoordinates(Bullet.X, Bullet.Y, Bullet.X, Bullet.Y);
			Draw(Bullet.X, Bullet.Y, olc::WHITE);

			//check collisions
			for (auto& Asteroid : VectorAsteroids)
			{
				if (IsPointInsideCircle(Asteroid.X, Asteroid.Y, Asteroid.Size, Bullet.X, Bullet.Y))
				{
					//move bullet out of screen so it can be deleted easy
					Bullet.X = -100;
					if (Asteroid.Size > 2)
					{
						//create 2 asteroids
						float Angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
						float Angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
																															// bit-shift
						NewAsteroids.push_back({ Asteroid.X, Asteroid.Y, 10.0f * sinf(Angle1), 10.0f * cosf(Angle1), (int)Asteroid.Size >> 1, 0.0f });
						NewAsteroids.push_back({ Asteroid.X, Asteroid.Y, 10.0f * sinf(Angle2), 10.0f * cosf(Angle2), (int)Asteroid.Size >> 1, 0.0f });
					}
					Asteroid.X = -100;
					Score += 10;
				}
			}
		}

		//append all new asteroids
		for (auto& Asteroid : NewAsteroids)
		{
			VectorAsteroids.push_back(Asteroid);
		}

		//remove off screen bullets
		if (VectorBullets.size() > 0)
		{
			auto i = std::remove_if(VectorBullets.begin(), VectorBullets.end(), 
				[&](FSpaceObject Bullet) 
				{
					return (Bullet.X < 1 || Bullet.Y < 1 || Bullet.X >= ScreenWidth() - 1 || Bullet.Y >= ScreenHeight() - 1); 
				}
			);
			if (i != VectorBullets.end())
			{
				VectorBullets.erase(i);
			}
		}

		//remove off screen Asteroids
		if (VectorAsteroids.size() > 0)
		{
			auto i = std::remove_if(VectorAsteroids.begin(), VectorAsteroids.end(),
				[&](FSpaceObject Asteroid)
				{
					return (Asteroid.X < 0);
				}
			);
			if (i != VectorAsteroids.end())
			{
				VectorAsteroids.erase(i);
			}
			if (VectorAsteroids.empty())
			{
				Score += 1000;
				ResetGame();
			}
		}

		//Draw Ship
		DrawWireFrameModel(VectorModelShip, Player.X, Player.Y, Player.Angle);

		DrawString(2, 2, "Score: " + std::to_string(Score));

		return true;
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	void DrawWireFrameModel(const std::vector<std::pair<float, float>>& InVector, float InX, float InY, float InAngle = 0.0f, float Scale = 1.0f, olc::Pixel InColor = olc::WHITE)
	{
		/*
		* pair.first = X
		* pair.second = Y
		*/
		std::vector<std::pair<float, float>> VectorTransformed;
		int Verts = InVector.size();
		VectorTransformed.resize(Verts);

		//rotate
		for (int i = 0; i < Verts; i++)
		{
			VectorTransformed[i].first = InVector[i].first * cosf(InAngle) - InVector[i].second * sinf(InAngle);
			VectorTransformed[i].second = InVector[i].first * sinf(InAngle) + InVector[i].second * cosf(InAngle);
		}
		//Scale
		for (int i = 0; i < Verts; i++)
		{
			VectorTransformed[i].first *= Scale;
			VectorTransformed[i].second *= Scale;
		}
		//translate
		for (int i = 0; i < Verts; i++)
		{
			VectorTransformed[i].first += InX;
			VectorTransformed[i].second += InY;
		}
		//Draw shape
		for (int i = 0; i < Verts + 1; i++)
		{
			int j = (i + 1);
			DrawLine(VectorTransformed[i % Verts].first, VectorTransformed[i % Verts].second,
					 VectorTransformed[j % Verts].first, VectorTransformed[j % Verts].second, InColor);
		}
	}
};

int main()
{
	srand(time(NULL));

	Ateroids Game;
	if (Game.Construct(300, 200, 4, 4))
	{
		Game.Start();
	}

	return 0;
}