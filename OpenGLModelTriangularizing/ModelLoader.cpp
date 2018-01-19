#include "ModelLoader.h"

Model* ModelLoader::createPrimitive(PrimitiveTypes type)
{
	std::vector<Vertex> vertexData;
	std::vector<int> indexData;

	switch (type)
	{
	case QUAD:
		vertexData = {	Vertex(	0.5f, 0.5f, 0,	//top right position
								1, 1, 1 ,1,		//color
								0, 0,			//uv
								0, 0, 1),		//normal

						Vertex(	0.5f, -0.5f, 0,	//top left position
								1, 1, 1 ,1,		
								0, 0,			
								0, 0, 1),				

						Vertex(	-0.5f, -0.5f, 0,//bottom left position
								1, 1, 1 ,1,		
								0, 0,			
								0, 0, 1),		

						Vertex(	-0.5f, 0.5f, 0,//bottom right position
								1, 1, 1 ,1,		
								0, 0,			
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
								0, 0,
								1, -1, 1),
						Vertex(	0.5f, 0.5f, -0.5f,	//back top right
								0, 0, 1, 1,
								0, 0,
								1, 1, -1),
						Vertex(	-0.5f, 0.5f, -0.5f,	//back top left
								0, 0, 1, 1,
								0, 0,
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
	default:
		break;
	}

	Model* model = new Model(vertexData, indexData);

	return model;
}


ModelLoader::~ModelLoader()
{
}
