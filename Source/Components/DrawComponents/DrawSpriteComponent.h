//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once
#include "DrawComponent.h"
#include <string>

class DrawSpriteComponent : public DrawComponent
{
public:
    // (Lower draw order corresponds with further back)
    DrawSpriteComponent(class Actor* owner, const std::string &texturePath, int width = 0, int height = 0, int drawOrder = 100);

    void Draw(SDL_Renderer* renderer) override;

    void FlipHorizontally(const bool flip) { mIsFlippedHorizontally = flip; };
    void FlipVertically(const bool flip) { mIsFlippedVertically = flip; };

protected:
    // Map of textures loaded
    SDL_Texture* mSpriteSheetSurface;

    bool mIsFlippedHorizontally;
    bool mIsFlippedVertically;

    int mWidth;
    int mHeight;
};