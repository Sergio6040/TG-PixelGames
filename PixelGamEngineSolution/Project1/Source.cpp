
#include <iostream>
#include <string>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class OneLoneCoder_Platformer : public olc::PixelGameEngine
{
public:
	OneLoneCoder_Platformer()
	{
		sAppName = "Tile Based Platform Game";
	}

private:
	// Level storage
	wstring sLevel;
	int nLevelWidth;
	int nLevelHeight;

	// Player Properties
	float fPlayerPosX = 1.0f;
	float fPlayerPosY = 1.0f;
	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;
	bool bPlayerOnGround = false;

	// Camera properties
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;

	// Sprite Resources
	olc::Sprite *spriteTiles = nullptr;
	olc::Sprite *spriteMan = nullptr;

	// Sprite selection flags
	int nDirModX = 0;
	int nDirModY = 0;

protected:
	virtual bool OnUserCreate()
	{
		nLevelWidth = 64;
		nLevelHeight = 16;
		sLevel += L"................................................................";
		sLevel += L"................................................................";
		sLevel += L".......ooooo....................................................";
		sLevel += L"........ooo.....................................................";
		sLevel += L".......................########.................................";
		sLevel += L".....BB?BBBB?BB.......###..............#.#......................";
		sLevel += L"....................###................#.#......................";
		sLevel += L"...................####.........................................";
		sLevel += L"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG.##############.....########";
		sLevel += L"...................................#.#...............###........";
		sLevel += L"........................############.#............###...........";
		sLevel += L"........................#............#.........###..............";
		sLevel += L"........................#.############......###.................";
		sLevel += L"........................#................###....................";
		sLevel += L"........................#################.......................";
		sLevel += L"................................................................";

		spriteTiles = new olc::Sprite("JarioSprites/leveljario.spr");
		spriteMan = new olc::Sprite("JarioSprites/minijario.spr");

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// Utility Lambdas
		auto GetTile = [&](int x, int y)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				return sLevel[y * nLevelWidth + x];
			else
				return L' ';
		};

		auto SetTile = [&](int x, int y, wchar_t c)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				sLevel[y*nLevelWidth + x] = c;
		};

		//fPlayerVelX = 0.0f;
		//fPlayerVelY = 0.0f;

		// Handle Input
		if (IsFocused())
		{
			if (GetKey(olc::UP).bHeld)
			{
				fPlayerVelY = -6.0f;
			}

			if (GetKey(olc::DOWN).bHeld)
			{
				fPlayerVelY = 6.0f;
			}

			if (GetKey(olc::LEFT).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
				nDirModY = 1;
			}

			if (GetKey(olc::RIGHT).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? 25.0f : 15.0f) * fElapsedTime;
				nDirModY = 0;
			}

			if (GetKey(olc::SPACE).bPressed)
			{
				if (fPlayerVelY == 0)
				{
					fPlayerVelY = -12.0f;
					nDirModX = 1;
				}
			}
		}

		// Gravity
		fPlayerVelY += 20.0f * fElapsedTime;

		// Drag
		if (bPlayerOnGround)
		{
			fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime;
			if (fabs(fPlayerVelX) < 0.01f)
				fPlayerVelX = 0.0f;
		}
	
		// Clamp velocities
		if (fPlayerVelX > 10.0f)
			fPlayerVelX = 10.0f;

		if (fPlayerVelX < -10.0f)
			fPlayerVelX = -10.0f;

		if (fPlayerVelY > 100.0f)
			fPlayerVelY = 100.0f;

		if (fPlayerVelY < -100.0f)
			fPlayerVelY = -100.0f;

		// Calculate potential new position
		float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

		// Check for pickups!
		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == L'o')
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, L'.');

		if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) == L'o')
			SetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, L'.');

		if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == L'o')
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, L'.');

		if (GetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f) == L'o')
			SetTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, L'.');

		// Check for Collision
		if (fPlayerVelX <= 0) // Moving Left
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != L'.' || GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != L'.')
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;
				fPlayerVelX = 0;
			}
		}
		else // Moving Right
		{
			if (GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != L'.' || GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != L'.')
			{
				fNewPlayerPosX = (int)fNewPlayerPosX;
				fPlayerVelX = 0;
				
			}
		}

		bPlayerOnGround = false;
		if (fPlayerVelY <= 0) // Moving Up
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != L'.' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != L'.')
			{
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}
		else // Moving Down
		{
			if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != L'.' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != L'.')
			{
				fNewPlayerPosY = (int)fNewPlayerPosY;
				fPlayerVelY = 0;
				bPlayerOnGround = true; // Player has a solid surface underfoot
				nDirModX = 0;
			}
		}

		// Apply new position
		fPlayerPosX = fNewPlayerPosX;
		fPlayerPosY = fNewPlayerPosY;

		// Link camera to player position
		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;

		// Draw Level
		int nTileWidth = 16;
		int nTileHeight = 16;
		int nVisibleTilesX = ScreenWidth() / nTileWidth;
		int nVisibleTilesY = ScreenHeight() / nTileHeight;

		// Calculate Top-Leftmost visible tile
		float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
		float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;

		// Clamp camera to game boundaries
		if (fOffsetX < 0) fOffsetX = 0;
		if (fOffsetY < 0) fOffsetY = 0;
		if (fOffsetX > nLevelWidth - nVisibleTilesX) fOffsetX = nLevelWidth - nVisibleTilesX;
		if (fOffsetY > nLevelHeight - nVisibleTilesY) fOffsetY = nLevelHeight - nVisibleTilesY;

		// Get offsets for smooth movement
		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;

		// Draw visible tile map
		for (int x = -1; x < nVisibleTilesX + 1; x++)
		{
			for (int y = -1; y < nVisibleTilesY + 1; y++)
			{
				wchar_t sTileID = GetTile(x + fOffsetX, y + fOffsetY);
				switch (sTileID)
				{
				case L'.': // Sky
					Fill(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, PIXEL_SOLID, FG_CYAN);
					break;
				case L'#': // Solid Block
					//Fill(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, PIXEL_SOLID, FG_RED);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'G': // Ground Block
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'B': // Brick Block
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'?': // Question Block
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 1 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
					break;
				case L'o': // Coin
					Fill(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, PIXEL_SOLID, FG_CYAN);
					DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
					break;
				default:
					Fill(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, PIXEL_SOLID, FG_BLACK);
					break;
				}
			}
		}

		// Draw Player
		//Fill((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileWidth, (fPlayerPosX - fOffsetX + 1.0f) * nTileWidth, (fPlayerPosY - fOffsetY + 1.0f) * nTileHeight, PIXEL_SOLID, FG_GREEN);
		DrawPartialSprite((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileWidth, spriteMan, nDirModX * nTileWidth, nDirModY * nTileHeight, nTileWidth, nTileHeight);
		return true;
	}
};

