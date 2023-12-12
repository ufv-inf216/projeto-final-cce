//
// Created by Campo on 29/11/2023.
//

#pragma once

#include "Component.h"

class StatBlock: public Component
{
    public:
        explicit StatBlock(class Actor* owner,int hp, int updateOrder = 10);
        int GetMaxHP() const {return  mMaxHp;}
        int GetHP() const {return  mHp;}

        void SetMaxHP(int i) {mMaxHp=i;mHp=i;}
        void TakeDmg(int d) {mHp-=d;}
        bool Is_dead() const { return mHp  <= 0;}

        float GetKnockbackMod() const {return  mKnockbackMod;}
        void SetKnockbackMod(float k) {mKnockbackMod=k;}

    private:
        int mMaxHp;
        int mHp;
        float mKnockbackMod;
};
