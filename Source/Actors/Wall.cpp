//
// Created by Daniel Monteiro Rosado on 06/12/23.
//

#include "Wall.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Wall::Wall(Game *game, const std::string &texturePath, int qt, float height)
    : Actor(game)
{
    this->SetPosition(Vector2(1000 + (qt*2000), height/2));
    new DrawSpriteComponent(this, texturePath, 2000, height+10, 0);
}