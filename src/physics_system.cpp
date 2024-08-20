// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include "world_system.hpp"
#include "iostream"
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

bool PhysicsSystem::collides_pos(const vec2 pos, const Motion& motion2)
{
	vec2 min2 = motion2.position - get_bounding_box(motion2) / 2.f;
	vec2 max2 = motion2.position + get_bounding_box(motion2) / 2.f;

	return pos.x <= max2.x &&
		pos.x >= min2.x &&
		pos.y <= max2.y &&
		pos.y >= min2.y;
}
bool PhysicsSystem::collidesVertically(const Motion& motion1, const Motion& motion2) {
	if (motion1.position.x != motion2.position.x) return false;
	if (motion1.position.y >= motion2.position.y) return false;
	float circle_bottom = motion1.position.y + (get_bounding_box(motion1).y) / 2.f;
	//if (motion1.position.x != motion2.position.x) return false;
	vec2 min2 = motion2.position - get_bounding_box(motion2) / 2.f;
	vec2 max2 = motion2.position + get_bounding_box(motion2) / 2.f;

	bool isColliding = circle_bottom >= min2.y && circle_bottom <= max2.y;

	return isColliding;
}

bool PhysicsSystem::collides(const Motion& motion1, const Motion& motion2)
{

	vec2 fur_pos = motion1.position + motion1.velocity / 1000.f;
	// motion1's position is the center of the circle
	vec2 circle_center = fur_pos;
	float circle_radius = (get_bounding_box(motion1).x) / 2.f; // Assuming x is width of the bounding box

	// Calculate AABB for motion2
	vec2 min2 = motion2.position - get_bounding_box(motion2) / 2.f;
	vec2 max2 = motion2.position + get_bounding_box(motion2) / 2.f;

	// Find the point in AABB of motion2 that is closest to the center of motion1's circle
	float closestX = std::max(min2.x, std::min(circle_center.x, max2.x));
	float closestY = std::max(min2.y, std::min(circle_center.y, max2.y));

	// Calculate the distance between the circle's center and this closest point
	float distanceX = circle_center.x - closestX;
	float distanceY = circle_center.y - closestY;

	// Check if the distance is less than the circle's radius
	return (distanceX * distanceX + distanceY * distanceY) <= ((circle_radius * circle_radius));
}
bool particles_collide(const Motion& motion1, const Motion& motion2)
{

	vec2 circle1_center = motion1.position;
	float circle1_radius = motion1.mass / 2;  //mass = radius

	vec2 circle2_center = motion2.position;
	float circle2_radius = motion2.mass / 2;

	vec2 center_diff = circle1_center - circle2_center;
	float distance_squared = sqrt(center_diff.x * center_diff.x + center_diff.y * center_diff.y);

	float radii_sum = circle1_radius + circle2_radius;

	return distance_squared <= radii_sum;
}
bool particles_blocks_collide(Motion& motion1, Motion& motion2)
{
	// motion1's position is the center of the circle
	vec2 circle_center = motion1.position;
	float circle_radius = motion1.mass / 2.f; // Assuming x is width of the bounding box

	// Calculate AABB for motion2
	vec2 min2 = motion2.position - get_bounding_box(motion2) / 2.f;
	vec2 max2 = motion2.position + get_bounding_box(motion2) / 2.f;

	// Find the point in AABB of motion2 that is closest to the center of motion1's circle
	float closestX = std::max(min2.x, std::min(circle_center.x, max2.x));
	float closestY = std::max(min2.y, std::min(circle_center.y, max2.y));
	if (motion1.position.y >= motion2.position.y - motion2.scale.x)
	{
		float i = 2;
	}

	// Calculate the distance between the circle's center and this closest point
	float distanceX = circle_center.x - closestX;
	float distanceY = circle_center.y - closestY;

	bool colliding = (distanceX * distanceX + distanceY * distanceY) <= (circle_radius * circle_radius);

	return colliding;
}
bool isHorizontal(const Motion& motion1, const Motion& motion2)
{
	// motion1's position is the center of the circle
	vec2 circle_center = motion1.position;
	float circle_radius = motion1.mass / 2.f; // Assuming x is width of the bounding box

	// Calculate AABB for motion2
	vec2 min2 = motion2.position - get_bounding_box(motion2) / 2.f;
	vec2 max2 = motion2.position + get_bounding_box(motion2) / 2.f;
	// Find the point in AABB of motion2 that is closest to the center of motion1's circle
	float closestX = std::max(min2.x, std::min(circle_center.x, max2.x));
	float closestY = std::max(min2.y, std::min(circle_center.y, max2.y));
	// Calculate the distance between the circle's center and this closest point
	float distanceX = circle_center.x - closestX;
	float distanceY = circle_center.y - closestY;

	// Check if the distance is less than the circle's radius
	bool isColliding = (distanceX * distanceX + distanceY * distanceY) <= (circle_radius * circle_radius);

	bool isVerticalEdge = closestY > min2.y && closestY < max2.y;


	return (closestX == min2.x || closestX == max2.x) && isVerticalEdge && isColliding;
}
bool collides1(Motion& motion1, Motion& motion2, bool* isParCollision)
{
	if (motion1.isblock && motion2.isPar)
	{
		*isParCollision = true;
		if (particles_blocks_collide(motion2, motion1))
			return true;
		return false;
	}
	else if (motion2.isblock && motion1.isPar)
	{
		*isParCollision = true;
		if (particles_blocks_collide(motion1, motion2))
			return true;
		return false;
	}
	return false;

}

