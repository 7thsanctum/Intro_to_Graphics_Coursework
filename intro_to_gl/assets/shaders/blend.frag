#version 330

uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D alpha;

vec4 blend(in vec2 texcoord)
{
	vec4 col1 = texture2D(tex, texcoord);
	vec4 col2 = texture2D(tex2, texcoord);
	vec4 blend = texture2D(alpha, texcoord);
	return mix(col1, col2, blend.a);
}