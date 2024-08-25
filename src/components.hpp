#pragma once
#include "common.hpp"
#include <vector>
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"

// Player component
struct Player
{
	bool horizontally_colliding = false;
	bool moveable_right = true;
	bool moveable_left = true;
	bool moveable_up = true;
	bool moveable_down = false;

	bool moving_right = false;
	bool moving_left =  false;

	int idle_state = 0;
	int max_idle_state = 4;

	int run_state = 0;
	int max_run_state = 8;

	int jump_state = 2;
	int max_jump_state = 8;

	int attack_state = 0;
	int max_attack_state = 14;

	bool win = false;

	void clear_horizontal_movement()
	{
		this->moveable_right = true;
		this->moveable_left = true;
	}
};

// Basic blocks
struct Block
{
	int type;

	int state = 0;
	int maxState = 8;

	bool explode = false;
	bool exploded = false;
};

struct Number
{
	int digLeft;
};

struct Background
{
	float width;
	float height;
	bool isDrawn;

};

struct MeshBlock
{

};

struct Explosion
{
	int range;
};

struct Explosion1
{

};


struct EntityHash {
    std::size_t operator()(const Entity& entity) const {
        return std::hash<unsigned int>()(static_cast<unsigned int>(entity));
    }
};

// Button entity for tutorial and future
struct Button
{
	bool is_clicked = false;
};


// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0.f, 0.f };
	float angle = 0.f;
	vec2 velocity = { 0.f, 0.f };
	vec2 velocityGoal = { 0.f, 0.f };
	vec2 scale = { 0.f, 0.f };
	bool turnRight = true;
	bool idling = true;
	bool attack = false;
	int mass = 1.f;
	float speed = 150.f;
	float jump_height = -350.f;

	bool isblock = false;
	bool isPar = false;
	bool isActive = true;
	bool isHorizontal = false;
};

// all relevant to gravity
struct Gravity {
	float mass;
	float velocity_y;
    float acceleration_y;
	bool moveable_down;
};
struct Particle
{
	float gravity = 98.f / 2.f;
	float timer = 3000.f;
	bool collision = false;

};
struct Lava
{
	bool ifHit = false;
};

struct Gate
{
	vec2 position;
	int id;
	bool used = false;
};

struct ParticleCollision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other_entity; // the second object involved in the collision
	ParticleCollision(Entity& other_entity) { this->other_entity = other_entity; };
};

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other_entity; // the second object involved in the collision
	Collision(Entity& other_entity) { this->other_entity = other_entity; };
};

// Data structure for toggling debug mode
struct Debug {
	bool in_debug_mode = 0;
	bool in_freeze_mode = 0;
};
extern Debug debugging;

enum class GameState {
	START = 0,
	PLAY = 1,
	GAMEOVER = 2,
};

// Sets the brightness of the screen
struct Game {
	GameState state = GameState::START;
};

enum class GameStateChange {
	MENU = 0,
	PLAY = MENU + 1,
};

// Sets the brightness of the screen
struct ScreenState
{
	float screen_darken_factor = -1;
};

// A struct to refer to debugging graphics in the ECS
struct DebugComponent
{
	// Note, an empty struct has size 1
};

// A timer that will be associated to dying salmon
struct DeathTimer
{
	float timer_ms = 3000.f;
};

struct Timer
{
	float timer_ms = 10000.f;
};

// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & salmon.vs.glsl)
struct ColoredVertex
{
	vec3 position;
	vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
	vec3 position;
	vec2 texcoord;
};

