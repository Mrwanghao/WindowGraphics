#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	ClearObjects();


}

void Scene::AddObject(Triangle * object)
{
	objects.push_back(object);
}

void Scene::ClearObjects()
{
	if (objects.size() > 0)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			delete objects[i];
		}
		objects.clear();
	}
}


