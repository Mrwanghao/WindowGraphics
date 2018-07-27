#include "Triangle.h"
#include "GraphicsMod.h"

Triangle::Triangle()
{
}

Triangle::Triangle(Vertex a, Vertex b, Vertex c)
{
	vertexA = a;
	vertexB = b;
	vertexC = c;
}

Triangle::~Triangle()
{
}

void Triangle::CopyVertexOut(VertexOut a, VertexOut b, VertexOut c)
{
	vertexoutA = a;
	vertexoutB = b;
	vertexoutC = c;
}

void Triangle::CalculateNDCVertex()
{
	float invClipAW = 1.0f / vertexoutA.clipPosition.w;
	float invClipBW = 1.0f / vertexoutB.clipPosition.w;
	float invClipCW = 1.0f / vertexoutC.clipPosition.w;

	ndcA.x = vertexoutA.clipPosition.x * invClipAW;
	ndcA.y = vertexoutA.clipPosition.y * invClipAW;
	ndcB.x = vertexoutB.clipPosition.x * invClipBW;
	ndcB.y = vertexoutB.clipPosition.y * invClipBW;
	ndcC.x = vertexoutC.clipPosition.x * invClipCW;
	ndcC.y = vertexoutC.clipPosition.y * invClipCW;

	ndcA.x = (ndcA.x + 1.0f) * 0.5f *  (WINDOW_WIDTH - 1);
	ndcA.y = (-ndcA.y + 1.0f) * 0.5f * (WINDOW_HEIGHT - 1);
	ndcB.x = (ndcB.x + 1.0f) * 0.5f *  (WINDOW_WIDTH - 1);
	ndcB.y = (-ndcB.y + 1.0f) * 0.5f * (WINDOW_HEIGHT - 1);
	ndcC.x = (ndcC.x + 1.0f) * 0.5f *  (WINDOW_WIDTH - 1);
	ndcC.y = (-ndcC.y + 1.0f) * 0.5f * (WINDOW_HEIGHT - 1);

	//ndcA.x = (-ndcA.x + 1.0f) * 0.5f *  (WINDOW_WIDTH  - 1);
	//ndcA.y = (ndcA.y + 1.0f) * 0.5f *  (WINDOW_HEIGHT - 1);
	//ndcB.x = (-ndcB.x + 1.0f) * 0.5f *  (WINDOW_WIDTH  - 1);
	//ndcB.y = (ndcB.y + 1.0f) * 0.5f *  (WINDOW_HEIGHT - 1);
	//ndcC.x = (-ndcC.x + 1.0f) * 0.5f *  (WINDOW_WIDTH  - 1);
	//ndcC.y = (ndcC.y + 1.0f) * 0.5f *  (WINDOW_HEIGHT - 1);
}
