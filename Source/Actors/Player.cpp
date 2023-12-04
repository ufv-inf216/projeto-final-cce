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

      int size = 3;

      mWidth = mHeight = 32*size;
      mRigidBodyComponent = new RigidBodyComponent(this,1.0,10);

      mShoeCollider = new AABBColliderComponent(this,-5,mHeight/3,mWidth/3 + 10,mHeight/4,ColliderLayer::Shoe);
      mColliderComponent = new AABBColliderComponent(this,0,0,mWidth,mHeight,ColliderLayer::MobHitBox);
      //mColliderComponent->SetEnabled(false);

      //mShoeCollider->SetEnabled(false);
      mShoeCollider->SetEnabled(true);
      mShoeCollider->SetName("Player Shoe collider");

      mColliderComponent->SetName("Player Hitbox collider");
      mRigidBodyComponent->SetName("Player Rigid body");


    std::vector<Vector2> verts;
    Vector2 min = mShoeCollider->GetMin(); Vector2 max = mShoeCollider->GetMax();
    verts.emplace_back(min);
    verts.emplace_back(max.x , min.y);
    verts.emplace_back(max);
    verts.emplace_back(min.x, max.y);
    mDrawPolygonComponent = new DrawPolygonComponent(this, verts);


      //mDrawComponent = new DrawSpriteComponent(this,"../Assets/placeholder.png",mWidth,mHeight,1000);
      mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Capivaristo/Capivaristo.png", "../Assets/Sprites/Capivaristo/Capivaristo.json",size);
      mDrawComponent->AddAnimation("idle", {1,0});
      mDrawComponent->AddAnimation("run", {4,5,6,5});
      mDrawComponent->AddAnimation("jump", {4});
      mDrawComponent->AddAnimation("punch", {2,3});

      mDrawComponent->SetAnimation("idle");
      mDrawComponent->SetAnimFPS(4.0f);



      SetUpdateDrawOrder(true);
      mPunch = nullptr;
      mJumpSpeed = -750.0f;
      SetIsJumping(false);

      mPunchCooldown = 0.0f;
      mIsAttacking = false;
      mStatBlock = new StatBlock(this,4);
      mStatBlock->SetName("Player Statblock componenent");
      SetShouldDie(false);
}


void Player::OnProcessInput(const Uint8 *keyState)
{


    if(keyState[SDL_SCANCODE_D])
    {
        mRigidBodyComponent->ApplyForce(Vector2(mForwardSpeed,0));
        mRotation = 0.0f;
    }

    if(keyState[SDL_SCANCODE_A])
    {
        mRigidBodyComponent->ApplyForce(Vector2(-1 * mForwardSpeed,0));
        mRotation = Math::Pi;
    }



    if(keyState[SDL_SCANCODE_W]  && !GetIsJumping() )
    {
        mRigidBodyComponent->ApplyForce(Vector2(0,-1 * mForwardSpeed));
    }

    if(keyState[SDL_SCANCODE_S] && !GetIsJumping())
    {
        mRigidBodyComponent->ApplyForce(Vector2(0,mForwardSpeed));
    }

    // Punch
    if(keyState[SDL_SCANCODE_P] && !mIsAttacking)
    {
         SDL_Log("punch");
         mPunch = new Hitbox(this,mWidth*2,1,mWidth,mHeight,ColliderLayer::AttackHitBox);
         mPunch->SetKnockback(2000);

         mPunch->DetectCollision(mRigidBodyComponent,mGame->GetColliders());
         mPunch->SetEnabled(false);
         mPunch->SetDestroy(true);
         mIsAttacking = true;
    }

    if(keyState[SDL_SCANCODE_SPACE] && !GetIsJumping())
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
    if(GetUpdateDrawOrder() && !GetIsJumping())
    {
        SetUpdateDrawOrder(false);
        mDrawComponent->SetDrawOrder((int)GetPosition().y);
        mGame->SetResort(true);
    }

    //Verificacao de ataque
    if(mIsAttacking) {
        mPunchCooldown += deltaTime;
        if(mPunchCooldown >= .5f) {
            mIsAttacking = false;
            mPunchCooldown = 0.0f;
        }
    }

    if( GetIsJumping() && GetPosition().y >= mOgY)
    {
        SetIsJumping(false);
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x,0));
    }

    //Morrer
    if(GetShouldDie())
    {
        Kill();
    }

    //Animacao
    ManageAnimations();

}

void Player::ManageAnimations() {
    Vector2 velocity = mRigidBodyComponent->GetVelocity();

    if (mIsAttacking) {
        mDrawComponent->SetAnimation("punch");
        mDrawComponent->SetAnimFPS(3.f);
    }
    else {
        mDrawComponent->SetAnimFPS(4.f);
        if ( !Math::NearZero(velocity.x, 15) || !Math::NearZero(velocity.y, 15)) {
            mDrawComponent->SetAnimation("run");
        }
        else {
            mDrawComponent->SetAnimation("idle");
        }
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
        SDL_Log("Player will die");
        //SetShouldDie(true);
    }
}

std::string Player::GetName() {return  "Player";}

void Player::ProcessMov() {
    //mRigidBodyComponent->SetVelocity(Vector2::Zero);
    auto pos = mShoeCollider->GetCenter();
    auto posCorrect = GetPosition();
    float upper_bound = mGame->GetFloorHeight();

    if(GetIsJumping())
    {
        upper_bound = mHeight;
    }

    //Descer
    if(pos.y > (float)mGame->GetWindowHeight() - ((float)mHeight/2))
    {
        //SDL_Log("don't go bellow");
        //SetPosition(Vector2(pos.x,mGame->GetWindowHeight()- ((float)mHeight/2)));
        posCorrect.y = (float)mGame->GetWindowHeight() - ((float)mHeight/2);
    }

    //Subir
    if(pos.y <  upper_bound)
    {
        //SetPosition(Vector2(pos.x,mGame->get_floor_height()));
        posCorrect.y = mGame->GetFloorHeight() - abs(posCorrect.y - pos.y);
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