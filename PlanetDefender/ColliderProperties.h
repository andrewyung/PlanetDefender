#pragma once

#include <string>

enum ColliderType {
	RAY,
	TRIANGLE,
	ELLIPSOID
};

struct ColliderProperties
{
public:
	ColliderType type;
	std::string name = "default";
	bool collidable = false;
};