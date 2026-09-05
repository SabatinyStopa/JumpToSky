#pragma once
#include <SDL3/SDL.h>
#include <random>

class PlatformManager {
private:
    float lastPlatformY = 0.0f; 
    float minGapY = 60.0f;      
    float maxGapY = 100.0f;

    std::mt19937 rng; // Random number generator ??? Gemini recommended!

    float getRandomFloat(float min, float max);

public:
    PlatformManager();

    void spawnPlatformAbove(float targetY);

    void update(float cameraY);

    void init(float jumpForce, float gravity);
};