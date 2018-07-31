#include "graphics.h"
#include "Vector.h"
#include "Triangle.h"
#include "MathUtil.h"
#include "GraphicsMod.h"
#include "Camera.h"
#include <ddraw.h>

using namespace Math;

Triangle tri1;
Triangle tri2;

Math::Matrix4 modelMatrix, viewMatrix, projectMatrix;

extern Camera mainCamera;

void DrawLineTriangle(Triangle& _tri, unsigned int* videoBuffer, int lPitch)
{
	//���ڼ���������ж����vertexout
	VertShader(_tri.vertexA, _tri.vertexoutA);
	VertShader(_tri.vertexB, _tri.vertexoutB);
	VertShader(_tri.vertexC, _tri.vertexoutC);

	//���Ƚ����������ж�
	//Ŀǰ����Ҫ

	//��ô���ڿ��Խ��вü�
	//���Ƚ��б�־Ϊ���
	int clipFlag = CheckTriangle(_tri);

	if (clipFlag != 111)
	{
		//��Ҫ�и�
		if (clipFlag == 000)
		{
			//ȫ�����и��� ���Բ�����Ⱦ��
			return;
		}

		//�ֽ�����޸�
		FixTriangle(_tri, clipFlag);
		
		//��Ⱦ��һ��������
		tri1.CalculateNDCVertex();
		//DrawGouraudTriangle(tri1.ndcA.x, tri1.ndcA.y, tri1.ndcB.x, tri1.ndcB.y, tri1.ndcC.x, tri1.ndcC.y, videoBuffer, lPitch);
		DrawTextureTriangle(tri1, videoBuffer, lPitch);
		
		//DrawClipLine(tri1.ndcA.x, tri1.ndcA.y, tri1.ndcB.x, tri1.ndcB.y, 128, 0, 0,videoBuffer, lPitch);
		//DrawClipLine(tri1.ndcA.x, tri1.ndcA.y, tri1.ndcC.x, tri1.ndcC.y, 128, 0, 0, videoBuffer, lPitch);
		//DrawClipLine(tri1.ndcB.x, tri1.ndcB.y, tri1.ndcC.x, tri1.ndcC.y, 128, 0, 0, videoBuffer, lPitch);

		//DrawTextureTriangle(tri1.ndcA.x, tri1.ndcA.y, tri1.ndcB.x, tri1.ndcB.y, tri1.ndcC.x, tri1.ndcC.y, videoBuffer, lPitch);

		//�п�����Ҫ��Ⱦ�ڶ���������
		if (clipFlag == 101 || clipFlag == 110 || clipFlag == 011)
		{
			//��Ⱦ�ڶ���������
			tri2.CalculateNDCVertex();

			//DrawGouraudTriangle(tri2.ndcA.x, tri2.ndcA.y, tri2.ndcB.x, tri2.ndcB.y, tri2.ndcC.x, tri2.ndcC.y, videoBuffer, lPitch);
			DrawTextureTriangle(tri2, videoBuffer, lPitch);
 			//DrawClipLine(tri2.ndcA.x, tri2.ndcA.y, tri2.ndcB.x, tri2.ndcB.y, 0, 128, 128, videoBuffer, lPitch);
			//DrawClipLine(tri2.ndcA.x, tri2.ndcA.y, tri2.ndcC.x, tri2.ndcC.y, 0, 128, 128, videoBuffer, lPitch);
			//DrawClipLine(tri2.ndcB.x, tri2.ndcB.y, tri2.ndcC.x, tri2.ndcC.y, 0, 128, 128, videoBuffer, lPitch);

			//DrawTextureTriangle(tri2.ndcA.x, tri2.ndcA.y, tri2.ndcB.x, tri2.ndcB.y, tri2.ndcC.x, tri2.ndcC.y, videoBuffer, lPitch);

		}

	}
	else if (clipFlag == 111)
	{
		//�����и�
		_tri.CalculateNDCVertex();

		//DrawGouraudTriangle(tri.ndcA.x, tri.ndcA.y, tri.ndcB.x, tri.ndcB.y, tri.ndcC.x, tri.ndcC.y, videoBuffer, lPitch);
		DrawTextureTriangle(_tri, videoBuffer, lPitch);
		//DrawClipLine(tri.ndcA.x, tri.ndcA.y, tri.ndcB.x, tri.ndcB.y, 128, 0, 0, videoBuffer, lPitch);
		//DrawClipLine(tri.ndcA.x, tri.ndcA.y, tri.ndcC.x, tri.ndcC.y, 128, 0, 0, videoBuffer, lPitch);
		//DrawClipLine(tri.ndcB.x, tri.ndcB.y, tri.ndcC.x, tri.ndcC.y, 128, 0, 0, videoBuffer, lPitch);

		//DrawTextureTriangle(tri.ndcA.x, tri.ndcA.y, tri.ndcB.x, tri.ndcB.y, tri.ndcC.x, tri.ndcC.y, videoBuffer, lPitch);
	}

}


