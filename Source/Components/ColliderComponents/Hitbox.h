//
// Created by Campo on 24/11/2023.
//

#pragma once

#include "AABBColliderComponent.h"


class Hitbox : public AABBColliderComponent
{
    public:
       explicit Hitbox(class Actor* owner, int dx, int dy, int w, int h,ColliderLayer layer, int updateOrder = 20,
               int dmgx=1,float knockx=400);

       ~Hitbox();
       void DetectCollision(RigidBodyComponent *rigidBody, std::vector<class AABBColliderComponent*>& colliders) override;



    int GetDmg()const {return  mDmg;}
    void SetDmg(int d){mDmg = d;}


    int GetKnockback()const {return  mKnockback;}
    void SetKnockback(int k){mKnockback = k;}


    [[nodiscard]] Vector2 GetKnockDir()const {return  mKnockDir;}
    void SetKnockDir(Vector2 v){mKnockDir = v;}

    private:

       Overlap GetMinOverlap(AABBColliderComponent* b) const;

       int mDmg;
       float mKnockback;
       Vector2 mKnockDir;
};
