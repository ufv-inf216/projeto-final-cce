//
// Created by Campo on 29/11/2023.
//

#include "StatBlock.h"


StatBlock::StatBlock(class Actor* owner,int hp, int updateOrder) : Component(owner,updateOrder)

{
         mMaxHp = mHp = hp;
         mKnockbackMod = 1.0f;
}