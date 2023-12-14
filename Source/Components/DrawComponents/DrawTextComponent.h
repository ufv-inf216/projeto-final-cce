//
// Created by Lucas N. Ferreira on 08/12/23.
//

#pragma once

#include "DrawComponent.h"
#include <string>

class DrawTextComponent : public DrawComponent
{
public:
    // (Lower draw order corresponds with further back)
    DrawTextComponent(class Actor* owner, const std::string &text, class Font *font, int width, int height, int fontSize = 24, int drawOrder = 100);
    ~DrawTextComponent();

    void SetText(const std::string &text);

    void Draw(SDL_Renderer* renderer) override;

    void AdjustSize();
    SDL_Rect GetRenderRect();

protected:
    // Map of textures loaded
    SDL_Texture* mTextSurface;
    class Font *mFont;

    int mSize;
    int mWidth;
    int mHeight;
};
