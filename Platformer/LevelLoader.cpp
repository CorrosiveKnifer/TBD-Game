// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: LevelLoader.cpp
// Description   	: Loads level data from 3d .obj files and translates it into box2d for collisions
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz
//
// local include
#include "LevelLoader.h"

// constructor
// levelMesh(string path, b2World& world)
//
// Description:	Loads level information from files for level collision 
//				
// @param	string  	The directory path to the files to load
// @param	b2World&  	The world reference to apply the collision creation to.
//
// 
// @return	NA
//
levelMesh::levelMesh(string path, b2World& world)
{
    string line;

    streampos begin, end;
    ifstream myfile(path);

    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            // cout << "read line" << '\n';

            std::size_t found = line.find("v ");

            // first vertex - extract info from .obj 3d file
            if (found != string::npos)
            {
                found = line.find(" ", found + 5);
                string SnewVert = line.substr(2, found - 2);

                vertices.push_back(new float(stof(SnewVert)));
                // cout << SnewVert << ":";

                 // Second vertex

                std::size_t found2 = line.find(" ", found + 5);

                string SnewVert2 = line.substr(found + 1, found2 - (found + 1));

                vertices.push_back(new float(stof(SnewVert2)));
                // cout << SnewVert2 << ":";

                 // third vertex

                std::size_t found3 = line.find(" ", found2 + 5);

                string SnewVert3 = line.substr(found2 + 1, found3 - found2);

                vertices.push_back(new float(stof(SnewVert3)));
                // cout << SnewVert3 << ":" << endl;
            }

            // indices - extract info from .obj 3d file
            std::size_t foundInd = line.find("f ");
            if (foundInd != string::npos)
            {

                std::size_t foundEnd = line.length();
                unsigned int count = 2;
                while (count < foundEnd)
                {
                    std::size_t foundSpace = line.find(" ", count);
                    std::size_t foundSlash = line.find("/", count);

                    string s_Indices = (line.substr(count, foundSlash - count));

                    Indices.push_back(new int(stoi(s_Indices)));

                    count = foundSpace + 1;

                    if (count == 0) // end of the list of indices, setup 2sided edgeShapes
                    {
                        b2Body* ground = NULL;
                        ground = world.CreateBody(&bd);

                        for (unsigned int ik = 0; ik < Indices.size() - 1; ik++)
                        {

                            int a, b;
                            a = ik;
                            b = ik + 1;

                            int face1a, face1b;
                            face1a = *Indices[a];
                            face1b = *Indices[b];


                            b2Vec2 vertex1, vertex2, vertex3;
                            vertex1.x = *vertices[(face1a) * 3 - 3];
                            vertex1.y = *vertices[(face1a) * 3 - 3 + 1];

                            vertex2.x = *vertices[(face1b) * 3 - 3];
                            vertex2.y = *vertices[(face1b) * 3 - 3 + 1];

                            shape.SetTwoSided(b2Vec2(vertex1.x / C_GlobalVariables::PPM, vertex1.y / C_GlobalVariables::PPM), b2Vec2(vertex2.x / C_GlobalVariables::PPM, vertex2.y / C_GlobalVariables::PPM));
                            ground->CreateFixture(&shape, 0.0f);
                        }
                        // finish off the complete shape, last indice connected to first indice.
                        shape.SetTwoSided(b2Vec2(*vertices[*Indices.back() * 3 - 3] / C_GlobalVariables::PPM, *vertices[*Indices.back() * 3 - 3 + 1] / C_GlobalVariables::PPM), b2Vec2(*vertices[*Indices.front() * 3 - 3] / C_GlobalVariables::PPM, *vertices[*Indices.front() * 3 - 3 + 1] / C_GlobalVariables::PPM));
                        ground->CreateFixture(&shape, 0.0f);


                        break;
                    }
                }
            }
        }
    }
    myfile.close();
}

// ~levelMesh()
//
// Description:	Destructor: Destroy the vertices and indices of collision information.
//				
//
// 
// @return	NA
//
levelMesh::~levelMesh()
{
    vector<float*>::iterator vertIter = vertices.begin();
    while (vertIter != vertices.end())
    {
        delete* vertIter;
        vertIter = vertices.erase(vertIter);
    }
    vector<int*>::iterator indIter = Indices.begin();
    while (indIter != Indices.end())
    {
        delete* indIter;
        indIter = Indices.erase(indIter);
    }
}

// levelSpawnPoints(string path, vector<b2Vec2>& myPlayerSpawnPoints, vector<b2Vec2>& myPowerUpSpawnPoints, b2Vec2& _specialPowerUp)
//
// Description:	Loads level information from files for level collision 
//				
// @param	string  	        The directory path to the files to load
// @param	vector<b2Vec2>&  	Load the player spawn points
// @param	vector<b2Vec2>&  	Load the powerup spawn points
// @param	vector<b2Vec2>&  	Load the waterfall powerup spawn point
//
// 
// @return	NA
//
void levelMesh::levelSpawnPoints(string path, vector<b2Vec2>& myPlayerSpawnPoints, vector<b2Vec2>& myPowerUpSpawnPoints, b2Vec2& _specialPowerUp)
{
    string line;

    streampos begin, end;
    ifstream myfile(path);

    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            std::size_t foundSpawn = line.find("spawn");

            if (foundSpawn != string::npos)
            {
                foundSpawn = line.find(" ");
                std::size_t foundComma = line.find(",");
                string posX = line.substr(foundSpawn+1, foundComma - (foundSpawn+1));
                foundSpawn = line.find(" ", foundComma);
                string posY = line.substr(foundComma + 1, foundSpawn - foundComma);

                myPlayerSpawnPoints.push_back(b2Vec2(stoi(posX), stoi(posY)));
            }

            std::size_t foundPowerUp = line.find("PU");

            if (foundPowerUp != string::npos)
            {
                foundPowerUp = line.find(" ");
                std::size_t foundComma = line.find(",");
                string posX = line.substr(foundPowerUp + 1, foundComma - (foundPowerUp + 1));
                foundPowerUp = line.find(" ", foundComma);
                string posY = line.substr(foundComma + 1, foundPowerUp - foundComma);

                myPowerUpSpawnPoints.push_back(b2Vec2(stoi(posX), stoi(posY)));
            }

            std::size_t foundSpecial = line.find("SPECIAL");

            if (foundSpecial != string::npos)
            {
                foundSpecial = line.find(" ");
                std::size_t foundComma = line.find(",");
                string posX = line.substr(foundSpecial + 1, foundComma - (foundSpecial + 1));
                foundSpecial = line.find(" ", foundComma);
                string posY = line.substr(foundComma + 1, foundSpecial - foundComma);

                _specialPowerUp = b2Vec2(stoi(posX), stoi(posY));
            }
        }
    }
}
