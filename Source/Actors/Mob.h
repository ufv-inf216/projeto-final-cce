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
        virtual void TakeDamage(int d);
        std::string GetName() override;



   private:
        float mForwardSpeed;
        int mWidth;
        int mHeight;

        class RigidBodyComponent* mRigidBodyComponent;
        class DrawSpriteComponent*  mDrawComponent;

        class AABBColliderComponent* mColliderComponent;
        class AABBColliderComponent* mShoeCollider;

        class StatBlock* mStatBlock;

        class DrawPolygonComponent* mDrawPolygonComponent;

};