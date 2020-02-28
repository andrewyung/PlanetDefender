#pragma once
#include "Vertex.h"
#include "Model.h"
#include "WindowCanvas.h"

#include <glm/glm.hpp>
#include <vector>
#include <math.h>

class Triangularization
{
public:
	// Checks if v2 is a concave vertex. v1 and v3 are it's adjacent vertices and v1, v2 and v3 is assumed to be counter-clockwise
	static bool isConvexVertex(Vertex v1, Vertex v2, Vertex v3);

	// http://www-cgrl.cs.mcgill.ca/~godfried/teaching/cg-projects/97/Ian/algorithm1.html
	// Triangularizes 2D polygons
	static std::vector<Model> EarTriangularize(Model m);
	static std::vector<Model> EarTriangularize(Model m, glm::vec3 centerSplit);
};