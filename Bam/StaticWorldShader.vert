#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec2 position_offset;
layout(location = 4) in float layer;

// Output data ; will be interpolated for each fragment.
out vec3 UVW;

// Values that stay constant for the whole mesh.
uniform mat4 VP;

void main(){
    // Output position of the vertex, in clip space : MVP * position
	gl_Position = VP * vec4(vertexPosition_modelspace.xy + position_offset, vertexPosition_modelspace.z, 1);
    //gl_Position = MVP * vec4(vertexPosition_modelspace,1);

    // UV of the vertex. No special space for this one.
    UVW = vec3(vertexUV, layer);
}