#pragma once
#include "Graphics.h"
#include "MenuNode.h"

#include "GLFW/glfw3.h"
#include <iostream>
#include <map>

class MenuRender {
public:
	MenuRender();
	void init(GLFWwindow* w);
	void render();
	~MenuRender();
private:
	void pushVertexTexture(std::vector<float>& v, float x, float y, float z, float s = 0.0, float t = 0.0);
	void pushRectangleIndices(std::vector<unsigned int>& v, unsigned int& value);

	void renderTexture(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture);
	void usePersProj();
	void useOrthoProj();
	void renderText(std::vector<float>& vArr, std::vector<unsigned int>& iArr, unsigned int t);
	void drawText(const char* text, float x, float y, float scl);
	void setTextColor(float r, float g, float b, float a = 1.0f);
	GLFWwindow* m_window = nullptr;

	glm::mat4 m_persProj = glm::mat4(1.0f);
	glm::mat4 m_orthoProj = glm::mat4(1.0f);

	unsigned int m_textureProgram = 0;
	unsigned int m_textProgram = 0;

	unsigned int m_textureVAO = 0;
	unsigned int m_textureVBO = 0;

	FT_Library m_FTLibrary;
	FT_Face m_font;
};