//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"

class Pacman : public Actor
{
public:
    explicit Pacman(Game* game, float forwardSpeed = 100.0f);

    void OnProcessInput(const Uint8* keyState) override;
    void OnUpdate(float deltaTime) override;

    void DetectCollision();
    void OnCollision(std::vector<AABBColliderComponent::Overlap>& responses) override;

    class PathNode* GetSpawnNode() const { return mSpawnNode; }
    void SetSpawnNode(class PathNode* node) { mSpawnNode = node; }

    class PathNode* GetPrevNode() const;
    Vector2 GetPointInFrontOf(float dist) const;

    void Start();

    void Die();
    bool IsDead() { return mIsDead; }

private:
    void UpdateAnimations();

    bool mIsDead;
    float mForwardSpeed;

    class PathNode* mSpawnNode;
    class PathNode* mPrevNode;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;
};