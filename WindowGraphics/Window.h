#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "DataType.h"
#include "Context.h"

class Scene;

class Window
{
private:
	char* title;
	int width;
	int height;
	Context* context;
	GLFWwindow* glfwwindow;
	Scene* scene;

public:
	Window();
	Window(int _width, int _height);
	Window(const char* _title, int _width, int _height);
	~Window();

private:
	bool initialize();
	void setTitle(const char *_title);
public:
	inline bool Closed() const { return glfwWindowShouldClose(glfwwindow) == 1 ? true : false; }
	void Clear() const;
	void Swap() const;
	void Update();
	
	inline void SetPixel(int row, int col, float invDepth, Color color) { context->SetPixel(row, col, invDepth, color); }


};
