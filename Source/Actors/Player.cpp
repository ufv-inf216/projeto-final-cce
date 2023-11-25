//
// Created by Campo on 20/11/2023.
//

#include <iostream>
#include "Player.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../Components/ColliderComponents/Hitbox.h"




Player::Player(Game *game, float forwardSpeed): Actor(game), mForwardSpeed(forwardSpeed)
{

      mWidth = mHeight = 64;
      mRigidBodyComponent = new RigidBodyComponent(this,1.0,10);

      mColliderComponent = new AABBColliderComponent(this,0,0,mWidth,mHeight,ColliderLayer::Wall);
      //mColliderComponent->SetEnabled(false);
      mShoeCollider = new AABBColliderComponent(this,0,0,mWidth,mHeight/2,ColliderLayer::Shoe);
      //mShoeCollider->SetEnabled(false);

      mDrawComponent = new DrawSpriteComponent(this,"../Assets/placeholder.png",mWidth,mHeight,1000);
      SetUpdateDrawOrder(true);
      mPunch= nullptr;
}


void Player::OnProcessInput(const Uint8 *keyState)
{


    if(keyState[SDL_SCANCODE_D])
    {
        mRigidBodyComponent->ApplyForce(Vector2(mForwardSpeed,0));
        SetRotation(0);
    }

    
    if(keyState[SDL_SCANCODE_A])
    {
        mRigidBodyComponent->ApplyForce(Vector2(-1 * mForwardSpeed,0));
        SetRotation(Math::Pi);
    }

    if(keyState[SDL_SCANCODE_W])
    {
        mRigidBodyComponent->ApplyForce(Vector2(0,-1 * mForwardSpeed));
    }

    if(keyState[SDL_SCANCODE_S])
    {
        mRigidBodyComponent->ApplyForce(Vector2(0,mForwardSpeed));
    }

    // Punch
    if(keyState[SDL_SCANCODE_P] && mPunch == nullptr)
    {
         SDL_Log("punch");
         mPunch = new Hitbox(this,mWidth*2,1,mWidth,mHeight,ColliderLayer::AttackHitBox);

         mPunch->DetectCollision(mRigidBodyComponent,mGame->GetColliders());
         mPunch->SetEnabled(false);
         mPunch->SetDestroy(true);
    }


}

void Player::OnUpdate(float deltaTime)
{

    ProcessMov();

    //Ordem de desenhar
    if(GetUpdateDrawOrder())
    {
        SetUpdateDrawOrder(false);
        mDrawComponent->SetDrawOrder((int)GetPosition().y);
        mGame->SetResort(true);
    }

    //Morrer
    if(GetShouldDie())
    {
        Kill();
    }
}


void Player::OnCollision(std::vector<AABBColliderComponent::Overlap> &responses)
{
    for(auto rp:responses)
    {
    }
}

void Player::TakeDamage(int d)
{
    SDL_Log("Player takes damage");
    if(d > 0)
    {
        SetShouldDie(true);
    }
}

std::string Player::GetName() {return  "Player";}

void Player::ProcessMov() {
    //mRigidBodyComponent->SetVelocity(Vector2::Zero);
    auto pos = GetPosition();
    auto posCorrect = Vector2::Zero;
    posCorrect.x = pos.x; posCorrect.y = pos.y;

    //Descer
    if(pos.y > (float)mGame->GetWindowHeight() - ((float)mHeight/2))
    {
        //SDL_Log("don't go bellow");
        //SetPosition(Vector2(pos.x,mGame->GetWindowHeight()- ((float)mHeight/2)));
        posCorrect.y = (float)mGame->GetWindowHeight()- ((float)mHeight/2);
    }

    //Subir
    if(pos.y < mGame->GetFloorHeight() )
    {
        //SetPosition(Vector2(pos.x,mGame->get_floor_height()));
        posCorrect.y = mGame->GetFloorHeight();
    }

    //Não pode voltar
    if(pos.x < mGame->GetCameraPos().x + ((float)mWidth/2))
    {
        //SetPosition(Vector2(mGame->GetCameraPos().x + ((float)mWidth/2),pos.y));
        posCorrect.x = mGame->GetCameraPos().x + ((float)mWidth/2);
    }

    //Camera começa andar
    if(pos.x > mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2))
    {
        //SetPosition(Vector2(mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2),pos.y));
        posCorrect.x = mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2);
    }

    SetPosition(posCorrect);
}