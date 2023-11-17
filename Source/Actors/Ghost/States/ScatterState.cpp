//
// Created by Lucas N. Ferreira on 26/10/23.
//

#include "ScatterState.h"
#include "../Ghost.h"
#include "../../../Components/AIComponents/FSMComponent.h"

ScatterState::ScatterState(FSMComponent *fsm)
            :GhostState(fsm, "scatter")
{

}

void ScatterState::Start()
{
    // TODO [Parte 4.1]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    mGhost->SetTargetNode(mGhost->GetScatterNode());
    mGhost->SetForwardSpeed(90.0f);
}

PathNode* ScatterState::FindNextNode()
{
    PathNode *nextNode = nullptr;

    // TODO [Parte 4.2]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    std::vector<PathNode*> mNextNode = mGhost->GetNextNode()->GetAdjacents();
    std::set<PathNode*> ignoreNode; ignoreNode.insert(mGhost->GetPreviousNode());
    std::set<PathNode::Type> ignoreType; ignoreType.insert(PathNode::Ghost); ignoreType.insert(PathNode::Tunnel);

    nextNode = FindNearestNode(mNextNode, mGhost->GetTargetNode()->GetPosition(), &ignoreType, &ignoreNode);
    if (nextNode == nullptr) {
        ignoreType.erase(PathNode::Tunnel);
        nextNode = FindNearestNode(mNextNode, mGhost->GetTargetNode()->GetPosition(), &ignoreType, &ignoreNode);
    }
    if (nextNode == nullptr) {
        nextNode = FindNearestNode(mNextNode, mGhost->GetTargetNode()->GetPosition(), {}, {});
    }
    return nextNode;
}

void ScatterState::HandleStateTransition(float stateTime)
{
    // TODO [Parte 4.3]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    if (stateTime > 5.0f) {
        mFSM->SetState("chase");
    }
}