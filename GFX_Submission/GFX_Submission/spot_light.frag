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

#ifndef SPOT_LIGHT
#define SPOT_LIGHT
struct SpotLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 lightPos;
	vec4 attenuation;
	vec4 lightDir;
	float power;
	float dist;
};
#endif

vec4 calculateSpot(in MaterialData material, in vec3 toEye, in SpotLight spot,
				   in vec3 transformedPosition, in vec3 transformedNormal)
{
	vec3 lightVec = normalize(spot.lightPos.xyz - transformedPosition);
	float s = max(dot(lightVec, transformedNormal), 0.0);
	float d = distance(spot.lightPos.xyz, transformedPosition);
	if (s > 0.0 && d < spot.dist)
	{
		vec4 ambientColour = material.ambient * spot.ambient;

		vec4 diffuseColour = s * (material.diffuse * spot.diffuse);

		vec3 r = reflect(-lightVec, transformedNormal);
		float t = pow(max(dot(r, toEye), 0.0), material.shininess);
		vec4 specularColour = t * (material.specular * spot.specular);

		float A = spot.attenuation.x + spot.attenuation.y * d + spot.attenuation.z * d * d;
		float sp = pow(max(dot(lightVec, -spot.lightDir.xyz), 0.0), spot.power);
		vec4 col = sp * (ambientColour + ((diffuseColour + specularColour) / A));
		col.a = 1.0;
		return col;
	}
	else
	return vec4(0.0, 0.0, 0.0, 1.0);
}