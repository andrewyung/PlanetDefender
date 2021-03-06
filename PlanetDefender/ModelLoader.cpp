#pragma once

#include "ModelLoader.h"

// OBJ_Loader - .obj Loader
#include "OBJ_Loader.h"

Model ModelLoader::createPrimitive(PrimitiveTypes type)
{
	std::vector<Vertex> vertexData;
	std::vector<int> indexData;

	switch (type)
	{
	case QUAD:
		vertexData = {	Vertex(	1.0f, 1.0f, 0,	//top right position
								1, 1, 1 ,1,		//color
								1, 1,			//uv
								0, 0, 1),		//normal

						Vertex(	1.0f, -1.0f, 0,	//top left position
								1, 1, 1 ,1,		
								0, 1,			
								0, 0, 1),				

						Vertex(	-1.0f, -1.0f, 0,//bottom left position
								1, 1, 1 ,1,		
								0, 0,			
								0, 0, 1),		

						Vertex(	-1.0f, 1.0f, 0,//bottom right position
								1, 1, 1 ,1,		
								1, 0,			
								0, 0, 1),		
		};

		indexData = {	0, 1, 2,	
						0, 2, 3 
		};

		break;
	case TRIANGLE:
		vertexData = {	Vertex(	0, 1.0f, 0,		//top vertex position
								1, 0, 0, 1,		//color
								0, 0,			//uv
								0, 0, 1),		//normal

						Vertex(	-0.5f, -0.5f, 0,	//left vertex position
								0, 1, 0, 1,
								0, 0,
								0, 0, 1),	

						Vertex(	0.5f, -0.5f, 0,	//right vertex position
								0, 0, 1, 1,
								0, 0,
								0, 0, 1)		
		};

		indexData = { 0, 1, 2
		};

		break;
	case CUBE: //optimized cube that shares vertices, normals are opposite to the sum of adjacent edges (as vectors)
		vertexData = {	Vertex(	0.5f, 0.5f, 0.5f,	//front top right
								0, 0, 1, 1,
								0, 0,
								1, 1, 1),
						Vertex(	-0.5f, 0.5f, 0.5f,	//front top left
								0, 0, 1, 1,
								0, 0,
								-1, 1, 1),
						Vertex(	-0.5f, -0.5f, 0.5f,	//front bottom left
								0, 0, 1, 1,
								0, 0,
								-1, -1, 1),
						Vertex(	0.5f, -0.5f, 0.5f,	//front bottom right
								0, 0, 1, 1,
								1, 1,
								1, -1, 1),
						Vertex(	0.5f, 0.5f, -0.5f,	//back top right
								0, 0, 1, 1,
								0, 0,
								1, 1, -1),
						Vertex(	-0.5f, 0.5f, -0.5f,	//back top left
								0, 0, 1, 1,
								1, 0,
								-1, 1, -1),
						Vertex(	-0.5f, -0.5f, -0.5f,//back bottom left
								0, 0, 1, 1,
								0, 0,
								-1, -1, -1),
						Vertex(	0.5f, -0.5f, -0.5f,	//back bottom right
								0, 0, 1, 1,
								0, 0,
								1, -1, -1),
		};

		indexData = {	0, 1, 2,	//front
						0, 2, 3,

						4, 0, 3,	//right
						4, 3, 7,

						1, 5, 6,	//left
						1, 6, 2,

						4, 5, 1,	//top
						4, 1, 0,

						3, 2, 6,	//bottom
						3, 6, 7,

						7, 6, 5,
						7, 5, 4
		};

		break;

	case SKYBOX_CUBE:
		vertexData = { Vertex(1, 1, 1,		//front top right
							0, 0, 1, 1,
							0.333f, 0.5f,
							1, 1, 1),
						Vertex(-1, 1, 1,	//front top left
							0, 0, 1, 1,
							0, 0.5f,
							-1, 1, 1),
						Vertex(-1, -1, 1,	//front bottom left
							0, 0, 1, 1,
							0, 0,
							-1, -1, 1),

						Vertex(1, -1, 1,	//front bottom right
							0, 0, 1, 1,
							0.333f, 0.0f,
							1, -1, 1),
						Vertex(1, 1, 1,		//front top right
							0, 0, 1, 1,
							0.333f, 0.5f,
							1, 1, 1),
						Vertex(-1, -1, 1,	//front bottom left
							0, 0, 1, 1,
							0, 0,
							-1, -1, 1),

						Vertex(1, 1, -1,	//back top right
							0, 0, 1, 1,
							0.666f, 0.5f,
							1, 1, -1),
						Vertex(1, 1, 1,		//front top right
							0, 0, 1, 1,
							0.333f, 0.5f,
							1, 1, 1),
						Vertex(1, -1, 1,	//front bottom right
							0, 0, 1, 1,
							0.333f, 0,
							1, -1, 1),

						Vertex(1, -1, -1,	//back bottom right
							0, 0, 1, 1,
							0.666f, 0,
							1, -1, -1),
						Vertex(1, 1, -1,	//back top right
							0, 0, 1, 1,
							0.666f, 0.5f,
							1, 1, -1),
						Vertex(1, -1, 1,	//front bottom right
							0, 0, 1, 1,
							0.333f, 0,
							1, -1, 1),

						Vertex(-1, 1, -1,	//back top left
							0, 0, 1, 1,
							1, 0.5f,
							-1, 1, -1),
						Vertex(1, 1, -1,	//back top right
							0, 0, 1, 1,
							0.666f, 0.5f,
							1, 1, -1),
						Vertex(1, -1, -1,	//back bottom right
							0, 0, 1, 1,
							0.666f, 0,
							1, -1, -1),

						Vertex(-1, -1, -1,//back bottom left
							0, 0, 1, 1,
							1, 0,
							-1, -1, -1),
						Vertex(-1, 1, -1,	//back top left
							0, 0, 1, 1,
							1, 0.5f,
							-1, 1, -1),
						Vertex(1, -1, -1,	//back bottom right
							0, 0, 1, 1,
							0.666f, 0,
							1, -1, -1),

						Vertex(-1, 1, 1,	//front top left
							0, 0, 1, 1,
							0.333f, 1,
							-1, 1, 1),
						Vertex(-1, 1, -1,	//back top left
							0, 0, 1, 1,
							0, 1,
							-1, 1, -1),
						Vertex(-1, -1, -1,//back bottom left
							0, 0, 1, 1,
							0, 0.5f,
							-1, -1, -1),

						Vertex(-1, -1, 1,	//front bottom left
							0, 0, 1, 1,
							0.333f, 0.5f,
							-1, -1, 1),
						Vertex(-1, 1, 1,	//front top left
							0, 0, 1, 1,
							0.333f, 1,
							-1, 1, 1),
						Vertex(-1, -1, -1,//back bottom left
							0, 0, 1, 1,
							0, 0.5f,
							-1, -1, -1),

						Vertex(1, 1, -1,	//back top right
							0, 0, 1, 1,
							0.666f, 1.0f,
							1, 1, -1),
						Vertex(-1, 1, -1,	//back top left
							0, 0, 1, 1,
							0.333f, 1,
							-1, 1, -1),
						Vertex(-1, 1, 1,	//front top left
							0, 0, 1, 1,
							0.333f, 0.5f,
							-1, 1, 1),

						Vertex(1, 1, 1,		//front top right
							0, 0, 1, 1,
							0.666f, 0.5f,
							1, 1, 1),
						Vertex(1, 1, -1,	//back top right
							0, 0, 1, 1,
							0.666f, 1,
							1, 1, -1),
						Vertex(-1, 1, 1,	//front top left
							0, 0, 1, 1,
							0.333f, 0.5f,
							-1, 1, 1),

						Vertex(1, -1, 1,	//front bottom right
							0, 0, 1, 1,
							1, 1,
							1, -1, 1),
						Vertex(-1, -1, 1,	//front bottom left
							0, 0, 1, 1,
							0.666f, 1,
							-1, -1, 1),
						Vertex(-1, -1, -1,//back bottom left
							0, 0, 1, 1,
							0.666f, 0.5f,
							-1, -1, -1),

						Vertex(1, -1, -1,	//back bottom right
							0, 0, 1, 1,
							1, 0.5f,
							1, -1, -1),
						Vertex(1, -1, 1,	//front bottom right
							0, 0, 1, 1,
							1, 1,
							1, -1, 1),
						Vertex(-1, -1, -1,//back bottom left
							0, 0, 1, 1,
							0.666f, 0.5f,
							-1, -1, -1),

		};

		indexData = {	0, 1, 2,	//front
						3, 4, 5,

						6, 7, 8,	//right
						9, 10, 11,

						12, 13, 14,	//left
						15, 16, 17,

						18, 19, 20,	//top
						21, 22, 23,

						24, 25, 26,	//bottom
						27, 28, 29,

						30, 31, 32,
						33, 34, 35
		};
		break;
	default:
		break;
	}

	Model model(vertexData, indexData);

	return model;
}

