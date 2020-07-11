#pragma once

#include "geometry.h"
#include "transform.h"
#include "effect.h"
#include "texture.h"
#include "camera.h"

/*
A class used to store a collection of points that can be rendered as 
point sprites 
*/
class point_sprite
{
private:
	/*
	geometry structure used to hold the point data
	*/
	geometry* _geom;

	/*
	Texture to use for point sprite
	*/
	texture* _tex;

public:
	/*
	Transform for the poin sprite object. Might be needed for future implementations

	*/
	transform transform;

	/*
	Creates the point sprite object with the given geometry and texture
	*/
	point_sprite(geometry* geom, texture* tex);

	/*
	Destructor for point sprite object. Frees up resources.
	*/
	~point_sprite();

	/*
	Initialises values required to use point sprites;
	*/
	bool create();

	/*
	Renders the point sprites to the screen
	*/
	void render(effect* eff, const camera* camera);
};

/*
helper function to create point data that can act as a star field
*/
geometry* createStarField(int num_stars);