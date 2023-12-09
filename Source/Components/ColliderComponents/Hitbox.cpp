//
// Created by Campo on 24/11/2023.
//


#include "Hitbox.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Components/DrawComponents/DrawPolygonComponent.h"
#include "../StatBlock.h"

Hitbox::Hitbox(class Actor *owner, int dx, int dy, int w, int h, ColliderLayer layer, int updateOrder,int dmgx,float knockx) :
        AABBColliderComponent(owner,dx,dy,w,h,layer,updateOrder)
        ,mDmg(dmgx)
        ,mKnockback(knockx)
        {
            //mKnockDir = Vector2::Zero;
            mKnockDir = Vector2(1,0);
            SetDestroy(false);
        }

Hitbox::~Hitbox()
{


    /*mOwner->GetGame()->RemoveCollider(this);
    mOwner->RemoveComponent(this);*/
}

AABBColliderComponent::Overlap Hitbox::GetMinOverlap(AABBColliderComponent* b) const
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

void Hitbox::DetectCollision(RigidBodyComponent *rigidBody, std::vector<class AABBColliderComponent*>& colliders)
{


    //std::cout << GetOffset().x << " , " << GetOffset().y << std::endl;
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

            //Se o alvo está pulando,devemos checar se a hitbox pode antingir-lo


            bool jump_check = target->GetOwner()->GetIsJumping() <= GetStopJump();
            Overlap minOverlap = GetMinOverlap(target);
            bool same_type = target->GetOwner()->GetName() == mOwner->GetName();

            if (jump_check&&!same_type&&target->GetLayer() == ColliderLayer::MobHitBox) {
                //ResolveCollisions(rigidBody, minOverlap);
                //target->GetOwner()->SetState(ActorState::Destroy);
                target->GetOwner()->TakeDamage(mDmg);
                float mod = mKnockback*target->GetOwner()->GetComponent<StatBlock>()->GetKnockbackMod();
                target->GetOwner()->GetComponent<RigidBodyComponent>()->SetFrictionCoefficient(1);
                target->GetOwner()->GetComponent<RigidBodyComponent>()->SetVelocity(Vector2(mKnockDir*mod));
                //target->GetOwner()->SetSentBack(true);

            }

            responses.emplace_back(minOverlap);
        }
    }

    // Callback only for closest (first) collision
    mOwner->OnCollision(responses);
}