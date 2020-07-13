
#if _WIN64
#pragma comment(lib, "glfw3dll") //Now using GLFWDLL.lib instead of GLFW.lib (compatibilty)
#else
#pragma comment(lib, "glfw3ddll") //Now using GLFWDLL.lib instead of GLFW.lib (compatibilty)
#endif
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "libglew_sharedd") //GLEW library (Allows us to use modern openGL functions)
// #pragma comment(lib, "DevIL")
// #pragma comment(lib, "ILU")
// #pragma comment(lib, "ILUT")

#define GLFW_DLL
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#define GLM_ENABLE_EXPERIMENTAL

#include <windows.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
// #include <IL\ilut.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\constants.hpp>
#include <glm\gtc\noise.hpp>
#include <cstdlib>
#include <math.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include "render_object.h"
#include "effect.h"
#include "light.h"
#include "scene.h"
#include "camera.h"
#include "util.h"
#include "skybox.h"
#include "post_process.h"
#include "point_sprite.h"
#include <glm\gtc\noise.hpp>

bool running = true;		  //Whether the program is running or not, when set the false program will terminate!
float resX = 800.0f;		  //Holder for the screen resolution
float resY = 600.0f;		  //Placeholder value incase no resolution is selected somehow
GLint midWindowX = resX / 2;  // Middle of the window horizontally
GLint midWindowY = resY / 2;  // Middle of the window vertically
float tim = 0.0f;			  //Timer for the starfield effect
float dissolveFactor = 10.0f; //The dissolve factor (modified by using - = on keyboard)
float power = 1.0f;			  //Power of the light
float lightAimSpeed = 0.1f;	  //How quickly the lights aim changes
float moveSpeed = 5.0f;		  //The movement speed of the FPS camera
float angle = 0.0f;			  //The angle of rotation for orbiting round objects
float rotateSpeed = 1.0f;	  //The rotation speed of all objects. Can be modified with + - on numpad
int frame = 0;				  //Used for motion blur effect
int numPadSelect = 1;		  // Holder for the selected movable object
int camSelect = 1;
int rad = 5.0f;
double tempTime = 0;
double prevTimeStamp;
double currentTimeStamp;

glm::vec3 orbit = glm::vec3(0.0f, 0.0f, 0.0f); //For use when orbiting around a specific point
glm::vec3 lightAim(0.0f, 0.0f, -500.0f);	   //The point the light aims at
glm::vec4 *data = new glm::vec4[1024 * 1024];  //Data for the post process effect

effect eff;					//Our primary render effect
lighting light;				//Our light
target_camera *tarCam;		//############
chase_camera *chsCam;		//#	  Our    #
first_person_camera *fpCam; //#  Cameras #
arc_ball_camera *abCam;		//############
scene_data *scene;			//Our Scene Data
effect star;				//Our Point Sprite Starfield effect

post_process *post_proc; //#############
post_process *post_edge;
post_process *post_sced;
post_process *post_shar; //	Our various
post_process *post_norm; //	Post Process effects
post_process *post_blur;
post_process *post_grey;
post_process *pass_2[2]; //#############

point_sprite *points;
skybox *sb;						  //Our skybox
texture *tex3 = new texture(" "); //Our holder for the procedurally generated texture
GLFWwindow *window;

DWORD WINAPI genTexture(LPVOID args)
{
	bool check = true; //Switches between the two procedural texture generation algorithms, I should've put this in texture.cpp
	int texXY = 1024;
	while (running)
	{
		if (check)
		{
			for (int i = 0; i < texXY; ++i) // TODO add these generation algorithms to texture.cpp
				for (int j = 0; j < texXY; ++j)
				{
					float f = (glm::simplex(glm::vec2((float)i / 160.0f, (float)j / 160.0f)));
					data[(i * 1024) + j] = glm::vec4(f, f, f, 1.0f);
				}
			check = false; //Once complete start creating the next texture
		}
		if (!check)
		{
			int spacing = 0;
			int spacingY = 0;
			int squareSize = 128;
			float colour = 1.0f;
			for (int i = 0; i < texXY; ++i)
			{
				if (spacing == squareSize)
				{
					spacing = 0;
					if (colour == 1.0f)
						colour = 0.0f;
					else
						colour = 1.0f;
				}
				for (int j = 0; j < texXY; ++j)
				{
					float f = colour;
					if (spacingY == squareSize)
					{
						spacingY = 0;
						if (colour == 1.0f)
							colour = 0.0f;
						else
							colour = 1.0f;
					}
					++spacingY;
					data[(i * texXY) + j] = glm::vec4(f, f, f, 1.0f);
				}
				++spacing;
			}
			check = true; //Once complete start creating the next texture
		}
	}
	return 1;
}

