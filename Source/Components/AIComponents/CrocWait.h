//
// Created by Campo on 01/12/2023.
//

#ifndef T1_CCE_CROCWAIT_H
#define T1_CCE_CROCWAIT_H

#endif //T1_CCE_CROCWAIT_H

#include "FSMComponent.h"
#include <string>

class CrocWait : public FSMState
{
    public:
        explicit CrocWait(class Mob* m,FSMComponent* fsm,const std::string &name, float spd, int updateOrder = 50);
        void HandleStateTransition(float stateTime) override;
        void Shuffle();
        void Start() override;
        void Update(float deltaTime) override;

    private:
        float mSpeed;
        int mDir;
        class Mob* mMob;
};
