#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in int character;
layout(location = 2) in vec3 offset;
layout(location = 3) in vec2 scale;

uniform ivec2 fontDim;
uniform ivec2 texDim;
uniform mat4 VP;
uniform sampler2D texture;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
//in int layer;


void main(){
	vec2 tSize = vec2(textureSize(texture,0));
	vec2 ratio = vec2(1, fontDim.y / float(fontDim.x));
	//vec2 tSize = vec2(1,1);
	//gl_Layer = layer;
	//gl_Position = vec4(vertexPosition_modelspace + vec3(offset, 0) ,1);
	gl_Position = vec4(vec3(vertexPosition_modelspace.xy * scale , 0) + offset ,1);
	int i = character % texDim.x;
	int j = character / texDim.x;
	UV = vertexPosition_modelspace.xy;
	UV.y -= 1;
	UV.y *= -1;
	UV = (UV + vec2(i,j)) / tSize * fontDim;
}

