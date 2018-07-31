#include "Vertex.h"


#pragma region Vertex Operator
Vertex::Vertex()
{
}

Vertex::Vertex(Math::Vec3 _localPosition, Math::Vec3 _localNormal, Math::Vec2 _uv)
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

VertexOut::VertexOut(Math::Vec4 _clipPosition, Math::Vec4 _viewPosition, Math::Vec4 _worldPosition, Math::Vec3 _worldNormal, Math::Vec2 _uv)
{
	worldPosition = _worldPosition;
	viewPosition = _viewPosition;
	clipPosition = _clipPosition;
	worldNormal = _worldNormal;
	uv = _uv;
}

VertexOut& VertexOut::operator=(const VertexOut & right)
{
	clipPosition = right.clipPosition;
	viewPosition = right.viewPosition;
	worldPosition = right.worldPosition;
	worldNormal = right.worldNormal;
	uv = right.uv;
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
	return result;
}

void VertexOutCopy(VertexOut * src, VertexOut * dest)
{
	dest->worldPosition = src->worldPosition;
	dest->viewPosition = src->viewPosition;
	dest->clipPosition = src->clipPosition;
	dest->worldNormal = src->worldNormal;
	dest->uv = src->uv;
}


#pragma endregion
