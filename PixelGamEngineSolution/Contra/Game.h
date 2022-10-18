#pragma once

#include "Player.h"
#include "InputHandler.h"

class FGame : public olc::PixelGameEngine
{
private:
    olc::Sprite* BackgroundSprite = nullptr;
    olc::Decal* BackGroundDecal = nullptr;

    //javidx thing
    std::wstring Level;
    int LevelWidth = 0;
    int LevelHeight = 0;

    float CameraX = 0.0f;
    float CameraY = 0.0f;

    float GroundCounter = 0;

    FPlayer Player ;
    FInputHandler Input = nullptr;


public:

    FGame()
    {
        sAppName = "Contra from outer space: The Game";
        Player = FPlayer(1, 0);
        Input = FInputHandler(this);
    }
    
    virtual bool OnUserCreate() override
    {
        BackgroundSprite = new olc::Sprite("./Assets/Level.png");
        BackGroundDecal = new olc::Decal(BackgroundSprite);

        LevelWidth = 108;
        LevelHeight = 14;
        /*
         *  Define The collisions from the level
         *  . -> Nothing
         *  G -> Ground
         *  B -> Bridge
         *  W -> Water
         *  L -> Wall
         *  T -> Turret
         *  K -> hidden turret
         * 
         */
        Level += L".........................................................................................................L..";
        Level += L".........................................................................................................L..";
        Level += L".........................................................................................................L..";
        Level += L".........................................................................................................L..";
        Level += L"............................................GGGGGGGGGGGGGGG.....GGGGG.............GG.....................L..";
        Level += L".........................................................................................................L..";
        Level += L".GGGGGGGGGGGGGGGGGGGGGGGBBBBGGGGGBBBBGGGGGGGG......T.....TGGGGGGG.....GG.........GG..GG...K.......GGGG...L..";
        Level += L".........................................................................................................L..";
        Level += L".....GGG.....GG..................................PGGGGGGG.........G.GGG..GG..GG....G..GGGGG.....GG....G..L..";
        Level += L".........................................................................................................L..";
        Level += L"........G..G........GGG........................GG...........GG.GG......T..GG..GGG............GG..T.GGT.G.LGG";
        Level += L".........................................................................................................L..";
        Level += L"WWWWWWWWWGGWWWWWWWWGGWWWWWWWWWWWWWWWWWWWWWWWGGGWWWWWWWGGGGGG.............G...G....G......GGG......GGGGGGGG..";
        Level += L"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG..................................................L..";

        return true;
    }

    auto GetTile(const int TileX, const int TileY) const
    {
        if (TileX >= 0 && TileX < LevelWidth && TileY >= 0 && TileY < LevelHeight)
        {
            return Level[TileY * LevelWidth + TileX];
        }
        else
        {
            return L' ';
        }
    }

    void SetTile(const int TileX, const int TileY, const wchar_t Character)
    {
        if (TileX >= 0 && TileX < LevelWidth && TileY >= 0 && TileY < LevelHeight)
        {
            Level[TileY * LevelWidth + TileX] = Character;
        }
    }

    bool PlayerCollideObjects(const float X1, const float X2, const float Y1, const float Y2, const wchar_t Character) const
    {
        return (GetTile(Player.GetX() + X1, Player.GetY() + Y1) == Character || GetTile(Player.GetX() + X2, Player.GetY() + Y2) == Character);
    }
    
