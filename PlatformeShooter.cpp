#define SDL_MAIN_USE_CALLBACKS 1 
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Entity.h"
#include "Player.h"
#include "Settings.h"
#include <vector>
#include <memory>
#include "PlatformManager.h"
#include "Camera.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

extern std::vector<std::unique_ptr<Entity>> worldEntities;
Player* player = nullptr;
Uint64 lastTime = 0;
static PlatformManager platformManager;
static Camera camera;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	SDL_SetAppMetadata("My game", "1.0", "No");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("My Game", Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	SDL_SetRenderLogicalPresentation(renderer, Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	auto playerEntity = std::make_unique<Player>(310.0f, 300.0f, 20.0f, 20.0f);
	player = playerEntity.get();
	worldEntities.push_back(std::move(playerEntity));

	platformManager.init(player->jumpForce, player->gravity);

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

	for (auto& entity : worldEntities) {
		entity->update(deltaTime);
	}

	camera.updateTarget(player, Settings::SCREEN_HEIGHT / 2.0f);

	float smoothScroll = camera.getSmoothScrollAmount(deltaTime);

	platformManager.applyScrollAndRecycle(smoothScroll);

	if (player&& player->rect.y > Settings::SCREEN_HEIGHT) {
		SDL_Log("Game Over!");
		return SDL_APP_SUCCESS;
	}

	SDL_SetRenderDrawColorFloat(renderer, 0.0f, 0.0f, 0.0f, SDL_ALPHA_OPAQUE_FLOAT);
	SDL_RenderClear(renderer);

	for (auto& entity : worldEntities) {
		entity->render(renderer);
	}

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	worldEntities.clear();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}