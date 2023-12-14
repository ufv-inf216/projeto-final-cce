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
#include "SDL_ttf.h"
#include "Hud.h"
#include "Scenes/Scene.h"
#include "Scenes/Menu.h"


#include "Actors/Actor.h"
#include "Actors/Player.h"
#include "Actors/Mob.h"
#include "Actors/Spawner.h"
#include "Actors/Background.h"
#include "Actors/Floor.h"
#include "Actors/Wall.h"
#include "Actors/WallDetail.h"
#include "AudioSystem.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "Components/DrawComponents/DrawSpriteComponent.h"
#include "Font.h"
#include "Scenes/Level1.h"


Game::Game(int windowWidth, int windowHeight)
        : mWindow(nullptr)
        , mRenderer(nullptr)
        , mTicksCount(0)
        , mIsRunning(true)
        , mUpdatingActors(false)
        , mWindowWidth(windowWidth)
        , mWindowHeight(windowHeight)
        , mRespawnTimer(RESPAWN_TIME)
        , mCameraIsBlocked(false)
        , mAliveMobs(0)
        , mMobId(1)
        , mMsg_tex(nullptr)
        ,mScene(nullptr)
        , mCurrentScene(GameScene::Menu)
{
    SetCameraPos(Vector2::Zero);
    mMsg_rect = SDL_Rect();
    mMsg_src= SDL_Rect ();

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

    auto ttf = TTF_Init();

    if(ttf==-1)
    {
        SDL_Log("Failed to load SDL_ttf: %s", TTF_GetError());
        return false;
    }

    mAudio = new AudioSystem(48);

    mHud = new Hud(this);

    mFont = new Font();

    mFont->Load("../Assets/Fonts/Carlito-Regular.ttf");
    
    Random::Init();

    mTicksCount = SDL_GetTicks();

    mAudio->PlaySound("bgm.mp3", true);
    // Init all game actors
    InitializeActors();

    return true;
}

void Game::InitPlayer()
{
    mPlayer =new Player(this);
}

void Game::InitializeActors()
{

    if(mCurrentScene!=GameScene::None)
    {
        switch (mCurrentScene) {
            case GameScene::Menu:
            {
                mScene = new Menu(this);
                break;
            }

            case GameScene::Level1:
            {
                mScene = new Level1(this);
                break;
            }

            default:
            {

                break;
            }
        }
        mScene->Load();
        return;
    }

    // Background
    LoadLevel("../Assets/Levels/Level0.txt");

    float floorHeight = GetFloorHeight();

    // Player
    mPlayer = new Player(this);
    auto playerPosInit = Vector2((float)mWindowWidth/2,floorHeight + 100.f);
    playerPosInit.y += 256;
    mPlayer->SetPosition(playerPosInit);

    SetGameState(State::Intro);

    // Line para background e chão
    /*
    auto* line = new Actor(this);
    line->SetPosition(Vector2(0.0f, GetFloorHeight() ));
    new DrawSpriteComponent(line, "../Assets/placeholder2.png", 10000, 1,3);
     */

    // Croc placeholder
    //auto croc = new Mob(this);
    //croc->SetPosition(Vector2(1200.0f, floorHeight+100.f));


}

void Game::LoadLevel(const std::string &levelPath) {


    SDL_Log("Load level");
    float overlay = 640 - 111.f;

    float floorHeight  = (float)mWindowHeight*4.5/10;
    SetFloorHeight(floorHeight);

    std::fstream level;
    level.open(levelPath);
    std::string row;

    std::string texPath = "../Assets/Sprites/Bg/";

    //Tamanho do mapa
    std::getline(level, row);
    int nBackgrounds = std::stoi(row);
    mLevelSize = 640 + ( overlay * (nBackgrounds-1) ) - 660;
    int nFloors = mLevelSize/300;

    for (int n=0;n<nBackgrounds;n++) {
        new Background(this, "../Assets/Sprites/Bg/floor-wall.png", n, mWindowHeight, mWindowWidth, overlay);
    }
    /*
    //Floors
    for (int n=0;n<nFloors;n++)
        new Floor(this, "../Assets/Sprites/Bg/floor.png", n, floorHeight);

    //Wall
    int nWalls = mLevelSize / (floorHeight*4);
    if (nWalls < 1) nWalls = 1;
    for (int n=0;n<nFloors;n++)
        new Wall(this, "../Assets/Sprites/Bg/wall-bg.png", n, floorHeight);
    */


    //Portas e portoes
    std::getline(level, row);
    for (int n=0;n<nBackgrounds;n++) {
        if (n >= row.size()) break;
        if (row[n] != '.')
            new WallDetail(this, texPath, row[n], n, floorHeight);
    }

    std::getline(level, row);
    //std::cout << row.length() << " " << nFloors << std::endl;
    for (int n=0;n<nBackgrounds;n++) {
        if (n >= row.size()) break;
        if (row[n] != '.' && isalnum(row[n]))
        {
            std::string eat(1,row[n] );
            int reps = std::stoi(eat );
            //std::cout << reps << std::endl;
            for(int a=0;a<reps;a++)
            {
                auto s= new Spawner(this,606.f);
                s->SetPosition(Vector2(606.f*(float)(n),floorHeight + (128.f * a)));

            }
        }

    }



}

