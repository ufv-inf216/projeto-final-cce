//
// Created by Campo on 11/12/2023.
//
#include <SDL.h>
#ifndef T1_CCE_HUD_H
#define T1_CCE_HUD_H

#endif //T1_CCE_HUD_H



class Hud
{
    public:
        explicit Hud(class Game* owner);
        ~Hud();
        void DrawHud();


    private:
        class Game* mOwner;
        SDL_Rect* mSrc;
        SDL_Rect* mDist;
        void PreparedRects(SDL_Texture* tex);

};