#include "Triangularization.h"
#include "ModelLoader.h"

using namespace std;

bool Triangularization::isConvexVertex(Vertex v1, Vertex v2, Vertex v3)
{
	// v1 - v2
	glm::vec3 a { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	// v3 - v2
	glm::vec3 b { v3.x - v2.x, v3.y - v2.y, v3.z - v2.z};

	// Calculate area. Negative means convex
	float areaSum = 0;
	areaSum += a[0] * (b[2] - b[1]);
	areaSum += a[1] * (b[0] - b[2]);
	areaSum += a[2] * (b[1] - b[0]);
	areaSum *= 0.5f;

	if (areaSum < 0)
	{
		return true;
	}
	return false;
}

vector<Model> Triangularization::EarTriangularize(Model m)
{
	// If only triangle remaining
	if (m.vertexData.size() == 3)
	{
		return vector<Model> {m};
	}

	vector<Model> ears;
	for (int i = 0; i < m.vertexData.size() - 2; i++)
	{
		if (Triangularization::isConvexVertex(m.vertexData[i], m.vertexData[i + 1], m.vertexData[i + 2]))
		{
			// split ear
			Model *ear = ModelLoader::createPrimitive(ModelLoader::TRIANGLE);
			// erase convex vertex from model
			// add model into ears vector
		}
	}

	vector<Model> nestedEars = EarTriangularize(m);
	ears.insert(ears.end(), nestedEars.begin(), nestedEars.end());
	return ears;
}