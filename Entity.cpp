#include "Entity.h"

Entity::Entity(float x, float y, float width, float height, SDL_FColor inColor)
    : rect{ x, y, width, height }, color(inColor)
{
}

void Entity::update(float deltaTime) {
    
}

void Entity::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColorFloat(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}