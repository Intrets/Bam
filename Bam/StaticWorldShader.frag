#version 330 core

// Interpolated values from the vertex shaders
in vec3 UVW;

// Ouput data
out vec4 color;


// Values that stay constant for the whole mesh.
uniform sampler2DArray myTextureSampler;
//uniform sampler2D myTextureSampler;
//uniform int layer;

void main(){
    // Output color = color of the texture at the specified UV
    color = texture(myTextureSampler, UVW);
	// absolute transparancy
	if (color.a < 0.5){
		discard;
	}
}