int main()
{
	OneLoneCoder_Platformer game;
	if (game.ConstructConsole(256, 240, 4, 4))
		game.Start();
	return 0;
}

// #define OLC_PGE_APPLICATION
// #include "olcPixelGameEngine.h"
//
// class Example : public olc::PixelGameEngine
// {
// public:
// 	Example()
// 	{
// 		sAppName = "Example";
// 	}
// 	olc::Sprite* sDemo = nullptr;
// 	olc::Decal* dec = nullptr;
//
// 	olc::Sprite* sDemo2 = nullptr;
// 	olc::Decal* dec2 = nullptr;
//
// public:
// 	bool OnUserCreate() override
// 	{
// 		// Called once at the start, so create things here
//
// 		sDemo = new olc::Sprite("./sprites/redbird-downflap.png");
// 		dec = new olc::Decal(sDemo);
//
//
// 		sDemo2 = new olc::Sprite("./sprites/background-day.png");
// 		dec2 = new olc::Decal(sDemo2);
// 		
//
// 		return true;
// 	}
//
// 	bool OnUserUpdate(float fElapsedTime) override
// 	{
// 		// called once per frame
// 		/*for (int x = 0; x < ScreenWidth(); x++)
// 			for (int y = 0; y < ScreenHeight(); y++)
// 				Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));*/
//
// 		Clear(olc::VERY_DARK_BLUE);
//
// 		DrawDecal(olc::vf2d(0, 0), dec2, { 0.15f, 0.15f });
//
// 		olc::vf2d mouse = { float(GetMouseX()), float(GetMouseY()) };
//
// 		/*SetPixelMode(olc::Pixel::ALPHA);
// 		DrawSprite(mouse, sDemo);
// 		SetPixelMode(olc::Pixel::NORMAL);*/
//
// 		DrawDecal(mouse, dec, {0.2f, 0.2f}, olc::Pixel(255, 255, 0, 64));
//
// 		return true;
// 	}
// };
//
//
// int main()
// {
// 	Example demo;
// 	if (demo.Construct(40, 60, 16, 16))
// 		demo.Start();
//
// 	return 0;
// }