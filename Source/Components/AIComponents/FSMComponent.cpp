//
// Created by Lucas N. Ferreira on 26/10/23.
//

#include "FSMComponent.h"
#include "FSMState.h"
#include <iostream>

FSMComponent::FSMComponent(class Actor *owner, int updateOrder)
    : Component(owner, updateOrder)
    ,mStateTime(.0f)
    ,mIsRunning(false)
    ,mCurrentState("start")
{
    // Create a default start state
    new FSMState(this, "start");
}

FSMComponent::~FSMComponent()
{
    for(const auto& state : mStates)
    {
        delete state.second;
    }

    mStates.clear();
}

void FSMComponent::Start(const std::string &startState)
{
    // TODO [Parte 2.1]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    mIsRunning=true;
    SetState(startState);
}

void FSMComponent::SetState(const std::string &stateName)
{
    if(mStates.count(stateName)<=0)
    {
        std::cout << "state rejected: " << stateName << std::endl;
        return;
    }
    //if(stateName == "scatter"){std::cout << "scatter " << std::endl;}
    // TODO [Parte 2.2]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    mStates[mCurrentState]->Exit();
    mStateTime=0;
    mCurrentState = stateName;
    mStates[mCurrentState]->Start();
}

void FSMComponent::Update(float deltaTime)
{
    // TODO [Parte 2.3]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    if(!mIsRunning){ return;}
    mStateTime += deltaTime;
    mStates[mCurrentState]->Update(deltaTime);
    mStates[mCurrentState]->HandleStateTransition(mStateTime);

}

void FSMComponent::AddState(std::string &stateName, FSMState *state)
{
    // TODO [Parte 2.4]: https://ufv-inf216.lucasnferreira.com/p5-pacman
    mStates[stateName]=state;
}