void calculate_new_velocity_lava(Motion& motion1, bool ifhit)
{
	float randomVelocity = (rand() % 201) - 100;

	if (!ifhit) motion1.velocity.x = randomVelocity;
	motion1.velocity.y = -motion1.velocity.y * 0.3;
}

bool PhysicsSystem::point_in_circle(const vec2& point, const vec2& circle_center, float circle_radius)
{
	float dx = point.x - circle_center.x;
	float dy = point.y - circle_center.y;
	return dx * dx + dy * dy <= circle_radius * circle_radius;
}

bool PhysicsSystem::edge_intersects_circle(const vec2& edge_start, const vec2& edge_end, const vec2& circle_center, float circle_radius)
{
	vec2 edge = edge_end - edge_start;
	vec2 circle_to_start = edge_start - circle_center;

	float t = dot(circle_to_start, edge) / dot(edge, edge);
	vec2 closest_point;
	if (t < 0)
		closest_point = edge_start;
	else if (t > 1)
		closest_point = edge_end;
	else
		closest_point = edge_start + t * edge;

	return point_in_circle(closest_point, circle_center, circle_radius);
}

bool PhysicsSystem::collides_with_polygon(const Motion& motion1, const std::vector<vec2>& vertices)
{
	vec2 circle_center = motion1.position;
	float circle_radius = get_bounding_box(motion1).x / 2.f;

	// Check if any vertex of the polygon is inside the circle
	for (const vec2& vertex : vertices)
	{
		if (point_in_circle(vertex, circle_center, circle_radius))
		{
			//std::cout << "Crashed with point!" << time << std::endl;
			return true;
		}
	}

	// Check if any edge of the polygon intersects the circle
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vec2 start = vertices[i];
		vec2 end = vertices[(i + 1) % vertices.size()];
		if (edge_intersects_circle(start, end, circle_center, circle_radius))
		{
			//std::cout << "Crashed with edge!" << time << std::endl;
			return true;
		}
	}

	return false;
}

/*
bool PhysicsSystem::collides(const Motion &motion1, const Motion &motion2)
{
	vec2 min1 = motion1.position - get_bounding_box(motion1) / 2.f;
	vec2 max1 = motion1.position + get_bounding_box(motion1) / 2.f;

	vec2 min2 = motion2.position - get_bounding_box(motion2) / 2.f;
	vec2 max2 = motion2.position + get_bounding_box(motion2) / 2.f;

	return min1.x <= max2.x &&
		   max1.x >= min2.x &&
		   min1.y <= max2.y &&
		   max1.y >= min2.y;
}
*/

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

vec2 getMaxMinVertices(Entity entity)
{
	Mesh* mesh = registry.meshPtrs.get(entity);
	std::vector<ColoredVertex> vectices1 = mesh->vertices;
	vec2 minVertex = vec2(0.f, 0.f);
	vec2 maxVertex = vec2(0.f, 0.f);

	for (const ColoredVertex& vertex : vectices1)
	{
		minVertex.x = std::min(minVertex.x, vertex.position.x);
		minVertex.y = std::min(minVertex.y, vertex.position.y);
		maxVertex.x = std::max(maxVertex.x, vertex.position.x);
		maxVertex.y = std::max(maxVertex.y, vertex.position.y);
	}
	float width = maxVertex.x - minVertex.x;
	float height = maxVertex.y - minVertex.y;
	return vec2(width, height);
}

bool MovingTowardsEachOther(const Motion& motion1, const Motion& motion2) {
	vec2 relativeVelocity = motion2.velocity - motion1.velocity;
	vec2 positionDifference = motion2.position - motion1.position;
	float dotProduct = relativeVelocity.x * positionDifference.x + relativeVelocity.y * positionDifference.y;

	return dotProduct < 0;
}

