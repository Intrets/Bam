#version 330 core

in vec2 position;
in int color_index;

uniform mat4 VP;

void main() {
	gl_PointSize = 10.0f;
	gl_Position = VP * vec4(position, 0.0f, 1.0f);
}