#pragma once

#include "geometry.h"
#include "material.h"
#include "transform.h"

struct render_object
{
	transform transform;
	geometry* geometry;
	material* material;
};