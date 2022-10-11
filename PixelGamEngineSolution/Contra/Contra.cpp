#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine
{
public:
    Example()
    {
        sAppName = "Example";
    }

private:

    olc::Sprite* BackgoundSprite = nullptr;
    olc::Decal* BackGroundDecal = nullptr;
    
    //javidx thing
    std::wstring Level;
    int LevelWidth;
    int LevelHeight;

    float CameraX;
    float CameraY;

    float PlayerX;
    float PlayerY;

public:
    virtual bool OnUserCreate() override
    {
        BackgoundSprite = new olc::Sprite("./Assets/Level.png");
        BackGroundDecal = new olc::Decal(BackgoundSprite);

        LevelWidth = 108;
        LevelHeight = 7;
        Level += L"101010101010101010101010101010101010101001010101010101010101010101010101010101010101011010101010101010101010";
        Level += L"010101010101010101010101010101010101010100101010101010101010101010101010101010101010101101010101010101010101";
        Level += L"101010101010101010101010101010101010101001010101010101010101010101010101010101010101011010101010101010101010";
        Level += L"010101010101010101010101010101010101010100101010101010101010101010101010101010101010101101010101010101010101";
        Level += L"101010101010101010101010101010101010101001010101010101010101010101010101010101010101011010101010101010101010";
        Level += L"010101010101010101010101010101010101010100101010101010101010101010101010101010101010101101010101010101010101";
        Level += L"101010101010101010101010101010101010101001010101010101010101010101010101010101010101011010101010101010101010";


        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) override
    {

        Clear(olc::BLACK);

        auto GetTile = [&](int x, int y)
        {
            if(x >= 0 && x < LevelWidth && y >= 0 && y < LevelHeight)
            {
                return Level[y * LevelWidth + x];
            }
            else
            {
                return L' ';
            }
        };

        auto SetTile = [&](int x, int y, wchar_t c)
        {
            if(x >= 0 && x < LevelWidth && y >= 0 && y < LevelHeight)
            {
                Level[y * LevelWidth + x] = c;
            }
        };

        if(GetKey(olc::RIGHT).bPressed)
        {
            PlayerX += 1;
        }
        if(GetKey(olc::LEFT).bPressed)
        {
            PlayerX -= 1;
        }


        CameraX = PlayerX;
        CameraY = PlayerY;

        int TileWidth = 32;
        int TileHeight = 32;
        int VisibleTilesX = ScreenWidth() / TileWidth;
        int VisibleTilesY = ScreenHeight() / TileHeight;

        float OffSetX = CameraX - (float)VisibleTilesX / 2.f;
        float OffSetY = CameraY - (float)VisibleTilesY / 2.f;

        //camera clamp
        if (OffSetX < 0) OffSetX = 0;
        if (OffSetY < 0) OffSetY = 0;
        if (OffSetX > LevelWidth - VisibleTilesX) OffSetX = LevelWidth - VisibleTilesX;
        if (OffSetY > LevelHeight - VisibleTilesY) VisibleTilesY = LevelHeight - VisibleTilesY;

        //smooth offset
        float fTileOffsetX = (OffSetX - (int)OffSetX) * TileWidth;
        float fTileOffsetY = (OffSetY - (int)OffSetY) * TileHeight;

        //Draw visible Tile map
        for (int x = -1; x < VisibleTilesX + 1; x++)
        {
            for (int y = -1; y < VisibleTilesY + 1; y++)
            {
                wchar_t TileId = GetTile(x + OffSetX, y + OffSetY);
                if (TileId == '0')
                {
                    FillRect(x * TileWidth - OffSetX, y * TileHeight - OffSetY, 32, 32, olc::GREEN);
                    // DrawPartialDecal();
                }
                else if (TileId == '1')
                {
                    FillRect(x * TileWidth - OffSetX, y * TileHeight - OffSetY, 32, 32, olc::RED);
                }
                else
                {
                    FillRect(x * TileWidth - OffSetX, y * TileHeight - OffSetY, 32, 32);
                }
            }
        }

        DrawDecal({0.f, 0.f}, BackGroundDecal);

        
        return true;
    }
};


int main()
{
    Example demo;
    if (demo.Construct(256, 224, 4, 4))
    {
        demo.Start();
    }

    return 0;
}