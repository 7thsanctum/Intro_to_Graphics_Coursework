#version 330

uniform sampler2D tex;
uniform sampler2D dissolveTexture;
uniform float dissolveFactor;

vec4 dissolve(in vec2 texcoord)
{
	vec4 dissColour = texture2D(dissolveTexture, texcoord);
	if (dissColour.r > dissolveFactor)
		discard;

	return texture2D(tex, texcoord);
}