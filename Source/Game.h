// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include <vector>
#include "Math.h"
#include <string>

class Game
{
public:
    enum State
    {
        Intro,
        Started,
        Over,
        Won
    };

    Game(int windowWidth, int windowHeight);

    bool Initialize();
    void RunLoop();
    void Shutdown();
    void Quit() { mIsRunning = false; }

    // Actor functions
    void InitializeActors();
    void UpdateActors(float deltaTime);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    // Draw functions
    void AddDrawable(class DrawComponent* drawable);
    void RemoveDrawable(class DrawComponent* drawable);

    // Camera functions
    Vector2& GetCameraPos() { return mCameraPos; };
    void SetCameraPos(const Vector2& position) { mCameraPos = position; };

    // Window functions
    int GetWindowWidth() const { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }

    SDL_Texture* LoadTexture(const std::string& texturePath);

    // Game-specific
    //const class Pacman* GetPlayer() { return mPacman; }

    void SetGameState(State gameState);
    bool GetGameState() { return mGameState; }
    void SetResort(bool b);

    float GetFloorHeight() const { return mFloorHeight; }
    void SetFloorHeight(float f) {mFloorHeight=f;};

    class PathNode* GetGhostPen() { return mGhostPen; }

private:
    const float RESPAWN_TIME = 1.1f;

    // Core game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Auxiliary updates
    void UpdateCamera();
    void UpdateState(float deltaTime);

    // Load data


    // AI stuff


    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    // All the draw components
    std::vector<class DrawComponent*> mDrawables;

    // SDL stuff
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    // Window properties
    int mWindowWidth;
    int mWindowHeight;
    float mFloorHeight;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mUpdatingActors;

    bool mCameraIsBlocked;

    Vector2 mCameraPos;

    // Game-specific
    class Player *mPlayer= nullptr;
    std::vector<class Item*> mItems;
    std::vector<class Wall*> mWalls;
    std::vector<class Ghost*> mGhosts;
    std::vector<class PathNode*> mPathNodes;

    class PathNode* mTunnelLeft = nullptr;
    class PathNode* mTunnelRight = nullptr;
    class PathNode* mGhostPen = nullptr;

    bool mShowGraph = false;
    bool mShowGhostPaths = false;
    bool mPrev1Input = false;
    bool mPrev2Input = false;
    int mGameState = State::Intro;

    float mRespawnTimer = 0.0f;

    //Se verdadeiro a ordem dos sprites é reordenada
    bool mResortSprites;
};
