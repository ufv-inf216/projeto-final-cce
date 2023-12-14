//
// Created by Lucas N. Ferreira on 07/12/23.
//

#pragma once

#include <SDL_stdinc.h>
#include "../Math.h"

class Scene
{
public:
    Scene(class Game* game);

    virtual void Load();
    virtual void ProcessInput(const Uint8* keyState);
    virtual void Draw();

    class Game* GetGame() { return mGame; }

    virtual const Vector2& GetCameraPos();
    virtual bool ShowHud(){return  true;}
    virtual void Unload();
    virtual int GetNextScene() {return  0;}

protected:
    class Game* mGame;

};
