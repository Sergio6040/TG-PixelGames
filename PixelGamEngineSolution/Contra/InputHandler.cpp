#include "InputHandler.h"

void FInputHandler::PlayerInput(FPlayer& Player) const
{
    MovementInputs(Player);

    TwoInputs(Player);

    StopAiming(Player);

    if (PixelEngine->GetKey(olc::Z).bHeld)
    {
        std::cout << "z" << std::endl;
        //shoot!!
    }
}

void FInputHandler::MovementInputs(FPlayer& Player) const
{
    if (PixelEngine->GetKey(olc::UP).bHeld)
    {
        //Look Up
        Player.SetAim(0, -1);
    }

    if (PixelEngine->GetKey(olc::DOWN).bHeld)
    {
        //crouch
        if (PixelEngine->GetKey(olc::X).bPressed && Player.GetOnGround())
        {
            //Get down
            Player.SetCollidesGround(false);
        }
    }
    else if (PixelEngine->GetKey(olc::X).bPressed)
    {
        Player.Jump();
    }

    if (PixelEngine->GetKey(olc::LEFT).bHeld)
    {
        Player.SetVelocity_X(-2.0f);
        Player.SetDirection(-1);
        Player.SetAim(-1, 0);
    }
    if (PixelEngine->GetKey(olc::RIGHT).bHeld)
    {
        Player.SetVelocity_X(2.0f);
        Player.SetDirection(1);
        Player.SetAim(1, 0);
    }
}

void FInputHandler::TwoInputs(FPlayer& Player) const
{
    if (PixelEngine->GetKey(olc::RIGHT).bHeld && PixelEngine->GetKey(olc::UP).bHeld)
    {
        Player.SetAim(1, -1);
    }
    if (PixelEngine->GetKey(olc::LEFT).bHeld && PixelEngine->GetKey(olc::UP).bHeld)
    {
        Player.SetAim(-1, -1);
    }
    if (PixelEngine->GetKey(olc::RIGHT).bHeld && PixelEngine->GetKey(olc::DOWN).bHeld)
    {
        Player.SetAim(1, 1);
    }
    if (PixelEngine->GetKey(olc::LEFT).bHeld && PixelEngine->GetKey(olc::DOWN).bHeld)
    {
        Player.SetAim(-1, 1);
    }
}

void FInputHandler::StopAiming(FPlayer& Player) const
{
    //Stop aiming Up/Down
    if (PixelEngine->GetKey(olc::UP).bReleased || PixelEngine->GetKey(olc::DOWN).bReleased)
    {
        Player.SetAimY(0);
        Player.SetAimX(Player.GetDirection());
    }
    if (Player.GetVelocity_Y() != 0.0f && PixelEngine->GetKey(olc::DOWN).bPressed)
    {
        Player.SetAim(0, 1);
    }
}
