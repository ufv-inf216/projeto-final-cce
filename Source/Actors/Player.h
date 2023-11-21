//
// Created by Campo on 20/11/2023.
//
#include "Actor.h"

#ifndef T1_CCE_PLAYER_H
#define T1_CCE_PLAYER_H

#endif //T1_CCE_PLAYER_H

class Player: public  Actor
{
    public:
        explicit  Player(Game* game,float forwardSpeed = 2500.0f);
        void OnProcessInput(const Uint8* keyState) override;
        void OnUpdate(float deltaTime) override;

    private:
        float mForwardSpeed;
        class RigidBodyComponent* mRigidBodyComponent;
        class DrawSpriteComponent*  mDrawComponent;
        class AABBColliderComponent* mColliderComponent;

};