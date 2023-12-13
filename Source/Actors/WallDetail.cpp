//
// Created by Daniel Monteiro Rosado on 06/12/23.
//

#include "WallDetail.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

WallDetail::WallDetail(Game *game, const std::string &texturePath, const char type, int id, float height)
    : Actor(game)
{
    if (type == 'P') {
        GeneratePlaque(texturePath, id, height);
    }
    else if (type == 'D' || type == 'G') {
        GenerateDoorGate(texturePath, id, type, height);
    }
}

void WallDetail::GeneratePlaque(const std::string &texturePath, int id, float height) {
    Vector2 pos(640*id + 320, height/2);
    this->SetPosition(pos);
    new DrawSpriteComponent(this, texturePath+"plaque.png", 70, 50, 1);
}

void WallDetail::GenerateDoorGate(const std::string &texturePath, int id, const char type, float height) {
    Vector2 pos(640*id, height/2);
    if (type == 'G') {
        this->SetPosition(pos);
        new DrawSpriteComponent(this, texturePath+"gate.png", height, height, 1);
    } else {
        this->SetPosition(pos);
        new DrawSpriteComponent(this, texturePath+"door.png", height/2, height, 1);
    }

}