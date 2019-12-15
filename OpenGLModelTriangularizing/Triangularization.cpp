#include "Triangularization.h"

using namespace std;

bool isConvexVertex(Vertex v1, Vertex v2, Vertex v3)
{
	// v1 - v2
	vector<float> a { v1.x, v1.y, v1.z };
	// v3 - v2
	vector<float> b { v1.x, v1.y, v1.z };

	//inner_product()
	// find cross product of a and b
	// det[abu]=a×b⋅u where u is cross prod of a and b
	// if result is positive then right-handed, if negative then left-handed

	// check angle (cos0)
	// if angle is < 180 and is left-handed then convex

}

void EarTriangularize(Model m)
{

}