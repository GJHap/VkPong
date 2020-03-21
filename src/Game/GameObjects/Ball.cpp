#include "Ball.hpp"
#include "BallVertices.hpp"

#include <algorithm>

namespace vkPong
{
	Ball::Ball()
		: GameObject({ 0.0f, 0.0f })
	{
	}

	const uint32_t Ball::vertexCount() const
	{
		return static_cast<uint32_t>(ballVertexData.size());
	}

	std::vector<VertexData> Ball::vertexData() const
	{
		std::vector<VertexData> transformation(vertexCount());
		std::transform(ballVertexData.cbegin(), ballVertexData.cend(), transformation.begin(), [this](VertexData vertex)
		{
			VertexData transformedVertex = { { vertex.position.x + m_position.x, vertex.position.y + m_position.y }, vertex.color };
			return transformedVertex;
		});

		return transformation;
	}
}