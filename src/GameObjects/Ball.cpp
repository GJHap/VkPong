#include "Ball.hpp"

#include <vector>

namespace vkPong
{
	std::vector<VertexData> Ball::ballVertexData
	{
		{ { 0, 0 }, GameObject::color() },
		{ { 0.1, 0.0 }, GameObject::color() },
		{ { 0.0999848, 0.0017452 }, GameObject::color() },
		{ { 0.0999391, 0.0034899 }, GameObject::color() },
		{ { 0.099863, 0.0052336 }, GameObject::color() },
		{ { 0.0997564, 0.0069756 }, GameObject::color() },
		{ { 0.0996195, 0.0087156 }, GameObject::color() },
		{ { 0.0994522, 0.0104528 }, GameObject::color() },
		{ { 0.0992546, 0.0121869 }, GameObject::color() },
		{ { 0.0990268, 0.0139173 }, GameObject::color() },
		{ { 0.0987688, 0.0156434 }, GameObject::color() },
		{ { 0.0984808, 0.0173648 }, GameObject::color() },
		{ { 0.0981627, 0.0190809 }, GameObject::color() },
		{ { 0.0978148, 0.0207912 }, GameObject::color() },
		{ { 0.097437, 0.0224951 }, GameObject::color() },
		{ { 0.0970296, 0.0241922 }, GameObject::color() },
		{ { 0.0965926, 0.0258819 }, GameObject::color() },
		{ { 0.0961262, 0.0275637 }, GameObject::color() },
		{ { 0.0956305, 0.0292372 }, GameObject::color() },
		{ { 0.0951057, 0.0309017 }, GameObject::color() },
		{ { 0.0945519, 0.0325568 }, GameObject::color() },
		{ { 0.0939693, 0.034202 }, GameObject::color() },
		{ { 0.093358, 0.0358368 }, GameObject::color() },
		{ { 0.0927184, 0.0374607 }, GameObject::color() },
		{ { 0.0920505, 0.0390731 }, GameObject::color() },
		{ { 0.0913545, 0.0406737 }, GameObject::color() },
		{ { 0.0906308, 0.0422618 }, GameObject::color() },
		{ { 0.0898794, 0.0438371 }, GameObject::color() },
		{ { 0.0891007, 0.045399 }, GameObject::color() },
		{ { 0.0882948, 0.0469472 }, GameObject::color() },
		{ { 0.087462, 0.048481 }, GameObject::color() },
		{ { 0.0866025, 0.05 }, GameObject::color() },
		{ { 0.0857167, 0.0515038 }, GameObject::color() },
		{ { 0.0848048, 0.0529919 }, GameObject::color() },
		{ { 0.0838671, 0.0544639 }, GameObject::color() },
		{ { 0.0829038, 0.0559193 }, GameObject::color() },
		{ { 0.0819152, 0.0573576 }, GameObject::color() },
		{ { 0.0809017, 0.0587785 }, GameObject::color() },
		{ { 0.0798636, 0.0601815 }, GameObject::color() },
		{ { 0.0788011, 0.0615661 }, GameObject::color() },
		{ { 0.0777146, 0.062932 }, GameObject::color() },
		{ { 0.0766044, 0.0642788 }, GameObject::color() },
		{ { 0.075471, 0.0656059 }, GameObject::color() },
		{ { 0.0743145, 0.0669131 }, GameObject::color() },
		{ { 0.0731354, 0.0681998 }, GameObject::color() },
		{ { 0.071934, 0.0694658 }, GameObject::color() },
		{ { 0.0707107, 0.0707107 }, GameObject::color() },
		{ { 0.0694658, 0.071934 }, GameObject::color() },
		{ { 0.0681998, 0.0731354 }, GameObject::color() },
		{ { 0.0669131, 0.0743145 }, GameObject::color() },
		{ { 0.0656059, 0.075471 }, GameObject::color() },
		{ { 0.0642788, 0.0766044 }, GameObject::color() },
		{ { 0.062932, 0.0777146 }, GameObject::color() },
		{ { 0.0615661, 0.0788011 }, GameObject::color() },
		{ { 0.0601815, 0.0798636 }, GameObject::color() },
		{ { 0.0587785, 0.0809017 }, GameObject::color() },
		{ { 0.0573576, 0.0819152 }, GameObject::color() },
		{ { 0.0559193, 0.0829038 }, GameObject::color() },
		{ { 0.0544639, 0.0838671 }, GameObject::color() },
		{ { 0.0529919, 0.0848048 }, GameObject::color() },
		{ { 0.0515038, 0.0857167 }, GameObject::color() },
		{ { 0.05, 0.0866025 }, GameObject::color() },
		{ { 0.048481, 0.087462 }, GameObject::color() },
		{ { 0.0469472, 0.0882948 }, GameObject::color() },
		{ { 0.045399, 0.0891007 }, GameObject::color() },
		{ { 0.0438371, 0.0898794 }, GameObject::color() },
		{ { 0.0422618, 0.0906308 }, GameObject::color() },
		{ { 0.0406737, 0.0913545 }, GameObject::color() },
		{ { 0.0390731, 0.0920505 }, GameObject::color() },
		{ { 0.0374607, 0.0927184 }, GameObject::color() },
		{ { 0.0358368, 0.093358 }, GameObject::color() },
		{ { 0.034202, 0.0939693 }, GameObject::color() },
		{ { 0.0325568, 0.0945519 }, GameObject::color() },
		{ { 0.0309017, 0.0951057 }, GameObject::color() },
		{ { 0.0292372, 0.0956305 }, GameObject::color() },
		{ { 0.0275637, 0.0961262 }, GameObject::color() },
		{ { 0.0258819, 0.0965926 }, GameObject::color() },
		{ { 0.0241922, 0.0970296 }, GameObject::color() },
		{ { 0.0224951, 0.097437 }, GameObject::color() },
		{ { 0.0207912, 0.0978148 }, GameObject::color() },
		{ { 0.0190809, 0.0981627 }, GameObject::color() },
		{ { 0.0173648, 0.0984808 }, GameObject::color() },
		{ { 0.0156434, 0.0987688 }, GameObject::color() },
		{ { 0.0139173, 0.0990268 }, GameObject::color() },
		{ { 0.0121869, 0.0992546 }, GameObject::color() },
		{ { 0.0104528, 0.0994522 }, GameObject::color() },
		{ { 0.0087156, 0.0996195 }, GameObject::color() },
		{ { 0.0069756, 0.0997564 }, GameObject::color() },
		{ { 0.0052336, 0.099863 }, GameObject::color() },
		{ { 0.0034899, 0.0999391 }, GameObject::color() },
		{ { 0.0017452, 0.0999848 }, GameObject::color() },
		{ { 0.0, 0.1 }, GameObject::color() },
		{ { -0.0017452, 0.0999848 }, GameObject::color() },
		{ { -0.0034899, 0.0999391 }, GameObject::color() },
		{ { -0.0052336, 0.099863 }, GameObject::color() },
		{ { -0.0069756, 0.0997564 }, GameObject::color() },
		{ { -0.0087156, 0.0996195 }, GameObject::color() },
		{ { -0.0104528, 0.0994522 }, GameObject::color() },
		{ { -0.0121869, 0.0992546 }, GameObject::color() },
		{ { -0.0139173, 0.0990268 }, GameObject::color() },
		{ { -0.0156434, 0.0987688 }, GameObject::color() },
		{ { -0.0173648, 0.0984808 }, GameObject::color() },
		{ { -0.0190809, 0.0981627 }, GameObject::color() },
		{ { -0.0207912, 0.0978148 }, GameObject::color() },
		{ { -0.0224951, 0.097437 }, GameObject::color() },
		{ { -0.0241922, 0.0970296 }, GameObject::color() },
		{ { -0.0258819, 0.0965926 }, GameObject::color() },
		{ { -0.0275637, 0.0961262 }, GameObject::color() },
		{ { -0.0292372, 0.0956305 }, GameObject::color() },
		{ { -0.0309017, 0.0951057 }, GameObject::color() },
		{ { -0.0325568, 0.0945519 }, GameObject::color() },
		{ { -0.034202, 0.0939693 }, GameObject::color() },
		{ { -0.0358368, 0.093358 }, GameObject::color() },
		{ { -0.0374607, 0.0927184 }, GameObject::color() },
		{ { -0.0390731, 0.0920505 }, GameObject::color() },
		{ { -0.0406737, 0.0913545 }, GameObject::color() },
		{ { -0.0422618, 0.0906308 }, GameObject::color() },
		{ { -0.0438371, 0.0898794 }, GameObject::color() },
		{ { -0.045399, 0.0891007 }, GameObject::color() },
		{ { -0.0469472, 0.0882948 }, GameObject::color() },
		{ { -0.048481, 0.087462 }, GameObject::color() },
		{ { -0.05, 0.0866025 }, GameObject::color() },
		{ { -0.0515038, 0.0857167 }, GameObject::color() },
		{ { -0.0529919, 0.0848048 }, GameObject::color() },
		{ { -0.0544639, 0.0838671 }, GameObject::color() },
		{ { -0.0559193, 0.0829038 }, GameObject::color() },
		{ { -0.0573576, 0.0819152 }, GameObject::color() },
		{ { -0.0587785, 0.0809017 }, GameObject::color() },
		{ { -0.0601815, 0.0798636 }, GameObject::color() },
		{ { -0.0615661, 0.0788011 }, GameObject::color() },
		{ { -0.062932, 0.0777146 }, GameObject::color() },
		{ { -0.0642788, 0.0766044 }, GameObject::color() },
		{ { -0.0656059, 0.075471 }, GameObject::color() },
		{ { -0.0669131, 0.0743145 }, GameObject::color() },
		{ { -0.0681998, 0.0731354 }, GameObject::color() },
		{ { -0.0694658, 0.071934 }, GameObject::color() },
		{ { -0.0707107, 0.0707107 }, GameObject::color() },
		{ { -0.071934, 0.0694658 }, GameObject::color() },
		{ { -0.0731354, 0.0681998 }, GameObject::color() },
		{ { -0.0743145, 0.0669131 }, GameObject::color() },
		{ { -0.075471, 0.0656059 }, GameObject::color() },
		{ { -0.0766044, 0.0642788 }, GameObject::color() },
		{ { -0.0777146, 0.062932 }, GameObject::color() },
		{ { -0.0788011, 0.0615661 }, GameObject::color() },
		{ { -0.0798636, 0.0601815 }, GameObject::color() },
		{ { -0.0809017, 0.0587785 }, GameObject::color() },
		{ { -0.0819152, 0.0573576 }, GameObject::color() },
		{ { -0.0829038, 0.0559193 }, GameObject::color() },
		{ { -0.0838671, 0.0544639 }, GameObject::color() },
		{ { -0.0848048, 0.0529919 }, GameObject::color() },
		{ { -0.0857167, 0.0515038 }, GameObject::color() },
		{ { -0.0866025, 0.05 }, GameObject::color() },
		{ { -0.087462, 0.048481 }, GameObject::color() },
		{ { -0.0882948, 0.0469472 }, GameObject::color() },
		{ { -0.0891007, 0.045399 }, GameObject::color() },
		{ { -0.0898794, 0.0438371 }, GameObject::color() },
		{ { -0.0906308, 0.0422618 }, GameObject::color() },
		{ { -0.0913545, 0.0406737 }, GameObject::color() },
		{ { -0.0920505, 0.0390731 }, GameObject::color() },
		{ { -0.0927184, 0.0374607 }, GameObject::color() },
		{ { -0.093358, 0.0358368 }, GameObject::color() },
		{ { -0.0939693, 0.034202 }, GameObject::color() },
		{ { -0.0945519, 0.0325568 }, GameObject::color() },
		{ { -0.0951057, 0.0309017 }, GameObject::color() },
		{ { -0.0956305, 0.0292372 }, GameObject::color() },
		{ { -0.0961262, 0.0275637 }, GameObject::color() },
		{ { -0.0965926, 0.0258819 }, GameObject::color() },
		{ { -0.0970296, 0.0241922 }, GameObject::color() },
		{ { -0.097437, 0.0224951 }, GameObject::color() },
		{ { -0.0978148, 0.0207912 }, GameObject::color() },
		{ { -0.0981627, 0.0190809 }, GameObject::color() },
		{ { -0.0984808, 0.0173648 }, GameObject::color() },
		{ { -0.0987688, 0.0156434 }, GameObject::color() },
		{ { -0.0990268, 0.0139173 }, GameObject::color() },
		{ { -0.0992546, 0.0121869 }, GameObject::color() },
		{ { -0.0994522, 0.0104528 }, GameObject::color() },
		{ { -0.0996195, 0.0087156 }, GameObject::color() },
		{ { -0.0997564, 0.0069756 }, GameObject::color() },
		{ { -0.099863, 0.0052336 }, GameObject::color() },
		{ { -0.0999391, 0.0034899 }, GameObject::color() },
		{ { -0.0999848, 0.0017452 }, GameObject::color() },
		{ { -0.1, 0.0 }, GameObject::color() },
		{ { -0.0999848, -0.0017452 }, GameObject::color() },
		{ { -0.0999391, -0.0034899 }, GameObject::color() },
		{ { -0.099863, -0.0052336 }, GameObject::color() },
		{ { -0.0997564, -0.0069756 }, GameObject::color() },
		{ { -0.0996195, -0.0087156 }, GameObject::color() },
		{ { -0.0994522, -0.0104528 }, GameObject::color() },
		{ { -0.0992546, -0.0121869 }, GameObject::color() },
		{ { -0.0990268, -0.0139173 }, GameObject::color() },
		{ { -0.0987688, -0.0156434 }, GameObject::color() },
		{ { -0.0984808, -0.0173648 }, GameObject::color() },
		{ { -0.0981627, -0.0190809 }, GameObject::color() },
		{ { -0.0978148, -0.0207912 }, GameObject::color() },
		{ { -0.097437, -0.0224951 }, GameObject::color() },
		{ { -0.0970296, -0.0241922 }, GameObject::color() },
		{ { -0.0965926, -0.0258819 }, GameObject::color() },
		{ { -0.0961262, -0.0275637 }, GameObject::color() },
		{ { -0.0956305, -0.0292372 }, GameObject::color() },
		{ { -0.0951057, -0.0309017 }, GameObject::color() },
		{ { -0.0945519, -0.0325568 }, GameObject::color() },
		{ { -0.0939693, -0.034202 }, GameObject::color() },
		{ { -0.093358, -0.0358368 }, GameObject::color() },
		{ { -0.0927184, -0.0374607 }, GameObject::color() },
		{ { -0.0920505, -0.0390731 }, GameObject::color() },
		{ { -0.0913545, -0.0406737 }, GameObject::color() },
		{ { -0.0906308, -0.0422618 }, GameObject::color() },
		{ { -0.0898794, -0.0438371 }, GameObject::color() },
		{ { -0.0891007, -0.045399 }, GameObject::color() },
		{ { -0.0882948, -0.0469472 }, GameObject::color() },
		{ { -0.087462, -0.048481 }, GameObject::color() },
		{ { -0.0866025, -0.05 }, GameObject::color() },
		{ { -0.0857167, -0.0515038 }, GameObject::color() },
		{ { -0.0848048, -0.0529919 }, GameObject::color() },
		{ { -0.0838671, -0.0544639 }, GameObject::color() },
		{ { -0.0829038, -0.0559193 }, GameObject::color() },
		{ { -0.0819152, -0.0573576 }, GameObject::color() },
		{ { -0.0809017, -0.0587785 }, GameObject::color() },
		{ { -0.0798636, -0.0601815 }, GameObject::color() },
		{ { -0.0788011, -0.0615661 }, GameObject::color() },
		{ { -0.0777146, -0.062932 }, GameObject::color() },
		{ { -0.0766044, -0.0642788 }, GameObject::color() },
		{ { -0.075471, -0.0656059 }, GameObject::color() },
		{ { -0.0743145, -0.0669131 }, GameObject::color() },
		{ { -0.0731354, -0.0681998 }, GameObject::color() },
		{ { -0.071934, -0.0694658 }, GameObject::color() },
		{ { -0.0707107, -0.0707107 }, GameObject::color() },
		{ { -0.0694658, -0.071934 }, GameObject::color() },
		{ { -0.0681998, -0.0731354 }, GameObject::color() },
		{ { -0.0669131, -0.0743145 }, GameObject::color() },
		{ { -0.0656059, -0.075471 }, GameObject::color() },
		{ { -0.0642788, -0.0766044 }, GameObject::color() },
		{ { -0.062932, -0.0777146 }, GameObject::color() },
		{ { -0.0615661, -0.0788011 }, GameObject::color() },
		{ { -0.0601815, -0.0798636 }, GameObject::color() },
		{ { -0.0587785, -0.0809017 }, GameObject::color() },
		{ { -0.0573576, -0.0819152 }, GameObject::color() },
		{ { -0.0559193, -0.0829038 }, GameObject::color() },
		{ { -0.0544639, -0.0838671 }, GameObject::color() },
		{ { -0.0529919, -0.0848048 }, GameObject::color() },
		{ { -0.0515038, -0.0857167 }, GameObject::color() },
		{ { -0.05, -0.0866025 }, GameObject::color() },
		{ { -0.048481, -0.087462 }, GameObject::color() },
		{ { -0.0469472, -0.0882948 }, GameObject::color() },
		{ { -0.045399, -0.0891007 }, GameObject::color() },
		{ { -0.0438371, -0.0898794 }, GameObject::color() },
		{ { -0.0422618, -0.0906308 }, GameObject::color() },
		{ { -0.0406737, -0.0913545 }, GameObject::color() },
		{ { -0.0390731, -0.0920505 }, GameObject::color() },
		{ { -0.0374607, -0.0927184 }, GameObject::color() },
		{ { -0.0358368, -0.093358 }, GameObject::color() },
		{ { -0.034202, -0.0939693 }, GameObject::color() },
		{ { -0.0325568, -0.0945519 }, GameObject::color() },
		{ { -0.0309017, -0.0951057 }, GameObject::color() },
		{ { -0.0292372, -0.0956305 }, GameObject::color() },
		{ { -0.0275637, -0.0961262 }, GameObject::color() },
		{ { -0.0258819, -0.0965926 }, GameObject::color() },
		{ { -0.0241922, -0.0970296 }, GameObject::color() },
		{ { -0.0224951, -0.097437 }, GameObject::color() },
		{ { -0.0207912, -0.0978148 }, GameObject::color() },
		{ { -0.0190809, -0.0981627 }, GameObject::color() },
		{ { -0.0173648, -0.0984808 }, GameObject::color() },
		{ { -0.0156434, -0.0987688 }, GameObject::color() },
		{ { -0.0139173, -0.0990268 }, GameObject::color() },
		{ { -0.0121869, -0.0992546 }, GameObject::color() },
		{ { -0.0104528, -0.0994522 }, GameObject::color() },
		{ { -0.0087156, -0.0996195 }, GameObject::color() },
		{ { -0.0069756, -0.0997564 }, GameObject::color() },
		{ { -0.0052336, -0.099863 }, GameObject::color() },
		{ { -0.0034899, -0.0999391 }, GameObject::color() },
		{ { -0.0017452, -0.0999848 }, GameObject::color() },
		{ { -0.0, -0.1 }, GameObject::color() },
		{ { 0.0017452, -0.0999848 }, GameObject::color() },
		{ { 0.0034899, -0.0999391 }, GameObject::color() },
		{ { 0.0052336, -0.099863 }, GameObject::color() },
		{ { 0.0069756, -0.0997564 }, GameObject::color() },
		{ { 0.0087156, -0.0996195 }, GameObject::color() },
		{ { 0.0104528, -0.0994522 }, GameObject::color() },
		{ { 0.0121869, -0.0992546 }, GameObject::color() },
		{ { 0.0139173, -0.0990268 }, GameObject::color() },
		{ { 0.0156434, -0.0987688 }, GameObject::color() },
		{ { 0.0173648, -0.0984808 }, GameObject::color() },
		{ { 0.0190809, -0.0981627 }, GameObject::color() },
		{ { 0.0207912, -0.0978148 }, GameObject::color() },
		{ { 0.0224951, -0.097437 }, GameObject::color() },
		{ { 0.0241922, -0.0970296 }, GameObject::color() },
		{ { 0.0258819, -0.0965926 }, GameObject::color() },
		{ { 0.0275637, -0.0961262 }, GameObject::color() },
		{ { 0.0292372, -0.0956305 }, GameObject::color() },
		{ { 0.0309017, -0.0951057 }, GameObject::color() },
		{ { 0.0325568, -0.0945519 }, GameObject::color() },
		{ { 0.034202, -0.0939693 }, GameObject::color() },
		{ { 0.0358368, -0.093358 }, GameObject::color() },
		{ { 0.0374607, -0.0927184 }, GameObject::color() },
		{ { 0.0390731, -0.0920505 }, GameObject::color() },
		{ { 0.0406737, -0.0913545 }, GameObject::color() },
		{ { 0.0422618, -0.0906308 }, GameObject::color() },
		{ { 0.0438371, -0.0898794 }, GameObject::color() },
		{ { 0.045399, -0.0891007 }, GameObject::color() },
		{ { 0.0469472, -0.0882948 }, GameObject::color() },
		{ { 0.048481, -0.087462 }, GameObject::color() },
		{ { 0.05, -0.0866025 }, GameObject::color() },
		{ { 0.0515038, -0.0857167 }, GameObject::color() },
		{ { 0.0529919, -0.0848048 }, GameObject::color() },
		{ { 0.0544639, -0.0838671 }, GameObject::color() },
		{ { 0.0559193, -0.0829038 }, GameObject::color() },
		{ { 0.0573576, -0.0819152 }, GameObject::color() },
		{ { 0.0587785, -0.0809017 }, GameObject::color() },
		{ { 0.0601815, -0.0798636 }, GameObject::color() },
		{ { 0.0615661, -0.0788011 }, GameObject::color() },
		{ { 0.062932, -0.0777146 }, GameObject::color() },
		{ { 0.0642788, -0.0766044 }, GameObject::color() },
		{ { 0.0656059, -0.075471 }, GameObject::color() },
		{ { 0.0669131, -0.0743145 }, GameObject::color() },
		{ { 0.0681998, -0.0731354 }, GameObject::color() },
		{ { 0.0694658, -0.071934 }, GameObject::color() },
		{ { 0.0707107, -0.0707107 }, GameObject::color() },
		{ { 0.071934, -0.0694658 }, GameObject::color() },
		{ { 0.0731354, -0.0681998 }, GameObject::color() },
		{ { 0.0743145, -0.0669131 }, GameObject::color() },
		{ { 0.075471, -0.0656059 }, GameObject::color() },
		{ { 0.0766044, -0.0642788 }, GameObject::color() },
		{ { 0.0777146, -0.062932 }, GameObject::color() },
		{ { 0.0788011, -0.0615661 }, GameObject::color() },
		{ { 0.0798636, -0.0601815 }, GameObject::color() },
		{ { 0.0809017, -0.0587785 }, GameObject::color() },
		{ { 0.0819152, -0.0573576 }, GameObject::color() },
		{ { 0.0829038, -0.0559193 }, GameObject::color() },
		{ { 0.0838671, -0.0544639 }, GameObject::color() },
		{ { 0.0848048, -0.0529919 }, GameObject::color() },
		{ { 0.0857167, -0.0515038 }, GameObject::color() },
		{ { 0.0866025, -0.05 }, GameObject::color() },
		{ { 0.087462, -0.048481 }, GameObject::color() },
		{ { 0.0882948, -0.0469472 }, GameObject::color() },
		{ { 0.0891007, -0.045399 }, GameObject::color() },
		{ { 0.0898794, -0.0438371 }, GameObject::color() },
		{ { 0.0906308, -0.0422618 }, GameObject::color() },
		{ { 0.0913545, -0.0406737 }, GameObject::color() },
		{ { 0.0920505, -0.0390731 }, GameObject::color() },
		{ { 0.0927184, -0.0374607 }, GameObject::color() },
		{ { 0.093358, -0.0358368 }, GameObject::color() },
		{ { 0.0939693, -0.034202 }, GameObject::color() },
		{ { 0.0945519, -0.0325568 }, GameObject::color() },
		{ { 0.0951057, -0.0309017 }, GameObject::color() },
		{ { 0.0956305, -0.0292372 }, GameObject::color() },
		{ { 0.0961262, -0.0275637 }, GameObject::color() },
		{ { 0.0965926, -0.0258819 }, GameObject::color() },
		{ { 0.0970296, -0.0241922 }, GameObject::color() },
		{ { 0.097437, -0.0224951 }, GameObject::color() },
		{ { 0.0978148, -0.0207912 }, GameObject::color() },
		{ { 0.0981627, -0.0190809 }, GameObject::color() },
		{ { 0.0984808, -0.0173648 }, GameObject::color() },
		{ { 0.0987688, -0.0156434 }, GameObject::color() },
		{ { 0.0990268, -0.0139173 }, GameObject::color() },
		{ { 0.0992546, -0.0121869 }, GameObject::color() },
		{ { 0.0994522, -0.0104528 }, GameObject::color() },
		{ { 0.0996195, -0.0087156 }, GameObject::color() },
		{ { 0.0997564, -0.0069756 }, GameObject::color() },
		{ { 0.099863, -0.0052336 }, GameObject::color() },
		{ { 0.0999391, -0.0034899 }, GameObject::color() },
		{ { 0.0999848, -0.0017452 }, GameObject::color() },
	};

	std::vector<VertexData> Ball::vertexData() const
	{
		return ballVertexData;
	}
}