#pragma region �ü�������

float CalculateZParam(float min, float max, float t)
{
	return (t - min) / (max - min);
}

void FixFail2Triangle(VertexOut fail1, VertexOut fail2, VertexOut succ)
{
	float z = mainCamera.nearZ;
	float invFail1VW = 1.0f / fail1.viewPosition.w;
	float invFail2VW = 1.0f / fail2.viewPosition.w;
	float invSuccVW  = 1.0f / succ.viewPosition.w;

	Vec3 viewFail1 = Vec3(fail1.viewPosition.x * invFail1VW, fail1.viewPosition.y * invFail1VW, fail1.viewPosition.z * invFail1VW);
	Vec3 viewFail2 = Vec3(fail2.viewPosition.x * invFail2VW, fail2.viewPosition.y * invFail2VW, fail2.viewPosition.z * invFail2VW);
	Vec3 viewSucc  = Vec3(succ.viewPosition.x  * invSuccVW , succ.viewPosition.y  * invSuccVW , succ.viewPosition.z  * invSuccVW);

	float zParam = CalculateZParam(viewFail1.z, viewSucc.z, z);
	Vec3 interParam = clamp(viewFail1, viewSucc, zParam);
	float leftLength = (viewFail1 - interParam).GetLength();
	float rightLength = (viewSucc - interParam).GetLength();
	float invLength = 1.0f / (leftLength + rightLength);
	leftLength *= invLength;

	VertexOut inter1 = clamp(fail1, succ, zParam);
	
	zParam = CalculateZParam(viewFail2.z, viewSucc.z, z);
	interParam = clamp(viewFail2, viewSucc, zParam);
	leftLength = (viewFail2 - interParam).GetLength();
	rightLength = (viewSucc - interParam).GetLength();
	invLength = 1.0f / (leftLength + rightLength);
	leftLength *= invLength;

	VertexOut inter2 = clamp(fail2, succ, zParam);

	tri1.CopyVertexOut(inter1, inter2, succ);
}