void Game::SetGameState(State gameState) {

    mGameState = gameState;
}

void Game::SetScene(GameScene gameState)
{
    // Stop all sounds
    //mAudio->StopAllSounds();


    // Handle scene transition
    mCurrentScene= gameState;
//    UnloadActors();
//    InitializeActors();
    InitializeActors();
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

    mAudio->ProcessInput(state);
    if(mScene!= nullptr)
    {
        mScene->ProcessInput(state);
    }

}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    if(mGameState == State::Won && mMsg_tex== nullptr)
    {
        PrepareScreenMsg("You win!",30);
    }

    if(mGameState == State::Over && mMsg_tex== nullptr)
    {
        PrepareScreenMsg("Game over",30);
    }


    mTicksCount = SDL_GetTicks();

    mAudio->Update(deltaTime);

    // Update all actors and pending actors
    UpdateActors(deltaTime);

    UpdateColliders();

    if(mAliveMobs>0)
    {
        mCameraIsBlocked=true;
    }
    else{
        mCameraIsBlocked=false;
    }

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

        if (GetCameraPos().x >= mLevelSize)
        {
            mCameraIsBlocked = true;
            SetGameState(Game::State::Won);
        }


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

bool Game::Add_to_AtkStack(Mob* m)
{
    if(mAtkStack.empty())
    {
        mAtkStack.emplace_back(m);
        //SDL_Log("Mob will attack");
        m->SetDoAtk(true);
        return  true;
    }
    else{
        //SDL_Log("In list");
        //mAtkStack[0]->PrintNameWithID();
    }
    return  false;
}

void Game::Remove_from_AtkStack(class Mob *m)
{
    auto iter = std::find(mAtkStack.begin(), mAtkStack.end(), m);
    if (iter != mAtkStack.end())
    {
        std::iter_swap(iter,mAtkStack.end()-1);
        mAtkStack.pop_back();
        m->SetDoAtk(false);
    }
}

void Game::RemoveActor(Actor* actor)
{

    //SDL_Log("Actor will be removed");
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
    if(mCurrentScene!=GameScene::Menu)
    {
        SDL_SetRenderDrawColor(mRenderer, 250, 250, 206, 255);
    }
    else{
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
    }


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

    if(mCurrentScene == GameScene::Menu)
    {
        mScene->Draw();
    }

    //std::cout << mDrawables.size() <<std::endl;
    for (auto drawable : mDrawables)
    {
        if (drawable->IsVisible())
        {
            drawable->Draw(mRenderer);
        }
    }

    if(mGameState!=State::Over && (mScene== nullptr||mScene->ShowHud()))
    {
        mHud->DrawHud();
    }


    if(mMsg_tex!= nullptr)
    {
        SDL_RenderCopy(mRenderer,mMsg_tex,&mMsg_src,&mMsg_rect);
    }



    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

bool Game::IsinAtkersWay(SDL_FRect col)
{
    if(mAtkStack.empty()){return false;}
    float x1 = mAtkStack[0]->GetPosition().x;
    float y1 = mAtkStack[0]->GetPosition().y;
    float x2 = mPlayer->GetPosition().x;
    float y2 = mPlayer->GetPosition().y;
    SDL_FRect* poi = &col;

    auto b = SDL_IntersectFRectAndLine(poi,&x1,&y1,&x2,&y2);

    return  b == SDL_bool::SDL_TRUE;


}

Vector2 Game::GetAtKPath()
{
    return  (mPlayer->GetPosition() - mAtkStack[0]->GetPosition()) * -1;
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

void Game::PrepareScreenMsg(std::string txt, int sz)
{
    mMsg_tex = mFont->RenderText(mRenderer,txt);
    uint32_t* junk = nullptr;
    int *junk2  = nullptr;
    mMsg_src.x = mMsg_src.y=0;
    SDL_QueryTexture(mMsg_tex,junk,junk2,&mMsg_src.w,&mMsg_src.h);
    mMsg_rect.w =mMsg_src.w;  mMsg_rect.h =mMsg_src.h;

    mMsg_rect.x = (mWindowWidth/2) - (mMsg_rect.w/2);
    mMsg_rect.y = (mWindowHeight/2) - (mMsg_rect.h/2);


}

void Game::DestroyScreenMsg() {
    delete mMsg_tex;
    mMsg_tex= nullptr;
}

SDL_Texture* Game::Render_text(std::string txt,Vector3 col,int pointsize)
{
    return  mFont->RenderText(mRenderer,txt,Color::White,pointsize);
}
