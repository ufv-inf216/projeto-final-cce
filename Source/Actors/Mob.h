//
// Created by Campo on 24/11/2023.
//

#ifndef T1_CCE_MOB_H
#define T1_CCE_MOB_H

#endif //T1_CCE_MOB_H

#include "Actor.h"


class Mob: public Actor
{
    public:
        explicit  Mob(Game* game,float forwardSpeed = 2500.0f);
        void OnUpdate(float deltaTime) override;
        virtual void TakeDamage(int d) override;
        std::string GetName() override;
        void Move(Vector2 mv) override;

        float GetFowardSpeed() const {return  mForwardSpeed;}

        bool GetDoAtk() const {return mDoAtk;}
        void SetDoAtk(bool b) {mDoAtk=b;}




   private:
        float mForwardSpeed;
        int mWidth;
        int mHeight;
        int mAtkRange;
        bool mDoAtk;

        class RigidBodyComponent* mRigidBodyComponent;
        class DrawAnimatedComponent*  mDrawComponent;

        class AABBColliderComponent* mColliderComponent;
        class AABBColliderComponent* mShoeCollider;

        class StatBlock* mStatBlock;

        class FSMComponent* mCrocAi;

        class DrawPolygonComponent* mDrawPolygonComponent;

};