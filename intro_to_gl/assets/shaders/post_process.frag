#version 330

uniform sampler2D tex;

in vec2 texCoordOut;

out vec4 colour; 

void main()
{
	colour = texture2D(tex, texCoordOut);
}