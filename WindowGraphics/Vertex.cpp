#include "Vertex.h"


#pragma region Vertex Operator
Vertex::Vertex()
{
}

Vertex::Vertex(Vec3 _localPosition, Vec3 _localNormal, Vec2 _uv)
{
	localPosition = _localPosition;
	localNormal = _localNormal;
	uv = _uv;
}

Vertex::~Vertex()
{
}

void VertexCopy(Vertex * src, Vertex * dest)
{
	dest->localPosition = src->localPosition;
	dest->localNormal = src->localNormal;
	dest->uv = src->uv;
}

#pragma endregion

#pragma region VertexOut Operator


VertexOut::VertexOut()
{

}

VertexOut::VertexOut(Vec4 _clipPosition, Vec4 _viewPosition, Vec4 _worldPosition, Vec3 _worldNormal, Vec2 _uv, float _onePerZ)
{
	worldPosition = _worldPosition;
	viewPosition = _viewPosition;
	clipPosition = _clipPosition;
	worldNormal = _worldNormal;
	uv = _uv;
	onePerZ = _onePerZ;
}

VertexOut& VertexOut::operator=(const VertexOut & right)
{
	if (this == &right)
		return *this;
	clipPosition = right.clipPosition;
	viewPosition = right.viewPosition;
	worldPosition = right.worldPosition;
	worldNormal = right.worldNormal;
	uv = right.uv;
	onePerZ = right.onePerZ;
	return *this;
}

VertexOut VertexOut::operator-(const VertexOut & right)
{
	VertexOut result;
	result.clipPosition = clipPosition - right.clipPosition;
	result.viewPosition = viewPosition - right.viewPosition;
	result.worldNormal = worldNormal - right.worldNormal;
	result.worldPosition = worldPosition - right.worldPosition;
	result.uv = uv - right.uv;
	result.onePerZ = onePerZ - right.onePerZ;
	return result;
}

void VertexOutCopy(VertexOut * src, VertexOut * dest)
{
	dest->worldPosition = src->worldPosition;
	dest->viewPosition = src->viewPosition;
	dest->clipPosition = src->clipPosition;
	dest->worldNormal = src->worldNormal;
	dest->uv = src->uv;
	dest->onePerZ = src->onePerZ;
}
#pragma endregion
