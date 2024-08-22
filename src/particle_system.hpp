#pragma once
#include "physics_system.hpp"


class ParticleSystem{
public:

	//colors

	vec3 BrownParticles = { 139.0 / 255.0, 69.0 / 255.0, 19.0 / 255.0 };
	vec3 YellowParticles = { 1.0, 1.0, 0.f };
	vec3 BlueParticles = { 0.0, 1.0, 1.0 };
	vec3 WhiteParticles = { 1.0, 1.0, 1.0 };

	void CreateParticlesAtCollisionEdge(const Motion& playerM, const Motion& blockM, float random, vec3 color);

	void CreateParticlesWhenBroken(const Motion& blockM, vec3 color);

	void CreateParticles(const Motion& blockM);

	void ActivateLavaParticle(std::vector<Entity>& particles);

	void CreateHundredsParticles(vec2 position, int amount);


	


private:
	std::vector<Entity> particles;
	const vec2 particle_size = { 3.5, 3.5 };

};