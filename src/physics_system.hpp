#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"
#include <queue>

// A simple physics system that moves rigid bodies and checks for collision
class PhysicsSystem
{
public:
	void step(float elapsed_ms, std::vector<std::vector<std::vector<Entity>>>& grid, bool IsTutorial);

	void checkCollisionInChunks(std::vector<std::vector<std::vector<Entity>>>& grid, int y, int x, bool IsTutorial, vec2 boundary);

	void parallelCollisionSetup(std::vector<std::vector<std::vector<Entity>>>& grid, bool IsTutorial, vec2 boundary);

	void worker(std::vector<std::vector<std::vector<Entity>>>& grid, bool IsTutorial, vec2 boundary, std::queue<std::pair<int, int>>& queue, int workerid);


	PhysicsSystem()
	{
	}
private:
	Entity PLAYER;
};