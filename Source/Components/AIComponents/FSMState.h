//
// Created by Lucas N. Ferreira on 26/10/23.
//

#pragma once

#include <string>

class FSMState
{
public:
    FSMState(class FSMComponent *fsm, const std::string &name);

    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void Exit();

    virtual void HandleStateTransition(float stateTime);

    const std::string &GetName() { return mName; }

protected:
    class FSMComponent *mFSM;
    std::string mName;
};
