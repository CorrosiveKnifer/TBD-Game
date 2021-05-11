#pragma once

// Library Includes
#include <Box2D.h>


class C_GlobalVariables
{
public:
	static int ScreenSizeX, ScreenSizeY;
	static float PPM; // Pixels Per Metre for conversion
	static float DeltaTime;
	static b2Vec2 Gravity;

	static float minimumSpeedForAnim;
	static float minimumSpeedForAnimJump;

	static float timeStep;

	static int maxLives, maxHealth;

private:
	C_GlobalVariables();

};