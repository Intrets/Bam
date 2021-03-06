#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 position;
layout(location = 2) in int texture_rotation;
layout(location = 3) in int textureID;
layout(location = 4) in int stencilID;

uniform float depth;
uniform mat4 VP;

out vec3 textureUVW;
out vec3 stencilUVW;

uniform mat2 rotations[5] = mat2[](
	mat2(1,0,
	     0,-1),
	mat2(0,-1,
	     -1,0),
	mat2(-1,0,
	     0,1),
	mat2(0,1,
	     1,0),
	mat2(1,0,
		 0,1)
);


void main(){
	gl_Position = VP * vec4(vertex + position, depth, 1);

	vec2 UV = vertex * rotations[texture_rotation];
	textureUVW = vec3(UV, float(textureID));
	stencilUVW = vec3(UV, float(stencilID));
}

