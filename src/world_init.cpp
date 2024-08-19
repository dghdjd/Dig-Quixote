#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

#include <iostream>
#include<random>

Entity createPlayer(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();

	// // Store a reference to the potentially re-used mesh object
	// Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SALMON);
	// registry.meshPtrs.emplace(entity, &mesh);

	// Store a reference to the potentially re-used mesh object
	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = {0.f, 0.f};
	motion.mass = 100.f;

	Game& game = registry.game.emplace(entity);
	game.state = GameState::PLAY;

	motion.scale = (mesh.original_size * 90.f) / 2.5f;

	// motion.scale = vec2({ PLAYER_BB_WIDTH, PLAYER_BB_HEIGHT });
	motion.scale.x *= -1; // point front to the right

	registry.players.emplace(entity);
	//Gravity &grav = registry.gravities.emplace(entity);
	//grav.mass = 10.f;
	//grav.velocity_y = 0.f;
	//grav.acceleration_y = 0.f;



	registry.renderRequests.insert(
		entity,
		{TEXTURE_ASSET_ID::PLAYER_IDLE, // TEXTURE_COUNT indicates that no txture is needed
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::PLAYER_IDLE});

	return entity;
}

Entity createEmptySuButton() {
	auto entity = Entity();
	auto& motion = registry.motions.emplace(entity);
	registry.buttons.emplace(entity);
	return entity;

}

Entity createEmptyQuButton() {
	auto entity = Entity();
	auto& motion = registry.motions.emplace(entity);
	registry.buttons.emplace(entity);
	return entity;

}

Entity createEmptyPlayer(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);
	Game& game = registry.game.emplace(entity);
	Motion& motion = registry.motions.emplace(entity);
	//Gravity grav = registry.gravities.emplace(entity);
	game.state = GameState::START;
	registry.players.emplace(entity);
	return entity;

}

Entity createBlock(RenderSystem *renderer, vec2 position, int type)
{
	// Reserve en entity
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	// Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	// registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the position, scale, and physics components
	Motion &motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = {0.f, 0.f};
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({BLOCK_BB_WIDTH, BLOCK_BB_HEIGHT});
	motion.mass = 80.f;
	motion.isblock = true;
	// Create an (empty) block component to be able to refer to all block
	Block &block = registry.blocks.emplace(entity);
	block.type = type;
	//registry.pebbleShells.emplace(entity);

	if (type == 2)
	{
		registry.renderRequests.insert(
			entity,
			{TEXTURE_ASSET_ID::BLOCK,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE});
	}
	else if (type == 1)
	{
		registry.renderRequests.insert(
			entity,
			{TEXTURE_ASSET_ID::STONE,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE});
		
			Gravity& grav = registry.gravities.emplace(entity);
			grav.mass = 10.f;
			grav.velocity_y = 0.f;
			grav.acceleration_y = 0.f;
			grav.moveable_down = false;
	}
	else if (type == 3)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::STONE,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == 6)
	{
		auto &exp = registry.explosions.emplace(entity);
		exp.range = 1;

		registry.renderRequests.insert(
			entity,
			{TEXTURE_ASSET_ID::LIGHTINGICON,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE});

	}
	else if (type == 7){

		registry.renderRequests.insert(
			entity,
			{TEXTURE_ASSET_ID::LAVA,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE});
	}
  else if (type == 8){

		registry.renderRequests.insert(
			entity,
			{TEXTURE_ASSET_ID::PORTAL,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::PORTAL});

		Gate &gate = registry.gates.emplace(entity);
		std::random_device rd;  
    	std::mt19937 gen(rd()); 
    	std::uniform_int_distribution<> distrib(1, 1000);
		block.maxState = 20;
    	int random_number = distrib(gen);
		gate.id = random_number;
		gate.position = motion.position;
		std::cout<<gate.id<<std::endl;
	}
	else if (type == 9)
	{
		registry.renderRequests.insert(
			entity,
			{TEXTURE_ASSET_ID::GRASS,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE});
	}
	else if (type == 992)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::ARROW_L,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		block.type = 2;
	}
	else if (type == 993)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::ARROW_R,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		block.type = 2;
	}
	else if (type == 994)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::ARROW,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		block.type = 2;
	}
	else if (type == 995)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CLOUD,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		block.type = 2;
	}
	else if (type == 996)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::T_LEFT,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		motion.scale = motion.scale * vec2(-2, 2);
		block.type = 1;
	}
	else if (type == 997)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::T_RIGHT,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		motion.scale = motion.scale * vec2(-2, 2);
		block.type = 1;
	}
	else if (type == 998)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::T_DIG,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		motion.scale = motion.scale * vec2(2, 2);
		block.type = 2;
	}
	else if (type == 999)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::T_JUMP,
			 EFFECT_ASSET_ID::TEXTURED,
			 GEOMETRY_BUFFER_ID::SPRITE });

		motion.scale = motion.scale * vec2(-2, 2);
		block.type = 1;
	}

	return entity;
}


Entity createTuButton(RenderSystem *renderer, vec2 position)
{
	auto entity = Entity();

	auto &motion = registry.motions.emplace(entity);
	motion.position = position;

	motion.scale = vec2({-BLOCK_BB_WIDTH, BLOCK_BB_HEIGHT});
	motion.scale *= vec2(2, 2);

	registry.buttons.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{TEXTURE_ASSET_ID::RESET,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE});

	return entity;
}

Entity createSuButton(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();


	auto& motion = registry.motions.emplace(entity);
	motion.position = position;

	motion.scale = vec2({ -BUTTON_BB_WIDTH * 4, BUTTON_BB_HEIGHT });

	registry.buttons.emplace(entity);
	/*
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::STRAT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	*/
	return entity;
}

