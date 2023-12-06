//
// Created by Daniel Monteiro Rosado on 06/12/23.
//
#pragma once

#include "Actor.h"

class Floor : public Actor{
public:
    explicit Floor(Game* game, const std::string &texturePath, int id, float height);
};

