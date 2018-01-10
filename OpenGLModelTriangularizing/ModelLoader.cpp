#include "ModelLoader.h"

Model* ModelLoader::createPrimitive(PrimitiveTypes type)
{
	std::vector<Vertex> vertexData;
	std::vector<int> indexData;

	switch (type)
	{
	case QUAD:
		vertexData = {	Vertex(1.0f, 0.3f, 0),		//top right
						Vertex(1.0f, -0.3f, 0),		//top left
						Vertex(-0.5f, -0.5f, 0),	//bottom left
						Vertex(-0.5f, 0.5f, 0)		//bottom right
		};

		indexData = {	0, 1, 2,	
						0, 2, 3 
		};

		break;
	case TRIANGLE:
		vertexData = {	Vertex(0, 1.0f, 0,		//top
								1, 0, 0, 1),	//color

						Vertex(-0.5f, -0.5f, 0,	//left
								0, 1, 0, 1),	//color

						Vertex(0.5f, -0.5f, 0,	//right
								0, 0, 1, 1)		//color
		};

		indexData = { 0, 1, 2
		};

		break;
	case CUBE:
		vertexData = {	Vertex(0.5f, 0.5f, 0.5f),	//front top right
						Vertex(-0.5f, 0.5f, 0.5f),	//front top left
						Vertex(-0.5f, -0.5f, 0.5f),	//front bottom left
						Vertex(0.5f, -0.5f, 0.5f),	//front bottom right
						Vertex(0.5f, 0.5f, -0.5f),	//back top right
						Vertex(-0.5f, 0.5f, -0.5f),	//back top left
						Vertex(-0.5f, -0.5f, -0.5f),//back bottom left
						Vertex(0.5f, -0.5f, -0.5f),	//back bottom right
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
	default:
		break;
	}

	Model* model = new Model(vertexData, indexData);

	return model;
}


ModelLoader::~ModelLoader()
{
}
