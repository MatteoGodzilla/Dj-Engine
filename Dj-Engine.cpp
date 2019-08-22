#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "header/Generator.h"


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR" << std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1024, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		std::cout << "GLFW WINDOW ERROR" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW INIT ERROR" << std::endl;
		return -1;
	}
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	

	const char* vshadersource = "\n"
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}";

	const char* fshadersource = "\n"
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"\n"
		"void main()"
		"{\n"
		"	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n";


	unsigned int vbo;
	unsigned int ibo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	float data[] = {
		-0.5,0.5,0.0,
		-0.5,-0.5,0.0,
		0.5,-0.5,0.0,
		0.5,0.5,0.0
	};

	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), data, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


	unsigned int vertexshader;
	unsigned int fragmentshader;
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexshader, 1, &vshadersource, NULL);
	glShaderSource(fragmentshader, 1, &fshadersource, NULL);

	glCompileShader(vertexshader);
	int  success;
	char vertexLog[256];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexshader, 256, NULL, vertexLog);
		std::cout << "ERROR::SHADER::VERTEX_SHADER::COMPILATION_FAILED\n" << vertexLog << std::endl;
	}

	char fragmentLog[256];
	glCompileShader(fragmentshader);
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentshader, 256, NULL, fragmentLog);
		std::cout << "ERROR::SHADER::FRAGMENT_SHADER::COMPILATION_FAILED\n" << fragmentLog << std::endl;
	}

	unsigned int program;
	program = glCreateProgram();

	glAttachShader(program, vertexshader);
	glAttachShader(program, fragmentshader);
	glLinkProgram(program);

	char linkLog[256];
	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		glGetProgramInfoLog(program, 256, NULL, linkLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << linkLog << std::endl;
	}

	bool even = true;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.3, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(program);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}