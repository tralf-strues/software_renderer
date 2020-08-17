#include "Display.h"
#include <iostream>
#include <minmax.h>

Display::Display(SDL_Window* window, int width, int height) :
	width(width),
	height(height),
	depthBuffer(std::vector<float>(width* height))
{
	surface = SDL_GetWindowSurface(window);

	surfacePixels = (Uint8*)surface->pixels;
	surfacePitch = surface->pitch;
	surfacePixelFormat = surface->format;

	updateProjectionMatrix();
}

Display::Display() :
	width(100),
	height(100),
	depthBuffer(std::vector<float>(100 * 100))
{
	surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

	surfacePixels = (Uint8*)surface->pixels;
	surfacePitch = surface->pitch;
	surfacePixelFormat = surface->format;

	updateProjectionMatrix();
}

Display::~Display()
{
}

int Display::getWidth()
{
	return width;
}

int Display::getHeight()
{
	return height;
}

SDL_Surface* Display::getSurface()
{
	return surface;
}

void Display::updateProjectionMatrix()
{
	switch (projectionType)
	{
		case PERSPECTIVE_PROJECTION:
		projectionMatrix = mat4::perspectiveFOV(0.78f, width / height, 5, 600);
		break;

		case ORTHOGRAPHIC_PROJECTION:
		float top = 100 * std::tan(0.78f / 2);
		float right = width / height * top;
		projectionMatrix = mat4::orthographic(-right, right, -top, top, 5, 600);
		break;
	}
}

