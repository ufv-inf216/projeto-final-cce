//
// Created by Lucas N. Ferreira on 26/10/23.
//

#pragma once

#include "GhostState.h"

class FrightenedState : public GhostState
{
public:
    explicit FrightenedState(class FSMComponent *fsm);

    void Start() override;
    void HandleStateTransition(float stateTime) override;

    class PathNode* FindNextNode() override;
};
