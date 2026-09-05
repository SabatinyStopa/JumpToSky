#pragma once
#include <SDL3/SDL.h>

class Entity {
public:
    SDL_FRect rect;
    SDL_FColor color;

    Entity(float x, float y, float width, float height, SDL_FColor inColor);
    virtual ~Entity() = default;

    virtual void update(float deltaTime);
    virtual void render(SDL_Renderer* renderer) const; // Adicionado virtual aqui também
};

