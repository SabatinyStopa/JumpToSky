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

static bool isPointInsideRect(float px, float py, const SDL_FRect& rect) {
	return (px >= rect.x && px <= rect.x + rect.w &&
		py >= rect.y && py <= rect.y + rect.h);
}

void Player::update(float deltaTime)
{
	handleInput();

	rect.x += xVelocity * deltaTime;

	if (rect.x < 0.0f) rect.x = 0.0f;
	if (rect.x + rect.w > Settings::SCREEN_WIDTH) rect.x = Settings::SCREEN_WIDTH - rect.w;

	yVelocity += gravity * deltaTime;
	rect.y += yVelocity * deltaTime;

	isGrounded = false;

	if (yVelocity >= 0.0f) {

		float footLeftX = rect.x + 2.0f;
		float footRightX = rect.x + rect.w - 2.0f;
		float footY = rect.y + rect.h + 1.0f;

		for (const auto& entity : worldEntities) {
			if (entity.get() == this) continue;

			const SDL_FRect& platform = entity->rect;

			bool leftFootOnPlatform = isPointInsideRect(footLeftX, footY, platform);
			bool rightFootOnPlatform = isPointInsideRect(footRightX, footY, platform);

			if (leftFootOnPlatform || rightFootOnPlatform) {
				rect.y = platform.y - rect.h;
				yVelocity = 0.0f;
				isGrounded = true;
				break;
			}
		}
	}
	else if (yVelocity < 0.0f) {
		float headCenterX = rect.x + (rect.w / 2.0f);
		float headY = rect.y - 1.0f;

		for (const auto& entity : worldEntities) {
			if (entity.get() == this) continue;

			const SDL_FRect& platform = entity->rect;

			if (isPointInsideRect(headCenterX, headY, platform)) {

				rect.y = platform.y + platform.h;
				yVelocity = 0.0f;
				break;
			}
		}
	}
}