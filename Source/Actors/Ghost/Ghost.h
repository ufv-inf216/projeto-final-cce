//
// Created by Lucas N. Ferreira on 30/09/23.
//

#pragma once

#include "../Actor.h"

class Ghost : public Actor
{
public:
    enum Type
    {
        Blinky,
        Pinky,
        Inky,
        Clyde
    };

    Ghost(Game* game, Type type);
    ~Ghost();

    // Animation functions
    void UpdateAnimations();

    // Start/Pause ghost components
    void Start();
    void Pause();

    // Get the type of ghost this is
    Type GetType() const { return mType; }

    // Get/set the spawn node and scatter target nodes
    void SetSpawnNode(class PathNode* node) { mSpawnNode = node; }
    void SetScatterNode(class PathNode* node) { mScatterNode = node; }

    class PathNode* GetSpawnNode() const { return mSpawnNode; }
    class PathNode* GetScatterNode() const { return mScatterNode; }

    // Get/set the node pointers used for performing the ghost movement
    void SetTargetNode(class PathNode* node) { mTargetNode = node; }
    void SetPreviousNode(class PathNode* node) { mPrevNode = node; }
    void SetNextNode(class PathNode* node) { mNextNode = node; }

    class PathNode* GetTargetNode() const { return mTargetNode; }
    class PathNode* GetPreviousNode() const { return mPrevNode; }
    class PathNode* GetNextNode() const { return mNextNode; }

    float GetForwardSpeed() const { return mForwardSpeed; }
    void SetForwardSpeed(float forwardSpeed) { mForwardSpeed = forwardSpeed; }

    // Helper function that sets correct color for ghost path drawing
    void DebugDrawPath(struct SDL_Renderer* render);

protected:
    void OnUpdate(float deltaTime) override;

private:
    Type mType;
    float mForwardSpeed;

    class PathNode* mSpawnNode;
    class PathNode* mScatterNode;

    // TargetNode is our current goal node
    class PathNode* mTargetNode = nullptr;
    // PrevNode is the last node we intersected with prior to the current position
    class PathNode* mPrevNode = nullptr;
    // NextNode is the next node we're trying to get to
    class PathNode* mNextNode = nullptr;

    class RigidBodyComponent* mRigidBodyComponent;
    class DrawAnimatedComponent* mDrawComponent;
    class AABBColliderComponent* mColliderComponent;

    class FSMComponent* mFSMComponent;
};