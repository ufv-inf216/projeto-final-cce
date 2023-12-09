//
// Created by Campo on 01/12/2023.
//

#pragma once

#include "FSMComponent.h"
#include <string>
#include "../../Math.h"

class CrocWait : public FSMState
{
    public:
        explicit CrocWait(class Mob* m,FSMComponent* fsm,const std::string &name, float spd, int updateOrder = 50);
        void HandleStateTransition(float stateTime) override;
        void Shuffle();
        void Start() override;
        void Update(float deltaTime) override;
        void Update_Move_dir();

    private:
        float mSpeed;
        int mDir;
        class Mob* mMob;
        Vector2 mDist;
        Vector2 mMoveDir;
};
