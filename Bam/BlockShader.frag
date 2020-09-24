#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2DArray textureSampler;

in vec3 textureUVW;
in vec3 stencilUVW;

void main(){
	color = texture(textureSampler, textureUVW) * texture(textureSampler, stencilUVW);
	
}