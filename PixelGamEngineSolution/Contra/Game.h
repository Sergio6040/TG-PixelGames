#pragma once

#include "Player.h"
#include "SteadyEnemy.h"
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

    std::vector<FBullet> PlayerBullets;
    std::vector<FGameObject> EnemyesArray;
    std::vector<olc::vi2d> AlreadySpawned;

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
         *  E -> enemy
         *  R -> Runner Enemy
         *  P -> Power Up
         */
        Level += L".........................................................................................................L..";
        Level += L".........................................................................................................L..";
        Level += L".........................................................................................................L..";
        Level += L"........R..R.............................................................................................L..";
        Level += L"............................................GGGGGGGGGGGGGGG.....GGGGG.............GG.....................L..";
        Level += L"......................R.....R........R...................................................................L..";
        Level += L".GGGGGGGGGGGGGGGGGGGGGGGBBBBGGGGGBBBBGGGGGGGG......T.....TGGGGGGG.....GG.........GG..GG...K.......GGGG...L..";
        Level += L".........................................................................................................L..";
        Level += L".....GGG..T..GG......ER................T.........PGGGGGGG.........G.GGG..GG..GG....G..GGGGG.....GG....G..L..";
        Level += L".........E...............................................................................................L..";
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

    void Shoot(const float InX, const float InY)
    {
        PlayerBullets.push_back(FBullet(InX, InY, Player.GetAim()));
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        if (IsFocused()) {
            Input.PlayerInput(Player);
        }
        
        

        //---------------------------------------------------------------------------------
        //link Camera to player position
        CameraX = Player.GetX();
        CameraY = 0.0f;

        //
        constexpr int TileWidth = 32;
        constexpr int TileHeight = 16;
        const int VisibleTilesX = ScreenWidth() / TileWidth; //8
        int VisibleTilesY = ScreenHeight() / TileHeight; //14

        float OffsetX = CameraX - (float)VisibleTilesX / 2.f; //offset de -4
        float OffsetY = CameraY - (float)VisibleTilesY / 2.f; //no es necesario

        //camera clamp
        if (OffsetX < 0) OffsetX = 0;
        if (OffsetY < 0) OffsetY = 0;
        //clamp to level end
        if (OffsetX > LevelWidth - VisibleTilesX) OffsetX = LevelWidth - VisibleTilesX; 
        if (OffsetY > LevelHeight - VisibleTilesY) VisibleTilesY = LevelHeight - VisibleTilesY;

        //smooth offset
        const float TileOffsetX = (OffsetX - (int)OffsetX) * TileWidth;
        const float TileOffsetY = (OffsetY - (int)OffsetY) * TileHeight;

        //DrawSprite({-(int)OffSetX * TileWidth, 0}, BackgroundSprite);//Debug


        // //Draw visible Tile map
        for (int x = -1; x < VisibleTilesX + 1; x++)
        {
            for (int y = -1; y < VisibleTilesY + 1; y++)
            {
                const wchar_t TileId = GetTile(x + OffsetX, y + OffsetY);
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


        //spawn enemies
        for (int x = 0; x < VisibleTilesX; x++)
        {
            for (int y = 0; y < VisibleTilesY; y++)
            {
                olc::vi2d TilePostition = {x + (int)OffsetX, y + (int)OffsetY};
                const wchar_t TileId = GetTile(TilePostition.x, TilePostition.y);
				if (TileId == 'E' || TileId == 'R')
				{
					if (std::find(AlreadySpawned.begin(), AlreadySpawned.end(), TilePostition) == AlreadySpawned.end())
					{
						//element not found
						AlreadySpawned.push_back(TilePostition);
						EnemyesArray.push_back(FSteadyEnemy(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY, x + (int)OffsetX));
					}
                    else
                    {
                        //element found
                        for (FGameObject& Enemy : EnemyesArray)
                        {
                            if (Enemy.GetID() == TilePostition.x)
                            {
                                Enemy.UpdatePosition(x * TileWidth - TileOffsetX, y * TileHeight - TileOffsetY);
                            }
                        }
                    }
				}
            }
        }

        //Draw enemies
        for (FGameObject& Enemy : EnemyesArray)
        {
            FillRect(Enemy.GetX(), Enemy.GetY(), Enemy.GetWidth(), Enemy.GetHeight(), olc::RED);
        }


        //DrawDecal({-OffsetX * TileWidth, 0}, BackGroundDecal);

        //---------------------------------------------------------------------------------
        Player.UpdatePosition(fElapsedTime, OffsetX, OffsetY, TileWidth, TileHeight);
        GroundCollision(fElapsedTime);


        
        FillRect(Player.GetAbsolutePosition().x, Player.GetAbsolutePosition().y, Player.GetWidth(), Player.GetHeight(), olc::MAGENTA);
        FillCircle(Player.GetCrosshair(), 1, olc::YELLOW);

        if (GetKey(olc::Z).bPressed)
        {
            Shoot(Player.GetCrosshair().x, Player.GetCrosshair().y);
        }

        for(FBullet& LoopBullet : PlayerBullets)
        {
            LoopBullet.UpdatePosition();
            FillCircle(LoopBullet.GetX(), LoopBullet.GetY(), 1, olc::RED);
        }


        return true;
    }
};