// Header
#include "world_system.hpp"
#include "world_init.hpp"
#include "physics_calculation.hpp"
#include "world_init.hpp"
#include "render_system.hpp"
#include "particle_system.hpp"
#include "map.hpp"
#include "SDL.h"
#include "iostream"
#include <chrono>
// stlib
#include <cassert>
#include <sstream>
#include <random>

#include "physics_system.hpp"
#include "iostream"
#include "unordered_set"
#include <thread>

// Game configuration
const size_t MAX_TURTLES = 15;
const size_t MAX_FISH = 1;
const size_t TURTLE_DELAY_MS = 2000 * 3;
const size_t FISH_DELAY_MS = 5000 * 3;
vec2 mouse_pos = { 0.f, 0.f };
bool game_over = false;
//const int MAP_WIDTH = 20;
//const int MAP_HEIGHT = 20;

std::vector<Entity> LavaParticles;

std::vector<Entity> b_exp;
std::vector<Entity> delayedExplosions;
bool HasExpoldingBlocks = false;


int WorldSystem::window_width;
int WorldSystem::window_height;

ParticleSystem particle_system;
Map map;

Timer* LavaTimer;

// Create the fish world
WorldSystem::WorldSystem()
	: points(0), next_turtle_spawn(0.f), next_fish_spawn(0.f)
{
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

WorldSystem::~WorldSystem()
{
	// Destroy music components
	if (background_music != nullptr)
		Mix_FreeMusic(background_music);
	if (background_music_1 != nullptr)
		Mix_FreeMusic(background_music_1);
	if (background_music_2 != nullptr)
		Mix_FreeMusic(background_music_2);
	if (background_music_3 != nullptr)
		Mix_FreeMusic(background_music_3);
	if (salmon_dead_sound != nullptr)
		Mix_FreeChunk(salmon_dead_sound);
	if (salmon_eat_sound != nullptr)
		Mix_FreeChunk(salmon_eat_sound);
	if (Attack != nullptr)
		Mix_FreeChunk(Attack);
	if (Break != nullptr)
		Mix_FreeChunk(Break);
	/*if (Win != nullptr)
		Mix_FreeMusic(Win);*/
	if (teleport != nullptr)
		Mix_FreeChunk(teleport);
	if (explode11 != nullptr)
		Mix_FreeChunk(explode11);


	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

// Debugging
namespace
{
	void glfw_err_cb(int error, const char* desc)
	{
		fprintf(stderr, "%d: %s", error, desc);
	}
}

void WorldSystem::get_resolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	window_width = mode->width;
	window_height = mode->height;
	initialize_scale_size(window_height);
	
}

// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow* WorldSystem::create_window()
{
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW");
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// If you are on Linux or Windows, you can change these 2 numbers to 4 and 3 and
	// enable the glDebugMessageCallback to have OpenGL catch your mistakes for you.
	// GLFW / OGL Initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create the main window (for rendering, keyboard, and mouse input)
	get_resolution();
	// Get the central position of the screen
	// const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	// double cen_screen_x = (mode->width - window_width) / 2;
	// double cen_screen_y = (mode->height - window_height) / 2;

	window = glfwCreateWindow(window_width, window_height, "DIG QUIXOTE", nullptr, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}

	// glfwSetWindowPos(window, cen_screen_x, cen_screen_y);

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3)
		{ ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_key(_0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1)
		{ ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_move({ _0, _1 }); };
	auto mouse_button_redirect = [](GLFWwindow* wnd, int button, int action, int mods)
		{ ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_button(button, action, mods); };

	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);
	glfwSetMouseButtonCallback(window, mouse_button_redirect);
	//////////////////////////////////////
	// Loading music and sounds with SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		fprintf(stderr, "Failed to initialize SDL Audio");
		return nullptr;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		fprintf(stderr, "Failed to open audio device");
		return nullptr;
	}
	background_music = Mix_LoadMUS(audio_path("music.wav").c_str());
	background_music_1 = Mix_LoadMUS(audio_path("Flourish.wav").c_str());
	background_music_2 = Mix_LoadMUS(audio_path("Worlds.wav").c_str());
	background_music_3 = Mix_LoadMUS(audio_path("Cinematic.wav").c_str());
	salmon_dead_sound = Mix_LoadWAV(audio_path("gameover.wav").c_str());
	salmon_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav").c_str());
	Attack = Mix_LoadWAV(audio_path("Attack.wav").c_str());
	Break = Mix_LoadWAV(audio_path("Break.wav").c_str());
	//Win = Mix_LoadMUS(audio_path("Win.wav").c_str());
	teleport = Mix_LoadWAV(audio_path("teleport.wav").c_str());
	explode11 = Mix_LoadWAV(audio_path("explode.wav").c_str());

	if (background_music == nullptr || salmon_dead_sound == nullptr || salmon_eat_sound == nullptr)
	{
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav").c_str(),
			audio_path("gameover.wav").c_str(),
			audio_path("salmon_eat.wav").c_str());
		return nullptr;
	}

	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg)
{
	this->renderer = renderer_arg;
	// Playing background music indefinitely
	Mix_PlayMusic(background_music, -1);
	fprintf(stderr, "Loaded music\n");

	// Set all states to default
	restart_game(GameStateChange::MENU);
}

