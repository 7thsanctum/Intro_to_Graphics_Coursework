#version 330

#define MAX_LIGHTS 10

layout (std140) uniform;

uniform Material
{
	vec4 emissive;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

uniform Lighting
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 lightDir;
} lighting;

struct PointLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 lightPos;
	vec4 attenuation;
	float dist;
};

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

uniform DynamicLights
{
	PointLight points[MAX_LIGHTS];
	SpotLight spots[MAX_LIGHTS];
} dynamic;

uniform int numPoints;
uniform int numSpots;

uniform vec3 eyePos;

in vec3 transformedPosition;
smooth in vec3 transformedNormal;

out vec4 colour;

vec4 calculateLighting(in vec3 toEye)
{
	vec4 ambientColour = material.ambient * lighting.ambient;

	float s = max(dot(lighting.lightDir.xyz, transformedNormal), 0.0);
	vec4 diffuseColour = s * (material.diffuse * lighting.diffuse);

	vec3 r = reflect(-lighting.lightDir.xyz, transformedNormal);
	float t = pow(max(dot(r, toEye), 0.0), material.shininess);
	vec4 specularColour = t * (material.specular * lighting.specular);

	vec4 light = ambientColour + diffuseColour + specularColour;
	light.a = 1.0;
	return light;
}

vec4 calculatePoint(in vec3 toEye, in PointLight point)
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

vec4 calculateSpot(in vec3 toEye, in SpotLight spot)
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

void main()
{
	vec3 toEye = normalize(eyePos - transformedPosition);
	vec4 col = material.emissive;
	col += calculateLighting(toEye);
	for (int i = 0; i < numPoints; ++i)
		col += calculatePoint(toEye, dynamic.points[i]);
	for (int i = 0; i < numSpots; ++i)
		col += calculateSpot(toEye, dynamic.spots[i]);
	col.a = 1.0;

	colour = col;
}