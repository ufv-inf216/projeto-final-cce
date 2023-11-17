//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Ghost.h"
#include "States/ScatterState.h"
#include "States/ChaseState.h"
#include "States/FrightenedState.h"
#include "States/DeadState.h"
#include "../../Game.h"
#include "../../Components/AIComponents/FSMComponent.h"
#include "../../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../../Components/DrawComponents/DrawPolygonComponent.h"

Ghost::Ghost(Game* game, Type type)
        :Actor(game)
        ,mType(type)
{
    mRigidBodyComponent = new RigidBodyComponent(this);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 4, 4, ColliderLayer::Enemy);

    std::string prefix = "../Assets/Sprites/Ghosts/";
    switch (mType)
    {
        case Blinky:
            prefix += "Blinky/Blinky";
            break;
        case Pinky:
            prefix += "Pinky/Pinky";
            break;
        case Inky:
            prefix += "Inky/Inky";
            break;
        case Clyde:
        default:
            prefix += "Clyde/Clyde";
            break;
    }

    mDrawComponent = new DrawAnimatedComponent(this, prefix + ".png", prefix + ".json");
    mDrawComponent->AddAnimation("idle", {5});
    mDrawComponent->AddAnimation("down", {0, 1});
    mDrawComponent->AddAnimation("left", {2, 3});
    mDrawComponent->AddAnimation("right", {4, 5});
    mDrawComponent->AddAnimation("up", {6, 7});
    mDrawComponent->AddAnimation("dead_down", {8});
    mDrawComponent->AddAnimation("dead_left", {9});
    mDrawComponent->AddAnimation("dead_right", {10});
    mDrawComponent->AddAnimation("dead_up", {11});
    mDrawComponent->AddAnimation("frightened", {12, 13});
    mDrawComponent->AddAnimation("frightened_end", {12, 12, 15, 15});

    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);

//    auto v1 = mColliderComponent->GetMin();
//    auto v4 = mColliderComponent->GetMax();
//
//    Vector2 v2 = Vector2(v4.x, v1.y);
//    Vector2 v3 = Vector2(v1.x, v4.y);
//
//    // Put them into array
//    std::vector<Vector2> vertices = { v1,v2, v4, v3 };
//
//    new DrawPolygonComponent(this, vertices);

    mFSMComponent = new FSMComponent(this);
    new ScatterState(mFSMComponent);
    new ChaseState(mFSMComponent);
    new FrightenedState(mFSMComponent);
    new DeadState(mFSMComponent);

    mGame->AddGhost(this);
}

Ghost::~Ghost()
{
    mGame->RemoveGhost(this);
}

void Ghost::OnUpdate(float deltaTime)
{
    UpdateAnimations();
}

void Ghost::Start()
{
    mState = ActorState::Active;

    mPrevNode = nullptr;
    mNextNode = mSpawnNode;
    mFSMComponent->SetState("start");

    if(mSpawnNode) {
        SetPosition(mSpawnNode->GetPosition());
    }
}


void Ghost::Pause()
{
    mState = ActorState::Paused;
    mRigidBodyComponent->SetVelocity(Vector2::Zero);
    mDrawComponent->SetAnimation("idle");
}

void Ghost::UpdateAnimations()
{
    Vector2 velocity = mRigidBodyComponent->GetVelocity();

    if(mFSMComponent->IsInState("frightened")) {
        if(mFSMComponent->GetStateTime() <= 4.0f) {
            mDrawComponent->SetAnimation("frightened");
        }
        else {
            mDrawComponent->SetAnimation("frightened_end");
        }

        return;
    }
    else if(mFSMComponent->IsInState("dead")) {
        if(velocity.y > .0f) {
            mDrawComponent->SetAnimation("dead_down");
        }
        else if(velocity.y < .0f) {
            mDrawComponent->SetAnimation("dead_up");
        }
        else if(velocity.x > .0f) {
            mDrawComponent->SetAnimation("dead_right");
        }
        else if(velocity.x < .0f) {
            mDrawComponent->SetAnimation("dead_left");
        }
    }
    else {
        if(velocity.y > .0f) {
            mDrawComponent->SetAnimation("down");
        }
        else if(velocity.y < .0f) {
            mDrawComponent->SetAnimation("up");
        }
        else if(velocity.x > .0f) {
            mDrawComponent->SetAnimation("right");
        }
        else if(velocity.x < .0f) {
            mDrawComponent->SetAnimation("left");
        }
    }

}

void Ghost::DebugDrawPath(struct SDL_Renderer* render)
{
    // Don't draw paths when paused
    if (GetState() == ActorState::Paused) {
        return;
    }

    // Set color based on type
    switch (mType)
    {
        case Blinky:
            SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
            break;
        case Pinky:
            SDL_SetRenderDrawColor(render, 255, 184, 255, 255);
            break;
        case Inky:
            SDL_SetRenderDrawColor(render, 0, 255, 255, 255);
            break;
        case Clyde:
        default:
            SDL_SetRenderDrawColor(render, 255, 184, 82, 255);
            break;
    }

    // Draw a rectangle at the target node
    if (mTargetNode != nullptr)
    {
        const int SIZE = 16;
        SDL_Rect r;
        r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
        r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
        r.w = SIZE;
        r.h = SIZE;
        SDL_RenderDrawRect(render, &r);
    }

    // Line from ghost to next node
    if (mNextNode != nullptr)
    {
        SDL_RenderDrawLine(render, static_cast<int>(GetPosition().x),
                           static_cast<int>(GetPosition().y),
                           static_cast<int>(mNextNode->GetPosition().x),
                           static_cast<int>(mNextNode->GetPosition().y));
    }
}