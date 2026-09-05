#define SDL_MAIN_USE_CALLBACKS 1 
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Entity.h"
#include "Player.h"
#include <vector>
#include <memory>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

std::vector<std::unique_ptr<Entity>> entities;

Uint64 lastTime = 0;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("My game", "1.0", "No");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("My Game", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    entities.push_back(std::make_unique<Player>(0.0f, 440.0f, 20.0f, 20.0f));

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    Uint64 currentTime = SDL_GetPerformanceCounter();
    if (lastTime == 0) lastTime = currentTime;

    float deltaTime = static_cast<float>(currentTime - lastTime) / static_cast<float>(SDL_GetPerformanceFrequency());
    lastTime = currentTime;

    SDL_SetRenderDrawColorFloat(renderer, 0.0f, 0.0f, 0.0f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    for (auto& entity : entities)
    {
        entity->update(deltaTime);
        entity->render(renderer);
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    entities.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}