bool playAttackSoundEffect(int state)
{
	return state == 3 || state == 7 || state == 11;
}

void WorldSystem::processExplosion(Entity block, Motion& playerM, vec3 colorStar)
{
	Explosion exp = registry.explosions.get(block);
	Motion& c_motion = registry.motions.get(block);

	for (uint i = 0; i < registry.blocks.size(); i++)
	{
		Entity entity = registry.blocks.entities[i];
		Motion& b_motion = registry.motions.get(entity);
		float min_range_x = c_motion.position.x - (exp.range * c_motion.scale.x + 1);
		float max_range_x = c_motion.position.x + (exp.range * c_motion.scale.x + 1);
		float min_range_y = c_motion.position.y - (exp.range * c_motion.scale.y + 1);
		float max_range_y = c_motion.position.y + (exp.range * c_motion.scale.y + 1);

		if (b_motion.position.x >= min_range_x && b_motion.position.x <= max_range_x &&
			b_motion.position.y >= min_range_y && b_motion.position.y <= max_range_y)
		{
			if (entity != block)
			{
				Block& block = registry.blocks.get(entity);
				if (block.type != 666 && block.type != 8)
				{
					/*if (block.type == 6)
					{
						auto doubleCheck = std::find(delayedExplosions.begin(), delayedExplosions.end(), entity);

						if (doubleCheck == delayedExplosions.end())
						{
							delayedExplosions.push_back(entity);
						}
					}
					else
					{*/
					b_exp.push_back(entity);
					registry.explosions1.emplace(entity);
					block.explode = true;
					RenderRequest& render_request = registry.renderRequests.get(entity);
					render_request.used_texture = TEXTURE_ASSET_ID::LIGHTING;
					render_request.used_geometry = GEOMETRY_BUFFER_ID::LIGHTING;
					//}
				}
			}
		}
	}

	//for (uint i = 0; i < b_exp.size(); i++)
	//{
	//	createParticlesAtCollisionEdge(playerM, registry.motions.get(b_exp[i]), uniform_dist(rng), colorStar, true);
	//	registry.remove_all_components_of(b_exp[i]);
	//}

	//registry.remove_all_components_of(block);
	b_exp.push_back(block);
	registry.explosions1.emplace(block);
	HasExpoldingBlocks = true;
	RenderRequest& render_request = registry.renderRequests.get(block);
	render_request.used_texture = TEXTURE_ASSET_ID::LIGHTING;
	render_request.used_geometry = GEOMETRY_BUFFER_ID::LIGHTING;

}

