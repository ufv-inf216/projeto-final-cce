cmake_minimum_required(VERSION 3.26)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 11)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "t1-cce")
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME}
        Source/Main.cpp
        Source/Math.cpp
        Source/Random.cpp
        Source/Actors/Actor.cpp
        Source/Components/Component.cpp
        Source/Game.cpp
        Source/Components/DrawComponents/DrawComponent.cpp
        Source/Components/DrawComponents/DrawComponent.h
        Source/Components/RigidBodyComponent.cpp
        Source/Components/RigidBodyComponent.h
        Source/Components/ColliderComponents/CircleColliderComponent.cpp
        Source/Components/ColliderComponents/CircleColliderComponent.h
        Source/Json.h
        Source/Components/DrawComponents/DrawSpriteComponent.cpp
        Source/Components/DrawComponents/DrawSpriteComponent.h
        Source/Components/DrawComponents/DrawPolygonComponent.cpp
        Source/Components/DrawComponents/DrawPolygonComponent.h
        Source/Actors/Pacman.cpp
        Source/Actors/Pacman.h
        Source/Components/ColliderComponents/AABBColliderComponent.cpp
        Source/Components/ColliderComponents/AABBColliderComponent.h
        Source/Components/DrawComponents/DrawAnimatedComponent.cpp
        Source/Components/DrawComponents/DrawAnimatedComponent.h
        Source/Actors/Item.cpp
        Source/Actors/Item.h
        Source/Actors/Ghost/Ghost.cpp
        Source/Actors/Ghost/Ghost.h
        Source/Actors/Wall.h
        Source/Actors/Wall.cpp
        Source/Actors/PathNode.cpp
        Source/Actors/PathNode.h
        Source/Components/AIComponents/FSMComponent.cpp
        Source/Components/AIComponents/FSMComponent.h
        Source/Components/AIComponents/FSMState.cpp
        Source/Components/AIComponents/FSMState.h
        Source/Actors/Ghost/States/ScatterState.cpp
        Source/Actors/Ghost/States/ScatterState.h
        Source/Actors/Ghost/States/GhostState.cpp
        Source/Actors/Ghost/States/GhostState.h
        Source/Actors/Ghost/States/ChaseState.cpp
        Source/Actors/Ghost/States/ChaseState.h
        Source/Actors/Ghost/States/FrightenedState.cpp
        Source/Actors/Ghost/States/FrightenedState.h
        Source/Actors/Ghost/States/DeadState.cpp
        Source/Actors/Ghost/States/DeadState.h
)

####### DO NOT EDIT BELLOW
find_package(SDL2 2.28.2 EXACT REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image::SDL2_image)