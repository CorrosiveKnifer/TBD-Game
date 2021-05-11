#pragma once
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: LevelLoader.h
// Description   	: Loads level data from 3d .obj files and translates it into box2d for collisions
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz
//

#ifndef _LEVEL_LOADER_
#define _LEVEL_LOADER_

//System Includes
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <iomanip>

//Library Includes
#include <box2d.h>

//local includes
#include "GlobalVariables.h"

namespace fs = std::filesystem;

using namespace std;

class levelMesh
{
public:
	levelMesh(string path, b2World& world);
	~levelMesh() {};

	vector <float*> vertices;
	vector <int*> Indices;

private:
	//ground
	b2Body* ground = NULL;
	b2BodyDef bd;
	b2EdgeShape shape;

	//const float PPM = 30.0f;
};

#endif // !_LEVEL_LOADER_