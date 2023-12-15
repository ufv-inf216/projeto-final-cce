//
// Created by Campo on 11/12/2023.
//

#include "Math.h"
#include "Hud.h"
#include "Game.h"
#include "Components/StatBlock.h"
#include <string>
#include <sstream>
#include "./Actors/Actor.h"
#include "./Components/DrawComponents/DrawSpriteComponent.h"

Hud::Hud(class Game *owner)
{
    mGame = owner;
    mSrc = mDist = nullptr;
}

Hud::~Hud()
{
    delete mSrc;
    delete mDist;

}

void Hud::PreparedRects(SDL_Texture *tex)
{
    mSrc = new SDL_Rect ();
    mDist= new SDL_Rect ();

    mSrc->x = mSrc->y =0;

    uint32_t* junk = nullptr;
    int* junk2 = nullptr;
    SDL_QueryTexture(tex,junk,junk2,&mSrc->w,&mSrc->h);
    mDist->w = mSrc->w;
    mDist->h = mSrc->h;

}

void Hud::DrawHud()
{

    //Icone
    int dimensionSize = 32 * 3; int padding = 10; int pos = dimensionSize/2 + padding;
    SDL_Texture* icon = mGame->LoadTexture("../Assets/Sprites/Capivaristo/Capivaristo-Icon.png");
    SDL_Rect dest = {padding, padding, dimensionSize, dimensionSize};

    SDL_RenderCopy(mGame->GetRenderer(), icon, nullptr, &dest);

    int qrt = dimensionSize;//mGame->GetWindowWidth()/4;
    int hx = dimensionSize/8;
    SDL_Rect hp = SDL_Rect();
    hp.x = padding;
    hp.y = padding + dimensionSize + 10;
    hp.w = qrt;
    hp.h = hx;

    SDL_SetRenderDrawColor(mGame->GetRenderer(),255,0,0,255);
    SDL_RenderFillRect(mGame->GetRenderer(),&hp);

    SDL_SetRenderDrawColor(mGame->GetRenderer(),0,255,0,255);


    float mod = (float)((float)mGame->GetPlayer()->GetComponent<StatBlock>()->GetHP()/
            (float)mGame->GetPlayer()->GetComponent<StatBlock>()->GetMaxHP());


    hp.w = (int)(qrt * mod);
    SDL_RenderFillRect(mGame->GetRenderer(),&hp);

    std::string liv ="";
    std::stringstream ss;
    ss << mGame->GetPlayer()->GetLives();
    liv = ss.str();
    liv = "x" + liv;

    SDL_Texture* lives= mGame->Render_text(liv);
    if(mSrc == nullptr)
    {
        PreparedRects(lives);
    }

    mDist->x = padding + dimensionSize + 5;
    mDist->y = dimensionSize - padding - 5;

    SDL_RenderCopy(mGame->GetRenderer(),lives,mSrc,mDist);


    SDL_SetRenderDrawColor(mGame->GetRenderer(),255,255,255,255);


}
