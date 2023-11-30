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
#include "../Components/StatBlock.h"




Player::Player(Game *game, float forwardSpeed): Actor(game), mForwardSpeed(forwardSpeed)
{

      mWidth = mHeight = 64;
      mRigidBodyComponent = new RigidBodyComponent(this,1.0,10);

      mColliderComponent = new AABBColliderComponent(this,0,0,mWidth,mHeight,ColliderLayer::Wall);
      //mColliderComponent->SetEnabled(false);
      mShoeCollider = new AABBColliderComponent(this,0,0,mWidth,mHeight/2,ColliderLayer::Shoe);
      //mShoeCollider->SetEnabled(false);



      //mDrawComponent = new DrawSpriteComponent(this,"../Assets/placeholder.png",mWidth,mHeight,1000);
      mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Capivaristo/Capivaristo.png", "../Assets/Sprites/Capivaristo/Capivaristo.json");
      mDrawComponent->AddAnimation("idle", {1,0});
      mDrawComponent->AddAnimation("run", {4,5,6,5});
      mDrawComponent->AddAnimation("jump", {4});
      mDrawComponent->AddAnimation("punch", {2,3});

      mDrawComponent->SetAnimation("idle");
      mDrawComponent->SetAnimFPS(4.0f);



      SetUpdateDrawOrder(true);
      mPunch= nullptr;
      mJumpSpeed = -750.0f;
      SetIsJumping(false);

      mPunchCooldown=0;
      mStatBlock = new StatBlock(this,4);
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

    if(keyState[SDL_SCANCODE_W]&&GetIsJumping()==false)
    {
        mRigidBodyComponent->ApplyForce(Vector2(0,-1 * mForwardSpeed));
    }

    if(keyState[SDL_SCANCODE_S]&&GetIsJumping()==false)
    {
        mRigidBodyComponent->ApplyForce(Vector2(0,mForwardSpeed));
    }

    // Punch

    if(keyState[SDL_SCANCODE_P] &&mPunchCooldown<=0)
    {
         SDL_Log("punch");
         mPunch = new Hitbox(this,mWidth*2,1,mWidth,mHeight,ColliderLayer::AttackHitBox);

         mPunch->DetectCollision(mRigidBodyComponent,mGame->GetColliders());
         mPunch->SetEnabled(false);
         mPunch->SetDestroy(true);
         mPunchCooldown=Punch_cooldown;
    }

    if(keyState[SDL_SCANCODE_SPACE]&&GetIsJumping()==false)
    {
        //SDL_Log("jump");
        SetIsJumping(true);
        mOgY=GetPosition().y;
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x,mJumpSpeed));


    }


}

void Player::OnUpdate(float deltaTime)
{

    ProcessMov();

    //Ordem de desenhar
    if(GetUpdateDrawOrder() && GetIsJumping()==false)
    {
        SetUpdateDrawOrder(false);
        mDrawComponent->SetDrawOrder((int)GetPosition().y);
        mGame->SetResort(true);
    }

    if(mPunchCooldown>0){ mPunchCooldown--;}

    //if(mPunchCooldown<=0){std::cout << "can punch" << std::endl;}

    if(  GetIsJumping()==true&&GetPosition().y>=mOgY)
    {
        SetIsJumping(false);
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x,0));
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
    mStatBlock->TakeDmg(d);
    if(mStatBlock->Is_dead())
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
    float upper_bound = mGame->GetFloorHeight();

    if(GetIsJumping()==true)
    {
        upper_bound=mHeight;
    }

    //Descer
    if(pos.y > (float)mGame->GetWindowHeight() - ((float)mHeight/2))
    {
        //SDL_Log("don't go bellow");
        //SetPosition(Vector2(pos.x,mGame->GetWindowHeight()- ((float)mHeight/2)));
        posCorrect.y = (float)mGame->GetWindowHeight()- ((float)mHeight/2);
    }

    //Subir
    if(pos.y <  upper_bound)
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