Light ModelLoader::createLight()
{
	std::vector<Vertex> vertexData;
	std::vector<int> indexData;

	//use cube as light model
	vertexData = { Vertex(0.2f, 0.2f, 0.2f,	//front top right
					0, 0, 1, 1,
					0, 0,
					1, 1, 1),
					Vertex(-0.2f, 0.2f, 0.2f,	//front top left
						0, 0, 1, 1,
						0, 0,
						-1, 1, 1),
					Vertex(-0.2f, -0.2f, 0.2f,	//front bottom left
						0, 0, 1, 1,
						0, 0,
						-1, -1, 1),
					Vertex(0.2f, -0.2f, 0.2f,	//front bottom right
						0, 0, 1, 1,
						0, 0,
						1, -1, 1),
					Vertex(0.2f, 0.2f, -0.2f,	//back top right
						0, 0, 1, 1,
						0, 0,
						1, 1, -1),
					Vertex(-0.2f, 0.2f, -0.2f,	//back top left
						0, 0, 1, 1,
						0, 0,
						-1, 1, -1),
					Vertex(-0.2f, -0.2f, -0.2f,//back bottom left
						0, 0, 1, 1,
						0, 0,
						-1, -1, -1),
					Vertex(0.2f, -0.2f, -0.2f,	//back bottom right
						0, 0, 1, 1,
						0, 0,
						1, -1, -1),
	};

	indexData = { 0, 1, 2,	//front
		0, 2, 3,

		4, 0, 3,	//right
		4, 3, 7,

		1, 5, 6,	//left
		1, 6, 2,

		4, 5, 1,	//top
		4, 1, 0,

		3, 2, 6,	//bottom
		3, 6, 7,

		7, 6, 5,
		7, 5, 4
	};

	Light light(vertexData, indexData);

	return light;
}

