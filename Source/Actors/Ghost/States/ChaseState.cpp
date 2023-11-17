//
// Created by Lucas N. Ferreira on 26/10/23.
//

#include "ChaseState.h"
#include "../Ghost.h"
#include "../../../Game.h"
#include "../../../Actors/Pacman.h"
#include "../../../Components/AIComponents/FSMComponent.h"

ChaseState::ChaseState(FSMComponent *fsm)
            :GhostState(fsm, "chase")
{

}

void ChaseState::Start()
{
    // TODO [Parte 5.1]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    mGhost->SetForwardSpeed(90.f);
}

PathNode* ChaseState::FindNextNode()
{
    PathNode *targetNode = FindTargetState();
    mGhost->SetTargetNode(targetNode);

    // Select next node
    PathNode *nextNode = nullptr;

    // TODO [Parte 5.2]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    std::vector<PathNode*> mNextNode = mGhost->GetNextNode()->GetAdjacents();
    std::set<PathNode::Type> ignoreType; ignoreType.insert(PathNode::Ghost);
    std::set<PathNode*> ignoreNode; ignoreNode.insert(mGhost->GetPreviousNode());

    nextNode = FindNearestNode(mNextNode, targetNode->GetPosition(), &ignoreType, &ignoreNode);
    if (nextNode == nullptr) nextNode = FindNearestNode(mNextNode, targetNode->GetPosition(), {}, {});

    return nextNode;
}

PathNode* ChaseState::FindTargetState()
{
    PathNode *targetNode = nullptr;

    // TODO [Parte 5.3]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    Ghost::Type gType = mGhost->GetType();
    auto pac = mGhost->GetGame()->GetPlayer();
    if (gType == Ghost::Blinky) {
        targetNode = pac->GetPrevNode();
    }
    else if (gType == Ghost::Pinky) {
        Vector2 pos = pac->GetPointInFrontOf(80.f);
        std::vector<PathNode*> mNextNode = mGhost->GetNextNode()->GetAdjacents();
        std::set<PathNode::Type> ignoreType; ignoreType.insert(PathNode::Ghost); ignoreType.insert(PathNode::Tunnel);
        targetNode = FindNearestNode(mNextNode, pos, &ignoreType, {});
    }
    else if (gType == Ghost::Inky) {
        Vector2 pos = pac->GetPointInFrontOf(40.f);
        Vector2 vp = pos - mGhost->GetPosition();
        vp *= 2;
        vp = vp + mGhost->GetPosition();
        std::vector<PathNode*> mNextNode = mGhost->GetNextNode()->GetAdjacents();
        std::set<PathNode::Type> ignoreType; ignoreType.insert(PathNode::Ghost); ignoreType.insert(PathNode::Tunnel);
        targetNode = FindNearestNode(mNextNode, vp, &ignoreType, {});
    }
    else {
        if ( (pac->GetPosition() - mGhost->GetPosition()).LengthSq() > 150 ) {
            targetNode = pac->GetPrevNode();
        }
        else
            targetNode = mGhost->GetScatterNode();
    }

    return targetNode;
}


void ChaseState::HandleStateTransition(float stateTime)
{
    // TODO [Parte 5.4]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    if (stateTime > 20.f) {
        mFSM->SetState("scatter");
    }
}