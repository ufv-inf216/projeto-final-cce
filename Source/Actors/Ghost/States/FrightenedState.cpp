//
// Created by Lucas N. Ferreira on 26/10/23.
//

#include "FrightenedState.h"
#include "../Ghost.h"
#include "../../../Game.h"
#include "../../../Actors/Pacman.h"
#include "../../../Components/AIComponents/FSMComponent.h"
#include "../../../Random.h"

FrightenedState::FrightenedState(FSMComponent *fsm)
        :GhostState(fsm, "frightened")
{

}

void FrightenedState::Start()
{
    // TODO [Parte 6.1]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    mGhost->SetTargetNode(nullptr);
    mGhost->SetForwardSpeed(65.f);

    PathNode* aux = mGhost->GetPreviousNode();
    mGhost->SetPreviousNode(mGhost->GetNextNode());
    mGhost->SetNextNode(aux);

    UpdateDirection();
}

PathNode* FrightenedState::FindNextNode()
{
    PathNode *nextNode = nullptr;

    // TODO [Parte 6.2]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    std::vector<PathNode*> mNextNode = mGhost->GetNextNode()->GetAdjacents();
    std::set<PathNode*> ignoreNode; ignoreNode.insert(mGhost->GetPreviousNode());
    std::set<PathNode::Type> ignoreType; ignoreType.insert(PathNode::Ghost); ignoreType.insert(PathNode::Tunnel);


    nextNode = FindNearestNode(mNextNode, mNextNode[0]->GetPosition(), &ignoreType, &ignoreNode);
    if (nextNode == nullptr) {
        ignoreType.erase(PathNode::Ghost);
        nextNode = FindNearestNode(mNextNode, mNextNode[0]->GetPosition(), &ignoreType, &ignoreNode);
    }
    if (nextNode == nullptr) {
        ignoreType.erase(PathNode::Tunnel);
        nextNode = FindNearestNode(mNextNode, mNextNode[0]->GetPosition(), {}, {});
    }

    return nextNode;
}

void FrightenedState::HandleStateTransition(float stateTime)
{
    // TODO [Parte 6.3]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    if (stateTime > 7.0f) {
        mFSM->SetState("scatter");
    }
}