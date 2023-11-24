//
// Created by Campo on 24/11/2023.
//

#ifndef T1_CCE_HITBOX_H
#define T1_CCE_HITBOX_H

#endif //T1_CCE_HITBOX_H

#include "AABBColliderComponent.h"


class Hitbox : public AABBColliderComponent
{
    public:
       explicit Hitbox(class Actor* owner, int dx, int dy, int w, int h,ColliderLayer layer, int updateOrder = 20,
               int dmgx=1,int knockx=1);

       ~Hitbox();
       void DetectCollision(RigidBodyComponent *rigidBody, std::vector<class AABBColliderComponent*>& colliders) override;



    int GetDmg()const {return  dmg;}
    void SetDmg(int d){dmg=d;}


    int GetKnockback()const {return  knockback;}
    void SetKnockback(int k){knockback=k;}


    [[nodiscard]] Vector2 GetKnockDir()const {return  knock_dir;}
    void SetKnockDir(Vector2 v){knock_dir=v;}

    private:

       Overlap GetMinOverlap(AABBColliderComponent* b) const;

       int dmg;
       int knockback;
       Vector2 knock_dir;
};