void WorldSystem::handleDigging(float elapsed_ms_since_last_update, bool hit, Motion& playerM)
{
	auto& blocks = registry.blocks;
	Entity* block_remove = nullptr;
	Motion m_block;
	Block* b_block;

	for (uint i = 0; i < blocks.size(); i++)   //loop through all the blocks to find the one that is colliding with the mouse and player
	{
		Entity entity = blocks.entities[i];
		Motion& b_motion = registry.motions.get(entity);
		Block& block = registry.blocks.get(entity);

		if (if_mouse_collides_pos(mouse_pos, b_motion))
		{
			if (collides(playerM, b_motion))
			{
				if (block.type == 2 || block.type == 6 || block.type == 9)
				{
					block_remove = &blocks.entities[i];
					m_block = b_motion;
					b_block = &block;
				}
			}
		}

		if (if_mouse_collides_pos(mouse_pos, b_motion))
		{
			if (block.type == 666)
			{
				b_motion.velocity = vec2{ 0.f, -600.f };
			}
		}
	}

	if (block_remove != nullptr)
	{
		holdTime += elapsed_ms_since_last_update;

		if (hit && registry.explosions.has(*block_remove))  //explosive block
			particle_system.CreateParticlesAtCollisionEdge(playerM, m_block, uniform_dist(rng), particle_system.BlueParticles);
		else if (mapState == 0)								//Tutorial map
			particle_system.CreateParticlesAtCollisionEdge(playerM, m_block, uniform_dist(rng), particle_system.WhiteParticles);
		else if (hit)										//Normal Hit
			particle_system.CreateParticlesAtCollisionEdge(playerM, m_block, uniform_dist(rng), particle_system.BrownParticles);


		if (holdTime >= requiredHoldTime) //remove block
		{
			if (registry.explosions.has(*block_remove))
			{
				processExplosion(*block_remove, playerM, vec3(0, 1, 1));
			}
			if (b_block->type == 6)
				b_block->explode = true;
			else
				registry.remove_all_components_of(*block_remove);


			if(mapState != 0 && !registry.explosions.has(*block_remove))
			particle_system.CreateParticlesWhenBroken(m_block, particle_system.BrownParticles);

			holdTime = 0.f;

			if (registry.explosions.has(*block_remove))
				Mix_PlayChannel(-1, explode11, 0);
			else
				Mix_PlayChannel(-1, Break, 0);

			Number& n = registry.number.get(number);
			n.digLeft -= 1;

			if (n.digLeft == 0)
			{
				registry.deathTimers.emplace(PLAYER);
				Mix_PlayChannel(-1, salmon_dead_sound, 0);
				registry.colors.get(PLAYER) = vec3(1, 0, 0);
				n.digLeft = 1;
			}
		}
	}
}

void WorldSystem::setGrid(ComponentContainer<Motion> motions)
{

	int xOffset = window_width / 2 - BLOCK_BB_WIDTH * (MAP_WIDTH / 2) + BLOCK_BB_WIDTH / 2 - BLOCK_BB_WIDTH / 2;
	int yOffset = 150.f - BLOCK_BB_HEIGHT / 2;
	const int gridSize = 21 * BLOCK_BB_WIDTH; // 21x21 grid
	const int cellSize = 3 * BLOCK_BB_WIDTH;  // Each cell is 3x3
	const int cellsPerRow = gridSize / cellSize;

	grid.clear();
	grid.resize(cellsPerRow);
	for (auto& row : grid) {
		row.resize(cellsPerRow);
	}

	//if (mapState == 0)
	//{
	//	return;
	//}

	for (int i = 0; i < motions.size(); i++)
	{
		Motion& motion = motions.components[i];
		Entity& entity = motions.entities[i];
		if (!motion.isActive) continue;
		int cellX = static_cast<int>(motion.position.x - xOffset) / cellSize;
		int cellY = static_cast<int>(motion.position.y - yOffset) / cellSize;

		if (cellX >= 0 && cellX < cellsPerRow && cellY >= 0 && cellY < cellsPerRow) {
			grid[cellY][cellX].push_back(entity);
		}
	}
}

