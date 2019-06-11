#version 330

uniform sampler2D tex;
uniform sampler2D tex2;

uniform float blend_factor;

in vec2 texCoordOut;

out vec4 colour;

void main()
{
	vec4 col1 = texture2D(tex, texCoordOut);
	vec4 col2 = texture2D(tex2, texCoordOut);
	colour = mix(col1, col2, blend_factor);
	colour.a = 1.0f;
}