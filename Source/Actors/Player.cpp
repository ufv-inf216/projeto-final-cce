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


Player::Player(Game *game, float forwardSpeed, int lives): Actor(game), mForwardSpeed(forwardSpeed)
{
  
      int size = 3;
      SetFriction(10);
      mWidth = mHeight = 32*size;
      mRigidBodyComponent = new RigidBodyComponent(this,1.0,GetFriction());

      mShoeCollider = new AABBColliderComponent(this,-5,mHeight/3,mWidth/3 + 10,mHeight/4,ColliderLayer::Shoe);
      mColliderComponent = new AABBColliderComponent(this,0,0,mWidth,mHeight,ColliderLayer::MobHitBox);
      //mColliderComponent->SetEnabled(false);

      //mShoeCollider->SetEnabled(false);
      mShoeCollider->SetEnabled(true);
      mShoeCollider->SetName("Player Shoe collider");

      mColliderComponent->SetName("Player Hitbox collider");
      mRigidBodyComponent->SetName("Player Rigid body");

      /*
      std::vector<Vector2> verts;
      Vector2 min = mShoeCollider->GetMin(); Vector2 max = mShoeCollider->GetMax();
      verts.emplace_back(min);
      verts.emplace_back(max.x , min.y);
      verts.emplace_back(max);
      verts.emplace_back(min.x, max.y);
      mDrawPolygonComponent = new DrawPolygonComponent(this, verts);*/


      //mDrawComponent = new DrawSpriteComponent(this,"../Assets/placeholder.png",mWidth,mHeight,1000);
      mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Capivaristo/Capivaristo.png", "../Assets/Sprites/Capivaristo/Capivaristo.json",size);
      mDrawComponent->AddAnimation("idle", {1,0});
      mDrawComponent->AddAnimation("run", {4,5,6,5});
      mDrawComponent->AddAnimation("jump", {4});
      mDrawComponent->AddAnimation("punch", {2,3});
      mDrawComponent->AddAnimation("hit", {7,8});

      mDrawComponent->SetAnimation("idle");
      mDrawComponent->SetAnimFPS(4.0f);


      mLives = lives;
      SetUpdateDrawOrder(true);
      mPunch = nullptr;
      mJumpSpeed = -750.0f;
      SetIsJumping(false);

      mPunchCooldown = 0.0f;
      mIsAttacking = false;
      mStatBlock = new StatBlock(this,4);
      mStatBlock->SetName("Player Statblock componenent");
      SetShouldDie(false);

      walk_sound_counter = 0;
}


void Player::OnProcessInput(const Uint8 *keyState)
{


  /* Mover pra direita */
    if(keyState[SDL_SCANCODE_D] && !GetSentBack())
    {
        mRigidBodyComponent->ApplyForce(Vector2(mForwardSpeed,0));
        mRotation = 0.0f;
	if(walk_sound_counter % 30 == 0)
	    mGame->GetAudio()->PlaySound("walk.wav");
    }

    /* Mover pra esquerda */
    if(keyState[SDL_SCANCODE_A] && !GetSentBack())
    {
        mRigidBodyComponent->ApplyForce(Vector2(-1 * mForwardSpeed,0));
        mRotation = Math::Pi;
	if(walk_sound_counter % 30 == 0)
	    mGame->GetAudio()->PlaySound("walk.wav");
    }


    /* Mudanca de direcao no pulo */
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

      // mGame->GetAudio()->PlaySound("soco.wav");
         //SDL_Log("punch");
         int inv = 1;
         if (mRotation == Math::Pi) {
             inv = -1;
         }

         mPunch = new Hitbox(this,mWidth/2*inv,0,mWidth/2,mHeight/2,ColliderLayer::AttackHitBox);
         mPunch->SetKnockback(8000);
         mPunch->SetKnockDir(Vector2(inv,0));

         mPunch->DetectCollision(mRigidBodyComponent,mGame->GetColliders());
         mPunch->SetEnabled(false);
         mPunch->SetDestroy(true);

         mIsAttacking = true;
	     mGame->GetAudio()->PlaySound("cap_attack.wav");

    }

    if(keyState[SDL_SCANCODE_SPACE] && !GetIsJumping())
    {
        //SDL_Log("jump");
        SetIsJumping(true);
        mOgY=GetPosition().y;
        mRigidBodyComponent->SetVelocity(Vector2(mRigidBodyComponent->GetVelocity().x,mJumpSpeed));
	    mGame->GetAudio()->PlaySound("jump.wav");
    }

    if(keyState[SDL_SCANCODE_ESCAPE]) {
        mGame->Quit();
    }

    walk_sound_counter++;
}

void Player::OnUpdate(float deltaTime)
{

    ProcessMov();
    if((int)deltaTime % 10 == 0 && mRigidBodyComponent->GetFrictionCoefficient() < GetFriction())
    {
        mRigidBodyComponent->SetFrictionCoefficient(mRigidBodyComponent->GetFrictionCoefficient()+1);
    }



    //Ordem de desenhar
    if(GetUpdateDrawOrder() && !GetIsJumping())
    {
        SetUpdateDrawOrder(false);
        //mDrawComponent->SetDrawOrder((int)GetPosition().y);
        mDrawComponent->SetDrawOrder((int)mShoeCollider->GetMin().y);
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


    if (mIsAttacking && mPunchCooldown <= .25f) {
        mDrawComponent->SetAnimation("punch", false);
        mDrawComponent->SetAnimFPS(8.f);
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
    //SDL_Log("Player takes damage");
    mStatBlock->TakeDmg(d);
    SDL_Log("TAKEN DAMAGE");
    mDrawComponent->SetAnimation("hit", false);
    mDrawComponent->SetAnimFPS(18.f);
    if(mStatBlock->Is_dead())
    {
        SDL_Log("Player will die");
	    mLives--;
	    //std::cout << "lives" << mLives << std::endl;
	    mStatBlock->SetMaxHP(4);
	    if(mLives < 1)
        {
	        SDL_Log("Player will die fr");
	        SetShouldDie(true);
	        //mGame->Quit();
            mGame->SetGameState(Game::State::Over);
	    }

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
    if(pos.y > (float)mGame->GetWindowHeight() - ((float)mHeight/6))
    {
        //SDL_Log("don't go bellow");
        //SetPosition(Vector2(pos.x,mGame->GetWindowHeight()- ((float)mHeight/2)));
        posCorrect.y = (float)mGame->GetWindowHeight() - ((float)mHeight/6) - abs(posCorrect.y - pos.y);
    }

    //Subir
    if(pos.y <  upper_bound)
    {
        //SetPosition(Vector2(pos.x,mGame->get_floor_height()));
        posCorrect.y = mGame->GetFloorHeight() - abs(posCorrect.y - pos.y);
    }

    //Não pode voltar
    if(pos.x < mGame->GetCameraPos().x + ((float)mWidth/5))
    {
        //SetPosition(Vector2(mGame->GetCameraPos().x + ((float)mWidth/2),pos.y));
        posCorrect.x = mGame->GetCameraPos().x + ((float)mWidth/5) + abs(posCorrect.x - pos.x);
    }

    //Camera começa andar

    if(pos.x > mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2))
    {
        //SetPosition(Vector2(mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2),pos.y));
        posCorrect.x = mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2);
    }


    SetPosition(posCorrect);
}
