#include "PlatformManager.h"
#include "World.h"
#include "Settings.h"
#include <algorithm>

PlatformManager::PlatformManager() {
    std::random_device rd;
    rng = std::mt19937(rd());
}

float PlatformManager::getRandomFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

void PlatformManager::spawnPlatformAbove(float newY) {
    float platformWidth = getRandomFloat(80.0f, 140.0f);
    float platformHeight = 15.0f;                       
    float platformX = getRandomFloat(0.0f, Settings::SCREEN_WIDTH - platformWidth);
    
    SDL_FColor color{ 0.2f, 0.6f, 1.0f, 1.0f };
    
    worldEntities.push_back(std::make_unique<Entity>(
        platformX, newY, platformWidth, platformHeight, color
    ));

    lastPlatformY = newY;
}

void PlatformManager::update(float cameraY) 
{
    if (lastPlatformY > cameraY - Settings::SCREEN_HEIGHT) {
        spawnPlatformAbove(lastPlatformY - getRandomFloat(minGapY, maxGapY));
    }

    float destroyThresholdY = cameraY + Settings::SCREEN_HEIGHT + 100.0f;

    worldEntities.erase(
        std::remove_if(worldEntities.begin(), worldEntities.end(),
            [destroyThresholdY](const std::unique_ptr<Entity>& entity) 
            {
                return entity->rect.y > destroyThresholdY;
            }),
        worldEntities.end()
    );
}

void PlatformManager::init(float jumpForce, float gravity) {
    // Calcula a altura máxima teórica do pulo
    float maxJumpHeight = (jumpForce * jumpForce) / (2.0f * gravity);

    // Define o gap máximo como 75% do pulo máximo para não exigir precisão de pixel
    this->maxGapY = maxJumpHeight * 0.75f;
    this->minGapY = maxJumpHeight * 0.40f;

    worldEntities.push_back(std::make_unique<Entity>(
        0.0f, Settings::SCREEN_HEIGHT - 30.0f, Settings::SCREEN_WIDTH, 30.0f,
        SDL_FColor{ 0.5f, 0.5f, 0.5f, 1.0f }
    ));

    lastPlatformY = Settings::SCREEN_HEIGHT - 30.0f;

    while (lastPlatformY > -Settings::SCREEN_HEIGHT)
    {
        spawnPlatformAbove(lastPlatformY - getRandomFloat(minGapY, maxGapY));
    }
}