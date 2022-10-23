#pragma once

#include "Player.h"
#include "SteadyEnemy.h"
#include "InputHandler.h"
#include "SpritesHandler.h"

class FGame : public olc::PixelGameEngine
{
private:
    olc::Decal* BackGroundDecal = nullptr;
    olc::Decal* PlayerIdleDecal = nullptr;
    olc::Decal* PlayerRunDecal = nullptr;
    olc::Decal* PlayerShootRunDecal = nullptr;
    olc::Decal* PlayerJumpDecal = nullptr;
    olc::Decal* PlayerCrouchDecal = nullptr;
    olc::Decal* PlayerAimDownDecal = nullptr;
    olc::Decal* PlayerAimUpDecal = nullptr;
    olc::Decal* PlayerRunAimUpDecal = nullptr;
    olc::Decal* PlayerDyingDecal = nullptr;
    olc::Decal* PlayerDeadDecal = nullptr;
    olc::Decal* MedalDecal = nullptr;
    olc::Decal* BulletDecal = nullptr;
    olc::Decal* Bullet2Decal = nullptr;

    std::wstring Level;
    int LevelWidth;
    int LevelHeight;

    float CameraX;
    float CameraY;

    int TileWidth;
    int TileHeight;
    int VisibleTilesX;
    int VisibleTilesY;
    float TileOffsetX;

    float OffsetX;

    float GroundCounter;
    float LevelStartOffsetX;

