//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once
#include "../Component.h"
#include "../../Math.h"
#include "../RigidBodyComponent.h"
#include <vector>
#include <set>
#include "SDL.h"


enum class ColliderLayer
{
    Player,
    Enemy,
    Wall,
    Item,
    Node,
    Shoe,
    AttackHitBox,
    MobHitBox,

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

    bool GetStopJump() const {return mStopJump;}
    void SetStopJump(bool b) {mStopJump=b;}

    Vector2 GetMin() const;
    Vector2 GetMax() const;
    Vector2 GetCenter() const;
    Vector2 GetOffset() const { return mOffset; }
    ColliderLayer GetLayer() const { return mLayer; }
    SDL_FRect AABBtoRect();



private:

    Overlap GetMinOverlap(AABBColliderComponent* b) const;

    Vector2 mOffset;
    int mWidth;
    int mHeight;
    bool mStopJump;

    ColliderLayer mLayer;
};