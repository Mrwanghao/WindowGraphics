#pragma once

#include "Vector.h"

struct Vertex
{
	Vec3 localPosition;
	Vec3 localNormal;
	Vec2 uv;

	Vertex();
	Vertex(Vec3 _localPosition, Vec3 _localNormal, Vec2 _uv);
	~Vertex();
};

void VertexCopy(Vertex* src, Vertex* dest);


struct VertexOut
{
	Vec4 worldPosition;
	Vec4 viewPosition;
	Vec4 clipPosition;
	Vec3 worldNormal;
	Vec2 uv;
	float onePerZ;
	VertexOut();
	VertexOut(Vec4 _clipPosition, Vec4 _viewPosition, Vec4 _worldPosition, Vec3 _worldNormal, Vec2 _uv, float _onePerZ);

	VertexOut& operator=(const VertexOut& right);
	VertexOut operator-(const VertexOut& right);
};

void VertexOutCopy(VertexOut* src, VertexOut* dest);





