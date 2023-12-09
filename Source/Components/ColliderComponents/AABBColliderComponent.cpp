//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AABBColliderComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>

AABBColliderComponent::AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h, ColliderLayer layer, int updateOrder)
        :Component(owner, updateOrder)
        ,mOffset(Vector2((float)dx, (float)dy))
        ,mWidth(w)
        ,mHeight(h)
        ,mLayer(layer)
{
        mOwner->GetGame()->AddCollider(this);
        SetStopJump(false);
        SetDestroy(false);
}

AABBColliderComponent::~AABBColliderComponent()
{
    //mOwner->GetGame()->RemoveCollider(this);
}


SDL_FRect AABBColliderComponent::AABBtoRect()
{
    SDL_FRect  outx = SDL_FRect();
    outx.x = GetMin().x;
    outx.y = GetMin().y;
    outx.w = GetMax().x - outx.x;
    outx.h = GetMax().y - outx.y;
    return  outx;

}
Vector2 AABBColliderComponent::GetMin() const
{
    return mOwner->GetPosition() - Vector2(mWidth/2.0f, mHeight/2.0f) + mOffset;
}

Vector2 AABBColliderComponent::GetMax() const
{
    return GetMin() + Vector2((float)mWidth, (float)mHeight);
}

Vector2 AABBColliderComponent::GetCenter() const
{
    return GetMin() + Vector2((float)mWidth/2.0f, (float)mHeight/2.0f);
}

bool AABBColliderComponent::Intersect(const AABBColliderComponent& b) const
{
    Vector2 aMin = GetMin(); Vector2 aMax = GetMax();
    Vector2 bMin = b.GetMin(); Vector2 bMax = b.GetMax();

    bool no = aMax.x < bMin.x || aMax.y < bMin.y ||
              bMax.x < aMin.x || bMax.y < aMin.y;

    // If none of these are true, they must intersect
    return !no;
}

AABBColliderComponent::Overlap AABBColliderComponent::GetMinOverlap(AABBColliderComponent* b) const
{
    Vector2 aMin = GetMin(); Vector2 aMax = GetMax();
    Vector2 bMin = b->GetMin(); Vector2 bMax = b->GetMax();

    std::unordered_map<int, Overlap> overlaps;
    overlaps[0] = {Vector2::UnitX * (bMax.x - aMin.x), CollisionSide::Left, b};
    overlaps[1] = {Vector2::UnitX * (bMin.x - aMax.x), CollisionSide::Right,b};
    overlaps[2] = {Vector2::UnitY * (bMax.y - aMin.y), CollisionSide::Top,  b};
    overlaps[3] = {Vector2::UnitY * (bMin.y - aMax.y), CollisionSide::Down, b};

    Overlap minOverlap = overlaps[0];
    for(int i = 1; i < overlaps.size(); i++) {
        if(overlaps[i].distance.LengthSq() < minOverlap.distance.LengthSq()) {
            minOverlap = overlaps[i];
        }
    }

    return minOverlap;
}

void AABBColliderComponent::ResolveCollisions(RigidBodyComponent *rigidBody, const Overlap& minOverlap)
{
    Vector2 velocity = rigidBody->GetVelocity();
    Vector2 position = mOwner->GetPosition();

    if ((velocity.y > .0f && minOverlap.side == CollisionSide::Down) ||
        (velocity.y < .0f && minOverlap.side == CollisionSide::Top)) {
        position += minOverlap.distance;
        velocity.y = .0f;
    }
    else if ((velocity.x > .0f && minOverlap.side == CollisionSide::Right) ||
             (velocity.x < .0f && minOverlap.side == CollisionSide::Left)) {
        position += minOverlap.distance;
        velocity.x = .0f;
    }

    mOwner->SetPosition(position);
    rigidBody->SetVelocity(velocity);
}

void AABBColliderComponent::DetectCollision(RigidBodyComponent *rigidBody, std::vector<class AABBColliderComponent*>& colliders)
{

    if(GetLayer()!=ColliderLayer::Shoe)
    {
        //SDL_Log("Not shoe");
        return;
    }


    // Sort colliders by distance to the player (center-to-center)
    // auto colliders = mOwner->GetGame()->GetColliders();
    std::sort(colliders.begin(), colliders.end(),[this](AABBColliderComponent* a, AABBColliderComponent* b) {
        return (a->GetCenter() - this->GetCenter()).LengthSq() <
               (b->GetCenter() - this->GetCenter()).LengthSq();
    });

    // Create a map to store collision responses
    std::vector<Overlap> responses;

    // Check collision against each target collider
    for(auto target : colliders)
    {
        if(target == this || !target->IsEnabled() || target->GetOwner() == this->GetOwner())
            continue;

        if(Intersect(*target))
        {
            bool jump_check = target->GetOwner()->GetIsJumping() <= GetStopJump() && mOwner->GetIsJumping() <=  target->GetStopJump();
            //std::cout << ((target->GetOwner()->GetIsJumping())? "true":"false") << std::endl;
            Overlap minOverlap = GetMinOverlap(target);
            bool same_type = target->GetOwner()->GetName() == mOwner->GetName();
            if (jump_check&&target->GetLayer() == ColliderLayer::Shoe) {
                ResolveCollisions(rigidBody, minOverlap);
            }

            responses.emplace_back(minOverlap);
        }
    }

    // Callback only for closest (first) collision
    mOwner->OnCollision(responses);
}