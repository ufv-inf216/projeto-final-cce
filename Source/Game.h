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
#include "Actors/Player.h"
#include "stack"
#include "AudioSystem.h"
//#include "Actors/Mob.h"

class Game
{
public:

    const float SCENE_TRANSITION_TIME = 0.75f;

    enum State
    {
        Intro,
        Started,
        Over,
        Won
    };

    enum class GameScene
    {
        None,
        Menu,
        Level1,
        Level2,
    };

    enum class FadeState
    {
        FadeIn,
        FadeOut,
        None
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

    AudioSystem* GetAudio(){return mAudio;}

    SDL_Texture* LoadTexture(const std::string& texturePath);

    // Game-specific
    //const class Pacman* GetPlayer() { return mPacman; }

    void SetGameState(State gameState);
    bool GetGameState() { return mGameState; }
    void SetResort(bool b);

    void InitPlayer();

    void AddCollider(class AABBColliderComponent* collider);
    void RemoveCollider(class AABBColliderComponent* collider);
    void UpdateColliders();
    std::vector<class AABBColliderComponent*>& GetColliders() { return mColliders; }

    float GetFloorHeight() const { return mFloorHeight; }
    void SetFloorHeight(float f) {mFloorHeight=f;}

    bool Add_to_AtkStack(class Mob* m);
    void Remove_from_AtkStack(class Mob* m);

    int GetAliveMobs() const {return  mAliveMobs;}
    void AddAliveMobs(int i) {mAliveMobs+=i;}

    bool IsinAtkersWay(SDL_FRect col);
    Vector2 GetAtKPath();

    int GetMobID() {return mMobId++;}

    class Player* GetPlayer(){return  mPlayer;}

    class Font* GetFont() const {return  mFont;}

    //class PathNode* GetGhostPen() { return mGhostPen; }
    void PrepareScreenMsg(std::string txt, int sz);
    void DestroyScreenMsg();
    SDL_Texture* Render_text(std::string txt,Vector3 col=Color::White,int pointsize=30);
    SDL_Renderer* GetRenderer() const {return  mRenderer;}

    //Game Scene
    GameScene GetGameScene() {return mCurrentScene;}
    void SetScene(GameScene gameState);
    void LoadLevel(const std::string& levelPath);

    void ClearLevel();

    float GetCameraOffset();

    bool GetStopActorInput() const {return mStopActorInput;}
    void SetStopActorInput(bool b) {mStopActorInput=b;}

    std::string GetSceneTitle(GameScene s);




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

    //Level Size
    int mLevelSize;

    // AI stuff

    //Inimigos vivos
    int mAliveMobs;

    //Id dos inimigos
    int mMobId;


    // All the actors in the game
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    // All the draw components
    std::vector<class DrawComponent*> mDrawables;
    std::vector<class AABBColliderComponent*> mColliders;
    std::vector<class Mob*> mAtkStack;

    // SDL stuff
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    AudioSystem* mAudio = nullptr;
    GameScene mCurrentScene;
    class Scene* mScene;

    // Window properties
    int mWindowWidth;
    int mWindowHeight;
    float mFloorHeight;

    float mCameraMult;

    // Scene transition effect
    FadeState mFadeState;
    float mSceneTransitionTime;

    //Menssagens na tela
    SDL_Texture* mMsg_tex;
    SDL_Rect mMsg_rect;
    SDL_Rect mMsg_src;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mUpdatingActors;

    bool mCameraIsBlocked;

    Vector2 mCameraPos;

    bool mStopActorInput;

    // Game-specific
    class Player *mPlayer= nullptr;


    class Font* mFont;
    class Hud* mHud;

    bool mShowGraph = false;
    bool mShowGhostPaths = false;
    bool mPrev1Input = false;
    bool mPrev2Input = false;
    int mGameState = State::Intro;

    float mRespawnTimer = 0.0f;

    //Se verdadeiro a ordem dos sprites é reordenada
    bool mResortSprites;
};
