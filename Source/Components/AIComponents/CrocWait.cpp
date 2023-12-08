//
// Created by Campo on 01/12/2023.
//

#include "FSMComponent.h"
#include <string>
#include <iostream>
#include "CrocWait.h"
#include "../../Random.h"
#include "../../Game.h"
#include "../../Actors/Mob.h"


CrocWait::CrocWait(Mob* m,FSMComponent *fsm, const std::string &name, float spd, int updateOrder) : FSMState(fsm,name)
{
       //mDir=0;
       mSpeed=spd;
       mMob=m;
}

void CrocWait::Start()
{
    auto mOwner = mFSM->GetOwner();
    mDist = mOwner->GetGame()->GetPlayer()->GetPosition() - mOwner->GetPosition();
    mDist = Vector2::Normalize(mDist);

    mDir = Random::GetIntRange(1,3);
    //std::cout << mDir << std::endl;

}


void CrocWait::HandleStateTransition(float stateTime)
{
    //mFSM->GetOwner()->GetComponent<RigidBodyComponent>()->SetVelocity(Vector2::Zero);
    if(stateTime>=2.0f)
    {
        mFSM->SetState("Chase");
    }
}

void CrocWait::Update(float deltaTime)
{
    auto mOwner = mFSM->GetOwner();
    CrocWait::Shuffle();
    if((int)deltaTime%3==0 && !mMob->GetDoAtk())
    {
        int tg = 10;
        auto roll = Random::GetIntRange(1,100);

        if(mOwner->GetGame()->GetPlayer()->GetRotation() == mOwner->GetRotation())
        {
            tg -= 5;
        }

        if(roll <= tg)
        {
            //mOwner->PrintNameWithID();
            //SDL_Log("wants to atk");
            mFSM->GetOwner()->GetGame()->Add_to_AtkStack(mMob);
        }
    }






}

void CrocWait::Shuffle()
{

    auto mOwner = mFSM->GetOwner();
    mDist = mOwner->GetGame()->GetPlayer()->GetPosition() - mOwner->GetPosition();
    mDist = Vector2::Normalize(mDist);

    auto spd = mSpeed/1.0f;
    auto apl =  spd* mDist * -1;

    

    switch (mDir)
    {
        case 1:
        {
            //mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(Vector2(0,-1*mSpeed));
            //mOwner->Move(Vector2(0,-1*mSpeed/8));
            mOwner->Move(Vector2(apl.x,0));
        }

        case 2:
        {
            //mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(Vector2(0,mSpeed));
            //mOwner->Move(Vector2(0,mSpeed/8));
            mOwner->Move(Vector2(0,apl.y*-1));
        }

        case 3:
        {
            //mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(Vector2(0,mSpeed));
            //mOwner->Move(Vector2(0,mSpeed/8));
            mOwner->Move(Vector2(apl.x,apl.y*-1));
        }
    }





}
