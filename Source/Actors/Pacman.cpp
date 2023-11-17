//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Pacman.h"
#include "PathNode.h"
#include "Ghost/Ghost.h"
#include "../Game.h"
#include "../Actors/Item.h"
#include "../Actors/Wall.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/AIComponents/FSMComponent.h"

Pacman::Pacman(Game* game,
               const float forwardSpeed)
        :Actor(game)
        ,mForwardSpeed(forwardSpeed)
        ,mIsDead(false)
{
    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Pacman/Pacman.png", "../Assets/Sprites/Pacman/Pacman.json");
    mDrawComponent->AddAnimation("idle", {0});
    mDrawComponent->AddAnimation("dead", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    mDrawComponent->AddAnimation("right", {13, 12, 0});
    mDrawComponent->AddAnimation("up", {15, 14, 0});

    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);

    mRigidBodyComponent = new RigidBodyComponent(this);
    mColliderComponent = new AABBColliderComponent(this, 0, 0, 16, 16, ColliderLayer::Player);

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
}

void Pacman::OnProcessInput(const uint8_t* state)
{
    Vector2 velocity = mRigidBodyComponent->GetVelocity();

    if (state[SDL_SCANCODE_D])
    {
        mRigidBodyComponent->SetVelocity(Vector2(mForwardSpeed, velocity.y));
    }
    else if (state[SDL_SCANCODE_A])
    {
        mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, velocity.y));
    }

    if (state[SDL_SCANCODE_W])
    {
        mRigidBodyComponent->SetVelocity(Vector2(velocity.x, -mForwardSpeed));
    }
    else if (state[SDL_SCANCODE_S])
    {
        mRigidBodyComponent->SetVelocity(Vector2(velocity.x, mForwardSpeed));
    }
}

void Pacman::OnUpdate(float deltaTime)
{
    if(mGame->GetGameState() != Game::State::Started) {
        return;
    }

    // Detect and resolve collisions
    DetectCollision();

    // Pacman controls
    Vector2 velocity = mRigidBodyComponent->GetVelocity();

    if(velocity.y == 0.f) {
        if (velocity.x > 0.f) {
            mCurrentDirection = Vector2::UnitX;
            mDrawComponent->FlipHorizontally(false);
        }
        else if (velocity.x < 0.f) {
            mCurrentDirection = Vector2::NegUnitX;
            mDrawComponent->FlipHorizontally(true);
        }
    }
    else {
        if (velocity.y > 0.f) {
            mCurrentDirection = Vector2::NegUnitY;
            mDrawComponent->FlipVertically(true);
        }
        else if (velocity.y < 0.f) {
            mCurrentDirection = Vector2::UnitY;
            mDrawComponent->FlipVertically(false);
        }
    }

    UpdateAnimations();
}

void Pacman::Die()
{
    mIsDead = true;
    mRigidBodyComponent->SetVelocity(Vector2::Zero);
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);
}

void Pacman::Start()
{
    mIsDead = false;
    mRigidBodyComponent->SetVelocity(Vector2::Zero);
    mRigidBodyComponent->SetEnabled(true);
    mColliderComponent->SetEnabled(true);
    mDrawComponent->SetAnimation("idle");

    if (mSpawnNode) {
        SetPosition(mSpawnNode->GetPosition());
    }
}

void Pacman::UpdateAnimations()
{
    Vector2 velocity = mRigidBodyComponent->GetVelocity();

    if(mIsDead) {
        mDrawComponent->SetIsPaused(false);
        SetDirection(Vector2::UnitY);
        mDrawComponent->SetAnimation("dead");
    }
    else {
        if(velocity.x != .0f) {
            mDrawComponent->SetAnimation("right");
            mDrawComponent->SetIsPaused(false);
        }
        else if(velocity.y != .0f) {
            mDrawComponent->SetAnimation("up");
            mDrawComponent->SetIsPaused(false);
        }
        else {
            mDrawComponent->SetIsPaused(true);
            mDrawComponent->ResetAnim();
        }
    }
}

void Pacman::DetectCollision()
{
    // Check collision against walls, items and ghosts
    std::vector<AABBColliderComponent *> colliders;

    for(auto *wall : mGame->GetWalls()) {
        colliders.emplace_back(wall->GetComponent<AABBColliderComponent>());
    }

    for(auto *item : mGame->GetItems()) {
        colliders.emplace_back(item->GetComponent<AABBColliderComponent>());
    }

    for(auto *ghost : mGame->GetGhosts()) {
        colliders.emplace_back(ghost->GetComponent<AABBColliderComponent>());
    }

    for(auto *node : mGame->GetPathNodes()) {
        colliders.emplace_back(node->GetComponent<AABBColliderComponent>());
    }

    mColliderComponent->DetectCollision(mRigidBodyComponent, colliders);
}

void Pacman::OnCollision(std::vector<AABBColliderComponent::Overlap>& responses)
{
    for(auto collision : responses)
    {
        if(collision.target->GetLayer() == ColliderLayer::Item)
        {
            auto *item = dynamic_cast<Item *>(collision.target->GetOwner());

            if(item->GetType() == Item::Type::Pellet)
            {
                // Destroy power pellet
                collision.target->GetOwner()->SetState(ActorState::Destroy);
            }
            else if(item->GetType() == Item::Type::PowerPellet)
            {
                // Destroy power pellet
                collision.target->GetOwner()->SetState(ActorState::Destroy);

                // Make ghosts frighten
                for (auto ghost : mGame->GetGhosts()) {
                    auto *ghostFSM = ghost->GetComponent<FSMComponent>();
                    if(!ghostFSM->IsInState("dead")) {
                        ghostFSM->SetState("frightened");
                    }
                }
            }
        }
        else if(collision.target->GetLayer() == ColliderLayer::Enemy)
        {
            auto *ghost = dynamic_cast<Ghost *>(collision.target->GetOwner());
            auto *ghostFSM = ghost->GetComponent<FSMComponent>();

            if(ghostFSM->IsInState("frightened")) {
                // kill ghost
                ghostFSM->SetState("dead");
            }
            else if(!ghostFSM->IsInState("dead")) {
                // kill pacman
                Die();
            }
        }
        else if(collision.target->GetLayer() == ColliderLayer::Node)
        {
            mPrevNode = dynamic_cast<PathNode *>(collision.target->GetOwner());
        }
    }
}

PathNode* Pacman::GetPrevNode() const
{
    return mPrevNode;
}

Vector2 Pacman::GetPointInFrontOf(float dist) const
{
    Vector2 facingDirection;
    Vector2 velocity = mRigidBodyComponent->GetVelocity();

    if (velocity.y < 0.0f) {
        facingDirection = Vector2::NegUnitY;
    }
    else if (velocity.y > 0.0f) {
        facingDirection = Vector2::UnitY;
    }
    else if (velocity.x > 0.0f) {
        facingDirection = Vector2::UnitX;
    }
    else {
        facingDirection = Vector2::NegUnitX;
    }

    return (GetPosition() + facingDirection * dist);
}