void Display::render(Camera& camera,
					 mat4& viewMatrix,
					 LightSource lightSource,
					 std::vector<Mesh>& meshes,
					 BackFaceCulling backFaceCulling,
					 RenderingType renderingType,
					 ProjectionType projectionType,
					 ShadingType shadingType)
{
	this->backFaceCulling = backFaceCulling;
	this->renderingType = renderingType;
	this->projectionType = projectionType;
	this->shadingType = shadingType;

	clear(vec4(SKY_COLOR));

	//mat4 viewMatrix = mat4::lootAt(camera.position, camera.to);
	//mat4 viewMatrix = mat4::lootAtDirectionVector(camera.position, camera.forward);

	mat4 rotationMatrix;
	mat4 translationMatrix;
	mat4 worldMatrix; // translation * rotation
	mat4 transformMatrix;
	Vertex v0, v1, v2;
	vec3 faceNormal;
	vec3 v0Normal, v1Normal, v2Normal;
	int faceIndex;
	for (Mesh mesh : meshes)
	{
		rotationMatrix = mat4::rotationPitchYawRoll(mesh.rotation);
		translationMatrix = mat4::translation(mesh.position);
		worldMatrix = translationMatrix * rotationMatrix;
		transformMatrix = projectionMatrix * (viewMatrix * worldMatrix);

		faceIndex = 0;
		for (Face face : mesh.faces)
		{
			v0.coordinates = mesh.vertices[face.vertexIndices.x];
			v1.coordinates = mesh.vertices[face.vertexIndices.y];
			v2.coordinates = mesh.vertices[face.vertexIndices.z];

			vec4 worldCoordinates;
			worldCoordinates = worldMatrix * vec4::homogeneous(v0.coordinates);
			// (worldCoordinates.w is 1 always, no need to divide each coordinate by it)
			v0.worldCoordinates = vec4::toVec3(worldCoordinates);
			worldCoordinates = worldMatrix * vec4::homogeneous(v1.coordinates);
			v1.worldCoordinates = vec4::toVec3(worldCoordinates);
			worldCoordinates = worldMatrix * vec4::homogeneous(v2.coordinates);
			v2.worldCoordinates = vec4::toVec3(worldCoordinates);

			faceNormal = vec3::normalize(
				vec3::cross(v1.worldCoordinates - v0.worldCoordinates,
							v2.worldCoordinates - v0.worldCoordinates));

			if (backFaceCulling == BACK_FACE_CULLING_ENABLED_WCS &&
				vec3::dot(vec3::normalize(camera.position - v0.worldCoordinates), faceNormal) < 0)
				continue;

			if (project(&v0, transformMatrix) == -1 ||
				project(&v1, transformMatrix) == -1 ||
				project(&v2, transformMatrix) == -1)
				continue;

			v0.normal = vec4::toVec3(rotationMatrix * vec4::homogeneous(mesh.normals[face.normalIndices.x]));
			v1.normal = vec4::toVec3(rotationMatrix * vec4::homogeneous(mesh.normals[face.normalIndices.y]));
			v2.normal = vec4::toVec3(rotationMatrix * vec4::homogeneous(mesh.normals[face.normalIndices.z]));

			/*if (backFaceCulling == BACK_FACE_CULLING_ENABLED_NDCS)
				if (signedArea2dTriangle(v0.screenCoordinates,
										 v1.screenCoordinates,
										 v2.screenCoordinates) < 0)
					continue;*/

			switch (renderingType)
			{
				case RASTERIZATION:
				{
					/*float color = 60 + 170 * (faceIndex % mesh.faces.size()) / mesh.faces.size();*/
					/*drawTriangle(p0, p1, p2, vec4(color, color, color, 255));*/
					FaceData faceData(mesh, face, faceNormal, v0, v1, v2);
					drawTriangle(faceData, lightSource);
					break;
				}
				case RASTERIZATION_WITH_WIREFRAME:
				{
					FaceData faceData(mesh, face, faceNormal, v0, v1, v2);
					drawTriangle(faceData, lightSource);

					drawLine(v0.screenCoordinates, v1.screenCoordinates, vec4(200, 200, 200, 255));
					drawLine(v0.screenCoordinates, v2.screenCoordinates, vec4(200, 200, 200, 255));
					drawLine(v1.screenCoordinates, v2.screenCoordinates, vec4(200, 200, 200, 255));
					break;
				}
				case WIREFRAME_RENDERING:
				{
					drawLine(v0.screenCoordinates, v1.screenCoordinates, vec4(255, 255, 0, 255));
					drawLine(v0.screenCoordinates, v2.screenCoordinates, vec4(255, 255, 0, 255));
					drawLine(v1.screenCoordinates, v2.screenCoordinates, vec4(255, 255, 0, 255));
					break;
				}
			}

			faceIndex++;
		}
	}
}

void Display::clear(vec4 color)
{
	Uint32 rgbaColor = SDL_MapRGBA(surface->format, color.x, color.y, color.z, color.w);
	SDL_FillRect(surface, 0, rgbaColor);

	// todo: set to 1.1 because z is in range [-1; 1]
	std::fill(depthBuffer.begin(), depthBuffer.end(), 100);
}

void Display::drawBitmap(int x, int y, Texture& texture)
{
	for (int v = 0; v < texture.height; v++)
		for (int u = 0; u < texture.width; u++)
		{
			vec4 color = texture.getPixel(vec2(u, v));
			drawPoint(vec2(x + u, y + v), color);
		}
}

int Display::project(Vertex* vertex, mat4& transformationMatrix)
{
	vec4 clip = transformationMatrix * vec4::homogeneous(vertex->coordinates);

	if (clip.x < -clip.w || clip.x > clip.w ||
		clip.y < -clip.w || clip.y > clip.w ||
		clip.z < -clip.w || clip.z > clip.w)
		return -1;

	vertex->projectedCoordinates = vec3(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);
	vertex->screenCoordinates =
		vec2((int)(vertex->projectedCoordinates.x * (width / 2) + (width / 2)),
		(int)(-vertex->projectedCoordinates.y * (height / 2) + (height / 2)));
	vertex->projectedCoordinates.x = vertex->screenCoordinates.x;
	vertex->projectedCoordinates.y = vertex->screenCoordinates.y;

	return 0;
}

