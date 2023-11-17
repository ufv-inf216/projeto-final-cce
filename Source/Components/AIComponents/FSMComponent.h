//
// Created by Lucas N. Ferreira on 26/10/23.
//

#pragma once

#include "FSMState.h"
#include "../Component.h"
#include <map>
#include <string>

class FSMComponent : public Component
{
public:
    explicit FSMComponent(struct Actor *owner, int updateOrder = 50);
    ~FSMComponent();

    void Start(const std::string &startState);
    void Update(float deltaTime) override;

    void AddState(std::string &stateName, FSMState *state);

    // Get the current state
    FSMState* GetState() { return mStates[mCurrentState]; }
    void SetState(const std::string &stateName);

    bool IsInState(const std::string &stateName) { return mCurrentState == stateName; }

    // Get time elapsed in current state
    float GetStateTime() const { return mStateTime; }

private:
    bool mIsRunning;

    std::string mCurrentState;
    std::map<std::string, class FSMState*> mStates;
    float mStateTime;
};
