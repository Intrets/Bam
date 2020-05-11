#version 330 core

// Ouput data
layout(location = 0) out vec4 color;

uniform sampler2D texture_a;

in vec2 UV;

void main(){
	color = texture(texture_a, UV);
	//color = ivec4(1,1,0,0);
}