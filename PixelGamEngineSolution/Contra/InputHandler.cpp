#include "InputHandler.h"

void FInputHandler::PlayerInput(FPlayer& Player) const
{
    if (!Player.GetIsDead())
    {
        MovementInputs(Player);

        TwoInputs(Player);

        StopAiming(Player);

        DownInputs(Player);
    }
}

void FInputHandler::MovementInputs(FPlayer& Player) const
{
    if (PixelEngine->GetKey(olc::UP).bHeld && !Player.GetIsCrouched())
    {
        //Look Up
        Player.SetAim(0, -1);
    }
    Player.SetIsMoving(false);
    if (PixelEngine->GetKey(olc::LEFT).bHeld && !Player.GetIsCrouched())
    {
        Player.SetIsMoving(true);
        Player.SetVelocity_X(-2.0f);
        Player.SetDirection(-1);
        Player.SetAim(-1, 0);
    }
    if (PixelEngine->GetKey(olc::RIGHT).bHeld && !Player.GetIsCrouched())
    {
        Player.SetIsMoving(true);
        Player.SetVelocity_X(2.0f);
        Player.SetDirection(1);
        Player.SetAim(1, 0);
    } 
}

void FInputHandler::TwoInputs(FPlayer& Player) const
{
    if (PixelEngine->GetKey(olc::RIGHT).bHeld && PixelEngine->GetKey(olc::UP).bHeld && !Player.GetIsCrouched())
    {
        Player.SetAim(1, -1);
    }
    if (PixelEngine->GetKey(olc::LEFT).bHeld && PixelEngine->GetKey(olc::UP).bHeld && !Player.GetIsCrouched())
    {
        Player.SetAim(-1, -1);
    }
    if (PixelEngine->GetKey(olc::RIGHT).bHeld && PixelEngine->GetKey(olc::DOWN).bHeld && !Player.GetIsCrouched())
    {
        Player.SetAim(1, 1);
    }
    if (PixelEngine->GetKey(olc::LEFT).bHeld && PixelEngine->GetKey(olc::DOWN).bHeld && !Player.GetIsCrouched())
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

void FInputHandler::DownInputs(FPlayer& Player) const
{
    if (PixelEngine->GetKey(olc::DOWN).bHeld)
    {
        if (Player.GetOnGround() && Player.GetVelocity_X() == 0.f)
        {
            Player.SetAim(Player.GetDirection(), 0);
            Player.SetHitOffsetY(10.0f);
            Player.SetIsCrouched(true);
        }
        //crouch
        if (PixelEngine->GetKey(olc::X).bPressed && Player.GetOnGround())
        {
            //Get down
            Player.SetCollidesGround(false);
            Player.SetHitOffsetY(0.0f);
            Player.SetIsCrouched(false);
        }
    }
    else if (PixelEngine->GetKey(olc::X).bPressed)
    {
        Player.Jump();
    }

    if (PixelEngine->GetKey(olc::DOWN).bReleased)
    {//Get Up
        Player.SetHitOffsetY(0.0f);
        Player.SetIsCrouched(false);
    }
}
