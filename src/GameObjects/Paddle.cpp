#include "Paddle.hpp"

namespace vkPong
{
	std::vector<VertexData> Paddle::vertexData() const
	{
		return
		{
			{ { 0.0, -0.5 }, { 1.0, 0.0, 0.0 } },
			{ { 0.5, 0.5 }, { 0.0, 1.0, 0.0 } },
			{ { -0.5, 0.5 },  { 0.0, 0.0, 1.0 } }
		};
	}
}