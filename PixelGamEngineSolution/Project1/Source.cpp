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

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		sDemo = new olc::Sprite("./sprites/redbird-downflap.png");
		dec = new olc::Decal(sDemo);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		/*for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));*/

		Clear(olc::VERY_DARK_BLUE);

		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };

		DrawSprite(mouse, sDemo);
		return true;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(64, 64, 16, 16))
		demo.Start();

	return 0;
}