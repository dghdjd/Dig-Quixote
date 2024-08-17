#include "particle_system.hpp"
#include "physics_system.hpp"
#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

void ParticleSystem::CreateParticlesAtCollisionEdge(const Motion& playerM, const Motion& blockM, float random, vec3 color)
{
	glm::vec2 newPosition = { 0.f, 0.f };
	float offset = 2.f;
	float rotationAngle = 0.0f; // Default is facing right

	if (playerM.position.y < blockM.position.y - blockM.scale.y / 2)
	{
		// Collision on the top side of the block
		newPosition = { blockM.position.x, blockM.position.y - blockM.scale.y / 2 - offset };
		rotationAngle = -M_PI / 2; // 90 degrees, facing up
	}
	else if (playerM.position.y > blockM.position.y + blockM.scale.y / 2)
	{
		// Collision on the bottom side of the block
		newPosition = { blockM.position.x, blockM.position.y + blockM.scale.y / 2 + offset };
		rotationAngle = M_PI / 2; // -90 degrees, facing down
	}
	else if (playerM.position.x < blockM.position.x - blockM.scale.x / 2)
	{
		// Collision on the left side of the block
		newPosition = { blockM.position.x - blockM.scale.x / 2 - offset, blockM.position.y };
		rotationAngle = M_PI; // 180 degrees, facing left
	}
	else if (playerM.position.x > blockM.position.x + blockM.scale.x / 2)
	{
		// Collision on the right side of the block
		newPosition = { blockM.position.x + blockM.scale.x / 2 + offset, blockM.position.y };
		// rotationAngle remains 0.0f, facing right
	}

	// Create particles at the new position with the calculated rotation
	Entity particle;
	for (int i = 0; i < 5; i++)
	{
		particle = createParticle(newPosition, particle_size, rotationAngle, i - 1);
		registry.colors.insert(particle, color);
	}
}


void ParticleSystem::CreateParticlesWhenBroken(const Motion& blockM, vec3 color)
{
	Entity particle;
	int offset = BLOCK_BB_WIDTH / 10;
	float x = blockM.position.x - blockM.scale.x / 2;
	float y = blockM.position.y - blockM.scale.y / 2;
	vec2 position = { x,y };
	vec2 new_position = { 0,0 };
	for (int i = 0; i < 10; i++)
	{
		new_position.y = position.y + offset * i;
		new_position.x = position.x;
		for (int j = 0; j < 10; j++)
		{
			new_position.x = position.x + offset * j;
			particle = createParticle(new_position, particle_size, 0, 0);
			registry.colors.insert(particle, color);
		}
	}
}

void ParticleSystem::CreateParticles(const Motion& blockM)
{
	float offset = 2.f;
	glm::vec2 newPosition = { blockM.position.x - blockM.scale.x / 2, blockM.position.y };
	for (int i = 0; i < 5; i++)
	{
		Entity particle;
		particle = createParticle(newPosition + i * offset * 2, { 3.5, 3.5 }, 0.f, i - 1);

		registry.colors.insert(particle, vec3{ 0,1,1 });
	}
}

void ParticleSystem::ActivateParticle() {
	for (auto& particle : particles) {
		Motion& m = registry.motions.get(particle);
		m.isActive = true; // Activate the particle
	}
}