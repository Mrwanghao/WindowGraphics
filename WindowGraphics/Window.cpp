#include "Window.h"
#include <iostream>
#include "Scene.h"
#include "Triangle.h"

#define WINDOW_DEFAULT_INITILIZE(w, h) \
	width(w), height(h)


Window::Window() : WINDOW_DEFAULT_INITILIZE(800, 600)
{
	setTitle("No name Window");
	initialize();
}

Window::Window(int _width, int _height) : WINDOW_DEFAULT_INITILIZE(_width, _height)
{
	setTitle("No name Window");
	initialize();
}

Window::Window(const char * _title, int _width, int _height) : WINDOW_DEFAULT_INITILIZE(_width, _height)
{
	setTitle(_title);
	initialize();
}

Window::~Window()
{
	glfwTerminate();

	if (context)
	{
		delete context;
		context = nullptr;
	}

	if (scene)
	{
		delete scene;
		scene = nullptr;
	}

}

bool Window::initialize()
{
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwwindow = glfwCreateWindow(width, height, title, NULL, NULL);
	if (glfwwindow == nullptr)
	{
		std::cout << "error : glfwwindow create when window initialize" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(glfwwindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "初始化GLAD失败" << std::endl;
		return false;
	}

	glViewport(0, 0, width, height);

	return true;
}



void Window::setTitle(const char * _title)
{
	if (title != 0)
		free(title);

	int len = strlen(_title) + 1;
	title = (char*)malloc(sizeof(char) * len + 1);
	strcpy(title, _title);
	title[len] = '\0';
}

void Window::Clear() const
{
	glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Swap() const
{
	glfwSwapBuffers(glfwwindow);
	glfwPollEvents();
}

void Window::Update()
{
	//循环场景中得渲染和逻辑
	auto objects = scene->GetObjects();
	for (auto& object : objects)
	{
		
	}
}
