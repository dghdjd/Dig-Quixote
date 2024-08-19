#include "particle_system.hpp"
#include "physics_system.hpp"
#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"
#include "map.hpp"


void Map::generateMap(std::vector<std::vector<int>> map, int window_width, RenderSystem* renderer)
{

	for (int i = 0; i < map.size(); i++)
	{
		float ypos = 150.f + BLOCK_BB_HEIGHT * i;
		for (int j = 0; j < map[i].size(); j++)
		{

			if (map[i][j] != 0) {

				float xpos = window_width / 2 - BLOCK_BB_WIDTH * (MAP_WIDTH / 2) + BLOCK_BB_WIDTH / 2 + BLOCK_BB_WIDTH * j;
				Entity entity = createBlock(renderer, { xpos, ypos }, map[i][j]);
			}

		}
	}
}


void CreateBlocksHelper(int window_width, int window_height, RenderSystem* renderer, float x, float y)
{
	float xpos;
	float ypos;
	for (int i = 0; i < 2; i++)
	{
		ypos = y + BLOCK_BB_HEIGHT * i;
		for (int j = 0; j < 2; j++)
		{
			xpos = x + BLOCK_BB_WIDTH * j;
			Entity entity = createBlock(renderer, { xpos, ypos }, 999);
		}
	}
}

void Map::generateTutorialMapDEMO(int window_width, int window_height, RenderSystem* renderer)
{
	

	float ypos = window_height / 2 - BLOCK_BB_HEIGHT / 2;
	float xpos = window_width / 2 - BLOCK_BB_WIDTH * 9;
	Entity entity = createBlock(renderer, { xpos, ypos }, 999);
	//CreateBlocksHelper(window_width, window_height, renderer, xpos, ypos);


	xpos = window_width / 2 + 9 * BLOCK_BB_WIDTH;
	entity = createBlock(renderer, { xpos, ypos }, 999);
	//CreateBlocksHelper(window_width, window_height, renderer, xpos, ypos);

	ypos = window_height / 2 + BLOCK_BB_HEIGHT - BLOCK_BB_HEIGHT / 2;
	xpos = window_width / 2;
	entity = createBlock(renderer, { xpos, ypos }, 998);

	ypos = window_height / 2 + BLOCK_BB_HEIGHT * 2 - BLOCK_BB_HEIGHT / 2;
	xpos = window_width / 2 - BLOCK_BB_HEIGHT * 5;
	entity = createBlock(renderer, { xpos, ypos }, 996);

	xpos = window_width / 2 + BLOCK_BB_HEIGHT * 5;
	entity = createBlock(renderer, { xpos, ypos }, 997);

	for (int i = 0; i < tr_map.size(); i++)
	{
		float ypos = window_height / 2 + BLOCK_BB_HEIGHT * i;
		for (int j = 0; j < tr_map[i].size(); j++)
		{
			if (tr_map[i][j] != 999)
			{
				if (tr_map[i][j] == 994)
				{
					float xpos = window_width / 2 - BLOCK_BB_WIDTH * (MAP_WIDTH / 2) + BLOCK_BB_WIDTH / 2 + BLOCK_BB_WIDTH * j;
					Entity entity = createBlock(renderer, { xpos, ypos }, 994);
				}
				else if (tr_map[i][j] == 993)
				{
					float xpos = window_width / 2 - BLOCK_BB_WIDTH * (MAP_WIDTH / 2) + BLOCK_BB_WIDTH / 2 + BLOCK_BB_WIDTH * j;
					Entity entity = createBlock(renderer, { xpos, ypos }, 993);
				}
				else if (tr_map[i][j] == 992)
				{
					float xpos = window_width / 2 - BLOCK_BB_WIDTH * (MAP_WIDTH / 2) + BLOCK_BB_WIDTH / 2 + BLOCK_BB_WIDTH * j;
					Entity entity = createBlock(renderer, { xpos, ypos }, 992);
				}
				else {
					float xpos = window_width / 2 - BLOCK_BB_WIDTH * (MAP_WIDTH / 2) + BLOCK_BB_WIDTH / 2 + BLOCK_BB_WIDTH * j;
					Entity entity = createBlock(renderer, { xpos, ypos }, 995);
				}
			}
		}
	}

	map_created = true;
}