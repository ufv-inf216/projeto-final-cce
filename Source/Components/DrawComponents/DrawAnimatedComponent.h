//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include "DrawSpriteComponent.h"
#include <unordered_map>

class DrawAnimatedComponent : public DrawSpriteComponent {
public:
    // (Lower draw order corresponds with further back)
    DrawAnimatedComponent(class Actor* owner, const std::string &spriteSheetPath, const std::string &spriteSheetData, int resize=1, int drawOrder = 100,bool do_flip=false);
    ~DrawAnimatedComponent() override;

    void Draw(SDL_Renderer* renderer) override;
    void Update(float deltaTime) override;

    // Use to change the FPS of the animation
    void SetAnimFPS(float fps) { mAnimFPS = fps; }

    // Set the current active animation
    void SetAnimation(const std::string& name, bool loop = true, bool priority = false);

    std::vector <SDL_Rect*>& GetSpriteSheetData() { return mSpriteSheetData; }

    void ResetAnim() { mAnimTimer = 0.0f; }

    // Use to pause/unpause the animation
    void SetIsPaused(bool pause) { mIsPaused = pause; }
    bool IsPaused() const { return mIsPaused; }

    // Add an animation of the corresponding name to the animation map
    void AddAnimation(const std::string& name, const std::vector<int>& images);

private:
    void LoadSpriteSheet(const std::string& texturePath, const std::string& dataPath);

    // Vector of sprites
    std::vector<SDL_Rect*> mSpriteSheetData;

    // Map of animation name to vector of textures corresponding to the animation
    std::unordered_map<std::string, std::vector<int>> mAnimations;

    // Name of current animation
    std::string mAnimName;

    // Tracks current elapsed time in animation
    float mAnimTimer = 0.0f;

    // The frames per second the animation should run at
    float mAnimFPS = 10.0f;

    // Whether or not the animation is paused (defaults to false)
    bool mIsPaused = false;

    // Whether or not the animation should loop (defaults to true)
    bool mShouldLoop = true;

    //Resize
    int mResize = 1;
};
