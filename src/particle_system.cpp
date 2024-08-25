#include "particle_system.hpp"
#include "physics_system.hpp"
#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

void ParticleSystem::CreateHundredsParticles(vec2 position, int amount)
{
	for (int i = 0; i < amount; i++)
	{
		Entity particle;
		particle = createParticle(position, particle_size, 0, 0);
		registry.colors.insert(particle, WhiteParticles);
	}
}

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
	int particle_num = 5;

	for (int i = 0; i < particle_num; i++)
	{
		particle = createParticle(newPosition, particle_size, rotationAngle, i - 1);
		registry.colors.insert(particle, color);
	}
}


void ParticleSystem::CreateParticlesWhenBroken(const Motion& blockM, vec3 color)
{
	Entity particle;

	int offset1 = 5;
	int offset = BLOCK_BB_WIDTH / offset1;
	float x = blockM.position.x - blockM.scale.x / 2;
	float y = blockM.position.y - blockM.scale.y / 2;
	vec2 position = { x,y };
	vec2 new_position = { 0,0 };
	for (int i = 0; i < offset1; i++)
	{
		new_position.y = position.y + offset * i;
		new_position.x = position.x;
		for (int j = 0; j < offset1; j++)
		{
			new_position.x = position.x + offset * j;
			particle = createParticle(new_position, particle_size, 0, 0);
			registry.colors.insert(particle, color);
		}
	}
}

void ParticleSystem::CreateParticlesWhenDead(const Motion& playerM)
{
	Entity particle;

	int offset = 2;  // Distance between particles
	float x = playerM.position.x;  // Adjusted starting x position
	float y = playerM.position.y - playerM.scale.y / 2;  // Adjusted starting y position

	vec2 position = { x,y };
	vec2 new_position = { 0,0 };


	// upper body
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 8; j++) {
			new_position.x = x + j;
			new_position.y = y + i * offset;
			if (i <= 2 && (j < 2 || j > 5)) continue;  //head
			particle = createParticle(new_position, { 2.f, 2.f }, 1, 0);
			registry.colors.insert(particle, RedParticles);
		}
	}


	// Legs
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			new_position.x = x + 1 + j ;
			new_position.y = y + 10 * offset + i * offset;
			if (j >= 2 && j <= 3) continue;   //gap
			particle = createParticle(new_position, { 2.f, 2.f }, 1, 0);
			registry.colors.insert(particle, RedParticles);


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

void ParticleSystem::ActivateLavaParticle(std::vector<Entity>& particles) {
	for (auto& particle : particles) {
		Motion& m = registry.motions.get(particle);
		m.isActive = true; // Activate the particle
	}
}

void ParticleSystem::CreateLavaParticles(std::vector<Entity>& lava, vec2 boundary)
{
	
	int particle_num_per_column = 200;
	int columns = 4;


	float pSize = 3.5 * 2;
	vec2 pos = { 800.f, -pSize };

	for (int i = 0; i < particle_num_per_column; i++)
	{
		pos.y -= pSize/2;
		pos.x = boundary.y - (2 * BLOCK_BB_WIDTH); //create lava particles at about the second block from the right
		for (int j = 0; j < columns; j++)
		{
			pos.x += pSize/2;
			lava.push_back(createLavaParticles(pos, { 2,2 }, 0.0, 0.0));
		}

	}
}