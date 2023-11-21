//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Component.h"
#include "../Math.h"

class RigidBodyComponent : public Component
{
public:
    // Lower update order to update first
    RigidBodyComponent(class Actor* owner, float mass = 1.0f, float friction = 0.0f, int updateOrder = 10);

    void Update(float deltaTime) override;

    const Vector2& GetVelocity() const { return mVelocity; }
    void SetVelocity(const Vector2& velocity) { mVelocity = velocity; }

    const Vector2& GetAcceleration() const { return mAcceleration; }
    void SetAcceleration(const Vector2& acceleration) { mAcceleration = acceleration; }

    bool Get_is_mobile() const { return is_mobile; }
    void Set_is_mobile(bool b)  {is_mobile=b;}

    void ApplyForce(const Vector2 &force);

private:
    void ScreenWrap(Vector2 &position);

    // Physical properties
    float mFrictionCoefficient;
    float mMass;
    float m{};

    // Deve ser verdade se o actor consegue se mover verticalmente
    bool is_mobile;


private:

    Vector2 mVelocity;
    Vector2 mAcceleration;
};
