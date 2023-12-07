//
// Created by Daniel Monteiro Rosado on 06/12/23.
//

#include "Wall.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Wall::Wall(Game *game, const std::string &texturePath, int qt, float height)
    : Actor(game)
{
    this->SetPosition(Vector2(height*2 + (qt*height*4), height/2+10));
    new DrawSpriteComponent(this, texturePath, height*4, height+20, 0);
}