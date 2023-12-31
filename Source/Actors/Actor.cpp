// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Actor.h"
#include "../Game.h"
#include "../Components/Component.h"
#include <algorithm>
#include <typeinfo>
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include <iostream>

Actor::Actor(Game* game)
        : mState(ActorState::Active)
        , mPosition(Vector2::Zero)
        , mScale(1.0f)
        , mRotation(0.0f)
        , mGame(game)
{
    mGame->AddActor(this);
    mShouldDie=false;
    SetIsJumping(false);


}

Actor::~Actor()
{
    mGame->RemoveActor(this);

    for(auto component : mComponents)
    {
        auto ac = dynamic_cast<AABBColliderComponent*>(component);
        if(ac != nullptr)
        {
             mGame->RemoveCollider(ac);
        }
        delete component;
    }
    mComponents.clear();
}

void Actor::RemoveComponent(Component *c)
{
    auto iter = std::find(mComponents.begin(), mComponents.end(), c);
    if(iter != mComponents.end())
    {
        std::iter_swap(iter, mComponents.end() - 1);
        mComponents.pop_back();
    }
}

void Actor::Update(float deltaTime)
{

    if (mState == ActorState::Active)
    {
        for (auto comp : mComponents)
        {
            if(comp->IsEnabled())
            {
                comp->Update(deltaTime);

            }
        }

        OnUpdate(deltaTime);
    }


    if(mShouldDie)
    {
        Kill();
    }
}

void Actor::OnUpdate(float deltaTime)
{

}

void Actor::OnCollision(std::vector<AABBColliderComponent::Overlap>& responses)
{

}



void Actor::ProcessInput(const Uint8* keyState)
{
    if (mState == ActorState::Active)
    {
        for (auto comp : mComponents)
        {
            comp->ProcessInput(keyState);
        }

        OnProcessInput(keyState);
    }
}

void Actor::OnProcessInput(const Uint8* keyState)
{

}

void Actor::AddComponent(Component* c)
{
    mComponents.emplace_back(c);
    std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
        return a->GetUpdateOrder() < b->GetUpdateOrder();
    });
}

void Actor::TakeDamage(int d) {
    //SDL_Log("take daMAGE");
    if(d>0)
    {
       mShouldDie = true;
    }
}

void Actor::Kill() {
    //SDL_Log("Actor will die");
    SetState(ActorState::Destroy);
}

void Actor::Move(Vector2 mv) {}

std::string Actor::GetName() {return  "Generic actor";}

void Actor::PrintNameWithID()
{
    std::cout << GetName() << " " << GetID() << std::endl;
}
