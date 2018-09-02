#pragma once
#include <vector>
#include "Triangle.h"

class Scene
{
private:
	std::vector<Triangle*> objects;

public:
	Scene();
	~Scene();

public:
	std::vector<Triangle*>& GetObjects() { return objects; }
	void AddObject(Triangle* object);
	void ClearObjects();
};