//
// Created by Lucas N. Ferreira on 26/10/23.
//

#include "GhostState.h"
#include "../Ghost.h"
#include "../../../Game.h"
#include "../../../Components/AIComponents/FSMComponent.h"

GhostState::GhostState(class FSMComponent *fsm, const std::string &name)
        : FSMState(fsm, name)
{
    mGhost = dynamic_cast<Ghost *>(mFSM->GetOwner());
}

void GhostState::Update(float deltaTime)
{
    // TODO [Parte 3.1]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    PathNode* next = mGhost->GetNextNode();
    if (!next) return;

    AABBColliderComponent* b = next->GetComponent<AABBColliderComponent>();

    if (mGhost->GetComponent<AABBColliderComponent>()->Intersect(*b)) {
        mGhost->SetPosition(next->GetPosition());

        PathNode* newNode = FindNextNode();
        if (newNode) {
            mGhost->SetPreviousNode(next);
            mGhost->SetNextNode(newNode);

            UpdateDirection();
        }
    }

    mGhost->GetComponent<RigidBodyComponent>()->SetVelocity(mGhost->GetForwardSpeed()*mGhost->GetDirection());

}

void GhostState::UpdateDirection()
{
    // TODO [Parte 3.2]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    PathNode* prev = mGhost->GetPreviousNode();
    PathNode* next = mGhost->GetNextNode();

    Vector2 newDirection = Vector2::Normalize(next->GetPosition() - prev->GetPosition());

    if (next->GetType() == PathNode::Tunnel && prev->GetType() == PathNode::Tunnel) {
        newDirection *= -1;
    }

    mGhost->SetDirection(newDirection);
}

PathNode *GhostState::FindNearestNode(std::vector<PathNode*>& nodes, const Vector2& targetPosition, const std::set<PathNode::Type>* ignoreTypes, const std::set<PathNode *>* ignoreNodes)
{
    PathNode *nearestNode = nullptr;

    // TODO [Parte 3.3]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    float dist = MAXFLOAT;
    for (auto node : nodes) {

        if ( (ignoreTypes != nullptr && ignoreTypes->find(node->GetType()) != ignoreTypes->end()) || (ignoreNodes != nullptr && ignoreNodes->find(node) != ignoreNodes->end())) continue;

        if ( (node->GetPosition() - targetPosition).LengthSq() < dist ) {
            dist = (node->GetPosition() - targetPosition).LengthSq();
            nearestNode = node;
        }
    }

    return nearestNode;
}
