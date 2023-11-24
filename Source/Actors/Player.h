//
// Created by Campo on 20/11/2023.
//
#include "Actor.h"
#include <string>


#ifndef T1_CCE_PLAYER_H
#define T1_CCE_PLAYER_H

#endif //T1_CCE_PLAYER_H

class Player: public  Actor
{
    public:
        explicit  Player(Game* game,float forwardSpeed = 2500.0f);
        void OnProcessInput(const Uint8* keyState) override;
        void OnUpdate(float deltaTime) override;
        void OnCollision(std::vector<AABBColliderComponent::Overlap>& responses) override;
        virtual void take_damage(int d);
        std::string GetName() override;



    private:
        float mForwardSpeed;
        int mWidth;
        int mHeight;
        class Hitbox* mPunch;
        class RigidBodyComponent* mRigidBodyComponent;
        class DrawSpriteComponent*  mDrawComponent;
        class AABBColliderComponent* mColliderComponent;
        class AABBColliderComponent* mShoeCollider;

};