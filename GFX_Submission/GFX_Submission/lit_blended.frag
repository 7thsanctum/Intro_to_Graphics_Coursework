#version 330

#define MAX_LIGHTS 10

#ifndef MATERIAL
#define MATERIAL
struct MaterialData
{
	vec4 emissive;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
#endif

uniform Material
{
	MaterialData material;
};

uniform vec3 eyePos;

in vec3 transformedPosition;
in vec3 transformedNormal;
in vec2 texCoordOut;

out vec4 colour;

vec4 calculateLighting(in MaterialData material, in vec3 toEye, in vec3 transformedNormal);

vec4 blend(in vec2 texcoord);

void main()
{
	vec3 toEye = normalize(eyePos - transformedPosition);

	vec4 col = blend(texCoordOut);

	col *= calculateLighting(material, toEye, transformedNormal);

	col += material.emissive;

	colour = col;
}