// Mesh datastructure for storing vertex and index buffers
struct Mesh
{
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	std::vector<vec2> Mesh::extractPolygonVertices()const;
	vec2 original_size = {1,1};
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

/**
 * The following enumerators represent global identifiers refering to graphic
 * assets. For example TEXTURE_ASSET_ID are the identifiers of each texture
 * currently supported by the system.
 *
 * So, instead of referring to a game asset directly, the game logic just
 * uses these enumerators and the RenderRequest struct to inform the renderer
 * how to structure the next draw command.
 *
 * There are 2 reasons for this:
 *
 * First, game assets such as textures and meshes are large and should not be
 * copied around as this wastes memory and runtime. Thus separating the data
 * from its representation makes the system faster.
 *
 * Second, it is good practice to decouple the game logic from the render logic.
 * Imagine, for example, changing from OpenGL to Vulkan, if the game logic
 * depends on OpenGL semantics it will be much harder to do the switch than if
 * the renderer encapsulates all asset data and the game logic is agnostic to it.
 *
 * The final value in each enumeration is both a way to keep track of how many
 * enums there are, and as a default value to represent uninitialized fields.
 */
namespace SpriteSheetWidths {
	constexpr float PLAYER_IDLE = 640.0f;
	constexpr float PLAYER_RUN  = 1024.0f;
	constexpr float PLAYER_JUMP = 1024.0f;
	constexpr float PLAYER_ATTACK = 1792.0f;
	constexpr float WATER = 512.f;
	constexpr float PORTAL = 9960.f;
	constexpr float LIGHTING = 2048.f;
}

enum class TEXTURE_ASSET_ID {
	BLOCK			=		0,
	TURTLE			=		BLOCK + 1,
	PLAYER_IDLE		=		TURTLE + 1,
	PLAYER_RUN		=		PLAYER_IDLE + 1,
	PLAYER_JUMP		=		PLAYER_RUN + 1,
	PLAYER_ATTACK	=		PLAYER_JUMP + 1,
	RESET			=		PLAYER_ATTACK + 1,
	STRAT			=		RESET + 1,
	QUIT			=		STRAT + 1,
	STONE			=		QUIT + 1,
	GRASS			=		STONE + 1,
	IRON			=		GRASS + 1,
	CRYSTAL			=		IRON + 1,
	BACKUI			=		CRYSTAL + 1,
	NUMBERS			=		BACKUI + 1,
	AXE				=		NUMBERS + 1,
	LAVA			=		AXE + 1,
	GATE 			=		LAVA + 1,
	T_LEFT			=		GATE + 1,
	T_RIGHT         =		T_LEFT + 1,
	T_JUMP			=		T_RIGHT + 1,
	T_DIG			=		T_JUMP + 1,
	CLOUD			=		T_DIG + 1,
	C_BACKGROUND	=		CLOUD + 1,
	BG_02			=		C_BACKGROUND + 1,
	BG_03			=		BG_02 + 1,
	BG_04			=		BG_03 + 1,
	BG_05			=		BG_04 + 1,
	ARROW			=		BG_05 + 1,
	ARROW_L			=		ARROW + 1,
	ARROW_R			=		ARROW_L + 1,
	PORTAL          =       ARROW_R + 1,
	LIGHTING        =       PORTAL + 1,
	LIGHTINGICON    =       LIGHTING + 1,
	TEXTURE_COUNT	=		LIGHTINGICON + 1
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
	COLOURED = 0,
	PARTICLE = COLOURED + 1,
	SALMON = PARTICLE + 1,
	TEXTURED = SALMON + 1,
	WATER = TEXTURED + 1,
	EFFECT_COUNT = WATER + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	SALMON = 0,
	SPRITE           = SALMON + 1,
	PLAYER_IDLE      = SPRITE + 1,
	PLAYER_RUN       = PLAYER_IDLE + 1,
	PLAYER_JUMP      = PLAYER_RUN + 1,
	PLAYER_ATTACK    = PLAYER_JUMP + 1,
	PARTICLE           = PLAYER_ATTACK + 1,
	DEBUG_LINE       = PARTICLE + 1,
	SCREEN_TRIANGLE  = DEBUG_LINE + 1,
	MESH_BLOCK       = SCREEN_TRIANGLE + 1,
	WATER            = MESH_BLOCK + 1,
	NUMBERS          = WATER + 1,
	PORTAL           = NUMBERS + 1,
	LIGHTING         = PORTAL + 1,
	GEOMETRY_COUNT   = LIGHTING + 1
};

const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};