void Display::drawPoint(vec2 coordinates, vec4 color)
{
	if (coordinates.x < 0 ||
		coordinates.y < 0 ||
		coordinates.x >= width ||
		coordinates.y >= height)
	{
		return;
	}

	/* Lock the screen for direct access to the pixels */
	if (SDL_MUSTLOCK(surface)) {
		if (SDL_LockSurface(surface) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}

	Uint32* target_pixel;
	target_pixel = (Uint32*)(surfacePixels +
		(int)coordinates.y * surfacePitch +
							 (int)coordinates.x * sizeof(*target_pixel));
	*target_pixel = SDL_MapRGB(surfacePixelFormat, (Uint8)color.x, (Uint8)color.y, (Uint8)color.z);

	if (SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
}

void Display::drawLine(vec2 p0, vec2 p1, vec4 color)
{
	int x0 = (int)p0.x;
	int y0 = (int)p0.y;
	int x1 = (int)p1.x;
	int y1 = (int)p1.y;

	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		drawPoint(vec2(x0, y0), color);

		if ((x0 == x1) && (y0 == y1))
			break;

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}

		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void Display::drawTriangle(FaceData& faceData, LightSource& lightSource)
{
	vec3 temp;
	vec3& p0 = faceData.v0.projectedCoordinates;
	vec3& p1 = faceData.v1.projectedCoordinates;
	vec3& p2 = faceData.v2.projectedCoordinates;
	if (p0.y > p1.y)
	{
		temp = p0;
		p0 = p1;
		p1 = temp;
	}

	if (p1.y > p2.y)
	{
		temp = p1;
		p1 = p2;
		p2 = temp;
	}

	if (p0.y > p1.y)
	{
		temp = p0;
		p0 = p1;
		p1 = temp;
	}

	// prepare for interpolation
	//////
	float normalDotToLight0 = min(1, max(0, vec3::dot(faceData.v0.normal,
													  vec3::normalize(lightSource.position - faceData.v0.worldCoordinates))));
	vec2 uv0 = faceData.mesh.textures[faceData.face.textureIndices.x];
	float normalDotToLight1 = min(1, max(0, vec3::dot(faceData.v1.normal,
													  vec3::normalize(lightSource.position - faceData.v1.worldCoordinates))));
	vec2 uv1 = faceData.mesh.textures[faceData.face.textureIndices.y];
	float normalDotToLight2 = min(1, max(0, vec3::dot(faceData.v2.normal,
													  vec3::normalize(lightSource.position - faceData.v2.worldCoordinates))));
	vec2 uv2 = faceData.mesh.textures[faceData.face.textureIndices.z];

	/*if ((p2.x - p1.x) / (p2.y - p1.y) > (p3.x - p1.x) / (p3.y - p1.y))*/
	if ((p0.y == p1.y && p1.x > p0.x) ||
		(p1.y == p2.y && p1.x > p2.x) ||
		(p1.x - p0.x) / (p1.y - p0.y) > (p2.x - p0.x) / (p2.y - p0.y))
	{
		// clockwise triangle
		for (int y = p0.y; y <= p2.y; y++)
		{
			if (y < p1.y)
				// step 1
				processScanLine(y, faceData.v0, faceData.v2, faceData.v0, faceData.v1, faceData,
								InterpolationData(normalDotToLight0, normalDotToLight2,
												  normalDotToLight0, normalDotToLight1,
												  uv0, uv2, uv0, uv1),
								lightSource);
			else
				// step 2		   
				processScanLine(y, faceData.v0, faceData.v2, faceData.v1, faceData.v2, faceData,
								InterpolationData(normalDotToLight0, normalDotToLight2,
												  normalDotToLight1, normalDotToLight2,
												  uv0, uv2, uv1, uv2),
								lightSource);
		}
	}
	else
	{
		// counter-clockwise triangle
		for (int y = p0.y; y <= p2.y; y++)
		{
			if (y < p1.y)
				// step 1
				processScanLine(y, faceData.v0, faceData.v1, faceData.v0, faceData.v2, faceData,
								InterpolationData(normalDotToLight0, normalDotToLight1,
												  normalDotToLight0, normalDotToLight2,
												  uv0, uv1, uv0, uv2),
								lightSource);

			else
				// step 2
				processScanLine(y, faceData.v1, faceData.v2, faceData.v0, faceData.v2, faceData,
								InterpolationData(normalDotToLight1, normalDotToLight2,
												  normalDotToLight0, normalDotToLight2,
												  uv1, uv2, uv0, uv2),
								lightSource);
		}
	}
}

void Display::processScanLine(int& y, Vertex& l1, Vertex& l2, Vertex& r1, Vertex& r2,
							  FaceData& faceData, InterpolationData interpolationData,
							  LightSource& lightSource)
{
	switch (shadingType)
	{
		case FLAT_SHADING:
		processScanLineFlatShading(y, l1, l2, r1, r2, faceData, interpolationData, lightSource);
		break;

		case GOURAUD_SHADING:
		processScanLineGouraudShading(y, l1, l2, r1, r2, faceData, interpolationData, lightSource);
		break;
	}
}

void Display::processScanLineFlatShading(int& y, Vertex& l1, Vertex& l2, Vertex& r1, Vertex& r2,
										 FaceData& faceData, InterpolationData interpolationData,
										 LightSource& lightSource)
{
	vec3 faceCenter = triangleCenter(faceData.v0.worldCoordinates,
									 faceData.v1.worldCoordinates,
									 faceData.v2.worldCoordinates);

	vec2 n01 = inwardEdgeNormal(faceData.v0.screenCoordinates, faceData.v1.screenCoordinates);
	vec2 n02 = inwardEdgeNormal(faceData.v0.screenCoordinates, faceData.v2.screenCoordinates);
	vec2 n12 = inwardEdgeNormal(faceData.v1.screenCoordinates, faceData.v2.screenCoordinates);

	float normalDotToLight0 = min(1, max(0, vec3::dot(faceData.v0.normal,
													  vec3::normalize(lightSource.position - faceData.v0.worldCoordinates))));
	vec4 color0(150 * normalDotToLight0,
				150 * normalDotToLight0,
				150 * normalDotToLight0,
				255);

	float normalDotToLight1 = min(1, max(0, vec3::dot(faceData.v1.normal,
													  vec3::normalize(lightSource.position - faceData.v1.worldCoordinates))));
	vec4 color1(150 * normalDotToLight1,
				150 * normalDotToLight1,
				150 * normalDotToLight1,
				255);

	float normalDotToLight2 = min(1, max(0, vec3::dot(faceData.v2.normal,
													  vec3::normalize(lightSource.position - faceData.v2.worldCoordinates))));
	vec4 color2(150 * normalDotToLight2,
				150 * normalDotToLight2,
				150 * normalDotToLight2,
				255);

	//float gradient1 = l1.y == l2.y ? 1 : getGradient2(y, l1.y, l2.y);
	//float gradient2 = r1.y == r2.y ? 1 : getGradient2(y, r1.y, r2.y);

	//// intersection of y = y and l1l2 or l2.x when l1.y = l2.y
	//int startX = linearInterpolation2(l1.x, l2.x, gradient1);
	//// intersection of y = y and r1r2 or r2.x when r1.y = r2.y
	//int endX = linearInterpolation2(r1.x, r2.x, gradient2);

	//// see my perspective-correct interpolation notes
	//float startZ = 1.0 / linearInterpolation2(1.0 / l1.z, 1.0 / l2.z, gradient1);
	//float endZ = 1.0 / linearInterpolation2(r1.z, r2.z, gradient2);

	//float gradientZ, z;
	//int pixelIndex;
	//vec2 point;
	//float a, b, g; // alpha, beta and gamma barycentric coordinates
	//for (int x = startX; x <= endX; x++)
	//{
	//	point = vec2(x, y);
	//	a = vec2::dot(point - faceData.v1.screenCoordinates, n12) /
	//		vec2::dot(faceData.v0.screenCoordinates - faceData.v1.screenCoordinates, n12);
	//	b = vec2::dot(point - faceData.v0.screenCoordinates, n02) /
	//		vec2::dot(faceData.v1.screenCoordinates - faceData.v0.screenCoordinates, n02);
	//	g = vec2::dot(point - faceData.v0.screenCoordinates, n01) /
	//		vec2::dot(faceData.v2.screenCoordinates - faceData.v0.screenCoordinates, n01);

	//	// TODO: try to find another solution
	//	if (a < 0 || a > 1 || b < 0 || b > 1 || g < 0 || g > 1)
	//		continue;

	//	gradientZ = getGradient2(x, startX, endX);
	//	z = 1.0 / linearInterpolation2(1.0 / startZ, 1.0 / endZ, gradientZ);

	//	pixelIndex = y * width + x;

	//	if (z < depthBuffer[pixelIndex])
	//	{
	//		depthBuffer[pixelIndex] = z;

	//		float faceNormalDotToLight = max(0, vec3::dot(faceData.faceNormal, vec3::normalize(lightSource.position - faceCenter)));

	//		drawPoint(vec2(x, y), vec4(150 * faceNormalDotToLight,
	//								   150 * faceNormalDotToLight,
	//								   150 * faceNormalDotToLight,
	//								   255));
	//	}
	//}
}

void Display::getStartAndEndValuesForInterpolation(vec3& l1, vec3& l2, vec3& r1, vec3& r2, FaceData& faceData,
												   float& gradient1,
												   float& gradient2,
												   float& normalDotToLight0,
												   float& normalDotToLight1,
												   float& normalDotToLight2,
												   float& startNormalDotToLight,
												   float& endNormalDotToLight,
												   vec2& uv0,
												   vec2& uv1,
												   vec2& uv2,
												   vec2& startUV,
												   vec2& endUV)
{
	if (l1 == faceData.v0.coordinates && l2 == faceData.v1.coordinates)
	{
		startNormalDotToLight = 1.0 / linearInterpolation2(1.0 / normalDotToLight0, 1.0 / normalDotToLight1, gradient1);
		startUV.x = 1.0 / linearInterpolation2(1.0 / uv0.x, 1.0 / uv1.x, gradient1); // u
		startUV.y = 1.0 / linearInterpolation2(1.0 / uv0.y, 1.0 / uv1.y, gradient1); // v
	}
	else if (l1 == faceData.v0.coordinates && l2 == faceData.v2.coordinates)
	{
		startNormalDotToLight = 1.0 / linearInterpolation2(1.0 / normalDotToLight0, 1.0 / normalDotToLight2, gradient1);
		startUV.x = 1.0 / linearInterpolation2(1.0 / uv0.x, 1.0 / uv2.x, gradient1); // u
		startUV.y = 1.0 / linearInterpolation2(1.0 / uv0.y, 1.0 / uv2.y, gradient1); // v
	}
	else if (l1 == faceData.v1.coordinates && l2 == faceData.v2.coordinates)
	{
		startNormalDotToLight = 1.0 / linearInterpolation2(1.0 / normalDotToLight1, 1.0 / normalDotToLight2, gradient1);
		startUV.x = 1.0 / linearInterpolation2(1.0 / uv1.x, 1.0 / uv2.x, gradient1); // u
		startUV.y = 1.0 / linearInterpolation2(1.0 / uv1.y, 1.0 / uv2.y, gradient1); // v
	}
	else;
	//std::cout << "ERROR!\";

	if (r1 == faceData.v0.coordinates && r2 == faceData.v1.coordinates)
	{
		endNormalDotToLight = 1.0 / linearInterpolation2(1.0 / normalDotToLight0, 1.0 / normalDotToLight1, gradient1);
		endUV.x = 1.0 / linearInterpolation2(1.0 / uv0.x, 1.0 / uv1.x, gradient1); // u
		endUV.y = 1.0 / linearInterpolation2(1.0 / uv0.y, 1.0 / uv1.y, gradient1); // v
	}
	else if (r1 == faceData.v0.coordinates && r2 == faceData.v2.coordinates)
	{
		endNormalDotToLight = 1.0 / linearInterpolation2(1.0 / normalDotToLight0, 1.0 / normalDotToLight2, gradient1);
		endUV.x = 1.0 / linearInterpolation2(1.0 / uv0.x, 1.0 / uv2.x, gradient1); // u
		endUV.y = 1.0 / linearInterpolation2(1.0 / uv0.y, 1.0 / uv2.y, gradient1); // v
	}
	else if (r1 == faceData.v1.coordinates && r2 == faceData.v2.coordinates)
	{
		endNormalDotToLight = 1.0 / linearInterpolation2(1.0 / normalDotToLight1, 1.0 / normalDotToLight2, gradient1);
		endUV.x = 1.0 / linearInterpolation2(1.0 / uv1.x, 1.0 / uv2.x, gradient1); // u
		endUV.y = 1.0 / linearInterpolation2(1.0 / uv1.y, 1.0 / uv2.y, gradient1); // v
	}
	else;
	//std::cout << "ERROR!\n";
}

void Display::processScanLineGouraudShading(int& y, Vertex& l1, Vertex& l2, Vertex& r1, Vertex& r2,
											FaceData& faceData, InterpolationData interpolationData,
											LightSource& lightSource)
{
	/*vec3 faceCenter = triangleCenter(faceData.v0.worldCoordinates,
									 faceData.v1.worldCoordinates,
									 faceData.v2.worldCoordinates);*/

									 /*vec2 n01 = inwardEdgeNormal(faceData.v0.screenCoordinates, faceData.v1.screenCoordinates);
									 vec2 n02 = inwardEdgeNormal(faceData.v0.screenCoordinates, faceData.v2.screenCoordinates);
									 vec2 n12 = inwardEdgeNormal(faceData.v1.screenCoordinates, faceData.v2.screenCoordinates);*/

	Texture* texture = faceData.mesh.texture;
	/*temp = texture->getPixel(faceData.mesh.textures[faceData.face.textureIndices.x] * 256);
	vec4 color0(temp.x * normalDotToLight0,
				temp.y * normalDotToLight0,
				temp.z * normalDotToLight0,
				temp.w);*/

				/*temp = texture->getPixel(faceData.mesh.textures[faceData.face.textureIndices.y] * 256);
				vec4 color1(temp.x * normalDotToLight1,
							temp.y * normalDotToLight1,
							temp.z * normalDotToLight1,
							temp.w);*/

							/*temp = texture->getPixel(faceData.mesh.textures[faceData.face.textureIndices.z] * 256);
							vec4 color2(temp.x * normalDotToLight2,
										temp.y * normalDotToLight2,
										temp.z * normalDotToLight2,
										temp.w);*/

	float gradient1 = l1.projectedCoordinates.y == l2.projectedCoordinates.y ? 1 :
		getGradient2(y, l1.projectedCoordinates.y, l2.projectedCoordinates.y);
	float gradient2 = r1.projectedCoordinates.y == r2.projectedCoordinates.y ? 1 :
		getGradient2(y, r1.projectedCoordinates.y, r2.projectedCoordinates.y);

	// intersection of y = y and l1l2 or l2.x when l1.y = l2.y
	int startX = linearInterpolation2(l1.projectedCoordinates.x, l2.projectedCoordinates.x, gradient1);
	// intersection of y = y and r1r2 or r2.x when r1.y = r2.y
	int endX = linearInterpolation2(r1.projectedCoordinates.x, r2.projectedCoordinates.x, gradient2);

	// see my perspective-correct interpolation notes
	float startZ = 1.0 / linearInterpolation2(1.0 / l1.worldCoordinates.z, 1.0 / l2.projectedCoordinates.z, gradient1);
	float endZ = 1.0 / linearInterpolation2(1.0 / r1.projectedCoordinates.z, 1.0 / r2.projectedCoordinates.z, gradient2);

	float startNormalDotToLight = 1.0 / linearInterpolation2(1.0 / interpolationData.l1NormalDotToLight,
															 1.0 / interpolationData.l2NormalDotToLight,
															 gradient1);
	float endNormalDotToLight = 1.0 / linearInterpolation2(1.0 / interpolationData.r1NormalDotToLight,
														   1.0 / interpolationData.r2NormalDotToLight,
														   gradient2);

	vec2 startUV, endUV;
	startUV.x = startZ * linearInterpolation2(interpolationData.l1UV.x / l1.projectedCoordinates.z, 1.0 / interpolationData.l2UV.x, gradient1); // u
	startUV.y = 1.0 / linearInterpolation2(1.0 / interpolationData.l1UV.y, 1.0 / interpolationData.l2UV.y, gradient1); // v
	endUV.x = 1.0 / linearInterpolation2(1.0 / interpolationData.r1UV.x, 1.0 / interpolationData.r2UV.x, gradient2); // u
	endUV.y = 1.0 / linearInterpolation2(1.0 / interpolationData.r1UV.y, 1.0 / interpolationData.r2UV.y, gradient2); // v

	/*getStartAndEndValuesForInterpolation(l1, l2, r1, r2, faceData,
										 gradient1,
										 gradient2,
										 normalDotToLight0,
										 normalDotToLight1,
										 normalDotToLight2,
										 startNormalDotToLight,
										 endNormalDotToLight,
										 uv0,
										 uv1,
										 uv2,
										 startUV,
										 endUV);*/

	float gradient, z;
	float normalDotToLight;
	int pixelIndex;
	/*vec2 point;*/
	float u, v;
	vec4 color(150, 150, 150, 255);
	vec4 colorTexture;
	//float a, b, g; // alpha, beta and gamma barycentric coordinates in 2D!!!
	for (int x = startX; x <= endX; x++)
	{
		/*point = vec2(x, y);*/
		//a = vec2::dot(point - faceData.v1.screenCoordinates, n12) /
		//	vec2::dot(faceData.v0.screenCoordinates - faceData.v1.screenCoordinates, n12);
		//b = vec2::dot(point - faceData.v0.screenCoordinates, n02) /
		//	vec2::dot(faceData.v1.screenCoordinates - faceData.v0.screenCoordinates, n02);
		//g = vec2::dot(point - faceData.v0.screenCoordinates, n01) /
		//	vec2::dot(faceData.v2.screenCoordinates - faceData.v0.screenCoordinates, n01);

		//// TODO: try to find another solution
		//if (a < 0 || a > 1 || b < 0 || b > 1 || g < 0 || g > 1)
		//	continue;

		gradient = startX == endX ? 0 : getGradient2(x, startX, endX);
		z = 1.0 / linearInterpolation2(1.0 / startZ, 1.0 / endZ, gradient);

		pixelIndex = y * width + x;

		if (z < depthBuffer[pixelIndex])
		{
			depthBuffer[pixelIndex] = z;

			/*float normalDotToLight = a * normalDotToLight0 +
				b * normalDotToLight1 +
				g * normalDotToLight2;

			float u = (a * uv0.x + b * uv1.x + g * uv2.x) * (float)faceData.mesh.texture->width;
			float v = (a * uv0.y + b * uv1.y + g * uv2.y) * (float)faceData.mesh.texture->height;*/

			normalDotToLight = 1.0 / linearInterpolation2(1.0 / startNormalDotToLight,
														  1.0 / endNormalDotToLight,
														  gradient);
			if (texture != NULL)
			{
				u = z * linearInterpolation2(startUV.x / startZ,
											 endUV.x / endZ,
											 gradient) * texture->width;
				v = z * linearInterpolation2(startUV.y / startZ,
											 endUV.y / endZ,
											 gradient) * texture->height;

				colorTexture = texture->getPixel(vec2(u, v));
				/*printf("%3.2f, %3.2f\n", u, v);*/
			}
			else
			{
				colorTexture = vec4(1, 1, 1, 1);
			}

			//drawPoint(vec2(x, y), color * normalDotToLight);
			//drawPoint(vec2(x, y), color * normalDotToLight * colorTexture);
			drawPoint(vec2(x, y), colorTexture);
		}
	}

	/*else
	{*/
	//float normalDotToLight0 = min(1, max(0, vec3::dot(faceData.v0.normal,
	//												  vec3::normalize(lightSource.position - faceData.v0.worldCoordinates))));
	//vec4 color0(150 * normalDotToLight0,
	//			150 * normalDotToLight0,
	//			150 * normalDotToLight0,
	//			255);

	//float normalDotToLight1 = min(1, max(0, vec3::dot(faceData.v1.normal,
	//												  vec3::normalize(lightSource.position - faceData.v1.worldCoordinates))));
	//vec4 color1(150 * normalDotToLight1,
	//			150 * normalDotToLight1,
	//			150 * normalDotToLight1,
	//			255);

	//float normalDotToLight2 = min(1, max(0, vec3::dot(faceData.v2.normal,
	//												  vec3::normalize(lightSource.position - faceData.v2.worldCoordinates))));
	//vec4 color2(150 * normalDotToLight2,
	//			150 * normalDotToLight2,
	//			150 * normalDotToLight2,
	//			255);

	//float gradient1 = l1.y == l2.y ? 1 : getGradient2(y, l1.y, l2.y);
	//float gradient2 = r1.y == r2.y ? 1 : getGradient2(y, r1.y, r2.y);

	//// intersection of y = y and l1l2 or l2.x when l1.y = l2.y
	//int startX = linearInterpolation2(l1.x, l2.x, gradient1);
	//// intersection of y = y and r1r2 or r2.x when r1.y = r2.y
	//int endX = linearInterpolation2(r1.x, r2.x, gradient2);

	//float startZ = linearInterpolation2(l1.z, l2.z, gradient1);
	//float endZ = linearInterpolation2(r1.z, r2.z, gradient2);

	//float gradientZ, z;
	//int pixelIndex;
	//vec2 point;
	//float a, b, g; // alpha, beta and gamma barycentric coordinates
	//for (int x = startX; x <= endX; x++)
	//{
	//	point = vec2(x, y);
	//	a = vec2::dot(point - faceData.v1.screenCoordinates, n12) /
	//		vec2::dot(faceData.v0.screenCoordinates - faceData.v1.screenCoordinates, n12);
	//	b = vec2::dot(point - faceData.v0.screenCoordinates, n02) /
	//		vec2::dot(faceData.v1.screenCoordinates - faceData.v0.screenCoordinates, n02);
	//	g = vec2::dot(point - faceData.v0.screenCoordinates, n01) /
	//		vec2::dot(faceData.v2.screenCoordinates - faceData.v0.screenCoordinates, n01);

	//	// TODO: try to find another solution
	//	if (a < 0 || a > 1 || b < 0 || b > 1 || g < 0 || g > 1)
	//		continue;

	//	gradientZ = getGradient2(x, startX, endX);
	//	z = linearInterpolation2(startZ, endZ, gradientZ);

	//	pixelIndex = y * width + x;

	//	if (z < depthBuffer[pixelIndex])
	//	{
	//		depthBuffer[pixelIndex] = z;

	//		drawPoint(vec2(x, y), color0 * a + color1 * b + color2 * g);
	//	}
	//}
/*}*/
}