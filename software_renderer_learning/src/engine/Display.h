#pragma once
#include <SDL.h>
#include <vector>

#include "meshes/Mesh.h"
#include "meshes/Vertex.h"
#include "entities/Camera.h"
#include "entities/LightSource.h"

#define SKY_COLOR 25, 100, 150, 255
//#define SKY_COLOR 100, 220, 255, 255

enum ShadingType
{
	FLAT_SHADING,
	GOURAUD_SHADING
};

enum ProjectionType
{
	PERSPECTIVE_PROJECTION,
	ORTHOGRAPHIC_PROJECTION
};

enum RenderingType
{
	RASTERIZATION,
	RASTERIZATION_WITH_WIREFRAME,
	WIREFRAME_RENDERING,
};

enum BackFaceCulling
{
	BACK_FACE_CULLING_ENABLED_WCS, // back-face culling in world coordinate system
	/*BACK_FACE_CULLING_ENABLED_NDCS,*/ // back-face culling in normalized device coordinate system
	BACK_FACE_CULLING_DISABLED
};

struct FaceData
{
	FaceData(Mesh& mesh, Face& face, vec3& faceNormal, Vertex& v0, Vertex& v1, Vertex& v2) :
		mesh(mesh),
		face(face),
		faceNormal(faceNormal),
		v0(v0),
		v1(v1),
		v2(v2)
	{
	}

	Mesh& mesh;
	Face& face;
	vec3& faceNormal;
	Vertex& v0;
	Vertex& v1;
	Vertex& v2;
};

class Display
{
private:
	SDL_Surface* surface;
	int width;
	int height;
	std::vector<float> depthBuffer;
	mat4 projectionMatrix;

	BackFaceCulling backFaceCulling = BACK_FACE_CULLING_ENABLED_WCS;
	RenderingType renderingType = RASTERIZATION;
	ProjectionType projectionType = PERSPECTIVE_PROJECTION;
	ShadingType shadingType = GOURAUD_SHADING;

	Uint8* surfacePixels;
	int surfacePitch;
	SDL_PixelFormat* surfacePixelFormat;

public:
	Display(SDL_Window* window, int width, int height);
	Display();
	~Display();

	int getWidth();
	int getHeight();
	SDL_Surface* getSurface();

	void updateProjectionMatrix();

	void render(Camera& camera,
				mat4& viewMatrix,
				LightSource lightSource,
				std::vector<Mesh>& meshes,
				BackFaceCulling backFaceCulling,
				RenderingType renderingType,
				ProjectionType projectionType,
				ShadingType shadingType);
private:
	void clear(vec4 color);
	int project(Vertex* vertex, mat4& transformationMatrix);
	void drawPoint(vec2 coordinates, vec4 color);
	void drawLine(vec2 p0, vec2 p1, vec4 color);
	void drawTriangle(FaceData& faceData, LightSource& lightSource);
	void processScanLine(int y, vec3 l1, vec3 l2, vec3 r1, vec3 r2,
						 FaceData& faceData, LightSource& lightSource);
	void processScanLineFlatShading(int& y, vec3& l1, vec3& l2, vec3& r1, vec3& r2,
									FaceData& faceData, LightSource& lightSource);
	void processScanLineGouraudShading(int& y, vec3& l1, vec3& l2, vec3& r1, vec3& r2,
									   FaceData& faceData, LightSource& lightSource);

};