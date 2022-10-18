#define OLC_PGE_APPLICATION

#include "Game.h"

int main()
{
    FGame Contra;
    if (Contra.Construct(256, 224, 4, 4))
    {
        Contra.Start();
    }

    return 0;
}
