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

    class FPlayer
    {
    public:
        float X = 1.0f;
        float Y = 1.0f;
        float VelX = 0.f;
        float VelY = 0.f;
        bool bOnGround = false;
        int Direction = 1;
    };

    float CameraX = 0.0f;
    float CameraY = 0.0f;
    
    FPlayer Player;

public:
    virtual bool OnUserCreate() override
    {
        BackgoundSprite = new olc::Sprite("./Assets/Level.png");
        BackGroundDecal = new olc::Decal(BackgoundSprite);

        LevelWidth = 108;
        LevelHeight = 7;
        Level += L"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
        Level += L"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
        Level += L"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
        Level += L"011111111111111111111111111111000000000000000000000000000000000000000000000000000000000000000000000000000000";
        Level += L"000011100001100000100000000111000000000000000000000000000000000000000000000000000000000000000000000000000000";
        Level += L"000000010010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
        Level += L"011111111111111111111111111100000000000000000000000000000000000000000000000000000000000000000000000000000000";


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

        if(IsFocused())
        {
            if(GetKey(olc::UP).bHeld)
            {
                Player.VelY = -6.0f;
            }
            if(GetKey(olc::DOWN).bHeld)
            {
                //crouch
                Player.VelY = 6.0f;
            }
            if(GetKey(olc::LEFT).bHeld)
            {
                Player.VelX += (Player.bOnGround ? -25.0f : -15.0f) * fElapsedTime;
                Player.Direction = -1;
            }
            if(GetKey(olc::RIGHT).bHeld)
            {
                Player.VelX += (Player.bOnGround ? 25.0f : 15.0f) * fElapsedTime;
                Player.Direction = 1;
            }
        }

        // Player.VelY += 20.f * fElapsedTime;
        
        //drag?
        // if(Player.bOnGround)
        // {
        //     Player.VelX += -3.0 * Player.VelX * fElapsedTime;
        //     if(fabs(Player.VelX) < 0.01f)
        //     {
        //         Player.VelX = 0.0f;
        //     }
        // }
        //
        //clamp 
        if(Player.VelX > 10.f) Player.VelX = 10.0f;
        if(Player.VelX < -10.f) Player.VelX = -10.0f;
        if(Player.VelY > 100.f) Player.VelY = 100.0f;
        if(Player.VelX < -100.f) Player.VelY = -100.0f;

        //calculate Potencial new position
        float NewPlayerPosX = Player.X + Player.VelX * fElapsedTime;
        float NewPlayerPosY = Player.Y + Player.VelY * fElapsedTime;

        //apply new position
        Player.X = NewPlayerPosX;
        Player.Y = NewPlayerPosY;

        //link Camera to player position
        CameraX = Player.X;
        CameraY = Player.Y;

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

        // DrawDecal({0.f, 0.f}, BackGroundDecal);


        //Draw Player
        FillRect((Player.X - OffSetX) * TileWidth, (Player.Y - OffSetY) * TileHeight, 16, 48);
        
        
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