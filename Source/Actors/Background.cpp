//
// Created by Daniel Monteiro Rosado on 13/12/23.
//

#include "Background.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Background::Background(Game *game, const std::string &texturePath, int id, int wHeight, int wWidth, float overlay)
        :Actor(game)
{
    this->SetPosition( Vector2(overlay*id + wWidth/2, wHeight/2) );
    new DrawSpriteComponent(this, texturePath, wWidth, wHeight, 0);
}