Model ModelLoader::loadModel(std::string objPath)
{
	objPath = "Models/" + objPath;
	std::vector<Vertex> vertexData;
	std::vector<int> indexData;

	// Initialize Loader
	objl::Loader Loader;

	// Load .obj File
	bool loadout = Loader.LoadFile(objPath);

	// Check to see if it loaded

	// If so continue
	if (loadout)
	{
		// Create/Open e1Out.txt
		std::ofstream file(objPath + "Out.txt");

		// Go through each loaded mesh and out its contents
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];

			// Print Mesh Name
			file << "Mesh " << i << ": " << curMesh.MeshName << "\n";

			// Print Vertices
			file << "Vertices:\n" << curMesh.Vertices.size() << "\n";

			// Go through each vertex and print its number,
			//  position, normal, and texture coordinate
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				Vertex vertex = Vertex(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
				vertex.setNormals(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z);
				vertex.setUVCoordinate(curMesh.Vertices[j].TextureCoordinate.X, curMesh.Vertices[j].TextureCoordinate.Y);

				vertexData.push_back(vertex);
				
				file << "V" << j << ": " <<
					"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
					"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
					"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
			}

			// Print Indices
			file << "Indices:\n";

			// Go through every 3rd index and print the
			//	triangle that these indices represent
			int lastVertexSize = vertexData.size() - curMesh.Vertices.size();
			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				Vertex* v1 = &vertexData[curMesh.Indices[j] + lastVertexSize];
				Vertex* v2 = &vertexData[curMesh.Indices[j + 1] + lastVertexSize];
				Vertex* v3 = &vertexData[curMesh.Indices[j + 2] + lastVertexSize];

				indexData.push_back(curMesh.Indices[j] + lastVertexSize);
				// Solve tangents
				if (!v1->tangentSolved()) v1->calculateTangent(glm::vec3(0, 1, 0));

				indexData.push_back(curMesh.Indices[j + 1] + lastVertexSize);
				if (!v2->tangentSolved()) v2->calculateTangent(glm::vec3(0, 1, 0));

				indexData.push_back(curMesh.Indices[j + 2] + lastVertexSize);
				if (!v3->tangentSolved()) v3->calculateTangent(glm::vec3(0, 1, 0));

				file << "T" << j / 3 << ": " << curMesh.Indices[j] + lastVertexSize << ", " << curMesh.Indices[j + 1] + lastVertexSize << ", " << curMesh.Indices[j + 2] + lastVertexSize << "\n";
			}

			// Print Material
			file << "Material: " << curMesh.MeshMaterial.name << "\n";
			file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
			file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
			file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
			file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
			file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
			file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
			file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
			file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
			file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
			file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
			file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
			file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";

			// Leave a space to separate from the next mesh
			file << "\n";
		}

		// Close File
		file.close();
	}
	// If not output an error
	else
	{
		std::cout << "Failed to load obj file " + objPath << std::endl;

		// Create/Open e1Out.txt
		std::ofstream file(objPath + "Out.txt");

		// Output Error
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";

		// Close File
		file.close();
	}

	Model model(vertexData, indexData);
	std::cout << "Loaded obj file... size : " << (model.vertexData).size() << " : indices : " << (model.indexData).size() << std::endl;

	return model;
}


ModelLoader::~ModelLoader()
{
}
