//
// Created by Campo on 01/12/2023.
//

#include "CrocChase.h"
#include "FSMComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Actors/Mob.h"
#include "../ColliderComponents/Hitbox.h"

CrocChase::CrocChase(class Mob* m,FSMComponent* fsm,const std::string &name, float atk_w, float atk_h, float spd, int updateOrder) : FSMState(fsm,name)
        {
            mAtkWidth=atk_w;
            mAtkHeight=atk_h;
            mSpeed=spd;
            mMob=m;
            mHitbox = nullptr;
        }



void CrocChase::Update(float deltaTime)
{

     ChangeRotation();
     if(!IsInRange())
     {
         //SDL_Log("Get in range");
         MoveToRange();
     }

}

void CrocChase::ChangeRotation()
{
    auto mOwner = mFSM->GetOwner();
    auto tg = mOwner->GetGame()->GetPlayer();
    if(tg->GetPosition().x > mOwner->GetPosition().x)
    {
        mOwner->SetRotation(0);
    }
    else if(tg->GetPosition().x < mOwner->GetPosition().x)
    {
        mOwner->SetRotation(Math::Pi);
    }
}

bool CrocChase::IsInRange()
{
    auto mOwner = mFSM->GetOwner();
    auto pl =mOwner->GetGame()->GetPlayer();
    float x_dif = pl->GetPosition().x - mOwner->GetPosition().x;
    float y_dif = pl->GetPosition().y - mOwner->GetPosition().y;

    float too_far = mAtkWidth * 2;

    if(mMob->GetDoAtk())
    {
        too_far /=2.0f;
    }


    if(Math::Abs(x_dif) >= too_far  || Math::Abs(x_dif) < mAtkWidth*9/10)
    {
        return  false;
    }



    if(Math::Abs(y_dif) >= mAtkHeight)
    {
        return  false;
    }


    return  true;

}

void CrocChase::MoveToRange()
{
    auto mOwner = mFSM->GetOwner();
    auto tg = mOwner->GetGame()->GetPlayer()->GetPosition();

    tg.x = (mOwner->GetPosition().x - tg.x);

    if(tg.x< 0)
    {
        tg.x += mAtkWidth;
    }
    else{
        tg.x -= mAtkWidth;
    }

    //float x_dif = mOwner->GetPosition().x - tg.x;



    tg.y = mOwner->GetPosition().y - tg.y ;
    //y_dif *= -1.0f;

    //Vector2 v = tg - mOwner->GetPosition();
    //mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(Vector2::Normalize(v) * mSpeed * -1);
    mOwner->Move(Vector2::Normalize(tg) * mSpeed*-1);


}

void CrocChase::HandleStateTransition(float stateTime)
{
    auto mOwner = mFSM->GetOwner();
    if(IsInRange())
    {

        if(mMob->GetDoAtk())
        {


            mMob->SetDoAtk(false);
            mMob->GetComponent<RigidBodyComponent>()->SetVelocity(Vector2::Zero);
            mMob->BeginBite();
            /*int x_flip = 1;
            mOwner->GetGame()->Remove_from_AtkStack(mMob);
            float dx = mAtkWidth*1;
            if(mOwner->GetRotation()==Math::Pi)
            {
                //dx *= -1;
                x_flip = -1;
            }
            mHitbox = new Hitbox(mMob,(int)dx*x_flip,1,(int)mAtkWidth,(int)mAtkHeight,ColliderLayer::AttackHitBox);
            mHitbox->SetKnockback(1000*x_flip);
            mHitbox->DetectCollision(mOwner->GetComponent<RigidBodyComponent>(),mOwner->GetGame()->GetColliders());
            mHitbox->SetDestroy(true);
            mHitbox->SetEnabled(false);
            mMob->RemoveComponent(mHitbox);*/



        }

    }

    if(mMob->GetDoAtk()== false && !mMob->GetIsBiting() &&IsInRange())
    {
        mFSM->SetState("Wait");
    }



}