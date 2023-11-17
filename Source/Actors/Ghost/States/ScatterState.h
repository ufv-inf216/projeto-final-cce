//
// Created by Lucas N. Ferreira on 26/10/23.
//

#pragma once

#include "GhostState.h"

class ScatterState : public GhostState
{
public:
    explicit ScatterState(class FSMComponent *fsm);

    void Start() override;
    void HandleStateTransition(float stateTime) override;

    class PathNode* FindNextNode() override;
};