// Update our game world
std::vector<std::vector<std::vector<Entity>>> WorldSystem::step(float elapsed_ms_since_last_update, bool& IsTutorial)
{

	// std::cout << "TIME!!!!!: " << time << std::endl;
	//  Updating window title with points
	std::stringstream title_ss;
	title_ss << "DIG QUIXOTE";
	glfwSetWindowTitle(window, title_ss.str().c_str());

	// Remove debug info from the last step
	while (registry.debugComponents.entities.size() > 0)
		registry.remove_all_components_of(registry.debugComponents.entities.back());

	// Removing out of screen entities
	auto& motion_container = registry.motions;
	auto& particle_container = registry.particle;
	for (int i = (int)particle_container.components.size() - 1; i >= 0; --i)
	{
		Particle& particleSystem = particle_container.components[i];
		Entity e = particle_container.entities[i];
		if (registry.lava.has(e)) continue;
		particleSystem.timer -= elapsed_ms_since_last_update;
		if (particleSystem.timer < 0.f)
		{
			registry.motions.remove(e);
			registry.particle.remove(e);
		}
	}


	if (HasExpoldingBlocks)
	{
		int num = registry.blocks.get(b_exp[0]).state;
		if (registry.blocks.get(b_exp[0]).exploded)
		{
			for (uint i = 0; i < b_exp.size(); i++)
			{
				particle_system.CreateParticlesWhenBroken(registry.motions.get(b_exp[i]), vec3(0, 1, 1));
				registry.remove_all_components_of(b_exp[i]);
			}
			b_exp.clear();
			HasExpoldingBlocks = false;

		}
	}

	// Remove entities that leave the screen on the left side
	// Iterate backwards to be able to remove without unterfering with the next object to visit
	// (the containers exchange the last element with the current)
	for (int i = (int)motion_container.components.size() - 1; i >= 0; --i)
	{
		Motion& motion = motion_container.components[i];
		if (motion.position.x + abs(motion.scale.x) < 0.f)
		{
			if (!registry.players.has(motion_container.entities[i])) // don't remove the player
				registry.remove_all_components_of(motion_container.entities[i]);
		}
	}

	assert(registry.screenStates.components.size() <= 1);
	ScreenState& screen = registry.screenStates.components[0];



	// Processing the player state
	Player& player1 = registry.players.get(PLAYER);
	Motion& playerM = registry.motions.get(PLAYER);
	time += elapsed_ms_since_last_update;

	// check for the explosive blocks
	/*for (Entity delayedBlock : delayedExplosions)
	{
		processExplosion(delayedBlock, playerM, colorStar);
	}
	delayedExplosions.clear();*/

	bool hit = false;
	if (playAttackSoundEffect(player1.attack_state) && time >= 200.f && playerM.attack)
	{
		Mix_PlayChannel(-1, Attack, 0);
		time = 0.f;
		hit = true;
	}

	if (isLeftMouseButtonPressed)
	{
		// dig
		//particle_system.CreateHundredsParticles(mouse_pos, 400);
		handleDigging(elapsed_ms_since_last_update, hit, playerM);
	}


	hit = false;

	if (playerM.position.y > window_height)
	{
		if (mapState < 4)
		{
			Game& game = registry.game.get(PLAYER);
			if (game.state != GameState::START) {
				mapState += 1;
				restart_game();
			}

			if (mapState == 1) {
				Mix_PlayMusic(background_music_2, -1);
			}

			if (mapState == 3) {
				Mix_PlayMusic(background_music_3, -1);
			}
		}
		else
		{
			//Wining state check
			player1.win = true;
		}
	}


	if (map.map_created) 
	setGrid(motion_container);



	float min_timer_ms = 3000.f;
	for (Entity entity : registry.deathTimers.entities)
	{
		// progress timer
		DeathTimer& timer = registry.deathTimers.get(entity);
		timer.timer_ms -= elapsed_ms_since_last_update;
		if (timer.timer_ms < min_timer_ms)
		{
			min_timer_ms = timer.timer_ms;
		}

		// restart the game once the death timer expired
		if (timer.timer_ms < 0)
		{
			registry.deathTimers.remove(entity);
			screen.screen_darken_factor = 0;
			restart_game();
			return grid;
		}
	}
	// reduce window brightness if any of the present salmons is dying
	screen.screen_darken_factor = 1 - min_timer_ms / 3000;

	mapState == 0 ? IsTutorial = true : IsTutorial = false;

	if (mapState == 2 && LavaTimer)
	{
		LavaTimer->timer_ms -= elapsed_ms_since_last_update;
		if (LavaTimer->timer_ms <= 0)
		{
			particle_system.ActivateLavaParticle(LavaParticles);
			registry.timer.remove(PLAYER);
			LavaTimer = nullptr;
		}
			

		
	}


	return grid;

}

