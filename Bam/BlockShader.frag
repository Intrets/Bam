#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2DArray textureSampler;

in vec3 textureUVW;
in vec3 stencilUVW;

void main(){
	vec4 stencil = texture(textureSampler, stencilUVW);
	color = texture(textureSampler, textureUVW) * vec4(stencil.g, stencil.g, stencil.g, stencil.r);
	if (color.a < 0.5){
		discard;
	}
}