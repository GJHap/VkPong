#include "Ball.hpp"
#include "BallVertices.hpp"

#include <algorithm>
#include <vector>

namespace vkPong
{
	Ball::Ball()
		: GameObject({ 0.0f, 0.0f })
	{
	}

	std::vector<VertexData> Ball::vertexData() const
	{
		std::vector<VertexData> transformation(ballVertexData.size());
		std::transform(ballVertexData.cbegin(), ballVertexData.cend(), transformation.begin(), [this](VertexData vertex)
		{
			VertexData transformedVertex = { { vertex.position.x + m_position.x, vertex.position.y + m_position.y }, vertex.color };
			return transformedVertex;
		});

		return transformation;
	}
}