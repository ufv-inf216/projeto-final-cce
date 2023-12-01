// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL_stdinc.h>
#include <string>

class Component
{
public:
    // Constructor
    // (the lower the update order, the earlier the component updates)
    explicit Component(class Actor* owner, int updateOrder = 100);
    // Destructor
    virtual ~Component();
    // Update this component by delta time
    virtual void Update(float deltaTime);
    // Process input for this component (if needed)
    virtual void ProcessInput(const Uint8* keyState);

    int GetUpdateOrder() const { return mUpdateOrder; }
    class Actor* GetOwner() const { return mOwner; }
    class Game* GetGame() const;

    void SetEnabled(const bool enabled) { mIsEnabled = enabled; };
    bool IsEnabled() const { return mIsEnabled; };

    void SetDestroy(bool enabled) { mDestroy = enabled; };
    bool GetDestroy() const { return mDestroy; };

    void SetName(std::string n) { mName =n; };
    std::string GetName() const { return mName; };

protected:
    // Owning actor
    class Actor* mOwner;
    // Update order
    int mUpdateOrder;
    std::string mName;

    bool mIsEnabled;
    bool mDestroy;
};
