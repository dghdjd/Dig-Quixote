#pragma once
#include <vector>

#include "tiny_ecs.hpp"
#include "components.hpp"

class ECSRegistry
{
	// Callbacks to remove a particular or all entities in the system
	std::vector<ContainerInterface*> registry_list;

public:
	// Manually created list of all components this game has
	// TODO: A1 add a LightUp component
	ComponentContainer<DeathTimer> deathTimers;
	ComponentContainer<Timer> timer;
	ComponentContainer<Motion> motions;
	ComponentContainer<Collision> collisions;
	ComponentContainer<Player> players;
	ComponentContainer<Mesh*> meshPtrs;
	ComponentContainer<RenderRequest> renderRequests;
	ComponentContainer<ScreenState> screenStates;
	ComponentContainer<Block> blocks;
	ComponentContainer<Explosion> explosions;
	ComponentContainer<Explosion1> explosions1;
	ComponentContainer<Button> buttons;
	ComponentContainer<Number> number;
	ComponentContainer<Background> background;
	ComponentContainer<Physics> physics;
	ComponentContainer<PebbleShell> pebbleShells;
	ComponentContainer<ParticleCollision> particleCollisions;

	ComponentContainer<Game> game;
//	ComponentContainer<HardShell> hardShells;
	ComponentContainer<DebugComponent> debugComponents;
	ComponentContainer<vec3> colors;
	ComponentContainer<Gravity> gravities;
	ComponentContainer<Gate> gates;

	// constructor that adds all containers for looping over them
	// IMPORTANT: Don't forget to add any newly added containers!
	ECSRegistry()
	{
		// TODO: A1 add a LightUp component
		registry_list.push_back(&deathTimers);
		registry_list.push_back(&timer);
		registry_list.push_back(&motions);
		registry_list.push_back(&collisions);
		registry_list.push_back(&players);
		registry_list.push_back(&meshPtrs);
		registry_list.push_back(&renderRequests);
		registry_list.push_back(&screenStates);
		registry_list.push_back(&blocks);
		registry_list.push_back(&explosions);
		registry_list.push_back(&explosions1);
		registry_list.push_back(&buttons);
		registry_list.push_back(&background);
		registry_list.push_back(&number);
//		registry_list.push_back(&hardShells);
		registry_list.push_back(&physics);
		registry_list.push_back(&pebbleShells);
		registry_list.push_back(&particleCollisions);
		registry_list.push_back(&debugComponents);
		registry_list.push_back(&colors);
		registry_list.push_back(&gravities);
		registry_list.push_back(&game);
		registry_list.push_back(&gates);
	}

	void clear_all_components() {
		for (ContainerInterface* reg : registry_list)
			reg->clear();
	}

	void list_all_components() {
		printf("Debug info on all registry entries:\n");
		for (ContainerInterface* reg : registry_list)
			if (reg->size() > 0)
				printf("%4d components of type %s\n", (int)reg->size(), typeid(*reg).name());
	}

	void list_all_components_of(Entity e) {
		printf("Debug info on components of entity %u:\n", (unsigned int)e);
		for (ContainerInterface* reg : registry_list)
			if (reg->has(e))
				printf("type %s\n", typeid(*reg).name());
	}

	void remove_all_components_of(Entity e) {
		for (ContainerInterface* reg : registry_list)
			reg->remove(e);
	}
};

extern ECSRegistry registry;