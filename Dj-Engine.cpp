#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb/stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>

int width = 1024;
int height = 600;

void getErrors() {
	std::cout << "START" << std::endl;
	unsigned int error = glGetError();
	while (error != GL_NO_ERROR) {
		std::cout << "GL_ERROR: ";
		switch (error) {
		case GL_NO_ERROR:
			std::cout << "NO ERROR" << std::endl;
			break;
		case GL_INVALID_ENUM:
			std::cout << "INVALID ENUM" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cout << "INVALID VALUE" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cout << "INVALID OPERATION" << std::endl;
			break;
		case GL_STACK_OVERFLOW:
			std::cout << "STACK OVERFLOW" << std::endl;
			break;
		case GL_STACK_UNDERFLOW:
			std::cout << "STACK UNDERFLOW" << std::endl;
			break;
		case GL_OUT_OF_MEMORY:
			std::cout << "OUT OF MEMORY" << std::endl;
			break;
		case GL_TABLE_TOO_LARGE:
			std::cout << "TABLE TOO LARGE" << std::endl;
			break;
		}
		error = glGetError();
	}
	std::cout << "END" << std::endl << std::endl;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR" << std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
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
		"layout(location = 0) in vec4 aPos;\n"
		"layout(location = 1) in vec2 tCoords;\n"
		"uniform mat4 u_proj;\n"
		"out vec2 tex_coords;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = u_proj * aPos;\n"
		"	tex_coords = tCoords;\n"
		"}";

	const char* fshadersource = "\n"
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 tex_coords;\n"
		"uniform sampler2D t;\n"
		"\n"
		"void main()"
		"{\n"
		"	FragColor = vec4(tex_coords.x,tex_coords.y,0.0,1.0);\n"
		"	//if(FragColor.a < 0.1)discard;\n"
		"}\n";


	unsigned int vbo;
	unsigned int ibo;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	float data[] = {
		-0.5f, -0.5f, -2.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -2.5f, 1.0f, 1.0f,
		-0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  1.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = {
		0,2,3,
		3,1,0
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data) , data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	int width, height,channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *texture = stbi_load("res/texture.png",&width,&height,&channels,0);

	unsigned int t;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

	stbi_image_free(texture);

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

	int location = glGetUniformLocation(program, "u_proj");

	bool even = true;

	float x = 0.0f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.3, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		


		glm::mat4 proj = glm::perspective(glm::radians(45.0), (double)width / height, 1.0, -1.0);
		
		glm::mat4 look = glm::lookAt(glm::vec3(0.0,1.0,2.5), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		proj = proj * look;

		glUseProgram(program);
		glUniformMatrix4fv(location, 1, GL_FALSE, &proj[0][0]);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(float), GL_UNSIGNED_INT, 0);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		x += 0.0005;
	}

	glfwTerminate();
	return 0;
}