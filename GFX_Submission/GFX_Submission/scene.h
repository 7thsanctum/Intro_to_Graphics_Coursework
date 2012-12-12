#pragma once

#include "render_object.h"
#include "light.h"
#include "texture.h"

// Contains data relevant to a scene
struct scene_data
{
	// The render objects loaded for this scene
	std::hash_map<std::string, render_object*> objects;

	// All the geometry created for the scene
	std::hash_map<std::string, geometry*> geometry;

	// All the materials created for the scene
	std::hash_map<std::string, material*> material;

	// All the textures loaded for the scene
	std::hash_map<std::string, texture*> textures;

	// The static lighting data for the scene
	lighting light;

	// The dynamic lighting data for the scene
	dynamic_lights dynamic;
};

// Loads a scene from a JSON file of the given filename
scene_data* loadScene(const std::string& fileName);