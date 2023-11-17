// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Math.h"

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

    static const int GHOST_COUNT = 4;

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
    const class Pacman* GetPlayer() { return mPacman; }

    void SetGameState(State gameState);
    bool GetGameState() { return mGameState; }

    void AddGhost(class Ghost* ghost);
    void RemoveGhost(class Ghost* ghost);
    std::vector<class Ghost*>& GetGhosts() { return mGhosts; }

    void AddItem(class Item* item);
    void RemoveItem(class Item* item);
    std::vector<class Item*>& GetItems() { return mItems; }

    void AddWall(class Wall* wall);
    void RemoveWall(class Wall* wall);
    std::vector<class Wall*>& GetWalls() { return mWalls; }

    void AddPathNode(class PathNode* node);
    void RemovePathNode(class PathNode* node);
    std::vector<class PathNode*>& GetPathNodes() { return mPathNodes; }

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
    void LoadLevel(const std::string& texturePath);
    void LoadPaths(const std::string& fileName);

    // AI stuff
    void DebugDrawPaths();
    void BuildPathGraphVertices(std::ifstream& file,
                                std::vector<std::vector<char>>& txtGrid,
                                std::vector<std::vector<PathNode*>> &nodeGrid);

    void BuildPathGraphEdges(std::vector<std::vector<char>>& txtGrid,
                             std::vector<std::vector<PathNode *>> &grid);

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

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mUpdatingActors;

    Vector2 mCameraPos;

    // Game-specific
    class Pacman *mPacman;
    std::vector<class Item*> mItems;
    std::vector<class Wall*> mWalls;
    std::vector<class Ghost*> mGhosts;
    std::vector<class PathNode*> mPathNodes;

    class PathNode* mTunnelLeft = nullptr;
    class PathNode* mTunnelRight = nullptr;
    class PathNode* mGhostPen = nullptr;

    bool mShowGraph = false;
    bool mShowGhostPaths = true;
    bool mPrev1Input = false;
    bool mPrev2Input = false;
    int mGameState = State::Intro;

    float mRespawnTimer = 0.0f;
};
