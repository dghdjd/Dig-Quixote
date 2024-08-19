#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are ahrd coded to the dimensions of the entity texture
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;

const float BLOCK_BB_WIDTH = 100.f / 2;		// 0.4f * 296.f;
const float BLOCK_BB_HEIGHT = 100.f / 2;		// 0.4f * 296.f;

const float BUTTON_BB_WIDTH = 200.f;		
const float BUTTON_BB_HEIGHT = 100.f;

const float PLAYER_BB_WIDTH = 40.f;
const float PLAYER_BB_HEIGHT = 40.f;

// the player
Entity createPlayer(RenderSystem* renderer, vec2 pos);
// empty palyer for runing sake
Entity createEmptyPlayer(RenderSystem* renderer, vec2 pos);
Entity createEmptySuButton();
Entity createEmptyQuButton();
// the prey
Entity createBlock(RenderSystem* renderer, vec2 position, int type);
Entity createHelperBlock(RenderSystem* renderer, vec2 position, int scale, int type);
Entity createTuButton(RenderSystem* renderer, vec2 position);
Entity createSuButton(RenderSystem* renderer, vec2 position);
Entity createQuButton(RenderSystem* renderer, vec2 position);
// the enemy
//Entity createTurtle(RenderSystem* renderer, vec2 position);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);
// a pebble
Entity createParticle(vec2 pos, vec2 size, float angle, float random);
Entity createLavaParticles(vec2 pos, vec2 size, float angle, float random);

Entity createMeshBlock(RenderSystem* renderer, vec2 pos);
Entity createBackground(int width, int height, int type);
Entity createUIBackground(int width, int height);
Entity createNumbers(int start, int width, int height);
Entity createAxe();