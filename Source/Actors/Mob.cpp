//
// Created by Campo on 24/11/2023.
//
#include "Mob.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/StatBlock.h"


Mob::Mob(Game *game, float forwardSpeed): Actor(game), mForwardSpeed(forwardSpeed)
{
    mHeight = 64;
    mWidth = mHeight*2;
    mRigidBodyComponent = new RigidBodyComponent(this,1.0,10);

    mColliderComponent = new AABBColliderComponent(this,0,0,mWidth,mHeight,ColliderLayer::MobHitBox);
    mShoeCollider = new AABBColliderComponent(this,0,0,mWidth,mHeight/2,ColliderLayer::Shoe);

    mDrawComponent = new DrawSpriteComponent(this,"../Assets/placeholder3.png",mWidth,mHeight,1000);
    SetUpdateDrawOrder(true);
    mStatBlock = new StatBlock(this,1);

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
        SetShouldDie(true);
    }
}
