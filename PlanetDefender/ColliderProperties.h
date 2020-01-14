#pragma once

#include <string>

struct ColliderProperties
{
public:
	std::string name = "default";
	bool collidable = false;

	virtual ~ColliderProperties();
};