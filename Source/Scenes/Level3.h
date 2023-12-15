//
// Created by Campo on 14/12/2023.
//

#ifndef T1_CCE_LEVEL3_H
#define T1_CCE_LEVEL3_H

#endif //T1_CCE_LEVEL3_H

#include "Scene.h"
#include <string>
#include <vector>
#include "Sdl.h"

class Level3: public Scene
{
public:
    explicit Level3(Game* game);
    void Load() override;
    void Unload() override;

    bool ShowHud()  override;
    int GetNextScene() override;
    std::string GetSceneTitle()  override;
};
