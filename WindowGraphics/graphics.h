#pragma once
#include "Matrix.h"

class Triangle;
class VertexOut;
class Vertex;
class Vec2;

void DrawLineTriangle(Triangle& tri, unsigned int* videoBuffer, int lPitch);


float CalculateZParam(float min, float max, float t);
void FixFail2Triangle(VertexOut fail1, VertexOut fail2, VertexOut succ);
void FixFail1Triangle(VertexOut succ1, VertexOut succ2, VertexOut fail);
void FixTriangle(const Triangle& tri, int clipFlag);
int  CheckTriangle(const Triangle& tri);
void VertShader(Vertex& input, VertexOut& output);

void Rasterize();