void calculate_new_velocity1(Motion& motion1, Motion& motion2, bool isBlock) {
	float m1 = motion1.mass;
	vec2 v1 = motion1.velocity;
	vec2 p1 = motion1.position;
	float m2 = motion2.mass;
	vec2 v2 = motion2.velocity;
	vec2 p2 = motion2.position;

	vec2 p_diff = p1 - p2;
	vec2 v_diff = v1 - v2;

	vec2 p_diff1 = p2 - p1;
	vec2 v_diff1 = v2 - v1;

	// Calculate the dot product of the velocity difference and position difference vectors.
	float diff_dot = dot(v_diff, p_diff);
	float diff_dot1 = dot(v_diff1, p_diff1);

	// Calculate the magnitude squared of the position difference vector.
	float diff_mag_squared = dot(p_diff, p_diff);
	float diff_mag_squared1 = dot(p_diff1, p_diff1);

	float f1 = 2 * m2;
	float f2 = 2 * m1;

	float sum = m1 + m2;

	float d1 = f1 / sum;
	float d2 = f2 / sum;

	float scalar = d1 * (diff_dot / diff_mag_squared);
	float scalar1 = d2 * (diff_dot1 / diff_mag_squared1);

	//// Calculate the scalar factor for the collision response.
	//float scalar = (2 * m2 / (m1 + m2)) * (diff_dot / diff_mag_squared);
	//float scalar1 = (2 * m1 / (m1 + m2)) * (diff_dot1 / diff_mag_squared1);
	// Calculate the new velocities based on the scalar factor and the position difference vector.
	vec2 new_speed1 = v1 - (scalar * p_diff);
	vec2 new_speed2 = v2 - (scalar1 * p_diff1);

	// Update the velocities of the motions.
	motion1.velocity = new_speed1;
	motion2.velocity = new_speed2;
}






void PhysicsSystem::handleParticleCollision()
{
	auto& collisionsRegistry1 = registry.particleCollisions;
	float restitutionCoefficient = 1.f;
	for (uint i = 0; i < collisionsRegistry1.components.size(); i++) {
		Entity entity = collisionsRegistry1.entities[i];
		Entity entity_other = collisionsRegistry1.components[i].other_entity;

		if (registry.particle.has(entity) && registry.lava.has(entity_other))
		{
			Particle& physics = registry.particle.get(entity);
			Motion& motion1 = registry.motions.get(entity);

			motion1.velocity.y = 0.f;
			motion1.velocity.x = 0.f;
			//motion1.velocity.x -= 1.f;
			//if (motion1.velocity.x < 0.f) motion1.velocity.x = 0.f;

			physics.gravity = 0.f;
			physics.collision = true;

		}
		if (registry.particle.has(entity) && registry.players.has(entity_other))
		{
			Motion& p_motion = registry.motions.get(entity_other);
			Motion& par_motion = registry.motions.get(entity);
			par_motion.velocity.x += p_motion.velocity.x * 100.f;

		}
	}
	registry.particleCollisions.clear();
}

vec2 PhysicsSystem::checkBoundaryPlayer(const Motion& motion, Player& player) {
	int w = WorldSystem::get_window_width();
	int h = WorldSystem::get_window_height();
	int b_left = w / 2.f - 10 * BLOCK_BB_WIDTH + BLOCK_BB_WIDTH / 4.f;
	int b_right = w / 2.f + 10 * BLOCK_BB_WIDTH - BLOCK_BB_WIDTH / 4.f;

	if (motion.position.x <= b_left) {
		player.moveable_left = false;
	}
	if (motion.position.x >= b_right) {
		player.moveable_right = false;
	}
	return vec2(b_left, b_right);
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



				if (motion_i.position.x - motion_i.scale.x <= boundary.x || motion_i.position.x + motion_i.scale.x >= boundary.y)
					motion_i.velocity.x *= -1;
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

			auto start1 = std::chrono::high_resolution_clock::now();

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
										Motion& motion1 = registry.motions.get(adjacentCell[k]);
										Motion& motion2 = registry.motions.get(adjacentCell[k]);
										if (is_lavai)
										{
											Lava& p = registry.lava.get(cell[i]);
											bool horizontal = isHorizontal(motion1, motion2);
											if (!horizontal)
											{
												calculate_new_velocity_lava(motion1, p.ifHit);
												p.ifHit = true;
											}
											if (horizontal)
											{
												motion1.velocity.x *= -1;
											}
										}
										else
										{
											motion1.velocity.y = 0.f;
											motion1.velocity.x = 0.f;
										}
										physics.gravity = 0.f;
										physics.collision = true;
									}
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
			vec2 widthHeight = getMaxMinVertices(entity);
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
	start = std::chrono::high_resolution_clock::now();
	parallelCollisionSetup(grid, IsTutorial, boundary);
	auto end = std::chrono::high_resolution_clock::now();

	// Calculate the duration
	if (performanceTimer)
	{
		std::chrono::duration<double, std::milli> duration = end - start;
		std::cout << "Time taken by loop: " << duration.count() << " ms" << std::endl;
	}
	
	

}
