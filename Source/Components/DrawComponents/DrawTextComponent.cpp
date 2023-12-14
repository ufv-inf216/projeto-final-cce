//
// Created by Lucas N. Ferreira on 08/12/23.
//

#include "DrawTextComponent.h"
#include "../../Font.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"

DrawTextComponent::DrawTextComponent(class Actor* owner, const std::string &text, Font *font, int width, int height, int size, int drawOrder)
        :DrawComponent(owner, drawOrder)
        ,mWidth(width)
        ,mHeight(height)
        ,mSize(size)
        ,mFont(font)
{
    mTextSurface = mFont->RenderText(owner->GetGame()->GetRenderer(), text, Vector3(1.0, 1.0, 1.0), size, 500);
}

DrawTextComponent::~DrawTextComponent()
{
    SDL_DestroyTexture(mTextSurface);
}

void DrawTextComponent::SetText(const std::string &text)
{
    SDL_DestroyTexture(mTextSurface);
    mTextSurface = mFont->RenderText(mOwner->GetGame()->GetRenderer(), text, Vector3(1.0, 1.0, 1.0), mSize, 500);
}

void DrawTextComponent::AdjustSize()
{
    uint32_t* junk = nullptr;
    int* junk2 = nullptr;
    SDL_QueryTexture(mTextSurface,junk,junk2,&mWidth,&mHeight);
}

SDL_Rect DrawTextComponent::GetRenderRect() {
    Vector2 pos = mOwner->GetPosition();
    SDL_Rect renderQuad = {static_cast<int>(pos.x - mWidth/2.0f) ,
                           static_cast<int>(pos.y - mHeight/2.0f),
                           mWidth,
                           mHeight};
    return renderQuad;
}

void DrawTextComponent::Draw(SDL_Renderer *renderer)
{
    Vector2 pos = mOwner->GetPosition();

    SDL_Rect renderQuad = {static_cast<int>(pos.x - mWidth/2.0f) ,
                           static_cast<int>(pos.y - mHeight/2.0f),
                           mWidth,
                           mHeight};

    SDL_RenderCopyEx(renderer, mTextSurface, nullptr, &renderQuad, .0f, nullptr, SDL_FLIP_NONE);
}