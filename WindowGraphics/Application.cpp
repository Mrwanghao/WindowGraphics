#include "Application.h"
#include "Vector.h"
#include "Matrix.h"

using namespace Math;

Application::Application()
{
}


Application::~Application()
{
}

void Application::update()
{
	Vec3 localStartPosition(1.0f, 2.0f, 3.0f);
	Vec3 localEndPosition(2.0f, 4.0f, 6.0f);
	
	// local - world
	Matrix3 worldMatrix;
	localStartPosition *= worldMatrix;
	localEndPosition *= worldMatrix;
	// world - camera

	// camera - perspection

	// perspection - screen

	// screen - π‚’§ªØ



}
