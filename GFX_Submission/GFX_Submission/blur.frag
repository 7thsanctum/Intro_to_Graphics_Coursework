#version 330

uniform sampler2D tex;

const float inverseWidth = 1.0/ 800.0;
const float inverseHeight = 1.0/ 600.0;

const vec4 samples[4] = vec4[4]
(
	vec4(-1.0, 0.0, 0.0, 0.25),
	vec4(1.0, 0.0, 0.0, 0.25),
	vec4(0.0, 1.0, 0.0, 0.25),
	vec4(0.0, -1.0, 0.0, 0.25)
);

in vec2 texCoordOut;

out vec4 colour;

void main()
{
	vec4 col = vec4(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < 4; ++i)
	{
		vec2 texCoord = texCoordOut +
				vec2(samples[i].x * inverseWidth, samples[i].y * inverseHeight);
		col += samples[i].w * texture2D(tex, texCoord);
	}
	colour = col;
}