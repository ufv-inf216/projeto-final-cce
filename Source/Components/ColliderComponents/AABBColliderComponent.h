//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once
#include "../Component.h"
#include "../../Math.h"
#include "../RigidBodyComponent.h"
#include <vector>
#include <set>


enum class ColliderLayer
{
    Player,
    Enemy,
    Wall,
    Item,
    Node,
    Shoe,
    attack_hitbox,
    mob_hitbox,

};

enum class CollisionSide
{
    Top,
    Down,
    Left,
    Right
};

class AABBColliderComponent : public Component
{
public:
    struct Overlap
    {
        Vector2 distance;
        CollisionSide side;
        AABBColliderComponent *target;
    };

    AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h,
                                ColliderLayer layer, int updateOrder = 20);

    ~AABBColliderComponent();

    bool Intersect(const AABBColliderComponent& b) const;
    virtual void DetectCollision(RigidBodyComponent *rigidBody, std::vector<class AABBColliderComponent*>& colliders);
    void ResolveCollisions(RigidBodyComponent *rigidBody, const Overlap& minOverlap);

    Vector2 GetMin() const;
    Vector2 GetMax() const;
    Vector2 GetCenter() const;
    Vector2 GetOffset() const { return mOffset; }
    ColliderLayer GetLayer() const { return mLayer; }

private:

    Overlap GetMinOverlap(AABBColliderComponent* b) const;

    Vector2 mOffset;
    int mWidth;
    int mHeight;

    ColliderLayer mLayer;
};