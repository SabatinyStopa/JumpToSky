#pragma once
#include <SDL3/SDL.h>
#include "Entity.h"

class Player : public Entity {
public:
    float xVelocity = 0.0f;
    float yVelocity = 0.0f;

    float speed = 300.0f;       
    float gravity = 980.0f;     
    float jumpForce = -500.0f;  

    bool isGrounded = false;    

    Player(float x, float y, float width, float height);

    void handleInput();

    void update(float deltaTime) override;
};