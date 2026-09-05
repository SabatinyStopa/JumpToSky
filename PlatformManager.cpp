#include "PlatformManager.h"
#include "World.h"
#include "Settings.h"
#include <algorithm>
#include <cmath>

PlatformManager::PlatformManager() {
	std::random_device rd;
	rng = std::mt19937(rd());
}

float PlatformManager::getRandomFloat(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}

void PlatformManager::init(float jumpForce, float gravity) {
	float absJumpForce = std::abs(jumpForce);
	float maxJumpHeight = (absJumpForce * absJumpForce) / (2.0f * gravity);

	this->minGapY = maxJumpHeight * 0.35f;
	this->maxGapY = maxJumpHeight * 0.55f;

	lastPlatformY = Settings::SCREEN_HEIGHT - 30.0f;

	worldEntities.push_back(std::make_unique<Entity>(
		0.0f, Settings::SCREEN_HEIGHT - 30.0f, Settings::SCREEN_WIDTH, 30.0f,
		SDL_FColor{ 0.5f, 0.5f, 0.5f, 1.0f }
	));

	while (lastPlatformY > 0.0f) {
		spawnOnePlatformAbove();
	}
}

void PlatformManager::spawnOnePlatformAbove() {
	float platformWidth = getRandomFloat(60.0f, 130.0f);
	float platformHeight = 15.0f;
	float nextY = lastPlatformY - getRandomFloat(minGapY, maxGapY);
	float platformX = 0.0f;

	Entity* lastPlatform = nullptr;
	for (auto it = worldEntities.rbegin(); it != worldEntities.rend(); ++it) {
		if ((*it)->rect.w != 20.0f) {
			lastPlatform = it->get();
			break;
		}
	}

	if (lastPlatform) {
		float lastX = lastPlatform->rect.x;

		float minOffsetX = 70.0f;
		float maxOffsetX = 150.0f;
		float dir = (getRandomFloat(0.0f, 1.0f) > 0.5f) ? 1.0f : -1.0f;

		platformX = lastX + (dir * getRandomFloat(minOffsetX, maxOffsetX));

		if (platformX < 10.0f) platformX = getRandomFloat(10.0f, 50.0f);
		if (platformX + platformWidth > Settings::SCREEN_WIDTH - 10.0f) {
			platformX = Settings::SCREEN_WIDTH - platformWidth - getRandomFloat(10.0f, 50.0f);
		}
	}
	else {
		platformX = getRandomFloat(10.0f, Settings::SCREEN_WIDTH - platformWidth - 10.0f);
	}

	SDL_FColor color{ 0.5f, 0.5f, 0.5f, 1.0f };

	worldEntities.push_back(std::make_unique<Entity>(
		platformX, nextY, platformWidth, platformHeight, color
	));

	lastPlatformY = nextY;
}

void PlatformManager::applyScrollAndRecycle(float scrollAmount) {
	if (scrollAmount <= 0.0f) return;


	for (auto& entity : worldEntities) {
		if (entity->rect.w != 20.0f) {
			entity->rect.y += scrollAmount;
		}
	}
	lastPlatformY += scrollAmount;


	size_t initialCount = worldEntities.size();

	worldEntities.erase(
		std::remove_if(worldEntities.begin(), worldEntities.end(),
			[](const std::unique_ptr<Entity>& entity) {
				return entity->rect.y > Settings::SCREEN_HEIGHT && entity->rect.w != 20.0f;
			}),
		worldEntities.end()
	);

	size_t removedCount = initialCount - worldEntities.size();
	for (size_t i = 0; i < removedCount; ++i) {
		spawnOnePlatformAbove();
	}
}