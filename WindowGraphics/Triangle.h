#pragma once

#include "Vertex.h"

class Triangle
{
private:
	Vertex * vertices[3];
	Math::Vec3* calPosition;
	Math::Vec3 normal;
public:
	Triangle();
	~Triangle();

public:
	void CalNormal();

};