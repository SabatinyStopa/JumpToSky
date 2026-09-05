#include "TextRenderer.h"

TextRenderer::TextRenderer() {}

TextRenderer::~TextRenderer() {
    cleanUp();
}

bool TextRenderer::init(const char* fontPath, float fontSize) {
    if (!TTF_Init()) {
        SDL_Log("Erro ao inicializar TTF: %s", SDL_GetError());
        return false;
    }

    font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        SDL_Log("Erro ao carregar fonte (%s): %s", fontPath, SDL_GetError());
        return false;
    }

    return true;
}

void TextRenderer::renderText(SDL_Renderer* renderer, const std::string& text, float x, float y, SDL_Color color, bool center) {
    if (!font || text.empty()) return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.length(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        SDL_FRect destRect = { x, y, (float)surface->w, (float)surface->h };

        if (center) {
            destRect.x -= destRect.w / 2.0f;
            destRect.y -= destRect.h / 2.0f;
        }

        SDL_RenderTexture(renderer, texture, NULL, &destRect);
        SDL_DestroyTexture(texture);
    }

    SDL_DestroySurface(surface);
}

void TextRenderer::cleanUp() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    TTF_Quit();
}