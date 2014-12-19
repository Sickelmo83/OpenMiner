/*
 * =====================================================================================
 *
 *       Filename:  World.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  16/12/2014 15:28:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quentin BAZIN, <quent42340@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "World.hpp"

World::World() {
	m_width = 1;
	m_depth = 1;
	
	//for(s32 z = -m_depth / 2 ; z < m_depth / 2 ; z++) {
	//	for(s32 x = -m_width / 2 ; x < m_width / 2 ; x++) {
	for(s32 z = 0 ; z < m_depth ; z++) {
		for(s32 x = 0 ; x < m_width ; x++) {
			m_chunks.push_back(std::unique_ptr<Chunk>(new Chunk(x, 0, z)));
		}
	}
	
	for(s32 z = 0 ; z < m_depth ; z++) {
		for(s32 x = 0 ; x < m_width ; x++) {
			if(x > 0)			getChunk(x, z)->setLeft(getChunk(x - 1, z));
			if(x < m_width - 1) getChunk(x, z)->setRight(getChunk(x + 1, z));
			if(z > 0)			getChunk(x, z)->setFront(getChunk(x, z - 1));
			if(z < m_depth - 1) getChunk(x, z)->setBack(getChunk(x, z + 1));
		}
	}
}

World::~World() {
}

#include "Debug.hpp"

void World::draw(Shader &shader, const glm::mat4 &pv) {
	float ud = 0.0;
	s32 ux = -1;
	s32 uz = -1;
	
	for(auto &it : m_chunks) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(it->x() * Chunk::width, it->y() * Chunk::height, it->z() * Chunk::depth));
		glm::mat4 mvp = pv * model;
		
		// Is this chunk on the screen?
		glm::vec4 center = mvp * glm::vec4(Chunk::width / 2, Chunk::height / 2, Chunk::depth / 2, 1);
		
		float d = glm::length(center);
		center.x /= center.w;
		center.y /= center.w;
		
		// If it is behind the camera, don't bother drawing it
		if(center.z < -Chunk::height / 2)
			continue;
		
		// It it is outside the screen, don't bother drawing it
		if(fabsf(center.x) > 1 + fabsf(Chunk::height * 2 / center.w) || fabsf(center.y) > 1 + fabsf(Chunk::height * 2 / center.w))
			continue;
		
		if(!it->initialized()) {
			if(ux < 0 || d < ud) {
				ud = d;
				ux = it->x();
				uz = it->z();
			}
			
			continue;
		}
		
		shader.setUniform("u_modelMatrix", model);
		shader.setUniform("u_viewProjectionMatrix", pv);
		
		it->draw(shader);
	}
	
	if(ux >= 0) {
		getChunk(ux, uz)->generate();
		if(getChunk(ux, uz)->left())  getChunk(ux, uz)->left()->generate();
		if(getChunk(ux, uz)->right()) getChunk(ux, uz)->right()->generate();
		if(getChunk(ux, uz)->front()) getChunk(ux, uz)->front()->generate();
		if(getChunk(ux, uz)->back())  getChunk(ux, uz)->back()->generate();
		getChunk(ux, uz)->setInitialized(true);
	}
}

