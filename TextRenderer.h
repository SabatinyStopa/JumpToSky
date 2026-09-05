#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

class TextRenderer {
private:
    TTF_Font* font = nullptr;

public:
    TextRenderer();
    ~TextRenderer();

    bool init(const char* fontPath, float fontSize);
    void renderText(SDL_Renderer* renderer, const std::string& text, float x, float y, SDL_Color color, bool center = false);
    void cleanUp();
};