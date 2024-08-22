#include "world_init.hpp"
#include "physics_calculation.hpp"



vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

bool if_mouse_collides_pos(const vec2 pos, const Motion& motion2)
{
	vec2 min2 = motion2.position - get_bounding_box(motion2) / 2.f;
	vec2 max2 = motion2.position + get_bounding_box(motion2) / 2.f;

	return pos.x <= max2.x &&
		pos.x >= min2.x &&
		pos.y <= max2.y &&
		pos.y >= min2.y;
}

bool collidesVertically(const Motion& motion1, const Motion& motion2) 
{
	if (motion1.position.x != motion2.position.x) return false;
	if (motion1.position.y >= motion2.position.y) return false;
	float circle_bottom = motion1.position.y + (get_bounding_box(motion1).y) / 2.f;
	//if (motion1.position.x != motion2.position.x) return false;
	vec2 min2 = motion2.position - get_bounding_box(motion2) / 2.f;
	vec2 max2 = motion2.position + get_bounding_box(motion2) / 2.f;

	bool isColliding = circle_bottom >= min2.y && circle_bottom <= max2.y;

	return isColliding;
}

bool collides(const Motion& motion1, const Motion& motion2)
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

void calculate_new_velocity_lava(Motion& motion1, bool ifhit)
{
	float randomVelocity = (rand() % 201) - 100;

	if (!ifhit) motion1.velocity.x = randomVelocity;
	motion1.velocity.y = -motion1.velocity.y * 0.3;
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

vec2 checkBoundaryPlayer(const Motion& motion, Player& player) {
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

