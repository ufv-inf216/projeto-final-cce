//
// Created by Campo on 20/11/2023.
//

#include "Player.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"


Player::Player(Game *game, float forwardSpeed): Actor(game), mForwardSpeed(forwardSpeed)
{
      mRigidBodyComponent= new RigidBodyComponent(this,1.0,10);

      mColliderComponent= new AABBColliderComponent(this,0,0,256,256,ColliderLayer::Player);


      mDrawComponent= new DrawSpriteComponent(this,"../Assets/placeholder.png",256,256,1000);
}


void Player::OnProcessInput(const Uint8 *keyState)
{


    if(keyState[SDL_SCANCODE_D])
    {
        mRigidBodyComponent->ApplyForce(Vector2(mForwardSpeed,0));
        SetRotation(0);

    }

    
    if(keyState[SDL_SCANCODE_A])
    {
        mRigidBodyComponent->ApplyForce(Vector2(-1 * mForwardSpeed,0));
        SetRotation(Math::Pi);

    }

    if(keyState[SDL_SCANCODE_W])
    {
        mRigidBodyComponent->ApplyForce(Vector2(0,-1 * mForwardSpeed));
        mGame->set_resort(true);

    }

    if(keyState[SDL_SCANCODE_S])
    {
        mRigidBodyComponent->ApplyForce(Vector2(0,mForwardSpeed));
        mGame->set_resort(true);

    }


}

void Player::OnUpdate(float deltaTime)
{
    //mRigidBodyComponent->SetVelocity(Vector2::Zero);
}