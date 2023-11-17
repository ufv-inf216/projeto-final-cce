//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Item.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Item::Item(Game* game, Type type, int width, int height)
        :Actor(game)
        ,mWidth(width)
        ,mHeight(height)
        ,mType(type)
{
    std::string texturePath = "../Assets/Sprites/Items/";
    switch (mType)
    {
        case PowerPellet:
            texturePath += "PowerPellet.png";
            break;
        case Pellet:
        default:
            texturePath += "Pellet.png";
            break;
    }

    new DrawSpriteComponent(this, texturePath, mWidth, mHeight);
    new AABBColliderComponent(this, 0, 0, mWidth, mHeight, ColliderLayer::Item);

    mGame->AddItem(this);
}

Item::~Item()
{
    mGame->RemoveItem(this);
}
