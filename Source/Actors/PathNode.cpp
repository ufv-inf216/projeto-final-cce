//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "PathNode.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

const int PATH_NODE_SIZE = 16;

PathNode::PathNode(Game* game, Type t)
        :Actor(game)
        ,mType(t)
{
    std::string texturePath = "../Assets/Sprites/Nodes/";
    switch (mType)
    {
        case Ghost:
            texturePath += "NodeGhost.png";
            break;
        case Tunnel:
            texturePath += "NodeTunnel.png";
            break;
        case Default:
        default:
            texturePath += "Node.png";
            break;
    }

    new DrawSpriteComponent(this, texturePath, PATH_NODE_SIZE, PATH_NODE_SIZE);
    new AABBColliderComponent(this, 0, 0, 1, 1, ColliderLayer::Node);

    SetNumber(static_cast<int>(mGame->GetPathNodes().size()));
    mGame->AddPathNode(this);
}

PathNode::~PathNode()
{
    mGame->RemovePathNode(this);
}

void PathNode::AddAdjacent(PathNode *pathNode)
{
    mAdjacent.emplace_back(pathNode);
}
