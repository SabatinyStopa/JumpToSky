#pragma once
#include "Player.h"

class Camera {
private:
    float targetScrollY = 0.0f; 
    float currentScrollY = 0.0f;
    float smoothness = 8.0f;    
    float totalY = 0.0f;      

public:
    float maxMeters = 0.0f;

    Camera();

    void addScroll(float amount);

    void updateTarget(Player* player, float targetScreenY);

    float getSmoothScrollAmount(float deltaTime);

    void reset();
};