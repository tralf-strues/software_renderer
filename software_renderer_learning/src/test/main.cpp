//Using SDL, standard IO, and strings
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "../engine/Display.h"
#include "../engine/math/engineMath.h"
#include "../engine/tools/OBJLoader.h"

#define FPS_LIMIT 75
#define WIDTH 1280 // 640, 1280
#define HEIGHT 720 // 480, 720

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

		// FPS counter
		Uint32 frameStart;
		int frameTime;

		// Camera:
		Camera camera(vec3(0, 5, 35), vec3(0, 0, 0));
		int angle = 0;
		while (!quit)
		{
			frameStart = SDL_GetTicks();

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
					quit = true;
			}


			//MY RENDERING
			//std::cout << angle << std::endl;
			for (int i = 0; i < meshes.size(); i++)
			{
				meshes.at(i).rotation.setCoordinate(1, degreesToRadians(angle));
			}
			angle = (angle + 1) % 360;

			display.render(camera, meshes, true, false);

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

			//std::cout << "FPS: " << 1000 / frameTime << std::endl;
		}
	}
	close();

	return 0;
}