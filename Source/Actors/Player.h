#pragma once

#include "Actor.h"
#include <string>

class Player: public  Actor
{
    public:
        explicit  Player(Game* game,float forwardSpeed = 2500.0f, int lives = 3);
        void OnProcessInput(const Uint8* keyState) override;
        void OnUpdate(float deltaTime) override;
        void OnCollision(std::vector<AABBColliderComponent::Overlap>& responses) override;
        virtual void TakeDamage(int d);
        std::string GetName() override;



    private:

        void ProcessMov();
        void ManageAnimations();
        const int Punch_cooldown =20;

        float mForwardSpeed;
        int mWidth;
        int mHeight;

        float mPunchCooldown;
        bool mIsAttacking;

        bool mIsRunning;
        //bool mIs_jumping;
        int mLives;

        float mOgY;
        float mJumpSpeed;

        unsigned walk_sound_counter;
  
        class Hitbox* mPunch;

        class RigidBodyComponent* mRigidBodyComponent;
        class DrawAnimatedComponent*  mDrawComponent;

        class AABBColliderComponent* mColliderComponent;
        class AABBColliderComponent* mShoeCollider;

        class StatBlock* mStatBlock;

        class DrawPolygonComponent* mDrawPolygonComponent;

};
