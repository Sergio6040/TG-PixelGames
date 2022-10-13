#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Player.h"

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
        /*
         *  . -> Nothing
         *  G -> Ground
         *  B -> Bridge
         *  W -> Water
         * 
         */
        Level += L"............................................................................................................";
        Level += L"............................................................................................................";
        Level += L"............................................................................................................";
        Level += L".GGGGGGGGGGGGGGGGGGGGGGG....................................................................................";
        Level += L".....GGG.....GG.............................................................................................";
        Level += L"........G..G........GGG.....................................................................................";
        Level += L"WWWWWWWWWGGWWWWWWWWGGWWWWWWWWWWWWWWWWWWWWWWWGGGWWWWWWWGGGGGG.............G...G....G......GGG......GGGGGGGGGG";

        Player = FPlayer(1,0);
        
        return true;
    }

    auto GetTile(const int TileX, const int TileY) const
    {
        if(TileX >= 0 && TileX < LevelWidth && TileY >= 0 && TileY < LevelHeight)
        {
            return Level[TileY * LevelWidth + TileX];
        }
        else
        {
            return L' ';
        }
    }

    void SetTile(const int TileX, const int TileY, wchar_t Character)
    {
        if(TileX >= 0 && TileX < LevelWidth && TileY >= 0 && TileY < LevelHeight)
        {
            Level[TileY * LevelWidth + TileX] = Character;
        }
    }

    void GroundCollision()
    {
        Player.SetOnGround(false);

        if(Player.GetVelocity_Y() <= 0) //jumping
        {
            if(GetTile(Player.GetX(), Player.GetY()) != L'G' || GetTile(Player.GetX() + 0.9f, Player.GetY()) != L'G')
            {
                Player.SetY((int)Player.GetX() + 1);
                Player.SetVelocity_Y(0.0f);
            }
        }
        else //moving down
        {
            auto MyTile = GetTile(Player.GetX(), Player.GetY());
            if(GetTile(Player.GetX(), Player.GetY() + 1) == L'G' || GetTile(Player.GetX() + 0.9f, Player.GetY() + 1) == L'G')
            {
                Player.SetY((int)Player.GetY());
                Player.SetVelocity_Y(0);
                Player.SetOnGround(true);
            }
        }
        
    }
    
    virtual bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        if(IsFocused())
        {
            if(GetKey(olc::UP).bHeld)
            {
                //lookUp
                // Player.SetVelocity_Y(-6.0);
            }
            if(GetKey(olc::DOWN).bHeld)
            {
                //crouch
                // Player.SetVelocity_Y(-6.0);
                if(GetKey(olc::X).bPressed)
                {
                    //Get down
                }
            }
            if(GetKey(olc::LEFT).bHeld)
            {
                Player.SetVelocity_X(-2.0f);
                Player.SetDirection(-1);
            }
            if(GetKey(olc::RIGHT).bHeld)
            {
                Player.SetVelocity_X(2.0f);
                Player.SetDirection(1);
            }
            if(GetKey(olc::X).bPressed)
            {
                //jumpp
                // if(Player.GetVelocity_Y() == 0)
                // {
                //     Player.AddToVelocity_Y(-10.0f);
                // }
            }
        }

        //Gravity
        // Player.VelY += 20.f * fElapsedTime;
        //on Playermovement
        
        Player.PlayerMovement(fElapsedTime);
        GroundCollision();

        //---------------------------------------------------------------------------------
        //link Camera to player position
        CameraX = Player.GetX();
        CameraY = 0.0f;
        
        //
        int TileWidth = 32;
        int TileHeight = 32;
        int VisibleTilesX = ScreenWidth() / TileWidth; //8
        int VisibleTilesY = ScreenHeight() / TileHeight; //7
        
        float OffSetX = CameraX - (float)VisibleTilesX / 2.f;
        float OffSetY = CameraY - (float)VisibleTilesY / 2.f;//no es necesario
        
        //camera clamp
        if (OffSetX < 0) OffSetX = 0;
        if (OffSetY < 0) OffSetY = 0;
        if (OffSetX > LevelWidth - VisibleTilesX) OffSetX = LevelWidth - VisibleTilesX; //revisar!!!!
        if (OffSetY > LevelHeight - VisibleTilesY) VisibleTilesY = LevelHeight - VisibleTilesY;
        
        //smooth offset
        float TileOffsetX = (OffSetX - (int)OffSetX) * TileWidth;
        float TileOffsetY = (OffSetY - (int)OffSetY) * TileHeight;
        
        // //Draw visible Tile map
        for (int x = -1; x < VisibleTilesX + 1; x++)
        {
            for (int y = -1; y < VisibleTilesY + 1; y++)
            {
                wchar_t TileId = GetTile(x + OffSetX, y + OffSetY);
                if (TileId == '0')
                {
                    FillRect(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, 32, 32, olc::CYAN);
                    // DrawPartialDecal();
                }
                else if (TileId == 'G')
                {
                    FillRect(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, 32, 32, olc::RED);
                }
                else if (TileId == 'W')
                {
                    FillRect(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, 32, 32, olc::BLUE);
                }
                else
                {
                    FillRect(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, 32, 32);
                }
            }
        }

        // DrawDecal({0.f, 0.f}, BackGroundDecal);


        //Draw Player
        FillRect((Player.GetX() - OffSetX) * TileWidth , (Player.GetY() - OffSetY) * TileHeight, 16, 48, olc::MAGENTA);
        
        
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