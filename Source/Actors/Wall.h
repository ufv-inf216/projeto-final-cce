//
// Created by Daniel Monteiro Rosado on 06/12/23.
//
#pragma once

#include "Actor.h"

class Wall : public Actor{
public:
    explicit Wall(Game* game, const std::string &texturePath, int id, float height);
};
