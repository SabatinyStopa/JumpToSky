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
#include "TextRenderer.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

extern std::vector<std::unique_ptr<Entity>> worldEntities;
Player* player = nullptr;
Uint64 lastTime = 0;
static PlatformManager platformManager;
static Camera camera;
static bool isGameOver = false;
static TextRenderer textRenderer;

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

	if (!textRenderer.init("BoldPixels.ttf", 24.0f)) {
		SDL_Log("Erro ao carregar a fonte no TextRenderer");
	}

	auto playerEntity = std::make_unique<Player>(310.0f, 300.0f, 20.0f, 20.0f);
	player = playerEntity.get();
	worldEntities.push_back(std::move(playerEntity));

	platformManager.init(player->jumpForce, player->gravity);

	return SDL_APP_CONTINUE;
}

void resetGame() {
	worldEntities.clear();

	auto playerEntity = std::make_unique<Player>(310.0f, 300.0f, 20.0f, 20.0f);
	player = playerEntity.get();
	worldEntities.push_back(std::move(playerEntity));

	camera.reset();
	platformManager.init(player->jumpForce, player->gravity);

	isGameOver = false;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT)
	{
		return SDL_APP_SUCCESS;
	}

	if (isGameOver && event->type == SDL_EVENT_KEY_DOWN)
	{
		if (event->key.scancode == SDL_SCANCODE_R || event->key.scancode == SDL_SCANCODE_SPACE) {
			resetGame();
		}
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	static Uint64 lastTime = SDL_GetTicks();
	Uint64 currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;

	if (!isGameOver) {
		for (auto& entity : worldEntities) {
			entity->update(deltaTime);
		}

		camera.updateTarget(player, Settings::SCREEN_HEIGHT / 2.0f);
		float smoothScroll = camera.getSmoothScrollAmount(deltaTime);

		platformManager.applyScrollAndRecycle(smoothScroll);
		camera.addScroll(smoothScroll);

		if (player && player->rect.y > Settings::SCREEN_HEIGHT) {
			isGameOver = true;
		}
	}

	SDL_SetRenderDrawColorFloat(renderer, 0.1f, 0.1f, 0.15f, SDL_ALPHA_OPAQUE_FLOAT);
	SDL_RenderClear(renderer);

	for (auto& entity : worldEntities) {
		entity->render(renderer);
	}

	std::string scoreText = std::to_string(static_cast<int>(camera.maxMeters)) + " m";

	textRenderer.renderText(
		renderer, scoreText,
		30.0f, 30.0f,
		SDL_Color{ 255, 255, 255, 255 }, false
	);

	if (isGameOver) {
		textRenderer.renderText(
			renderer, "GAME OVER",
			Settings::SCREEN_WIDTH / 2.0f, Settings::SCREEN_HEIGHT / 2.0f - 50.0f,
			SDL_Color{ 255, 0, 0, 255 }, true
		);

		std::string finalScore = "Max height: " + scoreText;
		textRenderer.renderText(
			renderer, finalScore,
			Settings::SCREEN_WIDTH / 2.0f, Settings::SCREEN_HEIGHT / 2.0f,
			SDL_Color{ 255, 215, 0, 255 }, true
		);

		textRenderer.renderText(
			renderer, "Press SPACE or R to try again",
			Settings::SCREEN_WIDTH / 2.0f, Settings::SCREEN_HEIGHT / 2.0f + 50.0f,
			SDL_Color{ 255, 255, 255, 255 }, true
		);
	}

	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	textRenderer.cleanUp();
	worldEntities.clear();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}