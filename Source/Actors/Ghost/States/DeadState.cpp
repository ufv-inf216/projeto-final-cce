//
// Created by Lucas N. Ferreira on 26/10/23.
//

#include "DeadState.h"
#include "../Ghost.h"
#include "../../../Game.h"
#include "../../../Actors/Pacman.h"
#include "../../../Components/AIComponents/FSMComponent.h"
#include "../../../Random.h"

DeadState::DeadState(FSMComponent *fsm)
        :GhostState(fsm, "dead")
{

}

void DeadState::Start()
{
    // TODO [Parte 7.1]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    mGhost->SetTargetNode(mGhost->GetSpawnNode());
    mGhost->SetForwardSpeed(125.f);
}

PathNode* DeadState::FindNextNode()
{
    PathNode *nextNode = nullptr;

    // TODO [Parte 7.2]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    std::vector<PathNode*> mNextNode = mGhost->GetNextNode()->GetAdjacents();
    std::set<PathNode*> ignoreNode; ignoreNode.insert(mGhost->GetPreviousNode());
    std::set<PathNode::Type> ignoreType; ignoreType.insert(PathNode::Default); ignoreType.insert(PathNode::Tunnel);

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


void DeadState::HandleStateTransition(float stateTime)
{
    // TODO [Parte 7.3]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    AABBColliderComponent* ghostPenC = mGhost->GetGame()->GetGhostPen()->GetComponent<AABBColliderComponent>();
    AABBColliderComponent* ghostC = mGhost->GetComponent<AABBColliderComponent>();
    if (ghostC->Intersect(*ghostPenC)) {
        mFSM->SetState("scatter");
    }
}