void initialise()
{
	srand(time(NULL));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_ARRAY);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	chsCam = new chase_camera(); //###############################Initialise all the cameras###########################
	chsCam->setProjection(glm::pi<float>() / 4.0f, resX / resY, 0.1f, 10000.0f);
	chsCam->setPosition(glm::vec3(0.0f, 4.0f, -500.0f));
	chsCam->setFollowPosition(glm::vec3(0.0f, 2.0f, -500.0f));
	chsCam->setPositionOffset(glm::vec3(5.0f, 0.0f, 5.0f));
	chsCam->setSpringiness(0.5f);

	abCam = new arc_ball_camera();
	abCam->setProjection(glm::pi<float>() / 4.0f, resX / resY, 0.1f, 10000.0f);
	abCam->setPosition(glm::vec3(0.0f, 4.0f, -500.0f));
	abCam->setTarget(glm::vec3(0.0f, 2.0f, -500.0f));
	abCam->setDistance(5.0f);
	abCam->setMinRotationY(90.0f);
	abCam->setMaxRotationY(-90.0f);
	abCam->setRotationX(0.0f);
	abCam->setRotationY(0.0f);

	fpCam = new first_person_camera();
	fpCam->setProjection(glm::pi<float>() / 4.0f, resX / resY, 0.1f, 10000.0f);
	fpCam->setPosition(glm::vec3(-10.0f, 2.0f, -500.0f));
	fpCam->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

	fpCam->rotate(glm::pi<float>() / 2, 0.0f);

	tarCam = new target_camera();
	tarCam->setProjection(glm::pi<float>() / 4.0f, resX / resY, 0.1f, 10000.0f);
	tarCam->setPosition(glm::vec3(10.0f, 10.0f, -490.0f));
	tarCam->setTarget(glm::vec3(0.0f, 2.0f, 0.0f)); //###############################End Camera Initialise################################

	if (!eff.addShader("assets/shaders/lit_blended.vert", GL_VERTEX_SHADER)) //###############################Load In All the Shader Files#########################
		exit(EXIT_FAILURE);													 //###############################And post process effects#############################
	if (!eff.addShader("assets/shaders/lighting.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("assets/shaders/point_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("assets/shaders/spot_light.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("assets/shaders/lit_texture.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.addShader("assets/shaders/dissolved.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!eff.create())
		exit(EXIT_FAILURE);

	if (!star.addShader("assets/shaders/starfield.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!star.addShader("assets/shaders/starfield.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!star.create())
		exit(EXIT_FAILURE);

	effect *post_eff = new effect();
	if (!post_eff->addShader("assets/shaders/post_process.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->addShader("assets/shaders/Motion_blur.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->create())
		exit(EXIT_FAILURE);

	post_proc = new post_process(post_eff);
	post_proc->create(resX, resY);

	post_eff = new effect();
	if (!post_eff->addShader("assets/shaders/post_process.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->addShader("assets/shaders/Greyscale.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->create())
		exit(EXIT_FAILURE);

	post_grey = new post_process(post_eff);
	post_grey->create(resX, resY);

	post_eff = new effect();
	if (!post_eff->addShader("assets/shaders/post_process.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->addShader("assets/shaders/sharpen.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->create())
		exit(EXIT_FAILURE);

	post_shar = new post_process(post_eff);
	post_shar->create(resX, resY);

	post_eff = new effect();
	if (!post_eff->addShader("assets/shaders/post_process.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->addShader("assets/shaders/edge.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->create())
		exit(EXIT_FAILURE);

	post_edge = new post_process(post_eff);
	post_edge->create(resX, resY);

	post_eff = new effect();
	if (!post_eff->addShader("assets/shaders/post_process.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->addShader("assets/shaders/scharredge.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->create())
		exit(EXIT_FAILURE);

	post_sced = new post_process(post_eff);
	post_sced->create(resX, resY);

	post_eff = new effect();
	if (!post_eff->addShader("assets/shaders/post_process.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->addShader("assets/shaders/blur.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->create())
		exit(EXIT_FAILURE);

	post_blur = new post_process(post_eff);
	post_blur->create(resX, resY);

	post_eff = new effect();
	if (!post_eff->addShader("assets/shaders/post_process.vert", GL_VERTEX_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->addShader("assets/shaders/post_process.frag", GL_FRAGMENT_SHADER))
		exit(EXIT_FAILURE);
	if (!post_eff->create())
		exit(EXIT_FAILURE);

	post_norm = new post_process(post_eff);
	post_norm->create(resX, resY);

	pass_2[0] = new post_process(post_eff);
	pass_2[0]->create(resX, resY);

	pass_2[1] = new post_process(post_eff);
	pass_2[1]->create(resX, resY); //####################End Shader Loading##############################################

	texture *tex = new texture("assets/images/star.tga");   //#####################################
	tex->create();										   //#	  Load the texture for the star	  #
	points = new point_sprite(createStarField(2500), tex); //#	and create points for point sprite#
	points->create();									   //#####################################

	std::vector<std::string> cubemap_texs;			   //#####################################
	cubemap_texs.push_back("assets/images/xneg2.png"); // Load in each image for the cube map
	cubemap_texs.push_back("assets/images/xpos2.png");
	cubemap_texs.push_back("assets/images/ypos2.png");
	cubemap_texs.push_back("assets/images/yneg2.png");
	cubemap_texs.push_back("assets/images/zpos2.png");
	cubemap_texs.push_back("assets/images/zneg2.png");

	cubemap *cm = new cubemap(cubemap_texs); // Create the cubemap
	cm->create();
	sb = new skybox(cm); // Create new Skybox from the cubemap!
	sb->create();		 //#####################################

	scene = loadScene("./assets/scenes/scene_3.json"); // Load in our scene from the JSON file.

	CreateThread(0, 0, genTexture, NULL, 0, 0); //Run the genTexture() on a seperate thread so that it can be
												//repeatedly updated whilst the program runs but without affecting performance
}

void handleKeyInput(int key, int action) //Whenever a key is pressed perform the code inside of here
{
	//if(action == GLFW_PRESS) { }
	// TODO add code to handleKeyInput as soon as a key is pressed
}

void handleMouseMove(GLFWwindow* window, double mouseX, double mouseY) //Whenever the mouse is moved perform the code inside of here
{
	float Sensitivity = 10.0f;

	int horizMovement = mouseX - midWindowX; // Find how far from the centre of the screen the mouse has moved
	int vertMovement = mouseY - midWindowY;

	glfwSetCursorPos(window, midWindowX, midWindowY); // Reset the mouse position to the centre of the window each time

	double deltaTime = (currentTimeStamp - prevTimeStamp);
	if (mouseX < midWindowX)
		fpCam->rotate(-horizMovement / Sensitivity * deltaTime, 0.0f); // Rotate the FPS camera
	if (mouseX > midWindowX)
		fpCam->rotate(-horizMovement / Sensitivity * deltaTime, 0.0f); // Depending on the sensitivity settings
	if (mouseY < midWindowY)
		fpCam->rotate(0.0f, vertMovement / Sensitivity * deltaTime); // And depending on the
	if (mouseY > midWindowY)
		fpCam->rotate(0.0f, vertMovement / Sensitivity * deltaTime); // direction of mouse movement
}

void fpsCameraMove(double deltaTime) // TODO input class
{
	if (glfwGetKey(window, GLFW_KEY_SPACE))
		fpCam->move(glm::vec3(0.0f, moveSpeed, 0.0f) * (float)deltaTime); //Move UP
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		fpCam->move(glm::vec3(0.0f, -moveSpeed, 0.0f) * (float)deltaTime); //and DOWN
	if (glfwGetKey(window, 'W'))
		fpCam->move(glm::vec3(0.0f, 0.0f, moveSpeed) * (float)deltaTime); //Forward
	if (glfwGetKey(window, 'S'))
		fpCam->move(glm::vec3(0.0f, 0.0f, -moveSpeed) * (float)deltaTime); //Backward
	if (glfwGetKey(window, 'A'))
		fpCam->move(glm::vec3(moveSpeed, 0.0f, 0.0f) * (float)deltaTime); //Left
	if (glfwGetKey(window, 'D'))
		fpCam->move(glm::vec3(-moveSpeed, 0.0f, 0.0f) * (float)deltaTime); //Right
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		fpCam->rotate(glm::pi<float>() / 4 * deltaTime, 0.0f); //Turn Left
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		fpCam->rotate(-glm::pi<float>() / 4 * deltaTime, 0.0f); //Turn Right
}

void update(double deltaTime)
{
	running = !glfwGetKey(window, GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(window); //If ESC pressed or program isn't open, stop the program.

	chsCam->update(float(deltaTime)); //Updates camera
	abCam->update(float(deltaTime));
	tarCam->update(float(deltaTime));
	fpCam->update(float(deltaTime));

	GLuint id = tex3->getImageID();
	glDeleteTextures(1, &id);		//Deletes the texture so we don't waste memory
	tex3->create(1024, 1024, data); //Update texture with the data that is being calcualated
	scene->textures["procedural"] = tex3;
	scene->material["mat_003"]->texture = tex3;

	tim += deltaTime; // Time for the starfield movement.

	angle = angle + rotateSpeed * deltaTime; // Constantly update the angle so that objects will rotate

	if (glfwGetKey(window, '=')) // Dissolve all the textures
		dissolveFactor = glm::clamp<float>(dissolveFactor - 1.0f * deltaTime, 0.0f, 1.0f);
	if (glfwGetKey(window, '-'))
		dissolveFactor = glm::clamp<float>(dissolveFactor + 1.0f * deltaTime, 0.0f, 1.0f);

	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT))
		rotateSpeed -= 0.1f; // Alter rotation speeds of all the objects
	if (glfwGetKey(window, GLFW_KEY_KP_ADD))
		rotateSpeed += 0.1f;

	if (glfwGetKey(window, GLFW_KEY_KP_0))
		numPadSelect = 0; // Assign a value so that the last
	if (glfwGetKey(window, GLFW_KEY_KP_1))
		numPadSelect = 1; // Numpad key is remembered
	if (glfwGetKey(window, GLFW_KEY_KP_2))
		numPadSelect = 2; // Used for changing the focus of the cameras
	if (glfwGetKey(window, GLFW_KEY_KP_3))
		numPadSelect = 3; // As well as which object is currently being moved
	if (glfwGetKey(window, GLFW_KEY_KP_4))
		numPadSelect = 4;
	if (glfwGetKey(window, GLFW_KEY_KP_5))
		numPadSelect = 5;
	if (glfwGetKey(window, GLFW_KEY_KP_6))
		numPadSelect = 6;
	if (glfwGetKey(window, GLFW_KEY_KP_7))
		numPadSelect = 7;
	if (glfwGetKey(window, GLFW_KEY_KP_8))
		numPadSelect = 8;
	if (glfwGetKey(window, GLFW_KEY_KP_9))
		numPadSelect = 9;

	if (glfwGetKey(window, '1'))
		camSelect = 1; // Assign values so that the
	else if (glfwGetKey(window, '2'))
		camSelect = 2; // last Number pressed is remembered
	else if (glfwGetKey(window, '3'))
		camSelect = 3; // Used for changing the post processing
	else if (glfwGetKey(window, '4'))
		camSelect = 4; // Effects
	else if (glfwGetKey(window, '5'))
		camSelect = 5;
	else if (glfwGetKey(window, '6'))
		camSelect = 6;
	else if (glfwGetKey(window, '7'))
		camSelect = 7;
	else if (glfwGetKey(window, '8'))
		camSelect = 8;

	if (numPadSelect == 1)
	{
		if (glfwGetKey(window, GLFW_KEY_UP))
			scene->objects["sun"]->transform.move(glm::vec3(0.0f, 0.0f, 0.1f));
		if (glfwGetKey(window, GLFW_KEY_DOWN))
			scene->objects["sun"]->transform.move(glm::vec3(0.0f, 0.0f, -0.1f));
		if (glfwGetKey(window, GLFW_KEY_LEFT))
			scene->objects["sun"]->transform.move(glm::vec3(0.1f, 0.0f, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
			scene->objects["sun"]->transform.move(glm::vec3(-0.1f, 0.0f, 0.0f));
	}
	else if (numPadSelect == 2)
	{
		if (glfwGetKey(window, GLFW_KEY_UP))
			scene->objects["crate"]->transform.move(glm::vec3(0.0f, 0.0f, 0.1f));
		if (glfwGetKey(window, GLFW_KEY_DOWN))
			scene->objects["crate"]->transform.move(glm::vec3(0.0f, 0.0f, -0.1f));
		if (glfwGetKey(window, GLFW_KEY_LEFT))
			scene->objects["crate"]->transform.move(glm::vec3(0.1f, 0.0f, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
			scene->objects["crate"]->transform.move(glm::vec3(-0.1f, 0.0f, 0.0f));
	}
	else if (numPadSelect == 3)
	{
		if (glfwGetKey(window, GLFW_KEY_UP))
			scene->objects["barrel_01"]->transform.move(glm::vec3(0.0f, 0.0f, 0.1f));
		if (glfwGetKey(window, GLFW_KEY_DOWN))
			scene->objects["barrel_01"]->transform.move(glm::vec3(0.0f, 0.0f, -0.1f));
		if (glfwGetKey(window, GLFW_KEY_LEFT))
			scene->objects["barrel_01"]->transform.move(glm::vec3(0.1f, 0.0f, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
			scene->objects["barrel_01"]->transform.move(glm::vec3(-0.1f, 0.0f, 0.0f));
	}

	//########################################################################################################################################################
	scene->objects["ball_03"]->transform.position = glm::vec3(0.0f, -40.0f, -500.0f);
	orbit = scene->objects["ball_03"]->transform.position;
	scene->objects["ball_03"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["ball"]->transform.position = glm::vec3(sin(angle) * rad, 0.0f, cos(angle) * rad) + orbit;
	scene->objects["ball"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	//scene->objects["ball_04"]->transform.position = glm::vec3(sin(-angle*8)*3 , sin(-angle*8)*3 , cos(-angle*4)*3) + orbit; //Cool movement
	scene->objects["ball_04"]->transform.position = glm::vec3(sin(-angle * 8) * 3 + 10, -10.0f, cos(-angle * 4) * 3 - 490.0f);
	scene->objects["ball_04"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	orbit = scene->objects["ball"]->transform.position;

	scene->objects["ball_02"]->transform.position = glm::vec3(sin(-angle * 4) * 3, sin(-angle * 4) * 3 + cos(-angle * 4) * 3, cos(-angle * 4) * 3) + orbit;
	scene->objects["ball_02"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	orbit = scene->objects["ball_02"]->transform.position;

	scene->objects["torus"]->transform.position = orbit;
	scene->objects["torus"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["torus_02"]->transform.rotate(glm::pi<float>() / 2 * deltaTime, glm::vec3(sin(-angle * 4) * 3, cos(-angle * 4) * 3, cos(-angle * 4) * 3));
	scene->objects["torus_03"]->transform.rotate(glm::pi<float>() / 2 * deltaTime, glm::vec3(sin(-angle * 4) * 3, cos(-angle * 4) * 3, cos(-angle * 4) * 3));

	lightAim = scene->objects["ball"]->transform.position;

	//Planetary orbits
	orbit = scene->objects["sun"]->transform.position;
	scene->objects["sun"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["mercury"]->transform.position = glm::vec3(sin(angle) * 12, 0.0f, cos(angle) * 18) + orbit;
	scene->objects["mercury"]->transform.rotate(glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["venus"]->transform.position = glm::vec3(sin(angle / 2.5) * 27.92, 0.0f, cos(angle / 2.5) * 28.432) + orbit;
	scene->objects["venus"]->transform.rotate(-glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["earth"]->transform.position = glm::vec3(sin(angle / 4.15) * 38.4, 0.0f, cos(angle / 4.15) * 39.728) + orbit;
	scene->objects["earth"]->transform.rotate(glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));
	scene->objects["earthmoon"]->transform.position = glm::vec3(sin(angle / 0.46) * 0.5218, sin(angle / 0.46) * 0.5218, -cos(angle / 0.46) * 0.5218) + scene->objects["earth"]->transform.position;
	scene->objects["earthmoon"]->transform.rotate(-glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["mars"]->transform.position = glm::vec3(sin(angle / 7.8) * 53.44, 0.0f, cos(angle / 7.8) * 64.96) + orbit;
	scene->objects["mars"]->transform.rotate(-glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));
	scene->objects["phobos"]->transform.position = glm::vec3(sin(angle / 0.004) * 0.2153728, sin(angle / 0.004) * 0.2153728, -cos(angle / 0.004) * 0.2153728) + scene->objects["mars"]->transform.position;
	scene->objects["phobos"]->transform.rotate(-glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["jupiter"]->transform.position = glm::vec3(sin(angle / 49.2) * 193.28, 0.0f, cos(angle / 49.2) * 213.12) + orbit;
	scene->objects["jupiter"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));
	scene->objects["ganymede"]->transform.position = glm::vec3(sin(angle / 0.122) * 4.89, sin(angle / 0.122) * 4.89, -cos(angle / 0.122) * 4.89) + scene->objects["jupiter"]->transform.position;
	scene->objects["ganymede"]->transform.rotate(-glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["saturn"]->transform.position = glm::vec3(sin(angle / 122.22) * 339.2, 0.0f, cos(angle / 122.22) * 393.92) + orbit;
	scene->objects["saturn"]->transform.rotate(-glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));
	scene->objects["titan"]->transform.position = glm::vec3(sin(angle / 0.18) * 4.2, sin(angle / 0.18) * 4.2, -cos(angle / 0.18) * 4.2) + scene->objects["saturn"]->transform.position;
	scene->objects["titan"]->transform.rotate(-glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));
	scene->objects["saturnring"]->transform.position = scene->objects["saturn"]->transform.position;
	scene->objects["saturnring"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["uranus"]->transform.position = glm::vec3(sin(angle / 348.68) * 717.36, 0.0f, cos(angle / 348.68) * 782.64) + orbit;
	scene->objects["uranus"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));
	scene->objects["titania"]->transform.position = glm::vec3(sin(angle / 0.068) * 1.745, sin(angle / 0.068) * 1.745, -cos(angle / 0.068) * 1.745) + scene->objects["uranus"]->transform.position;
	scene->objects["titania"]->transform.rotate(-glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["neptune"]->transform.position = glm::vec3(sin(angle / 683.50) * 1160.88, 0.0f, cos(angle / 683.50) * 1186.96) + orbit;
	scene->objects["neptune"]->transform.rotate(glm::pi<float>() / 4 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));
	scene->objects["triton"]->transform.position = glm::vec3(sin(angle / 0.1) * 1.76, sin(angle / 0.1) * 1.76, -cos(angle / 0.1) * 1.76) + scene->objects["neptune"]->transform.position;
	scene->objects["triton"]->transform.rotate(-glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));

	scene->objects["pluto"]->transform.position = glm::vec3(sin(angle / 1028.64) * 1158.24, 0.0f, cos(angle / 1028.64) * 1925.2) + orbit;
	scene->objects["pluto"]->transform.rotate(-glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));
	scene->objects["charon"]->transform.position = glm::vec3(sin(angle / 0.073) * 0.15, sin(angle / 0.073) * 0.15, -cos(angle / 0.073) * 0.15) + scene->objects["pluto"]->transform.position;
	scene->objects["charon"]->transform.rotate(-glm::pi<float>() / 2 * deltaTime, glm::vec3(1.0f, 0.0f, 1.0f));
	//########################################################################################################################################################

	if (numPadSelect == 1)
		orbit = scene->objects["sun"]->transform.position;
	if (numPadSelect == 2)
		orbit = scene->objects["mercury"]->transform.position;
	if (numPadSelect == 3)
		orbit = scene->objects["venus"]->transform.position;
	if (numPadSelect == 4)
		orbit = scene->objects["earth"]->transform.position;
	if (numPadSelect == 5)
		orbit = scene->objects["mars"]->transform.position;
	if (numPadSelect == 6)
		orbit = scene->objects["jupiter"]->transform.position;
	if (numPadSelect == 7)
		orbit = scene->objects["saturn"]->transform.position;
	if (numPadSelect == 8)
		orbit = scene->objects["uranus"]->transform.position;
	if (numPadSelect == 9)
		orbit = scene->objects["neptune"]->transform.position;
	if (numPadSelect == 0)
		orbit = scene->objects["pluto"]->transform.position;

	if (glfwGetKey(window, GLFW_KEY_KP_DIVIDE))
		chsCam->setPositionOffset(chsCam->getPositionOffset() + glm::vec3(0.1f, 0.0f, 0.1f));
	if (glfwGetKey(window, GLFW_KEY_KP_MULTIPLY))
		chsCam->setPositionOffset(chsCam->getPositionOffset() - glm::vec3(0.1f, 0.0f, 0.1f));
	if (glfwGetKey(window, GLFW_KEY_KP_ENTER))
		rotateSpeed = 0.0f;

	tarCam->setTarget(orbit);
	chsCam->setTarget(orbit);
	chsCam->setFollowPosition(orbit);
	abCam->setTarget(orbit);
	fpCam->setTarget(orbit);

	//if (glfwGetKey('O')) power = std::max<float>(power - lightAimSpeed, 0.0f);
	//if (glfwGetKey('P')) power += lightAimSpeed;

	if (glfwGetKey(window, GLFW_KEY_UP))
		lightAim.z -= lightAimSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN))
		lightAim.z += lightAimSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT))
		lightAim.x -= lightAimSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
		lightAim.x += lightAimSpeed;

	fpsCameraMove(deltaTime);

	if (glfwGetKey(window, 'I'))
		chsCam->rotate(glm::vec3(glm::pi<float>() / 100.0f, 0.0f, 0.0f));
	if (glfwGetKey(window, 'K'))
		chsCam->rotate(glm::vec3(-glm::pi<float>() / 100.0f, 0.0f, 0.0f));
	if (glfwGetKey(window, 'J'))
		chsCam->rotate(glm::vec3(0.0f, -glm::pi<float>() / 100.0f, 0.0f));
	if (glfwGetKey(window, 'L'))
		chsCam->rotate(glm::vec3(0.0f, glm::pi<float>() / 100.0f, 0.0f));

	//if (glfwGetKey('1')) scene = loadScene("scene2.json");	// TODO load in alternate scenes

	if (glfwGetKey(window, '['))
		glPolygonMode(GL_FRONT, GL_LINE); // Change to wire frame mode
	if (glfwGetKey(window, ']'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Change to normal mode
	if (glfwGetKey(window, 'P'))
		abCam->rotate(glm::pi<float>() / 100.0f, 0.0f); // Rotate the arcball camera
	if (glfwGetKey(window, ';'))
		abCam->rotate(-glm::pi<float>() / 100.0f, 0.0f);
}

void render(const effect *eff, const glm::mat4 &view, const glm::mat4 &projection, const render_object *object)
{
	CHECK_GL_ERROR
	glm::mat4 mvp = projection * view * object->transform.getTransformMatrix();
	glUniformMatrix4fv(eff->getUniformIndex("modelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
	glm::mat4 mit = glm::inverse(glm::transpose(object->transform.getTransformMatrix()));
	glUniformMatrix4fv(eff->getUniformIndex("modelInverseTranspose"), 1, GL_FALSE, glm::value_ptr(mit));
	glUniformMatrix4fv(eff->getUniformIndex("model"), 1, GL_FALSE, glm::value_ptr(object->transform.getTransformMatrix()));

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), object->transform.scale);
	glUniformMatrix4fv(eff->getUniformIndex("scale"), 1, GL_FALSE, glm::value_ptr(scale));
	CHECK_GL_ERROR

	object->material->bind(eff);

	glBindVertexArray(object->geometry->vao);
	if (object->geometry->indexBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->geometry->indexBuffer);
		glDrawElements(GL_TRIANGLES, object->geometry->indices.size(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, object->geometry->vertices.size());
	glBindVertexArray(0);
}

void sceneRender()
{
	glViewport(0, resY / 2, resX / 2, resY / 2);
	sb->render(chsCam);
	glViewport(resX / 2, resY / 2, resX / 2, resY / 2);
	sb->render(abCam);
	glViewport(resX / 2, 0, resX / 2, resY / 2);
	sb->render(tarCam);
	glViewport(0, 0, resX / 2, resY / 2);
	sb->render(fpCam);

	star.begin();

	float t = tim * 100.0f;
	t = fmod(t, 999.0f);
	glUniform1f(star.getUniformIndex("time"), t);
	glViewport(0, resY / 2, resX / 2, resY / 2);
	points->render(&star, chsCam);
	glViewport(resX / 2, resY / 2, resX / 2, resY / 2);
	points->render(&star, abCam);
	glViewport(resX / 2, 0, resX / 2, resY / 2);
	points->render(&star, tarCam);
	glViewport(0, 0, resX / 2, resY / 2);
	points->render(&star, fpCam);

	star.end();

	eff.begin();

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, scene->textures["dissolve"]->getImageID());
	glUniform1i(eff.getUniformIndex("dissolveTexture"), 1);
	glUniform1f(eff.getUniformIndex("dissolveFactor"), dissolveFactor);

	scene->light.bind(&eff);

	//Create light direction from lightAngle
	spot_light_data spot = scene->dynamic.data.spots[0];
	spot.lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	spot.lightDir = glm::vec4(lightAim, 1.0f) - spot.lightPos;
	spot.lightDir = glm::normalize(spot.lightDir);
	spot.power = 1.0f;

	//Set the light
	scene->dynamic.setSpot(0, spot);

	point_light_data point = scene->dynamic.data.points[0];
	point.lightPos = glm::vec4(orbit, 1.0f);

	//Set the light
	scene->dynamic.setPoint(0, point);

	scene->dynamic.bind(&eff);

	//Camera 1
	glViewport(0, resY / 2, resX / 2, resY / 2);

	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(chsCam->getPosition()));
	std::hash_map<std::string, render_object *>::const_iterator iter = scene->objects.begin();
	for (; iter != scene->objects.end(); ++iter)
		render(&eff, chsCam->getView(), chsCam->getProjection(), iter->second);

	//Camera 2
	glViewport(resX / 2, resY / 2, resX / 2, resY / 2);

	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(abCam->getPosition()));
	iter = scene->objects.begin();
	for (; iter != scene->objects.end(); ++iter)
		render(&eff, abCam->getView(), abCam->getProjection(), iter->second);

	//Camera 3
	glViewport(resX / 2, 0, resX / 2, resY / 2);

	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(tarCam->getPosition()));
	iter = scene->objects.begin();
	for (; iter != scene->objects.end(); ++iter)
		render(&eff, tarCam->getView(), tarCam->getProjection(), iter->second);

	//Camera 4
	glViewport(0, 0, resX / 2, resY / 2);

	glUniform3fv(eff.getUniformIndex("eyePos"), 1, glm::value_ptr(fpCam->getPosition()));
	iter = scene->objects.begin();
	for (; iter != scene->objects.end(); ++iter)
		render(&eff, fpCam->getView(), fpCam->getProjection(), iter->second);

	eff.end();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (camSelect == 1)
	{
		post_norm->beginRender(true);
		sceneRender();
		post_norm->endRender();
	}
	else if (camSelect == 2)
	{
		post_grey->beginRender(true);
		sceneRender();
		post_grey->endRender();
	}
	else if (camSelect == 3)
	{
		post_blur->beginRender(true);
		sceneRender();
		post_blur->endRender();
	}
	else if (camSelect == 4)
	{
		post_proc->beginRender(true);
		sceneRender();
		post_proc->endRender();
	}
	else if (camSelect == 5)
	{
		post_shar->beginRender(true);
		sceneRender();
		post_shar->endRender();
	}
	else if (camSelect == 6)
	{
		post_edge->beginRender(true);
		sceneRender();
		post_edge->endRender();
	}
	else if (camSelect == 7)
	{
		post_sced->beginRender(true);
		sceneRender();
		post_sced->endRender();
	}
	else if (camSelect == 8)
	{
		post_proc->beginRender(true);
		sceneRender();
		post_proc->endRender();

		post_grey->beginRender(true);
		sceneRender();
		post_grey->endRender();

		post_blur->beginRender(true);
		sceneRender();
		post_blur->endRender();

		post_norm->beginRender(true);
		sceneRender();
		post_norm->endRender();
	}

	glViewport(0, 0, resX, resY);
	pass_2[frame]->beginRender(true);

	post_proc->getEffect()->begin();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pass_2[(frame + 1) % 2]->getFrameBuffer()->getTextureID());

	// Set the texture uniform
	glUniform1i(post_proc->getEffect()->getUniformIndex("tex2"), 1);
	glUniform1f(post_proc->getEffect()->getUniformIndex("blend_factor"), 0.8f);
	post_proc->render(true);

	pass_2[frame]->endRender();

	pass_2[frame]->getEffect()->begin();

	if (camSelect == 1)
		post_norm->render(true);
	else if (camSelect == 2)
		post_grey->render(true);
	else if (camSelect == 3)
		post_blur->render(true);
	else if (camSelect == 4)
		pass_2[frame]->render(true);
	else if (camSelect == 5)
		post_shar->render(true);
	else if (camSelect == 6)
		post_edge->render(true);
	else if (camSelect == 7)
		post_sced->render(true);
	else if (camSelect == 8)
	{
		glViewport(0, 0, resX / 2, resY / 2);
		pass_2[frame]->render(true);

		glViewport(resX / 2, resY / 2, resX / 2, resY / 2);
		post_grey->render(false);

		glViewport(0, resY / 2, resX / 2, resY / 2);
		post_norm->render(false);

		glViewport(resX / 2, 0, resX / 2, resY / 2);
		post_blur->render(false);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();

	frame = (++frame) % 2;
}

void cleanup() //Allow us to clean up used resources in our application
{
}

bool resolutionSelect() //Sets the resolution of the application
{
	int val = 0;
	char SELEC;

	GLFWmonitor *return_struct = glfwGetPrimaryMonitor();

	const GLFWvidmode *mode = glfwGetVideoMode(return_struct); // Gets the current desktop resolution

	int width = mode->width;
	int height = mode->height;
	resX = width;
	resY = height;
	do
	{
		system("CLS");
		std::cout << "Please select your resolution\n1) 800x600\n2) 1024x768\n3) 1280x720\n4) 1920x1080\n5) Native : " << width << "x" << height;
		std::cout << "\n\nYour native resolution is " << width << "x" << height << " and has been set as the default\n";
		std::cout << "\nPlease press either 1,2,3,4 or 5.\n";
		SELEC = _getche(); //Takes in the users choice instantly without having to press enter
	} while (SELEC < '1' || SELEC > '5');

	val = SELEC - '0'; //Converts from ASCII to the actual value
	if (val == 1)
	{
		resX = 800;
		resY = 600;
	}
	else if (val == 2)
	{
		resX = 1024;
		resY = 768;
	}
	else if (val == 3)
	{
		resX = 1280;
		resY = 720;
	}
	else if (val == 4)
	{
		resX = 1920;
		resY = 1080;
	}
	else if (val == 5)
	{
	}
	else
		return (false);

	system("CLS"); //Clear screen so it isn't too crowded

	midWindowX = resX / 2; // Middle of the window horizontally
	midWindowY = resY / 2;
	return (true);
}

int main()
{
	SET_DEBUG
	// TODO: This is deprecated
	// int mode = GLFW_WINDOW;
	if (!glfwInit())		//initialise GLFW by calling gltwInit
		exit(EXIT_FAILURE); //If cant find it then close window

	int check = MessageBox(NULL, "Welcome! \nWould you like to run this application in full screen mode?", "Welcome to Mark's Program!", MB_YESNO | MB_ICONINFORMATION);
	if (check == IDYES)
	{
		// mode = GLFW_FULLSCREEN; // Set to fullscreen mode
	}
	else if (check == IDNO)
		std::cout << "OK, your loss ...\n";
	ShowCursor(FALSE);

	//glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);	//Anti Aliasing effect only seems to work at 16

	if (!resolutionSelect()) //Select resolution
		std::cout << "\nResolution selection failed, you have been set to the default resolution\n";

	window = glfwCreateWindow(int(resX), int(resY), "GF Submission", NULL, NULL);

	// Set window hints for GLFW

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// glfwWindowHint(GLFW_ICONIFIED, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	// glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
	// glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
	// glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
	// glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);

	if (!window) //Setting window size
	{
		glfwTerminate();	//If can open window this size Terminate GLFW
		exit(EXIT_FAILURE); //And exit
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Set swap interval - make the window refresh with the monitor
	glfwSwapInterval(1);

	GLenum error = glewInit();
	if (error != GLEW_OK) //Check for errors in glewInit
	{
		std::cout << "Error: " << glewGetErrorString(error) << std::endl;
		exit(EXIT_FAILURE);
	}

	// ilInit();
	// iluInit();
	// ilutRenderer(ILUT_OPENGL);

	std::cout << "GL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("Resolution : %g * %g\n", resX, resY);

	initialise(); //Intialise everything so far then enter main render loop

	glfwSetCursorPos(window, midWindowX, midWindowY);
	glfwSetCursorPosCallback(window, handleMouseMove);
	//glfwSetKeyCallback(handleKeyInput);

	prevTimeStamp = glfwGetTime();
	currentTimeStamp; //Place to store the current time
	while (running)	  //while game running
	{
		currentTimeStamp = glfwGetTime();		  //Store the current time in "currentTimeStamp"
		update(currentTimeStamp - prevTimeStamp); //Subtract prev time from current time to give us time passed since last frame
		render();								  //Then render
		prevTimeStamp = currentTimeStamp;		  //Then set previous frame time to current frame time
	}											  //Loop continues until running is set to false

	cleanup();

	glfwTerminate();

	exit(EXIT_SUCCESS);
}