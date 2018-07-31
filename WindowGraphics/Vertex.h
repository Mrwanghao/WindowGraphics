#pragma once

#include "Vector.h"

struct Vertex
{
	Math::Vec3 localPosition;
	Math::Vec3 localNormal;
	Math::Vec2 uv;

	Vertex();
	Vertex(Math::Vec3 _localPosition, Math::Vec3 _localNormal, Math::Vec2 _uv);
	~Vertex();
};

void VertexCopy(Vertex* src, Vertex* dest);


struct VertexOut
{
	Math::Vec4 worldPosition;
	Math::Vec4 viewPosition;
	Math::Vec4 clipPosition;
	Math::Vec3 worldNormal;
	Math::Vec2 uv;

	VertexOut();
	VertexOut(Math::Vec4 _clipPosition, Math::Vec4 _viewPosition, Math::Vec4 _worldPosition, Math::Vec3 _worldNormal, Math::Vec2 _uv);

	VertexOut& operator=(const VertexOut& right);
	VertexOut operator-(const VertexOut& right);
};

void VertexOutCopy(VertexOut* src, VertexOut* dest);





