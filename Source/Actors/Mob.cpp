//
// Created by Campo on 24/11/2023.
//
#include "Mob.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/StatBlock.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/AIComponents/CrocChase.h"
#include "../Components/AIComponents/CrocWait.h"
#include "../Components/ColliderComponents/Hitbox.h"



Mob::Mob(Game *game, float forwardSpeed): Actor(game), mForwardSpeed(forwardSpeed)
{

    mBiteCooldown=0.0f;
    mIsBiting=false;
    SetFriction(10);

    mHeight = 64;
    mWidth = mHeight*2;
    mRigidBodyComponent = new RigidBodyComponent(this,0.5,GetFriction());

    mShoeCollider = new AABBColliderComponent(this,0,0,mWidth,mHeight/2,ColliderLayer::Shoe);
    mColliderComponent = new AABBColliderComponent(this,0,0,mWidth,mHeight,ColliderLayer::MobHitBox);


    mDrawComponent = new DrawAnimatedComponent(this,"../Assets/Sprites/Croc/Croc.png", "../Assets/Sprites/Croc/Croc.json",3);
    mDrawComponent->AddAnimation("run", {1,2});
    mDrawComponent->AddAnimation("bite", {0});

    mDrawComponent->SetAnimation("run");
    mDrawComponent->SetAnimFPS(5.0f);



    SetUpdateDrawOrder(true);
    mStatBlock = new StatBlock(this,5);

    mShoeCollider->SetName("Mob Shoe collider");
    mColliderComponent->SetName("Mob Hitbox collider");

    std::vector<Vector2> verts;
    Vector2 min = mShoeCollider->GetMin(); Vector2 max = mShoeCollider->GetMax();
    verts.emplace_back(min);
    verts.emplace_back(max.x , min.y);
    verts.emplace_back(max);
    verts.emplace_back(min.x, max.y);
    mDrawPolygonComponent = new DrawPolygonComponent(this, verts);

    mAtkRange=mHeight;

    mCrocAi= new FSMComponent(this);
    new CrocChase(this,mCrocAi,"Chase",(float)mWidth,(float)mHeight,mForwardSpeed);
    new CrocWait(this,mCrocAi,"Wait",mForwardSpeed);

    mCrocAi->Start("start");
    mCrocAi->SetState("Chase");
    SetDoAtk(false);

    GetGame()->AddAliveMobs(1);

    SetID(mGame->GetMobID());



}

void Mob::OnUpdate(float deltaTime)
{
    auto pos = GetPosition();
    auto posCorrect = Vector2();
    posCorrect.x = pos.x; posCorrect.y = pos.y;
    if(pos.y > (float)mGame->GetWindowHeight() - ((float)mHeight/2))
    {
        //SDL_Log("don't go bellow");
        //SetPosition(Vector2(pos.x,mGame->GetWindowHeight()- ((float)mHeight/2)));
        posCorrect.y = (float)mGame->GetWindowHeight()- ((float)mHeight/2);
    }

    if(pos.y < mGame->GetFloorHeight() )
    {
        //SetPosition(Vector2(pos.x,mGame->get_floor_height()));
        posCorrect.y = mGame->GetFloorHeight();
    }

    if(mIsBiting==true)
    {
        mBiteCooldown+=deltaTime;
        if(mBiteCooldown>0.4f)
        {
            mBiteCooldown=0.0f;
            mIsBiting=false;
            DoBite();

        }
    }


//    if(pos.x < mGame->GetCameraPos().x + ((float)mWidth/2))
//    {
//        //SetPosition(Vector2(mGame->GetCameraPos().x + ((float)mWidth/2),pos.y));
//        posCorrect.x = mGame->GetCameraPos().x + ((float)mWidth/2);
//    }

      if(pos.x > mGame->GetCameraPos().x + (float)mGame->GetWindowWidth())
      {
          //SetPosition(Vector2(mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2),pos.y));
          posCorrect.x = mGame->GetCameraPos().x + (float)mGame->GetWindowWidth();
      }

    SetPosition(posCorrect);

    if(GetUpdateDrawOrder())
    {
        SetUpdateDrawOrder(false);
        //mDrawComponent->SetDrawOrder((int)GetPosition().y);
        mDrawComponent->SetDrawOrder((int)mShoeCollider->GetMin().y);
        mGame->SetResort(true);
    }

    if(GetShouldDie())
    {
        Kill();
    }
}

std::string Mob::GetName() {return  "Mob actor";}

void Mob::TakeDamage(int d)
{

    mStatBlock->TakeDmg(d);
    if(mStatBlock->Is_dead())
    {

        //SDL_Log("mob will die");
      mGame->GetAudio()->PlaySound("kill_croc.wav");
      SetShouldDie(true);
      mRigidBodyComponent->SetVelocity(Vector2(0,-1000));
      GetGame()->AddAliveMobs(-1);
      GetGame()->Remove_from_AtkStack(this);
    }
}

void Mob::Move(Vector2 mv) {

    /*if(GetRotation() == Math::Pi && mv.x > 0.0f)
    {
        SetRotation(0);
    }
    else if(GetRotation() ==0 && mv.x < 0.0f)
    {
        SetRotation(Math::Pi);
    }*/
    mRigidBodyComponent->ApplyForce(mv);
}


void Mob::BeginBite()
{
    mDrawComponent->SetAnimation("bite");
    mDrawComponent->SetAnimFPS(0.0f);
    mIsBiting=true;
    mGame->GetAudio()->PlaySound("before_bite.wav");
}

void Mob::ManageAnimations()
{

}

void Mob::DoBite()
{
    int x_flip = 1;
    GetGame()->Remove_from_AtkStack(this);
    float dx = mWidth*1;
    if(GetRotation()==Math::Pi)
    {
        //dx *= -1;
        x_flip = -1;
    }
    auto mHitbox = new Hitbox(this,(int)dx*x_flip,1,mWidth,(int)mHeight,ColliderLayer::AttackHitBox);
    mHitbox->SetKnockback(1000*x_flip);
    mHitbox->DetectCollision(GetComponent<RigidBodyComponent>(),GetGame()->GetColliders());
    mHitbox->SetDestroy(true);
    mHitbox->SetEnabled(false);
    RemoveComponent(mHitbox);
    mDrawComponent->SetAnimation("run");
    mDrawComponent->SetAnimFPS(5.0f);
    mGame->GetAudio()->PlaySound("bite.wav");
}



