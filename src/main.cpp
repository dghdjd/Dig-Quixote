
#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>

// internal
#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"

using Clock = std::chrono::high_resolution_clock;

// Entry point
int main()
{
	// Global systems
	WorldSystem world_system;
	RenderSystem render_system;
	PhysicsSystem physics_system;

	// Initializing window
	GLFWwindow* window = world_system.create_window();
	if (!window) {
		// Time to read the error message
		printf("Press any key to exit");
		getchar();
		return EXIT_FAILURE;
	}



	// initialize the main systems
	render_system.init(window);
	world_system.init(&render_system);

	// variable timestep loop
	auto t = Clock::now();
	while (!world_system.is_over()) {
		// Processes system messages, if this wasn't present the window would become unresponsive
		glfwPollEvents();

		// Calculating elapsed times in milliseconds from the previous iteration
		auto now = Clock::now();
		float elapsed_ms =
			(float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
		t = now;
		int flag;
		std::vector<std::vector<std::vector<Entity>>> list;
		list = world_system.step(elapsed_ms,flag);

		
		physics_system.step(elapsed_ms, list, flag);

		world_system.handle_collisions();

		render_system.draw(elapsed_ms);


		// Check for a game-over condition
		if (world_system.check_game_over_condition()) {
			world_system.set_game_over();
			// Create a new GLFW window (replace these parameters with your requirements)
			world_system.window2();
		}

	}

	return EXIT_SUCCESS;
}
