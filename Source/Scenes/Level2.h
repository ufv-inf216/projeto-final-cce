//
// Created by Campo on 14/12/2023.
//

#pragma once

#include "Scene.h"
#include <string>
#include <vector>
#include "Sdl.h"

class Level2: public Scene
{
public:
    explicit Level2(Game* game);
    void Load() override;
    void Unload() override;

    bool ShowHud()  override;
    int GetNextScene() override;
    std::string GetSceneTitle()  override;
};