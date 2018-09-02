#pragma once

#include "Vector.h"
#include "Vertex.h"

class Triangle
{
public:
	Vertex vertexA, vertexB, vertexC;
	VertexOut vertexoutA, vertexoutB, vertexoutC;
	Vec2 ndcA, ndcB, ndcC;

public:
	Triangle();
	Triangle(Vertex a, Vertex b, Vertex c);
	~Triangle();

public:
	void CopyVertexOut(VertexOut a, VertexOut b, VertexOut c);
	void CalculateNDCVertex();

private:
	
};