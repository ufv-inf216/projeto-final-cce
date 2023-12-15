//
// Created by Campo on 11/12/2023.
//
#pragma once

#include <SDL.h>

class Hud
{
public:
    explicit Hud(class Game* owner);
    ~Hud();
    void DrawHud();

private:
    class Game* mGame;
    SDL_Rect* mSrc;
    SDL_Rect* mDist;
    void PreparedRects(SDL_Texture* tex);

};