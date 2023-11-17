//
// Created by Lucas N. Ferreira on 26/10/23.
//

#include "FSMState.h"
#include "FSMComponent.h"

FSMState::FSMState(FSMComponent *fsm, const std::string &name)
        :mFSM(fsm)
        ,mName(name)
{
    mFSM->AddState(mName, this);
}

void FSMState::Start()
{

}

void FSMState::Update(float deltaTime)
{

}

void FSMState::Exit()
{

}

void FSMState::HandleStateTransition(float stateTime)
{

}