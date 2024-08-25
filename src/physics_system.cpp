// internal
#include "physics_system.hpp"
#include "physics_calculation.hpp"
#include "world_init.hpp"
#include "world_system.hpp"
#include "iostream"
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>




float lerp(float current, float goal, float delta)
{
	if (current < goal)
	{
		current += delta;
		if (current > goal)
		{
			return goal;
		}
	}
	else if (current > goal)
	{
		current -= delta;
		if (current < goal)
		{
			return goal;
		}
	}
	return current;
}

void PhysicsSystem::checkCollisionInChunks(std::vector<std::vector<std::vector<Entity>>>& grid, int y, int x, bool IsTutorial, vec2 boundary)
{
	//for (int y = 0; y < grid.size(); y++) {      // Iterate over rows
		//for (int x = 0; x < grid.size(); x++) {  // Iterate over columns in each row
			// Access the cell at (x, y)
	std::vector<Entity>& cell = grid[y][x];

	for (int i = 0; i < cell.size(); i++)
	{

		if (!registry.motions.has(cell[i])) continue;
		Entity& entity_i = cell[i];
		Motion& motion_i = registry.motions.get(cell[i]);
		bool is_playeri = registry.players.has(cell[i]);
		bool is_blocki = registry.blocks.has(cell[i]);
		bool is_rock1 = registry.gravities.has(cell[i]);
		bool is_particlei = registry.particle.has(cell[i]);
		bool is_lavai = registry.lava.has(cell[i]);

		for (int j = 0; j < cell.size(); j++)
		{
			if (!registry.motions.has(cell[j]) || i == j) continue;
			Motion& motion_j = registry.motions.get(cell[j]);
			Entity& entity_j = cell[j];
			bool is_blockj = registry.blocks.has(cell[j]);
			bool is_particlej = registry.particle.has(cell[j]);
			bool is_lavaj = registry.lava.has(cell[j]);
			if ((is_playeri && is_blockj))   //checking player
			{

				if (collides(motion_i, motion_j))
				{
					vec2 delta = motion_i.position - motion_j.position;
					if (abs(delta.x) < abs(delta.y))
					{
						if (delta.y <= 0)
						{
							motion_i.position.y = motion_j.position.y - motion_j.scale.y / 2 - motion_i.scale.y / 2;

						}

					}

					registry.collisions.emplace_with_duplicates(entity_i, entity_j);

				}
				
				
			}
			else if (is_playeri && is_lavaj)
			{
				if (collides(motion_i, motion_j))
					registry.collisions.emplace_with_duplicates(entity_i, entity_j);
			}
			else if (is_blocki && is_blockj)  //checking rocks
			{
				bool isRock = registry.gravities.has(cell[i]) == 1;
				bool collided = false;
				if (isRock)
				{
					Gravity& g = registry.gravities.get(cell[i]);
					g.moveable_down = true;
					if (collidesVertically(motion_i, motion_j))
					{
						vec2 delta = motion_i.position - motion_j.position;
						if (abs(delta.x) < abs(delta.y))
						{
							if (delta.y <= 0)
							{
								motion_i.position.y = motion_j.position.y - motion_j.scale.y / 2 - motion_i.scale.y / 2;

							}

						}
						g.moveable_down = false;
						break;

					}


				}
			}
			else if (is_particlei)
			{
				Particle& physics = registry.particle.get(cell[i]);
				//Motion& motion1 = registry.motions.get(cell[i]);
				//Motion& motion2 = registry.motions.get(cell[j]);
				physics.collision = false;
				if (is_particlej)  // particle collision
				{
					if (!registry.lava.has(entity_i) && !registry.lava.has(entity_j))
					{
						if (MovingTowardsEachOther(motion_i, motion_j) && particles_collide(motion_i, motion_j))
						{
							calculate_new_velocity1(motion_i, motion_j, false);
							continue;
						}
					}


				}
				if (is_blockj && !IsTutorial) //skip particle collision in tutorial
				{
					if (particles_blocks_collide(motion_i, motion_j))
					{
						if (is_lavai)
						{
							Lava& p = registry.lava.get(cell[i]);
							bool horizontal = isHorizontal(motion_i, motion_j);
							if (!horizontal)
							{
								calculate_new_velocity_lava(motion_i, p.ifHit);
								p.ifHit = true;
							}
							if (horizontal)
							{
								motion_i.velocity.x *= -1;
							}

						}
						else
						{
							motion_i.velocity.y = 0.f;
							motion_i.velocity.x = 0.f;

						}

						physics.gravity = 0.f;
						physics.collision = true;
						vec2 delta = motion_i.position - motion_j.position;
						if (abs(delta.x) < abs(delta.y))
						{
							if (delta.y <= 0)
							{
								motion_i.position.y = motion_j.position.y - motion_j.scale.y / 2 - motion_i.scale.y / 2;

							}

						}
					}

				}


				if (motion_i.position.x <= boundary.x ) //-motion_i.scale
				{
					motion_i.velocity.x *= -1;
					motion_i.position.x = boundary.x;
				}
				else if(motion_i.position.x >= boundary.y)
				{
					motion_i.velocity.x *= -1;
					motion_i.position.x = boundary.y;
				}
					

			}
		}


		if (is_rock1)
		{
			Gravity& g = registry.gravities.get(cell[i]);
			if (g.moveable_down == true)
			{
				// Check the cell that is one unit down
				int belowY = y + 1;
				if (belowY < grid.size()) {  // Ensure we are within grid bounds
					std::vector<Entity>& belowCell = grid[belowY][x];
					for (Entity& entity_below : belowCell) {
						Motion& motion_below = registry.motions.get(entity_below);

						if (collidesVertically(motion_i, motion_below)) {

							vec2 delta = motion_i.position - motion_below.position;
							if (abs(delta.x) < abs(delta.y)) {
								if (delta.y <= 0) {
									motion_i.position.y = motion_below.position.y - motion_below.scale.y / 2 - motion_i.scale.y / 2;
								}
							}
							g.moveable_down = false;
							break;
							//registry.collisions.emplace_with_duplicates(entity_i, entity_below);
						}
					}
				}
			}


		}
		else
		{

			

			// Check collisions with adjacent cells
			for (int adjY = y - 1; adjY <= y + 1; adjY++) {
				for (int adjX = x - 1; adjX <= x + 1; adjX++) {
					// Ensure we are within grid bounds and not checking the current cell
					if (adjX >= 0 && adjX < grid.size() && adjY >= 0 && adjY < grid.size() && (adjX != x || adjY != y)) {
						std::vector<Entity>& adjacentCell = grid[adjY][adjX];

						for (int k = 0; k < adjacentCell.size(); k++) {
							Motion& motion_j = registry.motions.get(adjacentCell[k]);
							Entity& entity_j = adjacentCell[k];
							bool is_blockj = registry.blocks.has(adjacentCell[k]);
							bool is_particlej = registry.particle.has(adjacentCell[k]);
							bool is_lavaj = registry.lava.has(adjacentCell[k]);


							if (is_playeri && is_blockj) {
								if (collides(motion_i, motion_j)) {
									vec2 delta = motion_i.position - motion_j.position;
									if (abs(delta.x) < abs(delta.y))
									{
										if (delta.y <= 0)
										{
											motion_i.position.y = motion_j.position.y - motion_j.scale.y / 2 - motion_i.scale.y / 2;

										}

									}

									registry.collisions.emplace_with_duplicates(entity_i, entity_j);
								}
							}
							else if (is_particlei)
							{
								if (is_particlej)
								{
									if (!registry.lava.has(entity_i) && !registry.lava.has(entity_j))
									{
										if (MovingTowardsEachOther(motion_i, motion_j) && particles_collide(motion_i, motion_j))
										{
											calculate_new_velocity1(motion_i, motion_j, false);
											continue;
										}
									}
								}

								if (is_blockj && !IsTutorial)//if is tutorial, skip particle collision check
								{
									if (particles_blocks_collide(motion_i, motion_j))
									{
										Particle& physics = registry.particle.get(cell[i]);
										//Motion& motion1 = registry.motions.get(adjacentCell[k]);
										//Motion& motion2 = registry.motions.get(adjacentCell[k]);
										if (is_lavai)
										{
											Lava& p = registry.lava.get(cell[i]);
											bool horizontal = isHorizontal(motion_i, motion_j);
											if (!horizontal)
											{
												calculate_new_velocity_lava(motion_i, p.ifHit);
												p.ifHit = true;
											}
											if (horizontal)
											{
												motion_i.velocity.x *= -1;
											}
										}
										else
										{
											motion_i.velocity.y = 0.f;
											motion_i.velocity.x = 0.f;
										}
										physics.gravity = 0.f;
										physics.collision = true;
									}
								}

								if (motion_i.position.x <= boundary.x) //-motion_i.scale
								{
									motion_i.velocity.x *= -1;
									motion_i.position.x = boundary.x;
								}
								else if (motion_i.position.x >= boundary.y)
								{
									motion_i.velocity.x *= -1;
									motion_i.position.x = boundary.y;
								}

							}
						}
					}
				}
			}


		}




		//checkCollisionsWithAdjacentCells(cell[i], grid, x, y, motion_i);
	}



	//}
//}



	return;
}

