//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include "Actor.h"

class PathNode : public Actor
{
public:
    enum Type
    {
        Default,
        Ghost,
        Tunnel
    };

    explicit PathNode(Game* game, Type t);
    ~PathNode();

    Type GetType() const { return mType; }

    int GetNumber() const { return mNumber; }
    void SetNumber(int number) { mNumber = number; }

    void AddAdjacent(PathNode *pathNode);
    std::vector<PathNode*> &GetAdjacents() { return mAdjacent; };

private:
    Type mType;

    // Adjacent nodes
    std::vector<PathNode*> mAdjacent;

    // An id for the node, to aid in debugging.
    int mNumber = 0;
};
