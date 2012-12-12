#pragma once

#include "texture.h"
#include <glm\glm.hpp>
#include "camera.h"
#include "cubemap.h"

class skybox
{
private:
	cubemap* _cubemap;

public:
	skybox(cubemap* cubemap);

	~skybox();

	cubemap* getCubemap() { return _cubemap; }

	bool create();

	void render(const camera* camera);
};