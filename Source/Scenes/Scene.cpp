//
// Created by Lucas N. Ferreira on 07/12/23.
//

#include "Scene.h"

Scene::Scene(Game* game)
{
    mGame = game;
}

void Scene::Load()
{
    // Load the scene
}

const Vector2& Scene::GetCameraPos()
{
    return Vector2::Zero;
}

void Scene::ProcessInput(const Uint8* keyState)
{
    // Process scene input
}