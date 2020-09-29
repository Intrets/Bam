#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;
layout(location = 1) in vec2 position_offset;
layout(location = 2) in vec2 scales;
layout(location = 3) in vec4 color;

// Output data ; will be interpolated for each fragment.
//out flat vec4 c;
out vec4 c;
// Values that stay constant for the whole mesh.
uniform mat4 VP;

void main(){
	c = color;

//	gl_Position = VP * vec4((scales*vertexPosition_modelspace.xy) + position_offset, vertexPosition_modelspace.z, 1);
	gl_Position = VP * vec4(vertexPosition_modelspace.xy*scales + position_offset, 0, 1);

}