bool WorldSystem::check_game_over_condition()
{
	Player& player1 = registry.players.get(PLAYER);
	return player1.win;
}

void WorldSystem::set_game_over()
{
	Mix_HaltMusic();
	//Mix_PlayMusic(Win, 1);
	SDL_Delay(2000);
	game_over = true;
}

// Reset the world state to its initial state
void WorldSystem::restart_game(GameStateChange sc)
{
	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Reset the game speed
	current_speed = 1.f;

	// Remove all entities that we created
	// All that have a motion, we could also iterate over all fish, turtles, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0)
		registry.remove_all_components_of(registry.motions.entities.back());

	// Debugging for memory/component leaks
	registry.list_all_components();

	//Check Game State
	if (sc == GameStateChange::MENU) {
		createUIBackground(window_width, window_height);

		PLAYER = createEmptyPlayer(renderer, { 625.f, 50.f });
		start = createSuButton(renderer, { window_width / 2, window_height / 2 + 400 });
		//quit = createQuButton(renderer, { window_width / 2, window_height / 2 + 100 });
		//generateRandomUIMap();
	}
	else {
		//mapState = 2;
		// Background control
		if (mapState == 0)
		{
			createBackground(window_width, window_height, 0);
		}
		else if (mapState == 1)
		{
			createBackground(window_width, window_height, 1);
		}
		else if (mapState == 2)
		{
			createBackground(window_width, window_height, 2);
		}
		else if (mapState == 3)
		{
			createBackground(window_width, window_height, 3);
		}
		else if (mapState == 4)
		{
			createBackground(window_width, window_height, 4);
		}

		// Create a new salmon
		PLAYER = createPlayer(renderer, { window_width / 2 - BLOCK_BB_WIDTH / 2, 0.f });
		registry.colors.insert(PLAYER, { 1, 0.8f, 0.8f });


		//////////////////////////////////////////////////////////////create lava particles
		Motion& player_m = registry.motions.get(PLAYER);
		Player& player_p = registry.players.get(PLAYER);
		vec2 boundary = checkBoundaryPlayer(player_m, player_p);

		particle_system.CreateLavaParticles(LavaParticles, boundary);

		/////////////////////////////////////////////////////////////

		// Map control
		initialize_scale_size(window_height);
		if (mapState == 0)
		{
			map.generateTutorialMapDEMO(window_width, window_height, renderer);
			number = createNumbers(20, window_width, window_height);
		}
		else if (mapState == 1)
		{
			map.generateMap(map1, window_width, renderer);
			number = createNumbers(25, window_width, window_height);
		}
		else if (mapState == 2)
		{

			map.generateMap(map2, window_width, renderer);
			number = createNumbers(50, window_width, window_height);
		}
		else if (mapState == 3)
		{
			map.generateMap(map3, window_width, renderer);
			number = createNumbers(50, window_width, window_height);

		}


		tutorial = createTuButton(renderer, { 75.f, 75.f });
		start = createEmptySuButton();
		//quit = createEmptyQuButton();
		createAxe();
	}
}

