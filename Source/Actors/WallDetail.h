//
// Created by Daniel Monteiro Rosado on 06/12/23.
//
#pragma once
#include "Actor.h"

class WallDetail : public Actor{
private:
    void GeneratePlaque(const std::string &texturePath, int id, float height);
    void GenerateDoorGate(const std::string &texturePath, int id, const char type, float height);
public:
    explicit WallDetail(Game* game, const std::string &texturePath, const char type, int id, float height);
};

