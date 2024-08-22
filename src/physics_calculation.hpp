#pragma once

#include <vector>

vec2 get_bounding_box(const Motion& motion);

bool if_mouse_collides_pos(const vec2 pos, const Motion& motion2);

bool collidesVertically(const Motion& motion1, const Motion& motion2);

bool collides(const Motion& motion1, const Motion& motion2);

bool particles_collide(const Motion& motion1, const Motion& motion2);

bool particles_blocks_collide(Motion& motion1, Motion& motion2);

bool isHorizontal(const Motion& motion1, const Motion& motion2);

void calculate_new_velocity_lava(Motion& motion1, bool ifhit);

bool MovingTowardsEachOther(const Motion& motion1, const Motion& motion2);

void calculate_new_velocity1(Motion& motion1, Motion& motion2, bool isBlock);

vec2 checkBoundaryPlayer(const Motion& motion, Player& player);