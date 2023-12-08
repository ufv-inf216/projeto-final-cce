//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Spawner.h"
#include "../Game.h"
#include "Mob.h"
#include "../Random.h"



Spawner::Spawner(Game* game, float spawnDistance)
        :Actor(game)
        ,mSpawnDistance(spawnDistance)
{

}

void Spawner::OnUpdate(float deltaTime)
{
    //SDL_Log("Spawner update");

    auto  v =GetGame()->GetPlayer()->GetPosition() - GetPosition();
    if(abs(v.x) < mSpawnDistance )
    {
        auto g = new Mob(GetGame());
        auto og_pos = GetPosition();

        if(Random::GetIntRange(1,2)==2)
        {
            og_pos.x =  og_pos.x - 640.f - 128;
        }


        g->SetPosition(og_pos);
        //g->GetComponent<RigidBodyComponent>()->SetVelocity(Vector2(-1 * GOOMBA_FORWARD_SPEED,0));

        mState = ActorState::Destroy;
    }
}