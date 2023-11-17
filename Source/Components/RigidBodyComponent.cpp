//
// Created by Lucas N. Ferreira on 08/09/23.
//

#include <SDL.h>
#include "../Actors/Actor.h"
#include "../Game.h"
#include "RigidBodyComponent.h"
#include "ColliderComponents/AABBColliderComponent.h"

const float MAX_SPEED_X = 300.0f;
const float MAX_SPEED_Y = 700.0f;
const float GRAVITY = 2000.0f;

RigidBodyComponent::RigidBodyComponent(class Actor* owner, float mass, float friction, int updateOrder)
        :Component(owner, updateOrder)
        ,mMass(mass)
        ,mFrictionCoefficient(friction)
        ,mVelocity(Vector2::Zero)
        ,mAcceleration(Vector2::Zero)
{

}

void RigidBodyComponent::ApplyForce(const Vector2 &force) {
    mAcceleration += force * (1.f/mMass);
}

void RigidBodyComponent::Update(float deltaTime)
{
    // Euler Integration
     mVelocity += mAcceleration * deltaTime;

     mVelocity.x = Math::Clamp<float>(mVelocity.x, -MAX_SPEED_X, MAX_SPEED_X);
     mVelocity.y = Math::Clamp<float>(mVelocity.y, -MAX_SPEED_Y, MAX_SPEED_Y);
     if(Math::NearZero(mVelocity.x, 0.5f)) {
         mVelocity.x = 0.f;
     }

    Vector2 position = mOwner->GetPosition();
    position += mVelocity * deltaTime;
    ScreenWrap(position);
    mOwner->SetPosition(position);

    SetAcceleration(Vector2::Zero);
}

void RigidBodyComponent::ScreenWrap(Vector2 &position)
{
    if (position.x < 0.0f) {
        position.x = mOwner->GetGame()->GetWindowWidth();
    }
    else if (position.x > mOwner->GetGame()->GetWindowWidth()) {
        position.x = 0.0f;
    }
}
