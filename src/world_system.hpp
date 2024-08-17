#pragma once

// internal
#include "common.hpp"

// stlib
#include <vector>
#include <random>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

#include "render_system.hpp"
#include <unordered_set>

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class WorldSystem
{
public:
	WorldSystem();

	// Creates a window
	GLFWwindow* create_window();

	// starts the game
	void init(RenderSystem* renderer);

	// Releases all associated resources
	~WorldSystem();

	void explode(Entity block, std::unordered_set<Entity, EntityHash>& explodedBlocks);
    void processExplosion(Entity block, Motion &player, vec3 colorStar);
    void handleDigging(float elapsed_ms_since_last_update, bool hit, Motion &playerM, vec3 colorDirt, vec3 colorStar);
    // Steps the game ahead by ms milliseconds

	std::vector<std::vector<std::vector<Entity>>> step(float elapsed_ms, int& flag);
	std::vector<std::vector<std::vector<Entity>>> grid;


	// Game Over 
	bool check_game_over_condition();
	void set_game_over();

	// Check for collisions
	void handle_collisions();
	
	// Get boundary
	static int get_window_width();
	static int get_window_height();

	// Create other windows
	void window2();

	float time =0.f;
	float holdTime = 0.f;
	float requiredHoldTime = 1300.f;
	bool isLeftMouseButtonPressed = false;
	static int window_width;
	static int window_height;

	// Should the game be over ?
	bool is_over()const;
private:
	// Input callback functions
	void on_key(int key, int, int action, int mod);
	void on_mouse_move(vec2 pos);
    void generateMap(std::vector<std::vector<int>> map);
    void on_mouse_button(int button, int action, int mod);
    // restart level
	void restart_game(GameStateChange gc = GameStateChange::PLAY);
  

	void setGrid(ComponentContainer<Motion> motions);

	// Generate map
	void generateRandomMap();
	void generateRandomUIMap();
	void generateTutorialMapDEMO();
	void get_resolution();
	
	int mapState = 0;

	// OpenGL window handle
	GLFWwindow* window;

	// Number of fish eaten by the salmon, displayed in the window title
	unsigned int points;

	// Game state
	RenderSystem* renderer;
	float current_speed;
	float next_turtle_spawn;
	float next_fish_spawn;
	Entity PLAYER;
	Entity number;
	Entity tutorial;
	Entity start;
	//Entity quit;
	Entity mesh_block;

	// music references
	Mix_Music* background_music;
	Mix_Music* background_music_1;
	Mix_Music* background_music_2;
	Mix_Music* background_music_3;
	Mix_Chunk* salmon_dead_sound;
	Mix_Chunk* salmon_eat_sound;
	Mix_Chunk* teleport;
	Mix_Chunk* explode11;



	Mix_Chunk* Attack;
	Mix_Chunk* Break;
	//Mix_Music* Win;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniform_dist; // number between 0..1


	//Particle system

	//ParticleSystem particle_system;
};
