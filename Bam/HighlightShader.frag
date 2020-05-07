#version 330 core


// Ouput data
out vec4 color;

//in flat vec4 c;
in vec4 c;

void main(){
    // Output color = color of the texture at the specified UV

	// absolute transparancy

	//color = vec3(255,0,0);
	color = c;
}