std::mutex queueMutex;
std::atomic<bool> done(false);
void PhysicsSystem::worker(std::vector<std::vector<std::vector<Entity>>>& grid, bool IsTutorial, vec2 boundary, std::queue<std::pair<int, int>>& queue, int workerid) {
	while (!done) {
		// Process the next available cell
		queueMutex.lock();
		if (!queue.empty()) {
			auto task = queue.front();
			queue.pop();
			queueMutex.unlock();
			checkCollisionInChunks(grid, task.first, task.second, IsTutorial, boundary);
		}
		else {
			// If no more tasks are available, exit.
			done = true;
			queueMutex.unlock();
		}
	}
}

std::queue<std::pair<int, int>> taskQueue; // Queue to hold the grid cell coordinates (y, x)

void PhysicsSystem::parallelCollisionSetup(std::vector<std::vector<std::vector<Entity>>>& grid, bool IsTutorial, vec2 boundary) {

	int numThreads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;

	int gridHeight = grid.size();
	int gridWidth = gridHeight; // 7*7



	// fill up task queue with coordinates
	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			taskQueue.push({ y, x });
		}
	}

	//create and start threads
	for (int i = 0; i < numThreads; ++i) {
		std::thread t(&PhysicsSystem::worker, this, std::ref(grid), IsTutorial, boundary, std::ref(taskQueue), i);
		threads.push_back(std::move(t));
	}

	//std::thread worker1(&PhysicsSystem::worker, this, std::ref(grid), IsTutorial, boundary, std::ref(taskQueue));
	//worker1.join();


	//worker(grid, IsTutorial, boundary, taskQueue);
	//std::thread worker1(&PhysicsSystem::checkCollisionInChunks,this, std::ref(grid), 0,0, IsTutorial, boundary);
	//worker1.join();


	for (auto& thread : threads) {
		thread.join();  // Wait for all threads to finish
	}
	
	done = false;
}

