#include "Player.h"

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

    yVelocity += gravity * deltaTime;
    rect.y += yVelocity * deltaTime;
}