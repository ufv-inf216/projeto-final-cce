//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawSpriteComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawSpriteComponent::DrawSpriteComponent(class Actor* owner, const std::string &texturePath, const int width, const int height, const int drawOrder)
        :DrawComponent(owner, drawOrder)
        ,mWidth(width)
        ,mHeight(height)
        ,mIsFlippedHorizontally(false)
        ,mIsFlippedVertically(false)
{
    // Load sprite sheet texture
    mSpriteSheetSurface = mOwner->GetGame()->LoadTexture(texturePath);
}

void DrawSpriteComponent::Draw(SDL_Renderer *renderer)
{
    Vector2 pos = mOwner->GetPosition();
    Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if(mOwner->GetDirection().y < .0f && mIsFlippedVertically) {
        flip = SDL_FLIP_VERTICAL;
    }
    else if (mOwner->GetDirection().x < .0f && mIsFlippedHorizontally) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    SDL_Rect renderQuad = {static_cast<int>(pos.x - mWidth/2.0f - cameraPos.x),
                           static_cast<int>(pos.y - mHeight/2.0f - cameraPos.y),
                           mWidth,
                           mHeight};

    SDL_RenderCopyEx(renderer, mSpriteSheetSurface, nullptr, &renderQuad, .0f, nullptr, flip);
}
