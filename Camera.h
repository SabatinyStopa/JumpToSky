#pragma once
#include "Player.h"

class Camera {
private:
    float targetScrollY = 0.0f; 
    float currentScrollY = 0.0f;
    float smoothness = 8.0f;    

public:
    Camera();

    void updateTarget(Player* player, float targetScreenY);

    float getSmoothScrollAmount(float deltaTime);

    void reset();
};