#pragma once
#include "Vertex.h"
#include "Model.h"

class Triangularization
{
public:
	// Checks if v2 is a concave vertex. v1 and v3 are it's adjacent vertices and v1, v2 and v3 is assumed to be counter-clockwise
	bool isConcaveVertex(Vertex v1, Vertex v2, Vertex v3);

	// http://www-cgrl.cs.mcgill.ca/~godfried/teaching/cg-projects/97/Ian/algorithm1.html
	// Triangularizes 2D polygons
	void EarTriangularize(Model m);
};