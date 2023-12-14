//
// Created by Campo on 13/12/2023.
//

#include "Scene.h"
#include <string>
#include <vector>
#include "Sdl.h"
#include "Level1.h"
#include "../Game.h"

Level1::Level1(Game* game) : Scene(game)
{
    mGame = game;
}

void Level1::Load()
{
    SDL_Log("Load level 1");
    mGame->LoadLevel("../Assets/Levels/Level0.txt");

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

}

bool Level1::ShowHud() {return  true;}