// Compute collisions between entities
void WorldSystem::handle_collisions()
{
	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	bool player_horizontal_collision = false;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++)
	{
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other_entity;

		// For now, we are only interested in collisions that involve the player
		if (registry.players.has(entity))
		{
			Player& player = registry.players.get(entity);
			Motion& p_motion = registry.motions.get(entity);
			if (registry.blocks.has(entity_other))
			{
				Block b = registry.blocks.get(entity_other);

				if (b.type == 7 && !registry.deathTimers.has(PLAYER))
				{

					registry.deathTimers.emplace(PLAYER);
					Mix_PlayChannel(-1, salmon_dead_sound, 0);
					particle_system.CreateParticlesWhenDead(p_motion);
					p_motion.velocity *= 0;
					p_motion.velocityGoal *= 0;
					registry.renderRequests.remove(PLAYER);
				}
				else if (b.type == 8)
				{

					//std::vector<Entity> entities_to_remove;
					Gate& gate = registry.gates.get(entity_other);
					//std::cout << "gateid" << gate.id << std::endl;
					for (auto& other_entity : registry.gates.entities)
					{
						Gate& temp_gate = registry.gates.get(other_entity);
						//std::cout << temp_gate.id << std::endl;
						if (temp_gate.id != gate.id && !temp_gate.used) //checking gate
						{
							
							p_motion.position = vec2{ temp_gate.position.x, temp_gate.position.y - BLOCK_BB_HEIGHT / 2 };
							temp_gate.used = true;
							gate.used = true;
							Mix_PlayChannel(-1, teleport, 0);
							if (mapState == 2)                       //if use gate in map 2, set a 5 sec timer to activate lava particles
							{
								LavaTimer = &registry.timer.emplace(PLAYER); 
							}
							break;
						}
						
						player.moveable_down = false;
					}
				}
				else
				{
					Motion& p_motion = registry.motions.get(entity);
					Motion& b_motion = registry.motions.get(entity_other);
					vec2 delta = p_motion.position - b_motion.position;

					//Player& player = registry.players.get(entity);
					if (abs(delta.x) > abs(delta.y))
					{
						// Horizontal collision
						if (delta.x < 0)
						{
							// collision on the right side of `a`
							if (b_motion.position.y - abs(b_motion.scale.y) / 2 < p_motion.position.y + abs(p_motion.scale.y))
							{
								player.moveable_right = false;
								player_horizontal_collision = true;
							}
								

							
						}
						else if (delta.x > 0)
						{
							// collision on the left side of `a`
							// prevent movement to the left
							if (b_motion.position.y - abs(b_motion.scale.y) / 2 < p_motion.position.y + abs(p_motion.scale.y))
							{
								player.moveable_left = false;
								player_horizontal_collision = true;
							}
								
							

						}
					}
					else if (abs(delta.x) < abs(delta.y))
					{
						// Vertical collision
						if (delta.y > 0)
						{
							// collision on the top of `a`
							// prevent upward movement
							//player.moveable_left = false;
							player.moveable_up = false;
							player_horizontal_collision = true;
							
						}
						else if (delta.y < 0)
						{
							// collision on the bottom of `a`
							// prevent downward movement
							//player.moveable_left = false;
							player.moveable_down = false;
							player_horizontal_collision = true;
							
						}
					}
				}
			} 
			
			if (registry.lava.has(entity_other))
			{

				if (!registry.deathTimers.has(entity)) {
					registry.deathTimers.emplace(PLAYER);
					Mix_PlayChannel(-1, salmon_dead_sound, 0);
					Motion& m = registry.motions.get(entity);
					m.velocityGoal *= 0.f;
					m.velocity *= 0.f;
					particle_system.CreateParticlesWhenDead(m);
					registry.renderRequests.remove(PLAYER);


				}
			}
		}
	}

	// Remove all collisions from this simulation step
	registry.collisions.clear();

	if (!player_horizontal_collision)
	{
		Player& player = registry.players.get(PLAYER);
		player.clear_horizontal_movement();
	}
}

// Should the game be over ?
bool WorldSystem::is_over() const
{
	return bool(glfwWindowShouldClose(window)) || game_over;
}

