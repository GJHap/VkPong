#include "Ball.hpp"
#include "BallVertices.hpp"

#include <algorithm>

namespace vkPong
{
	Ball::Ball()
		: GameObject{ {0.0f, 0.0f} }, m_direction{ Direction({0.0005f, 0.0005f}) } {}

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

	void Ball::move()
	{
		m_position = { m_position.x + m_direction.x, m_position.y + m_direction.y };
	}
}