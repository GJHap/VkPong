#include "Paddle.hpp"

namespace vkPong
{
	std::vector<VertexData> Paddle::vertexData() const
	{
		Color color = { 1.0f, 1.0f, 1.0f };
		float vulkan_x_left = -1.0f;
		float vulkan_y_bottom = 1.0f;
		float height = 0.7f;
		float width = 0.1f;

		return
		{
			{ { vulkan_x_left, vulkan_y_bottom }, color },
			{ { vulkan_x_left + width, vulkan_y_bottom }, color },
			{ { vulkan_x_left + width, vulkan_y_bottom - height },  color },
			{ { vulkan_x_left, vulkan_y_bottom - height },  color },
			{ { vulkan_x_left, vulkan_y_bottom }, color },
		};
	}
}