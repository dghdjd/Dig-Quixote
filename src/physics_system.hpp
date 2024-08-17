#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"

// A simple physics system that moves rigid bodies and checks for collision
class PhysicsSystem
{
public:
	void step(float elapsed_ms, std::vector<std::vector<std::vector<Entity>>> grid, int flag);
	static bool collides_pos(const vec2 pos, const Motion& motion2);
	static bool collides(const Motion& motion1, const Motion& motion2);
	static bool collidesVertically(const Motion& motion1, const Motion& motion2);
	bool point_in_circle(const vec2 &point, const vec2 &circle_center, float circle_radius);
	bool edge_intersects_circle(const vec2 &edge_start, const vec2 &edge_end, const vec2 &circle_center, float circle_radius);
	bool collides_with_polygon(const Motion &motion1, const std::vector<vec2> &vertices);
	void handleParticleCollision();
	vec2 checkBoundaryPlayer(const Motion& motion, Player& player);

	PhysicsSystem()
	{
	}
private:
	Entity PLAYER;
};