void FixFail1Triangle(VertexOut succ1, VertexOut succ2, VertexOut fail)
{
	float z = mainCamera.nearZ;
	float invSucc1VW = 1.0f / succ1.viewPosition.w;
	float invSucc2VW = 1.0f / succ2.viewPosition.w;
	float invFailVW  = 1.0f / fail.viewPosition.w;

	Vec3 viewSucc1 = Vec3(succ1.viewPosition.x * invSucc1VW, succ1.viewPosition.y * invSucc1VW, succ1.viewPosition.z * invSucc1VW);
	Vec3 viewSucc2 = Vec3(succ2.viewPosition.x * invSucc2VW, succ2.viewPosition.y * invSucc2VW, succ2.viewPosition.z * invSucc2VW);
	Vec3 viewFail  = Vec3(fail.viewPosition.x  * invFailVW, fail.viewPosition.y   * invFailVW, fail.viewPosition.z   * invFailVW);

	float zParam = CalculateZParam(viewFail.z, viewSucc1.z, z);
	Vec3 interParam = clamp(viewFail, viewSucc1, zParam);
	float leftLength = (viewFail - interParam).GetLength();
	float rightLength = (viewSucc1 - interParam).GetLength();
	float invLength = 1.0f / (leftLength + rightLength);
	leftLength *= invLength;
	VertexOut inter1 = clamp(fail, succ1, leftLength);

	zParam = CalculateZParam(viewFail.z, viewSucc2.z, z);
	interParam = clamp(viewFail, viewSucc2, zParam);
	leftLength = (viewFail - interParam).GetLength();
	rightLength = (viewSucc2 - interParam).GetLength();
	invLength = 1.0f / (leftLength + rightLength);
	leftLength *= invLength;
	VertexOut inter2 = clamp(fail, succ2, leftLength);

	tri1.CopyVertexOut(succ1, succ2, inter2);
	tri2.CopyVertexOut(inter1, succ1, inter2);
}

void FixTriangle(const Triangle & tri, int clipFlag)
{
	switch (clipFlag)
	{
	case 001:
		FixFail2Triangle(tri.vertexoutA, tri.vertexoutB, tri.vertexoutC);
		break;

	case 010:
		FixFail2Triangle(tri.vertexoutA, tri.vertexoutC, tri.vertexoutB);
		break;

	case 100:
		FixFail2Triangle(tri.vertexoutB, tri.vertexoutC, tri.vertexoutA);
		break;

	case 110:
		FixFail1Triangle(tri.vertexoutA, tri.vertexoutB, tri.vertexoutC);
		break;

	case 101:
		FixFail1Triangle(tri.vertexoutA, tri.vertexoutC, tri.vertexoutB);
		break;

	case 011:
		FixFail1Triangle(tri.vertexoutB, tri.vertexoutC, tri.vertexoutA);
		break;
	}
}

int CheckTriangle(const Triangle& tri)
{
	bool failA = false, failB = false, failC = false;
	int nFail = 0;

	if (tri.vertexoutA.viewPosition.z / tri.vertexoutA.viewPosition.w < mainCamera.nearZ)
	{
		failA = true;
		nFail++;
	}

	if (tri.vertexoutB.viewPosition.z / tri.vertexoutB.viewPosition.w < mainCamera.nearZ)
	{
		failB = true;
		nFail++;
	}

	if (tri.vertexoutC.viewPosition.z / tri.vertexoutC.viewPosition.w < mainCamera.nearZ)
	{
		failC = true;
		nFail++;
	}

	if (nFail == 3)
	{
		return 000;
	}
	else if (nFail == 2)
	{
		if (failA && failB)
		{
			return 001;
		}
		else if (failA && failC)
		{
			return 010;
		}
		else if (failB && failC)
		{
			return 100;
		}
	}
	else if (nFail == 1)
	{
		if (failA)
		{
			return 011;
		}
		else if (failB)
		{
			return 101;
		}
		else if (failC)
		{
			return 110;
		}
	}
	else if (nFail == 0)
	{
		return 111;
	}

	return 000;
}

void VertShader(Vertex& input, VertexOut& output)
{
	Matrix4 mv = modelMatrix * viewMatrix;
	Matrix4 mvp = mv * projectMatrix;

	Vec4 localPosition(input.localPosition.x, input.localPosition.y, input.localPosition.z, 1.0f);

	//�����ȱ任���㵽��������ϵ��
	output.worldPosition = localPosition * modelMatrix;

	//��������任�������������
	output.viewPosition = localPosition * mv;

	//���������任���ü�����
	output.clipPosition = localPosition * mvp;

	//���߱任����������ϵ��
	output.worldPosition = input.localNormal * modelMatrix;

	//��ֵuv
	output.uv = input.uv;
}

#pragma endregion