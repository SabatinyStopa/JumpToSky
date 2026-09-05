#include "Camera.h"
#include "Settings.h"
#include <algorithm>

Camera::Camera() {
    reset();
}

void Camera::addScroll(float amount) {
    totalY += amount;
    float currentMeters = totalY / Settings::PIXELS_PER_METER;
    if (currentMeters > maxMeters) {
        maxMeters = currentMeters;
    }
}

void Camera::reset() {
    targetScrollY = 0.0f;
    currentScrollY = 0.0f;
    totalY = 0.0f;
    maxMeters = 0.0f;
}

void Camera::updateTarget(Player* player, float targetScreenY) {
    if (!player) return;

    if (player->rect.y < targetScreenY) {
        float diff = targetScreenY - player->rect.y;

        targetScrollY += diff;

        player->rect.y = targetScreenY;
    }
}

float Camera::getSmoothScrollAmount(float deltaTime) {
    if (std::abs(targetScrollY - currentScrollY) < 0.01f) {
        return 0.0f;
    }

    float prevScroll = currentScrollY;

    currentScrollY += (targetScrollY - currentScrollY) * smoothness * deltaTime;

    return currentScrollY - prevScroll;
}