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


Mob::Mob(Game *game, float forwardSpeed): Actor(game), mForwardSpeed(forwardSpeed)
{
    mHeight = 64;
    mWidth = mHeight*2;
    mRigidBodyComponent = new RigidBodyComponent(this,1.0,10);

    mShoeCollider = new AABBColliderComponent(this,0,0,mWidth,mHeight/2,ColliderLayer::Shoe);
    mColliderComponent = new AABBColliderComponent(this,0,0,mWidth,mHeight,ColliderLayer::MobHitBox);


    mDrawComponent = new DrawAnimatedComponent(this,"../Assets/Sprites/Croc/Croc.png", "../Assets/Sprites/Croc/Croc.json",3);
    mDrawComponent->AddAnimation("run", {1,2});
    mDrawComponent->AddAnimation("bite", {1,0});

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



}

void Mob::OnUpdate(float deltaTime)
{
    auto pos = GetPosition();
    auto posCorrect = Vector2();
    posCorrect.x = pos.x; posCorrect.y = pos.y;
    if(pos.y > (float)mGame->GetWindowHeight() - ((float)mHeight/2))
    {
        SDL_Log("don't go bellow");
        //SetPosition(Vector2(pos.x,mGame->GetWindowHeight()- ((float)mHeight/2)));
        posCorrect.y = (float)mGame->GetWindowHeight()- ((float)mHeight/2);
    }

    if(pos.y < mGame->GetFloorHeight() )
    {
        //SetPosition(Vector2(pos.x,mGame->get_floor_height()));
        posCorrect.y = mGame->GetFloorHeight();
    }

//    if(pos.x < mGame->GetCameraPos().x + ((float)mWidth/2))
//    {
//        //SetPosition(Vector2(mGame->GetCameraPos().x + ((float)mWidth/2),pos.y));
//        posCorrect.x = mGame->GetCameraPos().x + ((float)mWidth/2);
//    }

//    if(pos.x > mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2))
//    {
//        //SetPosition(Vector2(mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2),pos.y));
//        posCorrect.x = mGame->GetCameraPos().x + (float)mGame->GetWindowWidth() - ((float)mWidth/2);
//    }

    SetPosition(posCorrect);

    if(GetUpdateDrawOrder())
    {
        SetUpdateDrawOrder(false);
        mDrawComponent->SetDrawOrder((int)GetPosition().y);
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

        SDL_Log("mob will die");
        SetShouldDie(true);
        mRigidBodyComponent->SetVelocity(Vector2(0,-1000));
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
