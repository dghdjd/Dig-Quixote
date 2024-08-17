// internal
#include "render_system.hpp"
#include <SDL.h>

#include "iostream"
#include "tiny_ecs_registry.hpp"

void RenderSystem::updatePortalAnimation(GLuint vbo, GLuint ibo, float elapsed_ms, float duration, Block& block, GEOMETRY_BUFFER_ID bufferID, float width, int& currentState, int maxState)
{
	vbo = vertex_buffers[(GLuint)bufferID];
	ibo = index_buffers[(GLuint)bufferID];
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//const float duration = 100.0f; //ms
//	time1 += elapsed_ms;
//	if (time1 >= duration)
//	{
	TexturedVertex* bufferData = (TexturedVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	currentState += 1;
	if (currentState == maxState)
	{
		currentState = 0;
	}

	float startTex = currentState * (498.f / width);     //128 gap to next sprite, 40 gap before first sprite
	float endTex = (498.f / width) + startTex;           //60 sprite width, 1792 sprite sheet length
	bufferData[0].texcoord = { startTex, 1.f };
	bufferData[1].texcoord = { endTex, 1.f };
	bufferData[2].texcoord = { endTex, 0.f };
	bufferData[3].texcoord = { startTex, 0.f };


	// Unmap the VBO
	glUnmapBuffer(GL_ARRAY_BUFFER);
	time2 = 0.f;
	changed1 = true;
	//	}

}
void RenderSystem::updateExplosion(GLuint vbo, GLuint ibo, float elapsed_ms, float duration, Block& block, GEOMETRY_BUFFER_ID bufferID, float width, int& currentState, int maxState)
{
	vbo = vertex_buffers[(GLuint)bufferID];
	ibo = index_buffers[(GLuint)bufferID];
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//const float duration = 100.0f; //ms
//	time1 += elapsed_ms;
//	if (time1 >= duration)
//	{
	TexturedVertex* bufferData = (TexturedVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	currentState += 1;
	if (currentState == maxState)
	{
		currentState = maxState;
		block.exploded = true;
		glUnmapBuffer(GL_ARRAY_BUFFER);
		time1 = 0.0f;
		changed = true;
		return;
	}

	float startTex = currentState * (258.f / width);     //128 gap to next sprite, 40 gap before first sprite
	float endTex = (258.f / width) + startTex;           //60 sprite width, 1792 sprite sheet length
	bufferData[0].texcoord = { startTex, 1.f };
	bufferData[1].texcoord = { endTex, 1.f };
	bufferData[2].texcoord = { endTex, 0.f };
	bufferData[3].texcoord = { startTex, 0.f };


	// Unmap the VBO
	glUnmapBuffer(GL_ARRAY_BUFFER);
	time1 = 0.0f;
	changed = true;
	//	}

}

void RenderSystem::updateBlockAnimation(GLuint vbo, GLuint ibo, float elapsed_ms, float duration, Block& block, GEOMETRY_BUFFER_ID bufferID, float width, int& currentState, int maxState)
{
	vbo = vertex_buffers[(GLuint)bufferID];
	ibo = index_buffers[(GLuint)bufferID];
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//const float duration = 100.0f; //ms
//	time1 += elapsed_ms;
//	if (time1 >= duration)
//	{
	TexturedVertex* bufferData = (TexturedVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	currentState += 1;
	if (currentState == maxState)
	{
		currentState = 0;
	}

	float startTex = currentState * (64.f / width);     //128 gap to next sprite, 40 gap before first sprite
	float endTex = (64.f / width) + startTex;           //60 sprite width, 1792 sprite sheet length
	bufferData[0].texcoord = { startTex, 1.f };
	bufferData[1].texcoord = { endTex, 1.f };
	bufferData[2].texcoord = { endTex, 0.f };
	bufferData[3].texcoord = { startTex, 0.f };


	// Unmap the VBO
	glUnmapBuffer(GL_ARRAY_BUFFER);
	time1 = 0.0f;
	changed = true;
	//	}

}
void RenderSystem::updatePlayerAnimation(GLuint vbo, GLuint ibo, float elapsed_ms, float duration, Player& player, GEOMETRY_BUFFER_ID bufferID, float width, int& currentState, int maxState, bool isJump)
{
	vbo = vertex_buffers[(GLuint)bufferID];
	ibo = index_buffers[(GLuint)bufferID];
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//const float duration = 100.0f; //ms
	time += elapsed_ms;
	if (time >= duration)
	{
		TexturedVertex* bufferData = (TexturedVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

		currentState += 1;
		if (currentState == maxState && isJump)
		{
			currentState = maxState - 1;
		}
		else if (currentState == maxState)
		{
			currentState = 0;
		}

		float startTex = currentState * (128.0f / width) + (40.f / width);     //128 gap to next sprite, 40 gap before first sprite
		float endTex = (60.f / width) + startTex;                              //60 sprite width, 1792 sprite sheet length
		bufferData[0].texcoord = { startTex, 1.f };
		bufferData[1].texcoord = { endTex, 1.f };
		bufferData[2].texcoord = { endTex, 0.f };
		bufferData[3].texcoord = { startTex, 0.f };


		// Unmap the VBO
		glUnmapBuffer(GL_ARRAY_BUFFER);
		time = 0.0f;
		if (!isJump) player.jump_state = 2;
	}
}

void RenderSystem::updateNumbers(int start, GLuint vbo, GLuint ibo, GEOMETRY_BUFFER_ID bufferID)
{
	vbo = vertex_buffers[(GLuint)bufferID];
	ibo = index_buffers[(GLuint)bufferID];
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	int columns = 10;
	float blockWidth = 44.f;
	float blockHeight = 43.f;
	float textureWidth = 445.f;
	float textureHeight = 428.f;
	int startFrom = (start - 1) / 10;
	int col = (start - 1) % 10;
	int row = 9 - ((start - 1) / 10);

	float xOffset = 15.0f ;  // Small horizontal offset
	float yOffset = 5.0f ;  // Small vertical offset

	float startTex = (col * blockWidth  + xOffset) / textureWidth; //2
	float endTex = ((col + 1) * blockWidth) / textureWidth ;			//44

	// Calculate vertical texture coordinates
	float startTexY = (row * blockHeight  + yOffset) / textureHeight;
	float endTexY = ((row + 1) * blockHeight - yOffset) / textureHeight ;

	TexturedVertex* bufferData = (TexturedVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);


	bufferData[0].texcoord = { startTex, 1.f - startTexY };
	bufferData[1].texcoord = { endTex, 1.f - startTexY };
	bufferData[2].texcoord = { endTex, 1.f - endTexY };
	bufferData[3].texcoord = { startTex, 1.f - endTexY };


	// Unmap the VBO
	glUnmapBuffer(GL_ARRAY_BUFFER);

}

void RenderSystem::drawTexturedMesh(Entity entity,
									const mat3 &projection, float elapsed_ms)
{
	Motion &motion = registry.motions.get(entity);
	// Transformation code, see Rendering and Transformation in the template
	// specification for more info Incrementally updates transformation matrix,
	// thus ORDER IS IMPORTANT
	Transform transform;
	bool isSalmon = false;
	bool isParticle = false;
	//std::cout << "Turn left: " << motion.turnLeft << std::endl;
	//if (motion.turnLeft) transform.transpose();
	transform.translate(motion.position);
	transform.scale(motion.scale);

	if (motion.turnRight) transform.scale(vec2(-1.f, 1.f));


	assert(registry.renderRequests.has(entity));
	RenderRequest &render_request = registry.renderRequests.get(entity);
	if (render_request.used_effect == EFFECT_ASSET_ID::SALMON) isSalmon = true;
	if (render_request.used_effect == EFFECT_ASSET_ID::PEBBLE) isParticle = true;
	const GLuint used_effect_enum = (GLuint)render_request.used_effect;
	assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	assert(render_request.used_geometry != GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
	GLuint vbo = vertex_buffers[(GLuint)render_request.used_geometry];
	GLuint ibo = index_buffers[(GLuint)render_request.used_geometry];

	//GLuint vbo;
	//GLuint ibo;


	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	bool isPlayer = registry.players.has(entity);
	bool isStar = registry.explosions1.has(entity);
	bool isNumber = registry.number.has(entity);
	bool isPortal = false;
	if (registry.blocks.has(entity))
	{
		isPortal = (registry.blocks.get(entity).type == 8);
	}
	
	if (isNumber)
	{
		Number n = registry.number.get(entity);
		updateNumbers(n.digLeft, vbo, ibo, GEOMETRY_BUFFER_ID::NUMBERS);

	}

	if (isPlayer)
	{
		Player& player = registry.players.get(entity);
		Motion& motion = registry.motions.get(entity);
		if (!motion.idling && motion.velocity.y == 0 && !motion.attack)   //RUN
		{

			updatePlayerAnimation(vbo, ibo, elapsed_ms, 150.f, player, GEOMETRY_BUFFER_ID::PLAYER_RUN, SpriteSheetWidths::PLAYER_RUN, player.run_state, player.max_run_state, false);

		}
		else if (motion.velocity.y != 0)   //JUMP
		{

			updatePlayerAnimation(vbo, ibo, elapsed_ms, 100.f, player, GEOMETRY_BUFFER_ID::PLAYER_JUMP, SpriteSheetWidths::PLAYER_JUMP, player.jump_state, player.max_jump_state, true);

		}
		else if (motion.attack)   //Attack
		{

			updatePlayerAnimation(vbo, ibo, elapsed_ms, 100.f, player, GEOMETRY_BUFFER_ID::PLAYER_ATTACK, SpriteSheetWidths::PLAYER_ATTACK, player.attack_state, player.max_attack_state, false);
		}
		else //Idle
		{


			updatePlayerAnimation(vbo, ibo, elapsed_ms, 200.f, player, GEOMETRY_BUFFER_ID::PLAYER_IDLE, SpriteSheetWidths::PLAYER_IDLE, player.idle_state, player.max_idle_state, false);
			
		}
					
		
	}

	if (isStar)
	{
		Block& block = registry.blocks.get(entity);
		if (block.explode && !changed && time1 >= 100.f && block.state != block.maxState)
		{
			//if (block.state == 0)
			//{
			//	render_request.used_texture = TEXTURE_ASSET_ID::LIGHTING;
			//	render_request.used_geometry = GEOMETRY_BUFFER_ID::LIGHTING;
			//}
			//

			//updateBlockAnimation(vbo, ibo, elapsed_ms, 10000.f, block, GEOMETRY_BUFFER_ID::WATER, SpriteSheetWidths::WATER, block.state, block.maxState);
			updateExplosion(vbo, ibo, elapsed_ms, 10000.f, block, GEOMETRY_BUFFER_ID::LIGHTING, SpriteSheetWidths::LIGHTING, block.state, block.maxState);

		}

	}
	if (isPortal && !changed1 && time2 >= 100.f)
	{
		Block& block = registry.blocks.get(entity);
		updatePortalAnimation(vbo, ibo, elapsed_ms, 10000.f, block, GEOMETRY_BUFFER_ID::PORTAL, SpriteSheetWidths::PORTAL, block.state, block.maxState);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	// Input data location as in the vertex buffer
	if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
							  sizeof(TexturedVertex), (void *)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void *)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id;

		if (isPlayer)
		{
			Player& player = registry.players.get(entity);
			if (!motion.idling && motion.velocity.y == 0 && !motion.attack)
			{
				texture_id = texture_gl_handles[(GLuint)GEOMETRY_BUFFER_ID::PLAYER_RUN];            //Check if player runing, change texture id
			}
			else if (motion.velocity.y != 0)
			{
				texture_id = texture_gl_handles[(GLuint)GEOMETRY_BUFFER_ID::PLAYER_JUMP];
			}
			else if (motion.attack)
			{
				texture_id = texture_gl_handles[(GLuint)GEOMETRY_BUFFER_ID::PLAYER_ATTACK];
			}
			else
			{
				texture_id =
					texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];
			}
		} 
		else
		{
			texture_id =
				texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();
	}
	else if (isParticle || isSalmon)
	{
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_color_loc = glGetAttribLocation(program, "in_color");
		gl_has_errors();

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(ColoredVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_color_loc);
		glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(ColoredVertex), (void*)sizeof(vec3));
		gl_has_errors();

		//if (isParticle)
		//{
		//	glVertexAttribDivisor(in_position_loc, 1);
		//	glVertexAttribDivisor(in_color_loc, 1);
		//	gl_has_errors();
		//}
		GLuint par_uloc = glGetUniformLocation(program, "time");
		glUniform1f(par_uloc, (float)(glfwGetTime() * 10.0f));

		GLint light_up_uloc = glGetUniformLocation(program, "light_up");
		const int lightup = registry.pebbleShells.has(entity) ? 1 : 0;
		glUniform1i(light_up_uloc, lightup);

		GLint p_pos = glGetUniformLocation(program, "p_pos");
		const vec2 pos = registry.motions.get(entity).position;
		glUniform2f(p_pos, pos.x, pos.y);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		assert(false && "Type of render request not supported");
	}

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	const vec3 color = registry.colors.has(entity) ? registry.colors.get(entity) : vec3(1);
	glUniform3fv(color_uloc, 1, (float *)&color);
	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float *)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float *)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

// draw the intermediate texture to the screen, with some distortion to simulate
// water
void RenderSystem::drawToScreen()
{
	// Setting shaders
	// get the water texture, sprite mesh, and program
	glUseProgram(effects[(GLuint)EFFECT_ASSET_ID::WATER]);
	gl_has_errors();
	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays
	/*const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	w = mode->width;
	h = mode->height;*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(1.f, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_has_errors();
	// Enabling alpha channel for textures
	glDisable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Draw the screen texture on the quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]);
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]); // Note, GL_ELEMENT_ARRAY_BUFFER associates
																	 // indices to the bound GL_ARRAY_BUFFER
	gl_has_errors();
	const GLuint water_program = effects[(GLuint)EFFECT_ASSET_ID::WATER];
	// Set clock
	GLuint time_uloc = glGetUniformLocation(water_program, "time");
	GLuint dead_timer_uloc = glGetUniformLocation(water_program, "screen_darken_factor");
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	ScreenState &screen = registry.screenStates.get(screen_state_entity);
	glUniform1f(dead_timer_uloc, screen.screen_darken_factor);
	gl_has_errors();
	// Set the vertex position and vertex texture coordinates (both stored in the
	// same VBO)
	GLint in_position_loc = glGetAttribLocation(water_program, "in_position");
	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	gl_has_errors();
	// Draw
	glDrawElements(
		GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
				  // no offset from the bound index buffer
	gl_has_errors();
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void RenderSystem::draw(float elapsed_ms)
{
	// Getting size of window
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays

	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();
	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	glClearColor(0, 0, 0, 1.0);
	glClearDepth(10.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST); // native OpenGL does not work with a depth buffer
							  // and alpha blending, one would have to sort
							  // sprites back to front
	gl_has_errors();
	mat3 projection_2D = createProjectionMatrix(w, h);
	// Draw all textured meshes that have a position and size component
	for (Entity entity : registry.renderRequests.entities)
	{
		if (!registry.motions.has(entity))
			continue;
		/*if (registry.background.has(entity))
		{
			Background Background = registry.background.get(entity);
			if (!Background.isDrawn)
			{
				drawTexturedMesh(entity, projection_2D, elapsed_ms);
				Background.isDrawn = true;
			}
		}
		else
		{*/
			drawTexturedMesh(entity, projection_2D, elapsed_ms);
		//}


	}




	//}
	time1 += elapsed_ms;
	time2 += elapsed_ms;
	changed = false;
	changed1 = false;

	// Truely render to the screen
	drawToScreen();

	// flicker-free display with a double buffer
	glfwSwapBuffers(window);
	gl_has_errors();
}

mat3 RenderSystem::createProjectionMatrix(int width,int height)
{
	// Fake projection matrix, scales with respect to window coordinates
	float left = 0.f;
	float top = 0.f;

	gl_has_errors();
	float right = (float) width;
	float bottom = (float) height;

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	return {{sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f}};
}

