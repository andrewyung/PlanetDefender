#include "Triangularization.h"

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

void Triangularization::EarTriangularize(Model m)
{
	vector<Model> ears;
	for (int i = 0; i < m.vertexData.size - 2; i++)
	{
		if (isConvexVertex(m.vertexData[i], m.vertexData[i + 1], m.vertexData[i + 2]))
		{
			// split ear
			// erase convex vertex from model
			// add model into ears vector
		}
	}
	// repeat until triangle left?
}