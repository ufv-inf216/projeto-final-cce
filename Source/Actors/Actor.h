// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <SDL_stdinc.h>
#include "../Math.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include <string>

enum class ActorState
{
    Active,
    Paused,
    Destroy
};

class Actor
{
public:
    explicit Actor(class Game* game);
    virtual ~Actor();

    // Update function called from Game (not overridable)
    void Update(float deltaTime);
    // ProcessInput function called from Game (not overridable)
    void ProcessInput(const Uint8* keyState);

    // Position getter/setter
    const Vector2& GetPosition() const { return mPosition; }
    void SetPosition(const Vector2& pos) { mPosition = pos; }

    Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

    // Scale getter/setter
    float GetScale() const { return mScale; }
    void SetScale(float scale) { mScale = scale; }


    bool GetUpdateDrawOrder() const { return mUpdateDrawOrder; }
    void SetUpdateDrawOrder(bool b) { mUpdateDrawOrder = b; }

    // Rotation getter/setter
    float GetRotation() const { return mRotation; }
    void SetRotation(float rotation) { mRotation = rotation; }

    // State getter/setter
    ActorState GetState() const { return mState; }
    void SetState(ActorState state) { mState = state; }

    // Game getter
    class Game* GetGame() { return mGame; }

    // Returns component of type T, or null if doesn't exist
    template <typename T>
    T* GetComponent() const
    {
        for (auto c : mComponents)
        {
            T* t = dynamic_cast<T*>(c);
            if (t != nullptr)
            {
                return t;
            }
        }

        return nullptr;
    }

    void RemoveComponent(Component* c);

    // Game specific
    Vector2& GetDirection() { return mCurrentDirection; };
    void SetDirection(const Vector2 &direction) { mCurrentDirection = direction; }

    // Any actor-specific collision code (overridable)
    virtual void OnCollision(std::vector<AABBColliderComponent::Overlap>& responses);
    virtual void TakeDamage(int d);
    virtual void Kill();
    virtual std::string GetName();



protected:
    class Game* mGame;

    // Any actor-specific update code (overridable)
    virtual void OnUpdate(float deltaTime);
    // Any actor-specific update code (overridable)
    virtual void OnProcessInput(const Uint8* keyState);


    // Actor's state
    ActorState mState;

    // Transform
    Vector2 mPosition;
    float mScale;
    float mRotation;

    // Components
    std::vector<class Component*> mComponents;

    // Game specific
    Vector2 mCurrentDirection;

    bool GetShouldDie() const {return  mShouldDie;};
    void SetShouldDie(bool b) {mShouldDie = b;}

private:
    friend class Component;
    bool mUpdateDrawOrder;
    bool mShouldDie;

    // Adds component to Actor (this is automatically called
    // in the component constructor)
    void AddComponent(class Component* c);

};