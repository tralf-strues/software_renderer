//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "../engine/entities/Terrain.h"
#include "../engine/Display.h"
#include "../engine/math/engineMath.h"
#include "../engine/tools/ResLoader.h"

#define FPS_LIMIT 120
#define WIDTH 1120 // 640, 960, 1120, 1280, 1360, 1440
#define HEIGHT 630 // 480, 540, 630, 720, 765, 810
#define CENTER_X 640
#define CENTER_Y 360
#define VELOCITY 0.3f
#define MOUSE_SENSITIVITY 0.001f

#define PLAYER_HIGHT 10.0f
#define GRAVITY 0

bool init();
void close();

Display display;
SDL_Window* gWindow = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Software renderer",
								   SDL_WINDOWPOS_UNDEFINED,
								   SDL_WINDOWPOS_UNDEFINED,
								   WIDTH,
								   HEIGHT,
								   SDL_WINDOW_SHOWN);

		/*SDL_SetWindowFullscreen(gWindow, 0);*/
		/*SDL_SetRelativeMouseMode(SDL_TRUE);*/ // hides cursor outside of the game window
		SDL_ShowCursor(SDL_DISABLE);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			SDL_GL_SetSwapInterval(0); // disable vsync
			//Display
			display = Display(gWindow, WIDTH, HEIGHT);
		}

	}

	return success;
}

