#include "GlobalVariables.h"

int C_GlobalVariables::ScreenSizeX = 1920;
int C_GlobalVariables::ScreenSizeY = 1080;

float C_GlobalVariables::PPM = 30.0f;

float C_GlobalVariables::DeltaTime = 0.15f; // updated per cycle from gameloop.or use Michaels. but box2d has its own timestep so maybe not needed except for animations.

b2Vec2 C_GlobalVariables::Gravity = b2Vec2(0.0f, 10.0f);

float C_GlobalVariables::minimumSpeedForAnim = 0.01f;
float C_GlobalVariables::minimumSpeedForAnimJump = 0.01f;

float  C_GlobalVariables::timeStep = 1 / 60.0f;

int C_GlobalVariables::maxLives = 3, C_GlobalVariables::maxHealth=100; 

int C_GlobalVariables::Player_1_Score = 0, C_GlobalVariables::Player_2_Score = 0, C_GlobalVariables::Player_3_Score = 0, C_GlobalVariables::Player_4_Score = 0;

uint16 C_GlobalVariables::GetCategoryFor(unsigned int playerID)
{
	return uint16(powf(2, playerID)); //Bit representation of the layer
}

uint16 C_GlobalVariables::GetLayerMaskFor(unsigned int playerID)
{
    return ~uint16(powf(2, playerID)); //('~' = Everything except for) the player's bit
}