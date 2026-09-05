#include "Player.h"
#include "World.h"
#include "Settings.h"

Player::Player(float x, float y, float width, float height)
    : Entity(x, y, width, height, SDL_FColor{ 0.0f, 1.0f, 0.0f, 1.0f })
{
}

void Player::handleInput() {
    const bool* keyState = SDL_GetKeyboardState(NULL);

    xVelocity = 0.0f;

    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
        xVelocity = -speed;
    }
    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
        xVelocity = speed;
    }

    if ((keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_UP]) && isGrounded) {
        yVelocity = jumpForce;
        isGrounded = false;
    }
}

void Player::update(float deltaTime)
{
    handleInput();

    rect.x += xVelocity * deltaTime;

    if (rect.x < 0.0f) {
        rect.x = 0.0f;
    }
    else if (rect.x + rect.w > Settings::SCREEN_WIDTH) {
        rect.x = Settings::SCREEN_WIDTH - rect.w;
    }

    for (const auto& entity : worldEntities) {
        if (entity.get() == this) continue;

        SDL_FRect otherRect = entity->rect;

        if (SDL_HasRectIntersectionFloat(&rect, &otherRect)) {
            if (rect.y + rect.h > otherRect.y + 2.0f && rect.y < otherRect.y + otherRect.h - 2.0f) {
                if (xVelocity > 0.0f) {
                    rect.x = otherRect.x - rect.w;
                }
                else if (xVelocity < 0.0f) {
                    rect.x = otherRect.x + otherRect.w;
                }
            }
        }
    }

    yVelocity += gravity * deltaTime;
    rect.y += yVelocity * deltaTime;

    isGrounded = false;

    for (const auto& entity : worldEntities) {
        if (entity.get() == this) continue;

        SDL_FRect otherRect = entity->rect;

        if (SDL_HasRectIntersectionFloat(&rect, &otherRect)) {
            if (yVelocity > 0.0f) {
                
                if (rect.y + rect.h - yVelocity * deltaTime <= otherRect.y + 12.0f) {
                    rect.y = otherRect.y - rect.h;
                    yVelocity = 0.0f;
                    isGrounded = true;
                }
            }
            else if (yVelocity < 0.0f) {
               
                rect.y = otherRect.y + otherRect.h;
                yVelocity = 0.0f;
            }
        }
    }
}