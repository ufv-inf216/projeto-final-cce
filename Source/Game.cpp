// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include "SDL_image.h"
#include "Random.h"
#include "Game.h"
#include "Actors/Actor.h"
#include "Actors/Player.h"
#include "AudioSystem.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Components/DrawComponents/DrawSpriteComponent.h"
#include "Actors/Mob.h"


Game::Game(int windowWidth, int windowHeight)
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mUpdatingActors(false)
        ,mWindowWidth(windowWidth)
        ,mWindowHeight(windowHeight)
        ,mRespawnTimer(RESPAWN_TIME)
        ,mCameraIsBlocked(false)
{
    SetCameraPos(Vector2::Zero);
}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("CCE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, 0);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    Random::Init();

    mTicksCount = SDL_GetTicks();

    // Init all game actors
    InitializeActors();

    /*auto s= new AudioSystem();
    s->CacheSound("punch-2-37333.mp3");
    s->PlaySound("punch-2-37333.mp3");*/



    return true;
}

void Game::InitializeActors()
{
    // Background
    float floorHeight  = (float)mWindowHeight*4.5/10;
    auto background = new Actor(this);

    float drawFloorHeight = floorHeight + 320;

    background->SetPosition(Vector2(0.0f,  drawFloorHeight));
    new DrawSpriteComponent(background, "../Assets/teste-floor.png", 640, 640,0);


    for(int a=1;a<=10;a++)
    {
        auto* background2 = new Actor(this);
        background2->SetPosition(Vector2(606.0f*(float)a, drawFloorHeight ));
        new DrawSpriteComponent(background2, "../Assets/teste-floor.png", 640, 640,1);
    }
    
    SetFloorHeight(floorHeight);

    auto plac = new Actor(this);
    plac->SetPosition(Vector2((float)mWindowHeight/2, (float)GetWindowHeight()*0.7f ));
    int posq = (int)((float)GetWindowHeight()*0.7f);
    new DrawSpriteComponent(plac,"../Assets/placeholder2.png",256,256,posq);


    // Player
    mPlayer = new Player(this);
    auto playerPosInit = Vector2((float)mWindowWidth/2,floorHeight + 100.f);
    playerPosInit.y += 256;
    mPlayer->SetPosition(playerPosInit);

    // Line para background e chão
    /*
    auto* line = new Actor(this);
    line->SetPosition(Vector2(0.0f, GetFloorHeight() ));
    new DrawSpriteComponent(line, "../Assets/placeholder2.png", 10000, 1,3);
     */

    // Croc placeholder
    auto croc = new Mob(this);
    croc->SetPosition(Vector2(1200.0f, floorHeight+100.f));

    SetGameState(State::Intro);
}

void Game::SetGameState(State gameState) {

    mGameState = gameState;
}


void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                Quit();
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    for (auto actor : mActors)
    {
        actor->ProcessInput(state);
    }


    mPrev1Input = static_cast<bool>(state[SDL_SCANCODE_1]);
    mPrev2Input = static_cast<bool>(state[SDL_SCANCODE_2]);
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    // Update all actors and pending actors
    UpdateActors(deltaTime);

    UpdateColliders();

    // Update camera position
    UpdateCamera();

    // Update game state
    UpdateState(deltaTime);
}

void Game::UpdateCamera()
{

    //SDL_Log("update camera");

    if(mPlayer!=nullptr)
    {
        auto v= GetCameraPos();
        v.x = mPlayer->GetPosition().x - ((float)mWindowWidth/2) ;

        if(!mCameraIsBlocked && v.x >= GetCameraPos().x)
        {
            //SDL_Log("camera move");
            SetCameraPos(v);
        }
    }
}


void Game::UpdateColliders()
{
    for(auto c: mColliders)
    {
        if(c->GetDestroy())
        {

            RemoveCollider(c);
        }
    }
}

SDL_Texture* Game::LoadTexture(const std::string& texturePath) {
    // Load from file
    SDL_Surface* surf = IMG_Load(texturePath.c_str());
    if (!surf)
    {
        SDL_Log("Failed to load texture file %s", texturePath.c_str());
        return nullptr;
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surf);
    SDL_FreeSurface(surf);
    if (!texture)
    {
        SDL_Log("Failed to convert surface to texture for %s", texturePath.c_str());
        return nullptr;
    }

    return texture;
}

void Game::UpdateActors(float deltaTime)
{
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors)
    {
        delete actor;
    }
}

void Game::UpdateState(float deltaTime)
{

}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
    mResortSprites = true;
}



void Game::RemoveActor(Actor* actor)
{

    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
    mResortSprites = true;
}

void Game::AddDrawable(class DrawComponent *drawable)
{
    //std::cout << "Add drawable" <<std::endl;
    mDrawables.emplace_back(drawable);

    std::sort(mDrawables.begin(), mDrawables.end(),[](DrawComponent* a, DrawComponent* b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });
    //resort_sprites=true;
}

void Game::RemoveDrawable(class DrawComponent *drawable)
{
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
    //resort_sprites=true;
    //std::cout << "Removed drawable" <<std::endl;
}



void Game::AddCollider(class AABBColliderComponent* collider)
{
    mColliders.emplace_back(collider);
}

void Game::RemoveCollider(AABBColliderComponent* collider)
{
    auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
    if(iter != mColliders.end())
    {

        std::iter_swap(iter, mColliders.end() - 1);
        mColliders.pop_back();
    }
    //mColliders.erase(iter);

}


void Game::GenerateOutput()
{
    // Set draw color to black
    SDL_SetRenderDrawColor(mRenderer, 250, 250, 206, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);



    if(mResortSprites)
    {
        //std::cout <<"rsort" <<std::endl;
        mResortSprites = false;
        std::sort(mDrawables.begin(), mDrawables.end(),[](DrawComponent* a, DrawComponent* b) {
            return a->GetDrawOrder() < b->GetDrawOrder();
        });
    }



    //std::cout << mDrawables.size() <<std::endl;
    for (auto drawable : mDrawables)
    {
        if (drawable->IsVisible())
        {
            drawable->Draw(mRenderer);
        }
    }

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}



void Game::Shutdown()
{
    while (!mActors.empty())
    {
        delete mActors.back();
    }

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::SetResort(bool b) {mResortSprites=b;}
