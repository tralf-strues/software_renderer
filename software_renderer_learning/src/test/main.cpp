//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "../engine/Display.h"
#include "../engine/math/engineMath.h"
#include "../engine/tools/OBJLoader.h"

#define FPS_LIMIT 120
#define WIDTH 1280 // 640, 1280
#define HEIGHT 720 // 480, 720
#define CENTER_X 640
#define CENTER_Y 360
#define VELOCITY 0.3
#define MOUSE_SENSITIVITY 0.001

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
		std::vector<Mesh> meshes;

		Mesh pine = loadOBJ("Pine", "pine");
		pine.position = vec3(10, -10, -10);
		Mesh box = loadOBJ("Box", "box");
		box.position = vec3(0, 0, 20);
		Mesh person = loadOBJ("Person", "person");
		person.position = vec3(-10, 0, 0);
		Mesh stall = loadOBJ("Stall", "stall");
		stall.position = vec3(-2, 2, 0);

		meshes.push_back(pine);
		meshes.push_back(box);
		meshes.push_back(person);
		meshes.push_back(stall);

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
		Camera camera(vec3(0, 10, 70), vec3(0, 0, 0), 0);
		int xMouse, yMouse;
		SDL_WarpMouseInWindow(gWindow, CENTER_X, CENTER_Y);

		int angle = 0;
		bool rotating = false;
		bool lightFollowingCamera = true;

		//Light Sources:
		LightSource lightSource(camera.position, vec4(255, 255, 255, 255), 1);

		//Rendering specification
		BackFaceCulling backFaceCulling = BACK_FACE_CULLING_ENABLED_WCS;
		RenderingType renderingType = RASTERIZATION;
		ProjectionType projectionType = PERSPECTIVE_PROJECTION;
		ShadingType shadingType = GOURAUD_SHADING;

		while (!quit)
		{
			frameStart = SDL_GetTicks();

			const Uint8* keystate = SDL_GetKeyboardState(NULL);

			if (keystate[SDL_SCANCODE_W])
				camera.position += vec3::normalize(camera.to - camera.position) * VELOCITY;

			if (keystate[SDL_SCANCODE_A])
			{
				// todo change (0, 1, 0) to a constant
				vec3 r = vec3::cross(vec3(0, 1, 0),
									 vec3::normalize(camera.position - camera.to));
				camera.position -= r * VELOCITY;
				camera.to -= r * VELOCITY;
			}

			if (keystate[SDL_SCANCODE_S])
				camera.position -= vec3::normalize(camera.to - camera.position) * VELOCITY;

			if (keystate[SDL_SCANCODE_D])
			{
				// todo change (0, 1, 0) to a constant
				vec3 r = vec3::cross(vec3(0, 1, 0),
									 vec3::normalize(camera.position - camera.to));
				camera.position += r * VELOCITY;
				camera.to += r * VELOCITY;
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
							backFaceCulling = BACK_FACE_CULLING_ENABLED_NDCS;
						else if (backFaceCulling == BACK_FACE_CULLING_ENABLED_NDCS)
							backFaceCulling = BACK_FACE_CULLING_DISABLED;
						else if (backFaceCulling == BACK_FACE_CULLING_DISABLED)
							backFaceCulling = BACK_FACE_CULLING_ENABLED_WCS;
					}

					if (keystate[SDL_SCANCODE_R])
					{
						if (renderingType == RASTERIZATION)
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
					}

					if (keystate[SDL_SCANCODE_L])
					{
						if (shadingType == GOURAUD_SHADING)
							shadingType = FLAT_SHADING;
						else
							shadingType = GOURAUD_SHADING;
					}

					if (keystate[SDL_SCANCODE_O])
						camera.to = vec3(0, 0, 0);
				}
				else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
				{
					SDL_GetMouseState(&xMouse, &yMouse);

					std::cout << "(" << xMouse << ", " << yMouse << ")\n";

					float xMouseDelta = xMouse - CENTER_X;
					float yMouseDelta = yMouse - CENTER_Y;

					SDL_WarpMouseInWindow(gWindow, CENTER_X, CENTER_Y);

					vec3 anglesXZ(0, -xMouseDelta * MOUSE_SENSITIVITY, 0);

					/* angle between forward and its projection on xz plane */
					float angleYDelta = -yMouseDelta * MOUSE_SENSITIVITY; 
					vec3 forward = camera.to - camera.position;
					vec3 forwardXZ(forward.x, 0, forward.z);

					forwardXZ = vec4::toVec3(mat4::rotationPitchYawRoll(anglesXZ) * vec4::homogeneous(forwardXZ));
					
					vec3 forwardY(0, 0, 0);
					if (camera.angleY == 0)
					{
						forwardY.y = vec3::length(forward) * sin(camera.angleY + angleYDelta);
						camera.angleY = camera.angleY + angleYDelta;
					}
					else if (camera.angleY + angleYDelta > ANGLE_Y_LIMIT)
					{
						forwardY.y = forward.y * sin(ANGLE_Y_LIMIT) / sin(camera.angleY);
						camera.angleY = ANGLE_Y_LIMIT;
					}
					else if (camera.angleY + angleYDelta < -ANGLE_Y_LIMIT)
					{
						forwardY.y = forward.y * sin(-ANGLE_Y_LIMIT) / sin(camera.angleY);
						camera.angleY = -ANGLE_Y_LIMIT;
					}
					else
					{
						forwardY.y = forward.y * sin(camera.angleY + angleYDelta) / sin(camera.angleY);
						camera.angleY = camera.angleY + angleYDelta;
					}

					forward = forwardXZ + forwardY;
					camera.to = camera.position + forward;
				}
			}


			//MY RENDERING
			//std::cout << angle << std::endl;
			for (int i = 0; i < meshes.size(); i++)
			{
				meshes.at(i).rotation.setCoordinate(1, degreesToRadians(angle));
			}

			if (rotating)
				angle = (angle + 1) % 360;

			if (lightFollowingCamera)
				lightSource.position = camera.position;
			else
				lightSource.position = vec3(0, 10, 70);

			display.render(camera, lightSource, meshes,
						   backFaceCulling, renderingType, projectionType, shadingType);

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