void close()
{
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

void generateSkybox()
{

}

void updateViewMatrix(mat4& viewMatrix, Camera& camera)
{
	viewMatrix = mat4::lootAtDirectionVector(camera.position, camera.forward);
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		bool quit = false;

		//Event handler
		SDL_Event e;

		// Objects:
		Texture textureMonkey = loadTexture("monkey", "monkey.bmp");
		Texture texturePurple = loadTexture("purple", "purple.bmp");
		Texture textureStall = loadTexture("stall", "stall.bmp");
		Texture textureGrass = loadTexture("grass", "grass.bmp");
		Texture textureSpyroLevel = loadTexture("spyro level", "spyro level.bmp");

		printf("%d - Vector comparison\n", vec3(0, 0, 1) == vec3(0, 0, 1));

		std::vector<Mesh> meshes;
		Mesh pine = loadOBJ("Pine", "pine");
		pine.position = vec3(10, 0, -10);
		Mesh box = loadOBJ("Box", "box");
		box.position = vec3(0, 1, 20);
		box.texture = &textureStall;
		Mesh person = loadOBJ("Person", "person");
		person.position = vec3(-10, 0, 0);
		Mesh stall = loadOBJ("Stall", "stall");
		stall.position = vec3(7, 0, 0);
		stall.setScale(2.5);
		stall.texture = &textureStall;
		/*Mesh bunny = loadOBJ("Bunny", "bunny");
		bunny.position = vec3(-20, 0, 0);
		Mesh dragon = loadOBJ("Dragon", "dragon");
		dragon.position = vec3(20, 0, 0);
		Mesh teapot = loadOBJ("Teapot", "utahteapot");
		teapot.setScale(0.1f);
		teapot.position = vec3(20, 0, 15);*/
		Mesh monkey = loadOBJ("Monkey", "suzanne");
		monkey.setScale(4.0f);
		monkey.position = vec3(-15, 30, -15);
		monkey.rotation = vec3(0, degreesToRadians(-135), 0);
		monkey.texture = &textureMonkey;

		Terrain terrain(0, 0);
		terrain.mesh.position = vec3(-25, 0, -25);
		terrain.mesh.texture = &textureGrass;
		/*std::cout << terrain.mesh.faces.size() << std::endl;
		std::cout << terrain.mesh.vertices.size() << std::endl;*/

		Mesh test("test", 3, 1);
		test.vertices = std::vector<vec3>{ vec3(0, 0, 0), vec3(5, 0, 0), vec3(5, 5, 0) };
		test.textures = std::vector<vec2>{ vec2(0, 0), vec2(0.25, 0), vec2(0.25, 0.25) };
		test.texture = &textureStall;
		//test.texture = &texturePurple;
		test.normals = std::vector<vec3>{ vec3(0, 0, -1), vec3(0, 0, -1), vec3(0, 0, -1) };
		Face testFace;
		testFace.vertexIndices = vec3(0, 1, 2);
		testFace.textureIndices = vec3(0, 1, 2);
		testFace.normalIndices = vec3(0, 1, 2);
		test.faces = std::vector<Face>{ testFace };
		test.position = vec3(-5, 2, -5);
		test.rotation = vec3(0, degreesToRadians(-180), 0);
		test.setScale(4);

		Mesh spyroLevel = loadOBJ("Spyro level", "spyro level2");
		spyroLevel.position = vec3(0, 0, 0);
		spyroLevel.setScale(0.75);
		spyroLevel.texture = &textureSpyroLevel;

		meshes.push_back(spyroLevel);
		//meshes.push_back(test);
		//meshes.push_back(pine);
		//meshes.push_back(box);
		//meshes.push_back(person);
		//meshes.push_back(stall);
		/*meshes.push_back(bunny);
		meshes.push_back(dragon);*/
		//meshes.push_back(teapot);
		//meshes.push_back(monkey);
		//meshes.push_back(terrain.mesh);

		int totalNumberOfFaces = 0;
		int totalNumberOfVertices = 0;
		for (Mesh mesh : meshes)
		{
			totalNumberOfFaces += mesh.faces.size();
			totalNumberOfVertices += mesh.vertices.size();
		}

		std::cout << "TOTAL {faces: " <<
			totalNumberOfFaces <<
			", vertices: " <<
			totalNumberOfVertices <<
			"}" <<
			std::endl;

		// FPS counter
		Uint32 frameStart;
		int frameTime;

		// Camera:
		Camera camera(vec3(-45, PLAYER_HIGHT, -45), vec3(-1, 0, -1));
		int xMouse, yMouse;
		SDL_WarpMouseInWindow(gWindow, CENTER_X, CENTER_Y);

		int angle = 0;
		bool rotating = false;
		bool lightFollowingCamera = true;

		//Light Sources:
		vec4 lightSourcePosition = vec4(camera.position.x,
										camera.position.y,
										camera.position.z,
										1);
		LightSource lightSource(lightSourcePosition, vec4(255, 255, 255, 255), 1);

		//Rendering specification
		BackFaceCulling backFaceCulling = BACK_FACE_CULLING_ENABLED_WCS;
		RenderingType renderingType = RASTERIZATION;
		ProjectionType projectionType = PERSPECTIVE_PROJECTION;
		ShadingType shadingType = GOURAUD_SHADING;

		mat4 viewMatrix;
		updateViewMatrix(viewMatrix, camera);

		while (!quit)
		{
			frameStart = SDL_GetTicks();

			const Uint8* keystate = SDL_GetKeyboardState(NULL);

			if (keystate[SDL_SCANCODE_W])
				camera.position -= vec3::normalize(camera.forward) * VELOCITY;

			if (keystate[SDL_SCANCODE_A])
			{
				// todo change (0, 1, 0) to a constant
				vec3 r = vec3::cross(vec3(0, 1, 0),
									 vec3::normalize(camera.forward));
				camera.position -= r * VELOCITY;
				/*camera.to -= r * VELOCITY;*/
			}

			if (keystate[SDL_SCANCODE_S])
				camera.position += vec3::normalize(camera.forward) * VELOCITY;

			if (keystate[SDL_SCANCODE_D])
			{
				// todo change (0, 1, 0) to a constant
				vec3 r = vec3::cross(vec3(0, 1, 0),
									 vec3::normalize(camera.forward));
				camera.position += r * VELOCITY;
				/*camera.to += r * VELOCITY;*/
			}

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
					quit = true;
				else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
				{
					if (keystate[SDL_SCANCODE_ESCAPE])
						quit = true;

					if (keystate[SDL_SCANCODE_SPACE])
						rotating = !rotating;

					if (keystate[SDL_SCANCODE_T])
						lightFollowingCamera = !lightFollowingCamera;

					if (keystate[SDL_SCANCODE_B])
					{
						if (backFaceCulling == BACK_FACE_CULLING_ENABLED_WCS)
							backFaceCulling = BACK_FACE_CULLING_DISABLED;
						else
							backFaceCulling = BACK_FACE_CULLING_ENABLED_WCS;
					}

					if (keystate[SDL_SCANCODE_R])
					{
						if (renderingType == RASTERIZATION)
							renderingType = RASTERIZATION_WITH_WIREFRAME;
						else if (renderingType == RASTERIZATION_WITH_WIREFRAME)
							renderingType = WIREFRAME_RENDERING;
						else
							renderingType = RASTERIZATION;
					}

					if (keystate[SDL_SCANCODE_P])
					{
						if (projectionType == PERSPECTIVE_PROJECTION)
							projectionType = ORTHOGRAPHIC_PROJECTION;
						else
							projectionType = PERSPECTIVE_PROJECTION;
						display.updateProjectionMatrix();
					}

					if (keystate[SDL_SCANCODE_L])
					{
						if (shadingType == GOURAUD_SHADING)
							shadingType = FLAT_SHADING;
						else
							shadingType = GOURAUD_SHADING;
					}

					if (keystate[SDL_SCANCODE_O])
						camera.forward = vec3(1, 0, 1); // todo

					/* no need to update it in all the cases above,
					but I'd rather just put it here than duplicate it several times */
					updateViewMatrix(viewMatrix, camera);
				}
				else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
				{
					SDL_GetMouseState(&xMouse, &yMouse);

					//std::cout << "ANGLE_Y: " << camera.angleY << "\n";

					float xMouseDelta = xMouse - CENTER_X;
					float yMouseDelta = yMouse - CENTER_Y;

					SDL_WarpMouseInWindow(gWindow, CENTER_X, CENTER_Y);

					vec3 anglesXZ(0, -xMouseDelta * MOUSE_SENSITIVITY, 0);

					/* angle between forward and its projection on xz plane */
					float angleYDelta = +yMouseDelta * MOUSE_SENSITIVITY;
					//vec3 forward = camera.to - camera.position;
					/*float original_length = vec3::length(forward);*/
					vec3 forwardXZ(camera.forward.x, 0, camera.forward.z);

					forwardXZ = vec4::toVec3(mat4::rotationPitchYawRoll(anglesXZ) * vec4::homogeneous(forwardXZ));

					vec3 forwardY(0, 0, 0);
					if (camera.angleY == 0)
					{
						forwardY.y = vec3::length(camera.forward) * sin(camera.angleY + angleYDelta);
						camera.angleY = camera.angleY + angleYDelta;
					}
					else if (camera.angleY + angleYDelta > ANGLE_Y_LIMIT)
					{
						forwardY.y = camera.forward.y * sin(ANGLE_Y_LIMIT) / sin(camera.angleY);
						camera.angleY = ANGLE_Y_LIMIT;
					}
					else if (camera.angleY + angleYDelta < -ANGLE_Y_LIMIT)
					{
						forwardY.y = camera.forward.y * sin(-ANGLE_Y_LIMIT) / sin(camera.angleY);
						camera.angleY = -ANGLE_Y_LIMIT;
					}
					else
					{
						forwardY.y = camera.forward.y * sin(camera.angleY + angleYDelta) / sin(camera.angleY);
						camera.angleY = camera.angleY + angleYDelta;
					}

					camera.forward = forwardXZ + forwardY;
					//camera.to = camera.position + vec3::normalize(forward) * original_length;
					/*	camera.to = camera.position + forward;*/

					/* no need to update it in all the cases above,
					but I'd rather just put it here than duplicate it several times */
					updateViewMatrix(viewMatrix, camera);
				}
			}

			// PHYSICS
			float playerPosition = camera.position.y - PLAYER_HIGHT;
			if (playerPosition - GRAVITY < 0)
				playerPosition = 0;
			else
				playerPosition -= GRAVITY;
			camera.position.y = playerPosition + PLAYER_HIGHT;

			//MY RENDERING
			if (rotating)
			{
				//std::cout << angle << std::endl;

				for (int i = 0; i < meshes.size(); i++)
				{
					meshes.at(i).rotation.setCoordinate(1, degreesToRadians(angle));
				}

				angle = (angle + 1) % 360;
			}

			if (lightFollowingCamera)
				lightSource.position = vec4(camera.position.x,
											camera.position.y,
											camera.position.z,
											1);
			else
				lightSource.position = vec4(-45, 5, -45, 1);

			display.render(camera, viewMatrix, lightSource, meshes,
						   backFaceCulling, renderingType, projectionType, shadingType);

			//display.drawBitmap(0, 0, textureMonkey);
			/*display.drawBitmap(0, 0, textureStall);*/

			SDL_UpdateWindowSurface(gWindow);

			frameTime = SDL_GetTicks() - frameStart;
			if (FPS_LIMIT > 0 && 1000 / frameTime > FPS_LIMIT)
			{
				SDL_Delay(1000 / FPS_LIMIT - frameTime);
				SDL_SetWindowTitle(gWindow, ("Software renderer [FPS: " + std::to_string(FPS_LIMIT) + "]").c_str());
			}
			else
			{
				SDL_SetWindowTitle(gWindow, ("Software renderer [FPS: " + std::to_string(1000 / frameTime) + "]").c_str());
			}
		}
	}
	close();

	return 0;
}