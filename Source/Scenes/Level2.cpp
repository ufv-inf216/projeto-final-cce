#include "Scene.h"
#include <string>
#include <vector>
#include "Sdl.h"
#include "Level2.h"
#include "../Game.h"

Level2::Level2(Game* game) : Scene(game)
{
    mGame = game;
}

void Level2::Load()
{
    SDL_Log("Load level 2");
    mGame->LoadLevel("../Assets/Levels/Level1.txt");

    float floorHeight =  mGame->GetFloorHeight();

    // Player
    if(mGame->GetPlayer()== nullptr)
    {
        mGame->InitPlayer();
    }
    auto playerPosInit = Vector2((float)mGame->GetWindowWidth()/2,floorHeight + 100.f);
    playerPosInit.y += 256;
    mGame->GetPlayer()->SetPosition(playerPosInit);

    mGame->SetGameState(Game::State::Intro);
    mGame->SetCameraPos(Vector2::Zero);

    mGame->SetStopActorInput(false);

}

bool Level2::ShowHud() {return  true;}

void Level2::Unload()
{
    mGame->ClearLevel();
}

int Level2::GetNextScene() { return 0; }//
// Created by Campo on 14/12/2023.
//