void PhysicsSystem::step(float elapsed_ms, std::vector<std::vector<std::vector<Entity>>>& grid, bool IsTutorial)
{
	// Move fish based on how much time has passed, this is to (partially) avoid
	// having entities move at different speed based on the machine.
	Player* player1;

	float gravity = 980.f;
	auto& motion_container = registry.motions;
	auto& grav_container = registry.gravities;
	bool performanceTimer = false;
	auto start = std::chrono::high_resolution_clock::now();
	if (motion_container.size() >= 400)
	{
		performanceTimer = true;
		start = std::chrono::high_resolution_clock::now();
	}

	vec2 boundary;
	for (uint i = 1; i < motion_container.size(); i++)
	{
		Motion& motion = motion_container.components[i];
		Entity entity = motion_container.entities[i];
		if (!motion.isActive) continue;
		float step_seconds = elapsed_ms / 1000.f;
		float time = elapsed_ms / 1.5;

		if (registry.players.has(entity))
		{
			//			std::cout << "Vector2D: (" << widthHeight.x << ", " << widthHeight.y << ")" << std::endl;
			float width = abs(motion.scale.x);
			float height = abs(motion.scale.y);

			Player& player = registry.players.get(entity);
			player1 = &player;
			PLAYER = entity;
			boundary = checkBoundaryPlayer(motion, player);

			// Horizontal movement
			if (player.moveable_right == false)
			{
				if (motion.velocityGoal.x <= 0.f)
				
				{
					motion.velocity.x = 0;
					motion.velocity.x = lerp(motion.velocity.x, motion.velocityGoal.x, time);
					motion.position.x = motion.position.x + motion.velocity.x * step_seconds;
					player.moveable_right = true;
				}
				else
				{
					motion.velocity.x = 0;
					motion.position.x = motion.position.x + motion.velocity.x * step_seconds;
				}
			}
			else if (player.moveable_left == false)
			{
				if (motion.velocityGoal.x >= 0.f)
				{
					motion.velocity.x = 0;
					motion.velocity.x = lerp(motion.velocity.x, motion.velocityGoal.x, time);
					motion.position.x = motion.position.x + motion.velocity.x * step_seconds;
					player.moveable_left = true;

				}
				else
				{
					motion.velocity.x = 0;
					motion.position.x = motion.position.x + motion.velocity.x * step_seconds;
				}
			}
			else
			{
				motion.velocity.x = lerp(motion.velocity.x, motion.velocityGoal.x, time);
				motion.position.x = motion.position.x + motion.velocity.x * step_seconds;
				/*if (motion.position.x - width / 2 <= 0.f)
					motion.position.x = width / 2; // Boundary check left
				if (motion.position.x + width / 2 >= window_width_px)
					motion.position.x = window_width_px - width / 2; // Boundary check Right
					*/
			}

			// Vertical movement
			if (player.moveable_down == true)
			{
				motion.velocity.y += gravity * step_seconds;
			}
			else
			{
				if (motion.velocity.y >= 0.f)
				{
					motion.velocity.y = 0.f;
					// std::cout << "a_moveable_down: " << player.moveable_left << std::endl;
				}
			}

			if (player.moveable_up == false)
			{
				if (motion.velocity.y < 0.f)
				{
					// motion.velocity.y = -motion.velocity.y;
					motion.velocity.y = 0.f;
					player.moveable_up = true;
				}
			}

			if (!registry.collisions.has(entity))
			{
				player.moveable_down = true;
			}

			motion.position.y += motion.velocity.y * step_seconds;
		}
		else if (registry.particle.has(entity))
		{
			Particle& physics = registry.particle.get(entity);
			if (!physics.collision) physics.gravity = 98.f / 2.f;
			float gravity = physics.gravity;
			motion.velocity.y += gravity * step_seconds;
			motion.position += motion.velocity * step_seconds;
		}
		else if (registry.meshPtrs.has(entity))
		{
			motion.velocity.y += gravity * step_seconds;

			if (motion.velocityGoal.x > 0.f) {
				motion.velocityGoal.x -= gravity * step_seconds;
				if (motion.velocityGoal.x < 0.f) motion.velocityGoal.x = 0;
			}
			else if (motion.velocityGoal.x < 0) {
				motion.velocityGoal.x += gravity * step_seconds;
				if (motion.velocityGoal.x > 0) motion.velocityGoal.x = 0;
			}
			//motion.velocity.x = lerp(motion.velocity.x, motion.velocityGoal.x, time);
			motion.velocity.x = lerp(motion.velocity.x, motion.velocityGoal.x, time);
			motion.position.x = motion.position.x + motion.velocity.x * step_seconds;
		}


	}

	////// Gravity
	for (Entity entity : registry.gravities.entities)
	{
		// Retrieve components
		float deltaTime = elapsed_ms / 1000.0f;
		Gravity& grav = registry.gravities.get(entity);
		Motion& motion = registry.motions.get(entity);
		if (grav.moveable_down == true)
		{
			grav.acceleration_y = gravity;
			motion.velocity.y += grav.acceleration_y * deltaTime;
			motion.position.y += motion.velocity.y * deltaTime;
		}

	}

	//return;
	parallelCollisionSetup(grid, IsTutorial, boundary);

}
