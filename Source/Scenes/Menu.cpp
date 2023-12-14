//
// Created by Campo on 13/12/2023.
//

#include "Scene.h"
#include "Menu.h"
#include "SDL.h"
#include "../Game.h"
#include "../Actors/Actor.h"
#include "../Components/DrawComponents/DrawTextComponent.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

Menu::Menu(Game* game) : Scene(game)
{
    mSel=0;
    mOptions = std::vector<std::string>();
    mOptions.emplace_back("Novo jogo");
    mOptions.emplace_back("Sair");
    mTitleActor=nullptr;
    mSubTitleActor=nullptr;
    mOptActors = std::vector<Actor*>();

}

void Menu::Load()
{
    float middle = mGame->GetWindowWidth()/2.0f;

    mTitleActor = new Actor(mGame);
    /*Como saber tamanho de uma textura de texto:
     * altura de um charactere é igual ao tamanho da fonte
     * largura é igual a metade do tamanho da fonte
     * largura total = numero de characteres * metade do tamanho da fonte
     * altura total = tamanho da fonte
     *
     */

    auto td = new DrawTextComponent(mTitleActor,"CCE",mGame->GetFont(),108,72,72);
    td->AdjustSize();

    mSubTitleActor = new Actor(mGame);
    std::string subn = "Capivara Combat Experience";
    auto sb = new DrawTextComponent(mSubTitleActor,subn,mGame->GetFont(),subn.length()*15,30,30);
    sb->AdjustSize();

    //auto title_tex = mGame->Render_text("CCE",Color::White,30);


    Vector2 tpos = Vector2::Zero;
    tpos.x = middle;
    tpos.y = 72;
    mTitleActor->SetPosition(tpos);

    Vector2 subtpos = Vector2::Zero;
    subtpos.x = middle;
    subtpos.y = 72 + 30;
    mSubTitleActor->SetPosition(subtpos);

    float st_pos_y = mGame->GetWindowHeight()/2;
    int i =0;
    int max_str_l=0;

    for (auto st : mOptions)
    {
         auto at = new Actor(mGame);
         auto dtc = new DrawTextComponent(at,st,mGame->GetFont(),st.length()*14,28,28);
         dtc->AdjustSize();
         Vector2 newp = Vector2(middle,0);
         newp.y = st_pos_y + (42.f*(float)i);
         at->SetPosition(newp);
         mOptActors.emplace_back(at);

         if(st.length() > max_str_l)
         {
             mRectwRef=i;
             max_str_l = st.length();
         }


         i++;
    }

    mBgActor = new Actor(mGame);
    mBgActor->SetPosition( Vector2(mGame->GetWindowWidth()/2, mGame->GetWindowHeight()/2) );
    new DrawSpriteComponent(mBgActor, "../Assets/Sprites/Bg/menu-cce-bg.png", mGame->GetWindowWidth(), mGame->GetWindowHeight(), 0);
}

void Menu::ProcessInput(const Uint8 *keyState)
{


    //mSel = (mSel >= mOptions.size()) ? 0: mSel;
    //mSel = (mSel < 0) ? (int)mOptions.size()-1: mSel;

    if(mKeyCooldown>0)
    {
        mKeyCooldown--;
        return;
    }



    if(keyState[SDL_SCANCODE_W])
    {
        if(mSel>0){--mSel;}
        mKeyCooldown=10;
    }

    if(keyState[SDL_SCANCODE_S])
    {
        if(mSel<mOptions.size()-1){++mSel;}
        mKeyCooldown=10;
    }

    if(keyState[SDL_SCANCODE_RETURN])
    {
        SelChoice();
    }



}

bool Menu::ShowHud() {return  false;}


void Menu::Draw()
{
    if(mOptions.empty()||mOptActors.empty()){ return;}


    //SDL_Log("Menu draw");
    auto dtc = mOptActors[mRectwRef]->GetComponent<DrawTextComponent>();
    auto rec =dtc->GetRenderRect();
    rec.y = mOptActors[mSel]->GetComponent<DrawTextComponent>()->GetRenderRect().y;
    rec.y -=4;



    SDL_SetRenderDrawColor(GetGame()->GetRenderer(),255,0,0,255);

    SDL_RenderFillRect(mGame->GetRenderer(),&rec);
}

void Menu::SelChoice()
{
    if(mOptions[mSel]=="Sair")
    {
        mGame->Quit();
    }

    if(mOptions[mSel]=="Novo jogo")
    {

        mTitleActor->SetState(ActorState::Destroy);

        mSubTitleActor->SetState(ActorState::Destroy);
        for(auto st : mOptActors)
        {
            st->SetState(ActorState::Destroy);
        }
        mGame->SetScene(Game::GameScene::Level1);
    }

}



