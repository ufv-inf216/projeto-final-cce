//
// Created by Campo on 13/12/2023.
//

#pragma once

#include "Scene.h"
#include <string>
#include <vector>
#include "Sdl.h"

class Level1: public Scene
{
    public:
        explicit Level1(Game* game);
        void Load() override;
        void Unload() override;

        bool ShowHud()  override;
        int GetNextScene() override;
        std::string GetSceneTitle()  override;
};