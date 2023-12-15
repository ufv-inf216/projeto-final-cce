//
// Created by Campo on 01/12/2023.
//

#pragma once

#include "FSMComponent.h"
#include <string>
#include "../../Actors/Actor.h"


class CrocChase : public FSMState
{

    public:
        explicit CrocChase(class Mob* m,FSMComponent* fsm,const std::string &name, float atk_w, float atk_h, float spd, int updateOrder = 50);
        void Update(float deltaTime) override;
        void Start() override;
        bool IsInRange();
        void MoveToRange();
        void ChangeRotation();
        void HandleStateTransition(float stateTime) override;

        //bool GetDoAtk() const {return  mDoAtk;}
        //void setDoAtk(bool b){mDoAtk=b;}



    private:
        //bool mDoAtk;
        float mAtkWidth;
        float mAtkHeight;
        float mSpeed;

        class Mob* mMob;
        class Hitbox* mHitbox;
        float mAtkWait;





};