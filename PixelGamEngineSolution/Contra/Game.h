#pragma once

#include "Player.h"
#include "SteadyEnemy.h"
#include "InputHandler.h"
#include "SpritesHandler.h"

class FGame : public olc::PixelGameEngine
{
private:
    // olc::Sprite* BackgroundSprite = nullptr;
    olc::Decal* BackGroundDecal = nullptr;

    //javidx thing
    std::wstring Level;
    int LevelWidth = 0;
    int LevelHeight = 0;

    float CameraX = 0.0f;
    float CameraY = 0.0f;

    float GroundCounter = 0.0f;
    float LevelStartOffsetX = 0.0f;

    FPlayer Player ;
    FInputHandler Input = nullptr;

    std::vector<FBullet> PlayerBullets;
    std::vector<FBullet> EnemyBullets;
    std::vector<FSteadyEnemy> EnemyesArray;
    std::vector<olc::vi2d> AlreadySpawned;

    FSpritesHandler SpriteHandler;

public:

    FGame()
    {
        sAppName = "Contra from outer space: The Game";
        Player = FPlayer(1, 0);
        Input = FInputHandler(this);
    }
    
    virtual bool OnUserCreate() override
    {
        // BackgroundSprite = new olc::Sprite("./Assets/Level.png");
        BackGroundDecal = new olc::Decal(SpriteHandler.BackgroundSprite);

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
        Level += L"..................................................................K......................................L..";
        Level += L"........R..R.............................................................................................L..";
        Level += L"......................R.....R........R......GGGGGGGGGGGGGGG.....GGGGG.............GG.....................L..";
        Level += L".........................................................................................................L..";
        Level += L".GGGGGGGGGGGGGGGGGGGGGGGBBBBGGGGGBBBBGGGGGGGG......T.....TGGGGGGG.....GG.........GG..GG...K.......GGGG...L..";
        Level += L".........................................................................................................L..";
        Level += L".....GGG..P..GG.......R................T..........GGGGGGG.......K...GGG..GG..GG....G..GGGGG.....GG....G..L..";
        Level += L".........E.......................................P................G......................................L..";
        Level += L"........G..G.......EGGG........................GG...........GG.GG......P..GG..GGG............GG..T.GGT.G.LGG";
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
                    if (GroundCounter > 0.65f)
                    {
                        Player.SetCollidesGround(true);
                    }
                }
            }
        }
    }

    void ClampPlayer(const float InOffsetX)
    {
        if (Player.GetX() <= InOffsetX + 0.5f)
        {
            Player.SetX(InOffsetX + 0.52f);
            Player.SetVelocity_X(0);
            //wiggle <--- needs fix
        }
    }

    void PlayerShoot(const float InX, const float InY)
    {
        PlayerBullets.push_back(FBullet(InX, InY, Player.GetAim()));
    }

    void EnemyShoot(FSteadyEnemy& InEnemy, const float fElapsedTime)
    {
        InEnemy.AddToShootCoolDown(-fElapsedTime);
        if(InEnemy.GetShootCoolDown() <= 0.0f)
        {
            EnemyBullets.push_back(FBullet(InEnemy.GetCrosshair().x, InEnemy.GetCrosshair().y, InEnemy.GetAim()));
            InEnemy.AddToShootCount(1);
            InEnemy.SetShootCoolDown(0.3f);

            if(InEnemy.GetShootCount() >= 3)
            {
                InEnemy.SetShootCoolDown(2.0f);
                InEnemy.SetShootCount(0);
            }
        }
    }

    void BulletCollision(FBullet& InBullet)
    {
        for (FGameObject& Enemy : EnemyesArray) 
        {
            if (InBullet.GetX() > Enemy.GetX() && InBullet.GetX() < Enemy.GetX() + Enemy.GetWidth() && 
                InBullet.GetY() > Enemy.GetY() && InBullet.GetY() < Enemy.GetY() + Enemy.GetHeight())
            {
                Enemy.SetIsDead(true);
                Enemy.SetX(-100);
                InBullet.SetX(-150);
            }
        }
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
        const int VisibleTilesY = ScreenHeight() / TileHeight; //14

        float OffsetX = CameraX - (float)VisibleTilesX / 2.f; //offset de -4

        //camera clamp
        if (OffsetX <= LevelStartOffsetX)
        {
            OffsetX = LevelStartOffsetX;
        }
        else
        {
            LevelStartOffsetX = OffsetX;
        }
        ClampPlayer(OffsetX);
        
        //clamp to level end
        if (OffsetX > LevelWidth - VisibleTilesX)
        {
            OffsetX = LevelWidth - VisibleTilesX;
        }
       
        //smooth offset
        const float TileOffsetX = (OffsetX - (int)OffsetX) * TileWidth;


        //DrawSprite({-(int)OffSetX * TileWidth, 0}, BackgroundSprite);//Debug

        //std::cout << Player.GetX() << " | " << OffsetX + 0.5f << std::endl;

        // //Draw visible Tile map
        for (int x = -1; x < VisibleTilesX + 1; x++)
        {
            for (int y = -1; y < VisibleTilesY + 1; y++)
            {
                const wchar_t TileId = GetTile(x + OffsetX, y + 0.0f);
                switch (TileId)
                {
                case '0':
                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight, 32, 32, olc::CYAN);
                    break;
                case 'G':
                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight, 32, 32, olc::RED);
                    break;
                case 'W':

                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight, 32, 32, olc::BLUE);
                    break;
                case 'B':
                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight, 32, 32, olc::GREY);

                    break;
                default:
                    DrawRect(x * TileWidth - TileOffsetX, y * TileHeight, 32, 32);
                }
                
            }
        }


        //spawn enemies
        for (int x = 0; x < VisibleTilesX; x++)
        {
            for (int y = 0; y < VisibleTilesY; y++)
            {
                olc::vi2d TilePostition = {x + (int)OffsetX, y};
                const wchar_t TileId = GetTile(TilePostition.x, TilePostition.y);
				if (TileId == 'E' || TileId == 'R')
				{
					if (std::find(AlreadySpawned.begin(), AlreadySpawned.end(), TilePostition) == AlreadySpawned.end())
					{
						//element not found
						AlreadySpawned.push_back(TilePostition);
						EnemyesArray.push_back(FSteadyEnemy(x * TileWidth - TileOffsetX, (y + 0.4f) * TileHeight, x + (int)OffsetX));
					}
                    else
                    {
                        //element found
                        for (FGameObject& Enemy : EnemyesArray)
                        {
                            if (Enemy.GetID() == TilePostition.x)
                            {
                                Enemy.UpdatePosition(x * TileWidth - TileOffsetX, (y + 0.4f) * TileHeight);
                            }
                        }
                    }
				}
            }
        }

        //Draw enemies
        for (FSteadyEnemy& Enemy : EnemyesArray)
        {
            FillRect(Enemy.GetX(), Enemy.GetY(), Enemy.GetWidth(), Enemy.GetHeight(), olc::DARK_CYAN);
            Enemy.AimToPlayer(Player);
            EnemyShoot(Enemy, fElapsedTime);
            FillCircle(Enemy.GetCrosshair(), 1, olc::YELLOW);
        }

        //romove Enemy
        if (!EnemyesArray.empty())
        {
            const auto i = std::remove_if(EnemyesArray.begin(), EnemyesArray.end(),
                [&](const FGameObject& Enemy)
                {
                    return (Enemy.GetX() / (float)TileWidth < -0.9f);
                }
            );
            if (i != EnemyesArray.end())
            {
                EnemyesArray.erase(i);
            }
        }

        //remove bullet
        if (!PlayerBullets.empty())
        {
            const auto i = std::remove_if(PlayerBullets.begin(), PlayerBullets.end(),
                [&](const FBullet& Bullet)
                {
                    return (Bullet.GetX() < 0 || Bullet.GetX() > ScreenWidth() ||
                            Bullet.GetY() < 0 || Bullet.GetY() > ScreenHeight());
                }
            );
            if (i != PlayerBullets.end())
            {
                PlayerBullets.erase(i);
            }
        }

        for (FBullet& Bullet : PlayerBullets)
        {
            BulletCollision(Bullet);
        }


        //DrawDecal({-OffsetX * TileWidth, 0}, BackGroundDecal);

        //---------------------------------------------------------------------------------
        Player.UpdatePosition(fElapsedTime, OffsetX, TileWidth, TileHeight);
        GroundCollision(fElapsedTime);


        
        FillRect(Player.GetAbsolutePosition().x, Player.GetAbsolutePosition().y, Player.GetWidth(), Player.GetHeight(), olc::MAGENTA);
        FillCircle(Player.GetCrosshair(), 1, olc::YELLOW);
        DrawCircle(Player.GetHitPosition(), Player.GetHitRadius(), olc::WHITE);

        if (GetKey(olc::Z).bPressed)
        {
            PlayerShoot(Player.GetCrosshair().x, Player.GetCrosshair().y);
        }

        for(FBullet& LoopBullet : PlayerBullets)
        {
            LoopBullet.UpdatePosition();
            FillCircle(LoopBullet.GetX(), LoopBullet.GetY(), 1, olc::WHITE);
        }

        for(FBullet& LoopBullet : EnemyBullets)
        {
            LoopBullet.UpdatePosition();
            FillCircle(LoopBullet.GetX(), LoopBullet.GetY(), 1, olc::RED);
        }


        return true;
    }
};