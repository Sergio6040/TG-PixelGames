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
        LevelHeight = 14;
        /*
         *  . -> Nothing
         *  G -> Ground
         *  B -> Bridge
         *  W -> Water
         *  L -> Wall
         *  T -> Turret
         *  K -> hiden turret
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
        Level += L".........GG........GG.......................GGG.......GGGGGG.............................................L..";


        Player = FPlayer(1, 0);

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
    
    void GroundCollision()
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
        else //moving down
        {
            if (PlayerCollideObjects(0.f, 1.f, 2.6f, 2.6f, L'G') ||
                PlayerCollideObjects(0.f, 1.f, 1, 1, L'W'))
                // GetTile(Player.GetX(), Player.GetY() + 1) == L'G' || GetTile(Player.GetX() + 0.9f, Player.GetY() + 1) == L'G' ||
                // GetTile(Player.GetX(), Player.GetY() + 1) == L'B' || GetTile(Player.GetX() + 0.9f, Player.GetY() + 1) == L'B')
            {
                Player.SetY((int)Player.GetY() + 0.4f);
                Player.SetVelocity_Y(0);
                Player.SetOnGround(true);
            }
        }
    }

    virtual bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);

        if (IsFocused())
        {
            if (GetKey(olc::UP).bHeld)
            {
                //lookUp
                // Player.SetVelocity_Y(-6.0);
            }
            if (GetKey(olc::DOWN).bHeld)
            {
                //crouch
                // Player.SetVelocity_Y(-6.0);
                if (GetKey(olc::X).bPressed)
                {
                    //Get down
                }
            }
            if (GetKey(olc::LEFT).bHeld)
            {
                Player.SetVelocity_X(-2.0f);
                Player.SetDirection(-1);
            }
            if (GetKey(olc::RIGHT).bHeld)
            {
                Player.SetVelocity_X(2.0f);
                Player.SetDirection(1);
            }
            if (GetKey(olc::X).bPressed)
            {
                //jump
                // if(Player.GetVelocity_Y() == 0)
                // {
                //     Player.AddToVelocity_Y(-3.0f);
                // }
                Player.Jump();
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
        int TileHeight = 16;
        int VisibleTilesX = ScreenWidth() / TileWidth; //8
        int VisibleTilesY = ScreenHeight() / TileHeight; //7

        float OffSetX = CameraX - (float)VisibleTilesX / 2.f; //offset de -4
        float OffSetY = CameraY - (float)VisibleTilesY / 2.f; //no es necesario

        //camera clamp
        if (OffSetX < 0) OffSetX = 0;
        if (OffSetY < 0) OffSetY = 0;
        if (OffSetX > LevelWidth - VisibleTilesX) OffSetX = LevelWidth - VisibleTilesX; //revisar!!!!
        if (OffSetY > LevelHeight - VisibleTilesY) VisibleTilesY = LevelHeight - VisibleTilesY;

        //smooth offset
        float TileOffsetX = (OffSetX - (int)OffSetX) * TileWidth;
        float TileOffsetY = (OffSetY - (int)OffSetY) * TileHeight;

        DrawSprite({-(int)OffSetX * TileWidth, 0}, BackgoundSprite);//Debug


        // //Draw visible Tile map
        for (int x = -1; x < VisibleTilesX + 1; x++)
        {
            for (int y = -1; y < VisibleTilesY + 1; y++)
            {
                wchar_t TileId = GetTile(x + OffSetX, y + OffSetY);
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


        //Draw Player
        FillRect((Player.GetX() - OffSetX) * TileWidth, (Player.GetY() - OffSetY) * TileHeight, 32, 42, olc::MAGENTA);


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
