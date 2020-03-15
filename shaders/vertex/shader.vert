#version 450

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec3 i_color;


layout (location = 0) out vec3 o_color;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() {
	gl_Position = vec4(i_position, 0.0, 1.0);
	o_color = i_color;
}