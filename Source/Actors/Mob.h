//
// Created by Campo on 24/11/2023.
//

#pragma once

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

        bool GetColHappened() const {return mColHappened;}
        void SetColHappened(bool b) {mColHappened=b;}

        bool GetIsBiting() const {return mIsBiting;}

        void BeginBite();

        void OnCollision(std::vector<AABBColliderComponent::Overlap>& responses) override;







   private:
        float mForwardSpeed;
        int mWidth;
        int mHeight;
        int mAtkRange;
        bool mDoAtk;
        void ManageAnimations();
        float mBiteCooldown;
        bool  mIsBiting;
        bool mColHappened;

        void DoBite();




        class RigidBodyComponent* mRigidBodyComponent;
        class DrawAnimatedComponent*  mDrawComponent;

        class AABBColliderComponent* mColliderComponent;
        class AABBColliderComponent* mShoeCollider;

        class StatBlock* mStatBlock;

        class FSMComponent* mCrocAi;


        class DrawPolygonComponent* mDrawPolygonComponent;

};