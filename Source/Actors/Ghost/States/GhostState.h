//
// Created by Lucas N. Ferreira on 26/10/23.
//

#pragma once

#include "../../PathNode.h"
#include "../../../Components/AIComponents/FSMState.h"

class GhostState : public FSMState
{
public:
    GhostState(class FSMComponent *fsm, const std::string &name);

    void Update(float deltaTime) override;
    void UpdateDirection();

    virtual PathNode* FindNextNode() { return nullptr; };

    static PathNode *FindNearestNode(std::vector<PathNode*>& nodes, const Vector2& target,
                                     const std::set<PathNode::Type>* ignoreTypes = nullptr,
                                     const std::set<PathNode *>* ignoreNodes = nullptr);

protected:
    class Ghost* mGhost;
};
