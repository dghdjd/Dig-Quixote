#pragma once

#include <array>
#include <utility>

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"
#include <iostream>

// System responsible for setting up OpenGL and for rendering all the
// visual entities in the game
class RenderSystem {
	/**
	 * The following arrays store the assets the game will use. They are loaded
	 * at initialization and are assumed to not be modified by the render loop.
	 *
	 * Whenever possible, add to these lists instead of creating dynamic state
	 * it is easier to debug and faster to execute for the computer.
	 */
	std::array<GLuint, texture_count> texture_gl_handles;
	std::array<ivec2, texture_count> texture_dimensions;

	// Make sure these paths remain in sync with the associated enumerators.
	// Associated id with .obj path
	const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
	{
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::SALMON, mesh_path("salmon.obj")),
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::MESH_BLOCK, mesh_path("mesh_block.obj"))
		  // specify meshes of other assets here
	};

	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, texture_count> texture_paths = {
			textures_path("dirt.png"),
			textures_path("turtle.png"),
			textures_path("player.png"),
			textures_path("Run.png"),
			textures_path("Jump.png"),
			textures_path("attack.png"),
			textures_path("t_reset.png"),
			textures_path("start.png"),
			textures_path("quit.png"),
			textures_path("gravel.png"),
			textures_path("grass.png"),
			textures_path("iron.jpg"),
			textures_path("Star.png"), //star
			textures_path("ui_back.png"),
			textures_path("numbers1.png"),
			textures_path("Pickaxe.png"),
			textures_path("lava.png"),
			textures_path("gate.png"),
			textures_path("t_left.png"),
			textures_path("t_right.png"),
			textures_path("t_jump.png"),
			textures_path("t_dig.png"),
			textures_path("cloud.jpg"),
			textures_path("bg_01.jpg"),
			textures_path("bg_02.png"),
			textures_path("bg_03.png"),
			textures_path("bg_04.png"),
			textures_path("bg_05.png"),
			textures_path("arrow.png"),
			textures_path("arrow_l.png"),
			textures_path("arrow_r.png"),
			textures_path("protal.png"),
			textures_path("spritesheet.png"),
			textures_path("Icon9.png"),


	};

	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("particle"),
		shader_path("salmon"),
		shader_path("textured"),
		shader_path("water") };

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;

public:
	// Initialize the window
	bool init(GLFWwindow* window);

	template <class T>
	void bindVBOandIBO(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices);

	void updateBlockAnimation(GLuint vbo, GLuint ibo, float elapsed_ms, float duration, Block& block, GEOMETRY_BUFFER_ID bufferID, float width, int& currentState, int maxState);
	void updatePortalAnimation(GLuint vbo, GLuint ibo, float elapsed_ms, float duration, Block& block, GEOMETRY_BUFFER_ID bufferID, float width, int& currentState, int maxState);

	void updatePlayerAnimation(GLuint vbo, GLuint ibo, float elapsed_ms, float duration, Player& player, GEOMETRY_BUFFER_ID bufferID, float width, int& currentState, int maxState, bool isJump);
	void setupPlayerAnimation(float width, GEOMETRY_BUFFER_ID bufferID); //new fun
	void setupNumbers(); //new fun
	void updateNumbers(int start, GLuint vbo, GLuint ibo, GEOMETRY_BUFFER_ID bufferID);
	void updateExplosion(GLuint vbo, GLuint ibo, float elapsed_ms, float duration, Block& block, GEOMETRY_BUFFER_ID bufferID, float width, int& currentState, int maxState);
	void initializeGlTextures();

	void initializeGlEffects();

	void initializeGlMeshes();
	Mesh& getMesh(GEOMETRY_BUFFER_ID id) { 
		//std::cout << "Rendersys.hpp Requested mesh for ID: " << (int)id << ". Vertex count: " << meshes[(int)id].vertices.size() << std::endl;
		return meshes[(int)id]; 
		};

	void initializeGlGeometryBuffers();
	// Initialize the screen texture used as intermediate render target
	// The draw loop first renders to this texture, then it is used for the water
	// shader
	bool initScreenTexture();

	// Destroy resources associated to one or all entities created by the system
	~RenderSystem();

	// Draw all entities
	void draw(float elapsed_ms);

	mat3 createProjectionMatrix(int width, int height);


private:
	// Internal drawing functions for each entity type
	void drawInstancedParticles(Entity entity, const mat3& projection, float elapsed_ms);
	void drawTexturedMesh(Entity entity, const mat3& projection, float elapsed_ms);
	void drawToScreen();
	float time = 0.0f;
	float time1 = 0.0f;
	float time2 = 0.0f;
	bool changed = false;
	bool changed1 = false;
	// Window handle
	GLFWwindow* window;

	// Screen texture handles
	GLuint frame_buffer;
	GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

	Entity screen_state_entity;
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, GLuint& out_program);
