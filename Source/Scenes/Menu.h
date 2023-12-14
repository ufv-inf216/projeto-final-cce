//
// Created by Campo on 13/12/2023.
//

#pragma once

#include "Scene.h"
#include <string>
#include <vector>
#include "Sdl.h"

class Menu : public Scene
{
    public:
        explicit  Menu(Game* game);
        void Load() override;
        void ProcessInput(const Uint8* keyState) override;
        bool ShowHud()  override;
        void Draw() override;

    private:
        void SelChoice();

        int mSel;
        std::vector<std::string> mOptions;
        class Actor* mTitleActor;
        class Actor* mSubTitleActor;
        std::vector<class Actor*> mOptActors;
        int mKeyCooldown;
        int mRectwRef;

        class Actor* mBgActor;

};
