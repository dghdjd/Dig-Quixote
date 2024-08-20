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
	static bool collides_pos(const vec2 pos, const Motion& motion2);
	static bool collides(const Motion& motion1, const Motion& motion2);
	static bool collidesVertically(const Motion& motion1, const Motion& motion2);
	bool point_in_circle(const vec2 &point, const vec2 &circle_center, float circle_radius);
	bool edge_intersects_circle(const vec2 &edge_start, const vec2 &edge_end, const vec2 &circle_center, float circle_radius);
	bool collides_with_polygon(const Motion &motion1, const std::vector<vec2> &vertices);
	void handleParticleCollision();
	vec2 checkBoundaryPlayer(const Motion& motion, Player& player);
	void checkCollisionInChunks(std::vector<std::vector<std::vector<Entity>>>& grid, int y, int x, bool IsTutorial, vec2 boundary);
	void parallelCollisionSetup(std::vector<std::vector<std::vector<Entity>>>& grid, bool IsTutorial, vec2 boundary);
	void worker(std::vector<std::vector<std::vector<Entity>>>& grid, bool IsTutorial, vec2 boundary, std::queue<std::pair<int, int>>& queue, int workerid);


	PhysicsSystem()
	{
	}
private:
	Entity PLAYER;
};