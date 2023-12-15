//
// Created by Campo on 14/12/2023.
//

#include "Scene.h"
#include <string>
#include <vector>
#include "Sdl.h"
#include "Level4.h"
#include "../Game.h"

Level4::Level4(Game* game) : Scene(game)
{
    mGame = game;
}

void Level4::Load()
{
    SDL_Log("Load level 3");
    mGame->LoadLevel("../Assets/Levels/Level2.txt");

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

bool Level4::ShowHud() {return  true;}

void Level4::Unload()
{
    mGame->ClearLevel();
}

int Level4::GetNextScene() { return (int)Game::GameScene::None; }//
// Created by Campo on 14/12/2023.
//

std::string Level4::GetSceneTitle()
{
    return  "Nível 4";
}