Entity createQuButton(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	// registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.position = position;

	motion.scale = vec2({ -BUTTON_BB_WIDTH, BUTTON_BB_HEIGHT });

	registry.buttons.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::QUIT,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createMeshBlock(RenderSystem *renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh &mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::MESH_BLOCK);
	registry.meshPtrs.emplace(entity, &mesh);

	// std::cout << "worldinit.cpp Entity created with Mesh having " << mesh.vertices.size() << " vertices." << std::endl;

	// Setting initial motion values
	Motion &motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = {0.f, 0.f};

	motion.scale = vec2(mesh.original_size* 0.05f);
	motion.mass = 2.f;

	//motion.scale = vec2({ BLOCK_BB_WIDTH, BLOCK_BB_HEIGHT });

	Gravity &grav = registry.gravities.emplace(entity);
	grav.mass = 1.f;
	grav.velocity_y = 0.f;
	grav.acceleration_y = 0.f;

	auto& block = registry.blocks.emplace(entity);
	block.type = 666;

	registry.renderRequests.insert(
		entity,
		{TEXTURE_ASSET_ID::IRON,
		 EFFECT_ASSET_ID::SALMON,
		 GEOMETRY_BUFFER_ID::MESH_BLOCK	});

	return entity;
}

Entity createLine(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::PARTICLE,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE});

	// Create motion
	Motion &motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = {0.f, 0.f};
	motion.position = position;
	motion.scale = scale;

	registry.debugComponents.emplace(entity);
	return entity;
}

Entity createParticle(vec2 pos, vec2 size, float angle, float random)
{
	auto entity = Entity();
	registry.particle.emplace(entity);
	Particle& particleSystem = registry.particle.get(entity);
	float gravity = particleSystem.gravity;

	float bubble_speed = 5.f;
	float bubble_angle = (random / 2) * (M_PI / 4); // [-pi/4, pi/4] 


	//std::cout << "Random number: " << bubble_angle << std::endl;

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);

	float combined_angle = bubble_angle / 2.f + angle;
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { bubble_speed * cos(combined_angle), gravity * sin(combined_angle) };
	//motion.velocity = { bubble_speed , 0.f };
	motion.scale = size;
	motion.mass = 3.5;
	motion.isPar = true;

	// Create and (empty) Salmon component to be able to refer to all turtles
	//registry.hardShells.emplace(entity);
	//registry.renderRequests.insert(
	//	entity,
	//	{ TEXTURE_ASSET_ID::TEXTURE_COUNT, // TEXTURE_COUNT indicates that no txture is needed
	//		EFFECT_ASSET_ID::PARTICLE,
	//		GEOMETRY_BUFFER_ID::PARTICLE });

	return entity;
}

Entity createLavaParticles(vec2 pos, vec2 size, float angle, float random)
{
	auto entity = Entity();
	registry.particle.emplace(entity);
	registry.lava.emplace(entity);
	Particle& particleSystem = registry.particle.get(entity);
	float gravity = particleSystem.gravity;

	float bubble_speed = 5.f;
	float bubble_angle = (1 / 2) * (M_PI / 4); // [-pi/4, pi/4] 

	//std::cout << "Random number: " << bubble_angle << std::endl;

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	float combined_angle = bubble_angle / 2.f + M_PI / 2;


	motion.position = pos;
	motion.angle = 0.f;
	//motion.velocity = { 0.f, gravity };
	//motion.velocity = { 0.f,0.f };
	//motion.velocity = { bubble_speed , 0.f };
	motion.velocity = { bubble_speed * cos(combined_angle), gravity * sin(combined_angle) };
	motion.scale = size * 2.f;
	motion.mass = 3.5f;
	motion.isPar = true;
	motion.isActive = false;


	// Create and (empty) Salmon component to be able to refer to all turtles
	//registry.hardShells.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::PARTICLE,
			GEOMETRY_BUFFER_ID::PARTICLE });

	return entity;
}

Entity createBackground(int width, int height, int type)
{
	auto entity = Entity();
	Motion& motion = registry.motions.emplace(entity);
	Background b = registry.background.emplace(entity);
	motion.position = { width / 2, height / 2 };
	motion.scale = vec2{ -width, height };
	
	if (type == 0)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::C_BACKGROUND,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == 1)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::BG_02,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == 2)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::BG_03,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == 3)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::BG_04,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
	}
	else if (type == 4)
	{
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::BG_05,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE });
	}

	return entity;

}

Entity createUIBackground(int width, int height)
{
	auto entity = Entity();
	Motion& motion = registry.motions.emplace(entity);
	Background b = registry.background.emplace(entity);
	motion.position = { width / 2, height / 2 };
	motion.scale = vec2{ -width, height };
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BACKUI,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });

	return entity;

}
Entity createNumbers(int start, int width, int height)
{
	auto entity = Entity();
	Motion& motion = registry.motions.emplace(entity);
	Number& number = registry.number.emplace(entity);
	number.digLeft = start;
	motion.position = vec2{ 225.f, 75.f };
	motion.scale = vec2({ -BLOCK_BB_WIDTH, BLOCK_BB_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::NUMBERS,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::NUMBERS });
	return entity;
}

Entity createAxe()
{
	auto entity = Entity();
	Motion& motion = registry.motions.emplace(entity);
	motion.position = vec2{ 175.f, 75.f };
	motion.scale = vec2({ -BLOCK_BB_WIDTH, BLOCK_BB_HEIGHT });
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::AXE,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });
	return entity;
}