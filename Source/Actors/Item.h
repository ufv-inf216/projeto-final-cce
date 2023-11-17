//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include "Actor.h"

class Item : public Actor
{
public:
    enum Type
    {
        Pellet,
        PowerPellet,
    };

    Item(Game* game, Type type, int width, int height);
    ~Item();

    // Get the type of ghost this is
    Type GetType() const { return mType; }

private:
    Type mType;

    int mWidth;
    int mHeight;
};
