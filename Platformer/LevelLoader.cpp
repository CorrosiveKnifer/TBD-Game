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
                int count = 2;
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

                            int a, b, c;
                            a = ik;
                            b = ik + 1;

                            int face1a, face1b, face1c;
                            face1a = *Indices[a];
                            face1b = *Indices[b];


                            b2Vec2 vertex1, vertex2, vertex3;
                            vertex1.x = *vertices[(face1a) * 3 - 3];
                            vertex1.y = *vertices[(face1a) * 3 - 3 + 1];

                            vertex2.x = *vertices[(face1b) * 3 - 3];
                            vertex2.y = *vertices[(face1b) * 3 - 3 + 1];

                            shape.SetTwoSided(b2Vec2(vertex1.x / PPM, vertex1.y / PPM), b2Vec2(vertex2.x / PPM, vertex2.y / PPM));
                            ground->CreateFixture(&shape, 0.0f);
                        }
                        // finish off the complete shape, last indice connected to first indice.
                        shape.SetTwoSided(b2Vec2(*vertices[*Indices.back() * 3 - 3] / PPM, *vertices[*Indices.back() * 3 - 3 + 1] / PPM), b2Vec2(*vertices[*Indices.front() * 3 - 3] / PPM, *vertices[*Indices.front() * 3 - 3 + 1] / PPM));
                        ground->CreateFixture(&shape, 0.0f);


                        break;
                    }
                }
            }
        }
        myfile.close();
    }
}