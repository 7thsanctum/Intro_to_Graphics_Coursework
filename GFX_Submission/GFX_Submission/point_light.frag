#version 330

layout (std140) uniform;

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

#ifndef POINT_LIGHT
#define POINT_LIGHT
struct PointLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 lightPos;
	vec4 attenuation;
	float dist;
};
#endif

vec4 calculatePoint(in MaterialData material, in vec3 toEye, in PointLight point,
					in vec3 transformedPosition, in vec3 transformedNormal)
{
	vec3 lightVec = normalize(point.lightPos.xyz - transformedPosition);
	float s = max(dot(lightVec, transformedNormal), 0.0);
	float d = distance(point.lightPos.xyz, transformedPosition);
	if (s > 0.0 && d < point.dist)
	{
		vec4 ambientColour = material.ambient * point.ambient;

		vec4 diffuseColour = s * (material.diffuse * point.diffuse);

		vec3 r = reflect(-lightVec, transformedNormal);
		float t = pow(max(dot(r, toEye), 0.0), material.shininess);
		vec4 specularColour = t * (material.specular * point.specular);

		float A = point.attenuation.x + point.attenuation.y * d + point.attenuation.z * d * d;
		vec4 col = ambientColour + ((diffuseColour + specularColour) / A);
		col.a = 1.0;
		return col;
	}
	else
		return vec4(0.0, 0.0, 0.0, 1.0);
}