    FPlayer Player;
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
        LevelWidth = 0;
        LevelHeight = 0;
        CameraX = 0.f;
        CameraY = 0.f;
        TileWidth = 32;
        TileHeight = 16;
        VisibleTilesX = 0;
        VisibleTilesY = 0;
        TileOffsetX = 0.f;
        OffsetX = 0.f;
        GroundCounter = 0.f;
        LevelStartOffsetX = 0.f;
    }

    virtual bool OnUserCreate() override
    {
        BackGroundDecal = new olc::Decal(SpriteHandler.BackgroundSprite);
        PlayerIdleDecal = new olc::Decal(SpriteHandler.PlayerIdleSprite);
        PlayerRunDecal = new olc::Decal(SpriteHandler.PlayerRunSprite);
        PlayerShootRunDecal = new olc::Decal(SpriteHandler.PlayerShootRunSprite);
        PlayerJumpDecal = new olc::Decal(SpriteHandler.PlayerJumpSprite);
        PlayerCrouchDecal = new olc::Decal(SpriteHandler.PlayerCrouchSprite);
        PlayerAimDownDecal = new olc::Decal(SpriteHandler.PlayerAimDownSprite);
        PlayerAimUpDecal = new olc::Decal(SpriteHandler.PlayerAimUpSprite);
        PlayerRunAimUpDecal = new olc::Decal(SpriteHandler.PlayerRunAimUpSprite);
        PlayerDyingDecal = new olc::Decal(SpriteHandler.PlayerDyingSprite);
        PlayerDeadDecal = new olc::Decal(SpriteHandler.PlayerDeadSprite);
        MedalDecal = new olc::Decal(SpriteHandler.MedalSprite);
        BulletDecal = new olc::Decal(SpriteHandler.BulletSprite);
        Bullet2Decal = new olc::Decal(SpriteHandler.Bullet2Sprite);

        Player = FPlayer(1, 0);
        Input = FInputHandler(this);

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
        Level += L"......................R.....R........R...................................................................L..";
        Level += L"............................................GGGGGGGGGGGGGGG.....GGGGG.............GG.....................L..";
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
                if (Player.GetCollidesGround())
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
        if (Player.GetX() <= InOffsetX + 0.3f)
        {
            Player.SetX(InOffsetX + 0.32f);
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
        if (InEnemy.GetShootCoolDown() <= 0.0f)
        {
            EnemyBullets.push_back(FBullet(InEnemy.GetCrosshair().x, InEnemy.GetCrosshair().y, InEnemy.GetAim()));
            InEnemy.AddToShootCount(1);
            InEnemy.SetShootCoolDown(0.3f);

            if (InEnemy.GetShootCount() >= 3)
            {
                InEnemy.SetShootCoolDown(2.0f);
                InEnemy.SetShootCount(0);
            }
        }
    }

    void PlayerBulletCollision(FBullet& InBullet)
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

    void MoveLevel()
    {
        CameraX = Player.GetX();

        VisibleTilesX = ScreenWidth() / TileWidth; //8
        VisibleTilesY = ScreenHeight() / TileHeight; //14
        OffsetX = CameraX - (float)VisibleTilesX / 2.f; //offset de -4

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
        TileOffsetX = (OffsetX - (int)OffsetX) * TileWidth;
    }

    void DrawDebugMode()
    {
        //Draw visible Tile map
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
        FillRect(Player.GetAbsolutePosition().x, Player.GetAbsolutePosition().y, Player.GetWidth(), Player.GetHeight(), olc::MAGENTA);
        FillCircle(Player.GetCrosshair(), 1, olc::YELLOW);
        DrawCircle(Player.GetHitPosition(), Player.GetHitRadius(), olc::WHITE);
    }

    void SpawnEnemies()
    {
        for (int x = 0; x < VisibleTilesX; x++)
        {
            for (int y = 0; y < VisibleTilesY; y++)
            {
                olc::vi2d TilePostition = { x + (int)OffsetX, y };
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
    }

    template <typename T>
    void RemoveElement(std::vector<T>& InVector, const std::function<void(FGameObject)>& Lambda)
    {
        if (!InVector.empty())
        {
            const auto i = std::remove_if(InVector.begin(), InVector.end(), Lambda);
            if (i != InVector.end())
            {
                InVector.erase(i);
            }
        }
    }

    void RemoveEnemy()
    {
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
    }

    void RemoveBullets()
    {
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

        if (!EnemyBullets.empty())
        {
            const auto i = std::remove_if(EnemyBullets.begin(), EnemyBullets.end(),
                [&](const FBullet& Bullet)
                {
                    return (Bullet.GetX() < 0 || Bullet.GetX() > ScreenWidth() ||
                        Bullet.GetY() < 0 || Bullet.GetY() > ScreenHeight());
                }
            );
            if (i != EnemyBullets.end())
            {
                EnemyBullets.erase(i);
            }
        }
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        if (IsFocused()) {
            Input.PlayerInput(Player);
        }

        if (GetKey(olc::Z).bPressed)
        {
            PlayerShoot(Player.GetCrosshair().x, Player.GetCrosshair().y);
        }

        MoveLevel();

        SpawnEnemies();

        RemoveEnemy();
        //RemoveElement<FSteadyEnemy>(EnemyesArray, [&](const FGameObject& Element) {return Element.GetX() / (float)TileWidth < -0.9f;});

        RemoveBullets();

        Player.UpdatePosition(fElapsedTime, OffsetX, TileWidth, TileHeight);
        GroundCollision(fElapsedTime);

        //Draw
        DrawDecal({ -OffsetX * TileWidth, 0 }, BackGroundDecal);

        if (Player.GetDirection() < 0)
        {
            DrawPartialDecal({ Player.GetAbsolutePosition().x + 32, Player.GetAbsolutePosition().y }, PlayerIdleDecal, { 0, 0 }, { 32, 42 }, { (float)Player.GetDirection(), 1.f });
        }
        else
        {
            DrawPartialDecal(Player.GetAbsolutePosition(), PlayerIdleDecal, { 0, 0 }, { 32, 42 }, { (float)Player.GetDirection(), 1.f });
        }


        //Draw enemies
        for (FSteadyEnemy& Enemy : EnemyesArray)
        {
            DrawPartialDecal({ Enemy.GetX() + 32, Enemy.GetY()}, PlayerIdleDecal, {0, 0}, {32, 42}, {(float)Enemy.GetDirection(), 1.f});
            Enemy.AimToPlayer(Player);
            EnemyShoot(Enemy, fElapsedTime);
            FillCircle(Enemy.GetCrosshair(), 1, olc::YELLOW);
        }

        for (FBullet& LoopBullet : PlayerBullets)
        {
            LoopBullet.UpdatePosition();
            PlayerBulletCollision(LoopBullet);
            DrawDecal({ LoopBullet.GetX(), LoopBullet.GetY() }, BulletDecal);
        }

        for (FBullet& LoopBullet : EnemyBullets)
        {
            LoopBullet.UpdatePosition();
            DrawDecal({ LoopBullet.GetX(), LoopBullet.GetY() }, BulletDecal);
        }


        return true;
    }
};