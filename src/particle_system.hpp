#pragma once
#include "physics_system.hpp"


class ParticleSystem{
public:

	void CreateParticlesAtCollisionEdge(const Motion& playerM, const Motion& blockM, float random, vec3 color);

	void CreateParticlesWhenBroken(const Motion& blockM, vec3 color);

	void CreateParticles(const Motion& blockM);

	void ActivateParticle();

	


private:
	std::vector<Entity> particles;
	const vec2 particle_size = { 3.5, 3.5 };

};