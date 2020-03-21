#pragma once

#include "GameObject.hpp"

namespace vkPong
{
	class Paddle : public GameObject
	{
	private:
		constexpr static float height{ 0.7f };
		constexpr static float width{ 0.1f };
		constexpr static float verticalIncrement{ 0.1f };
	public:
		const uint32_t vertexCount() const override;
		std::vector<VertexData> vertexData() const override;
		void moveUp();
		void moveDown();
	protected:
		explicit Paddle(const Position&);
	};
}