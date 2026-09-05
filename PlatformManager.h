#pragma once
#include <SDL3/SDL.h>
#include <random>

class PlatformManager {
private:
    float minGapY = 0.0f;
    float maxGapY = 0.0f;
    float lastPlatformY = 0.0f;

    std::mt19937 rng;
    float getRandomFloat(float min, float max);

public:
    PlatformManager();

    void init(float jumpForce, float gravity);

    void spawnOnePlatformAbove();

    void applyScrollAndRecycle(float scrollAmount);
};