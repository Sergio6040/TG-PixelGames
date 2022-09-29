#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}
	olc::Sprite* sDemo = nullptr;
	olc::Decal* dec = nullptr;

	olc::Sprite* sDemo2 = nullptr;
	olc::Decal* dec2 = nullptr;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		sDemo = new olc::Sprite("./sprites/redbird-downflap.png");
		dec = new olc::Decal(sDemo);


		sDemo2 = new olc::Sprite("./sprites/background-day.png");
		dec2 = new olc::Decal(sDemo2);
		

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		/*for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));*/

		Clear(olc::VERY_DARK_BLUE);

		DrawDecal(olc::vf2d(0, 0), dec2, { 0.15f, 0.15f });

		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

		/*SetPixelMode(olc::Pixel::ALPHA);
		DrawSprite(mouse, sDemo);
		SetPixelMode(olc::Pixel::NORMAL);*/

		DrawDecal(mouse, dec, {0.2f, 0.2f}, olc::Pixel(255, 255, 0, 64));

		return true;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(40, 60, 16, 16))
		demo.Start();

	return 0;
}