    void GroundCollision(const float fElapsedTime)
    {
        Player.SetOnGround(false);

        if (Player.GetVelocity_Y() <= 0) //jumping
        {
            if (GetTile(Player.GetX(), Player.GetY()) == L'G' || GetTile(Player.GetX() + 0.9f, Player.GetY()) == L'G')
            {
                // Player.SetY((int)Player.GetX() + 1);
                // Player.SetVelocity_Y(0.0f);
            }
        }
        else //Falling
        {
            if (PlayerCollideObjects(0.f, 1.f, 2.6f, 2.6f, L'G') ||
                PlayerCollideObjects(0.f, 1.f, 1, 1, L'W'))
                // GetTile(Player.GetX(), Player.GetY() + 1) == L'G' || GetTile(Player.GetX() + 0.9f, Player.GetY() + 1) == L'G' ||
                // GetTile(Player.GetX(), Player.GetY() + 1) == L'B' || GetTile(Player.GetX() + 0.9f, Player.GetY() + 1) == L'B')
            {
                if(Player.GetCollidesGround())
                {
                    Player.SetY((int)Player.GetY() + 0.4f);
                    Player.SetVelocity_Y(0);
                    Player.SetOnGround(true);  
                    GroundCounter = 0;
                }
                else
                {
                    GroundCounter += fElapsedTime;
                    std::cout << GroundCounter << std::endl;
                    if (GroundCounter > 0.65f)
                    {
                        Player.SetCollidesGround(true);
                    }
                }
            }
        }
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        
            Input.PlayerInput(Player);
        
        
        //Update Player Position
        Player.PlayerMovement(fElapsedTime);
        GroundCollision(fElapsedTime);

        //---------------------------------------------------------------------------------
        //link Camera to player position
        CameraX = Player.GetX();
        CameraY = 0.0f;

        //
        constexpr int TileWidth = 32;
        constexpr int TileHeight = 16;
        const int VisibleTilesX = ScreenWidth() / TileWidth; //8
        int VisibleTilesY = ScreenHeight() / TileHeight; //7

        float OffSetX = CameraX - (float)VisibleTilesX / 2.f; //offset de -4
        float OffSetY = CameraY - (float)VisibleTilesY / 2.f; //no es necesario

        //camera clamp
        if (OffSetX < 0) OffSetX = 0;
        if (OffSetY < 0) OffSetY = 0;
        if (OffSetX > LevelWidth - VisibleTilesX) OffSetX = LevelWidth - VisibleTilesX; //revisar!!!!
        if (OffSetY > LevelHeight - VisibleTilesY) VisibleTilesY = LevelHeight - VisibleTilesY;

        //smooth offset
        const float TileOffsetX = (OffSetX - (int)OffSetX) * TileWidth;
        const float TileOffsetY = (OffSetY - (int)OffSetY) * TileHeight;

        //DrawSprite({-(int)OffSetX * TileWidth, 0}, BackgroundSprite);//Debug


        // //Draw visible Tile map
        for (int x = -1; x < VisibleTilesX + 1; x++)
        {
            for (int y = -1; y < VisibleTilesY + 1; y++)
            {
                const wchar_t TileId = GetTile(x + OffSetX, y + OffSetY);
                switch (TileId)
                {
                case '0':
                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, 32, 32, olc::CYAN);
                    break;
                case 'G':
                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, 32, 32, olc::RED);
                    break;
                case 'W':

                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, 32, 32, olc::BLUE);
                    break;
                case 'B':
                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, 32, 32, olc::GREY);

                    break;
                default:
                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, 32, 32);
                }
                
            }
        }

        // DrawDecal({-OffSetX * TileWidth, 0}, BackGroundDecal);


        //-----------------------¡¡¡¡Change Names!!!!----------------------------------------------------
        
        //Draw Player
        const olc::vf2d PlayerPos = {(Player.GetX() - OffSetX) * TileWidth, (Player.GetY() - OffSetY) * TileHeight}; 
        FillRect(PlayerPos.x, PlayerPos.y, 32, 42, olc::MAGENTA);

        //Draw Aim
        const olc::vf2d aux = Player.GetAim();
        //attach to player hitbox
        const olc::vf2d Crosshair = {(aux.x * 10) + PlayerPos.x + 32.f/2, (aux.y * 10) + PlayerPos.y + 42.f/2};
        FillCircle(Crosshair, 1, olc::YELLOW);


        return true;
    }
};