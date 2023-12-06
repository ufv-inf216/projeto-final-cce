//
// Created by Daniel Monteiro Rosado on 06/12/23.
//

#include "Floor.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Floor::Floor(Game *game, const std::string &texturePath, int id, float height)
    :Actor(game)
{
    this->SetPosition( Vector2(606.f*id, height) );
    auto drawComponent = new DrawSpriteComponent(this, texturePath, 640, 640, 0);
}