// On key callback
void WorldSystem::on_key(int key, int, int action, int mod)
{
	Game& game = registry.game.get(PLAYER);
	// do nothing if game state is not play
	if (game.state != GameState::PLAY) return;
	if (registry.motions.has(PLAYER) && !registry.deathTimers.has(PLAYER))
	{
		Motion& motion = registry.motions.get(PLAYER);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			if (key == GLFW_KEY_D)
			{
				motion.velocityGoal.x = motion.speed;
				motion.turnRight = true;
				motion.idling = false;
			}
			else if (key == GLFW_KEY_A)
			{
				motion.velocityGoal.x = -motion.speed;
				motion.turnRight = false;
				motion.idling = false;
			}
			else if (key == GLFW_KEY_I)
			{
				motion.idling ^= true;
			}

			else if (key == GLFW_MOUSE_BUTTON_LEFT)
			{
				if (motion.idling)
				{
					motion.attack = true;
					motion.idling = false;
				}
			}
		}

		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_SPACE)
			{
				Player p = registry.players.get(PLAYER);
				if (!p.moveable_down)
				{
					motion.velocity.y = motion.jump_height;
				}
			}
		}

		if (action == GLFW_RELEASE)
		{
			if (key == GLFW_KEY_A || key == GLFW_KEY_D)
			{
				motion.velocityGoal.x = 0.f;
				motion.idling = true;
			}
			else if (key == GLFW_MOUSE_BUTTON_LEFT)
			{
				motion.attack = false;
				motion.idling = true;
			}
		}
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R)
	{
		int w, h;
		glfwGetWindowSize(window, &w, &h);

		restart_game();
	}

	// Debugging
	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_RELEASE)
			debugging.in_debug_mode = false;
		else
			debugging.in_debug_mode = true;
	}

	if (action == GLFW_RELEASE && key == GLFW_KEY_P)
	{
		particle_system.ActivateLavaParticle(LavaParticles);
	}


	// Control the current speed with `<` `>`
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_COMMA)
	{
		current_speed -= 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD)
	{
		current_speed += 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	current_speed = fmax(0.f, current_speed);
}

void WorldSystem::on_mouse_button(int button, int action, int mods)
{
	Game& game = registry.game.get(PLAYER);
	if (game.state == GameState::PLAY) {
		// double m_xpos, m_ypos;
		// glfwGetCursorPos(window, &m_xpos, &m_ypos);
		Button& t_button = registry.buttons.get(tutorial);
		Motion& t_motion = registry.motions.get(tutorial);

		bool is_entered = if_mouse_collides_pos(mouse_pos, t_motion);

		if (is_entered && (!t_button.is_clicked))
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				if (action == GLFW_PRESS)
				{
					t_button.is_clicked = true;
					//window1();
					restart_game();
				}
			}
		}
		else
		{
			if (registry.motions.has(PLAYER))
			{
				Motion& motion = registry.motions.get(PLAYER);

				if (button == GLFW_MOUSE_BUTTON_LEFT)
				{
					if (action == GLFW_PRESS || action == GLFW_REPEAT)
					{
						isLeftMouseButtonPressed = true;
						motion.attack = true;
						motion.idling = false;
						
						
					}
					else if (action == GLFW_RELEASE)
					{
						motion.attack = false;
						motion.idling = true;
						isLeftMouseButtonPressed = false;
						holdTime = 0.f;
					}
				}
			}
		}
	}
	else if (game.state == GameState::START) {
		//Button& s_button = registry.buttons.get(start);
		//Motion& s_motion = registry.motions.get(start);

		//bool is_entered = PhysicsSystem::collides_pos(mouse_pos, s_motion);

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			game.state = GameState::PLAY;
			Mix_PlayMusic(background_music_1, -1);
			restart_game();
		}


	}
}

void WorldSystem::on_mouse_move(vec2 mouse_position)
{
	mouse_pos = mouse_position; // dummy to avoid compiler warning
}

int WorldSystem::get_window_width()
{
	return window_width;
}

int WorldSystem::get_window_height()
{
	return window_height;
}

void WorldSystem::window2()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return;
	}

	// Create an SDL window
	SDL_Window* window = SDL_CreateWindow(
		"Game Over",			// title
		SDL_WINDOWPOS_CENTERED, // x position
		SDL_WINDOWPOS_CENTERED, // y position
		1920,					// width
		1080,					// height
		SDL_WINDOW_SHOWN		// flags
	);

	if (window == nullptr)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	std::string s = data_path() + "/textures/end.bmp";

	SDL_Surface* bmp = SDL_LoadBMP(s.c_str());
	if (bmp == nullptr)
	{
		std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);

	bool running = true;
	SDL_Event e;

	while (running)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				break;
			}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return;
}
