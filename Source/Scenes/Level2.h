//
// Created by Campo on 14/12/2023.
//

#ifndef T1_CCE_LEVEL2_H
#define T1_CCE_LEVEL2_H

#endif //T1_CCE_LEVEL2_H

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
};