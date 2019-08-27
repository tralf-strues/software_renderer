#pragma once

#include<string>
#include<vector>
#include "../meshes/Mesh.h"

std::vector<std::string> split(std::string s, std::string separator);
Mesh loadOBJ(std::string meshName, std::string fileName);