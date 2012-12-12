#version 330

uniform mat4 modelViewProjection;
uniform float time;

layout (location = 0) in vec3 position;
layout (location = 3) in vec4 colour;

out vec4 colourOut;

void main()
{
	vec4 vertex = vec4(position, 1.0);
	colourOut = colour;

	vertex.z += time;

	if (vertex.z > -1.0)
		vertex.z -= 999.0;

	gl_PointSize = 4.0;
	//gl_PointSize = 30.0 + (vertex.z / sqrt(-vertex.z));

	if (gl_PointSize > 4.0)
		colourOut = smoothstep(0.0, 4.0, gl_PointSize) * colourOut;

	gl_Position = modelViewProjection * vertex;
}