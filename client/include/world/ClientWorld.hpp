/*
 * =====================================================================================
 *
 *       Filename:  ClientWorld.hpp
 *
 *    Description:
 *
 *        Created:  12/01/2019 14:00:57
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef CLIENTWORLD_HPP_
#define CLIENTWORLD_HPP_

#include <memory>

#include "ClientChunk.hpp"
#include "Network.hpp"

class ClientWorld : public gk::IDrawable {
	public:
		ClientWorld();

		void update();

		void receiveChunkData(sf::Packet &packet);

		ClientChunk *getChunk(int cx, int cy, int cz) const;

		u16 getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, u16 id);

	private:
		void draw(gk::RenderTarget &target, gk::RenderStates states) const override;

		// FIXME: Duplicated with those in ServerWorld
		const s32 m_width = 8;
		const s32 m_height = 4;
		const s32 m_depth = 8;

		std::vector<std::unique_ptr<ClientChunk>> m_chunks;

		gk::Texture &m_texture;
};

#endif // CLIENTWORLD_HPP_
