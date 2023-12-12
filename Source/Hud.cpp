//
// Created by Campo on 11/12/2023.
//

#include "Math.h"
#include "Hud.h"
#include "Game.h"
#include "Components/StatBlock.h"
#include <string>
#include <sstream>

Hud::Hud(class Game *owner)
{
    mOwner = owner;
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
    int qrt = mOwner->GetWindowWidth()/4;
    int hx = mOwner->GetWindowHeight()/16;
    SDL_Rect hp = SDL_Rect();
    hp.x = hp.y=0;
    hp.w = qrt;
    hp.h = hx;



    SDL_SetRenderDrawColor(mOwner->GetRenderer(),255,0,0,255);
    SDL_RenderFillRect(mOwner->GetRenderer(),&hp);

    SDL_SetRenderDrawColor(mOwner->GetRenderer(),0,255,0,255);


    float mod = (float)((float)mOwner->GetPlayer()->GetComponent<StatBlock>()->GetHP()/
            (float)mOwner->GetPlayer()->GetComponent<StatBlock>()->GetMaxHP());


    hp.w = (int)(qrt * mod);
    SDL_RenderFillRect(mOwner->GetRenderer(),&hp);

    std::string liv ="";
    std::stringstream ss;
    ss << mOwner->GetPlayer()->GetLives();
    liv = ss.str();
    liv = "x" + liv;

    SDL_Texture* lives= mOwner->Render_text(liv);
    if(mSrc == nullptr)
    {
        PreparedRects(lives);
    }

    mDist->x = qrt + 8;
    mDist->y = (hp.h/2) - (mDist->h/2);

    SDL_RenderCopy(mOwner->GetRenderer(),lives,mSrc,mDist);


    SDL_SetRenderDrawColor(mOwner->GetRenderer(),255,255,255,255);


}
