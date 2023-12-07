//
// Created by Campo on 01/12/2023.
//

#include "FSMComponent.h"
#include <string>
#include "CrocWait.h"
#include "../../Random.h"
#include "../../Game.h"


CrocWait::CrocWait(Mob* m,FSMComponent *fsm, const std::string &name, float spd, int updateOrder) : FSMState(fsm,name)
{
       //mDir=0;
       mSpeed=spd;
       mMob=m;
}

void CrocWait::Start()
{
    auto mOwner = mFSM->GetOwner();
    if(mOwner->GetPosition().y < (float)mOwner->GetGame()->GetWindowHeight()/2)
    {
        mDir=1;
    }
    else{
        mDir=2;
    }
}


void CrocWait::HandleStateTransition(float stateTime)
{
    //mFSM->GetOwner()->GetComponent<RigidBodyComponent>()->SetVelocity(Vector2::Zero);
    if(stateTime>=3.0f)
    {
        mFSM->SetState("Chase");
    }
}

void CrocWait::Update(float deltaTime)
{
    auto mOwner = mFSM->GetOwner();
    CrocWait::Shuffle();
    int tg = 10;
    auto roll = Random::GetIntRange(1,100);

    if(mOwner->GetGame()->GetPlayer()->GetRotation() == mOwner->GetRotation())
    {
        tg -= 5;
    }

    if(roll <= tg)
    {
        mFSM->GetOwner()->GetGame()->Add_to_AtkStack(mMob);
    }






}

void CrocWait::Shuffle()
{
    auto mOwner = mFSM->GetOwner();

    switch (mDir)
    {
        case 1:
        {
            //mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(Vector2(0,-1*mSpeed));
            mOwner->Move(Vector2(0,-1*mSpeed/8));
        }

        case 2:
        {
            //mOwner->GetComponent<RigidBodyComponent>()->ApplyForce(Vector2(0,mSpeed));
            mOwner->Move(Vector2(0,mSpeed/8));
        }
    }





}
