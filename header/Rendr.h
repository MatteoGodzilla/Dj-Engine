#pragma once

#include "Graphics.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Rendr {
public:
	Rendr();
	void init(GLFWwindow* w);
	GLFWwindow* getWindowPtr();
	virtual ~Rendr();

protected:
	void renderTexture(std::vector<float>& vArr, std::vector<unsigned int>& iArr, unsigned int t);
	void renderColor(std::vector<float>& vArr, std::vector<unsigned int>& iArr);
	void renderText(std::vector<float>& vArr, std::vector<unsigned int>& iArr, unsigned int t);
	void drawText(const std::string& s, float x, float y, float scl);

	void usePersProj();
	void useOrthoProj();
	void setTextColor(float r, float g, float b, float a);
	void checkError();

	void pushVertexColor(std::vector<float>& v, float x, float y, float z, float r, float g, float b, float a = 1.0);
	void pushVertexTexture(std::vector<float>& v, float x, float y, float z, float s = 0.0, float t = 0.0);
	void pushRectangleIndices(std::vector<unsigned int>& v, unsigned int& value);
	void pushTriangleIndices(std::vector<unsigned int>& v, unsigned int& value);

	void loadTexture(const std::string& s, unsigned int* destination);

	float getTextWidth(const std::string& s, float scale);
	float getTextHeight(const std::string& s, float scale);

	void startImGuiFrame(std::string winName, ImGuiBackendFlags flags);
	void renderImGuiFrame();

	GLFWwindow* m_window = nullptr;

	glm::mat4 m_persProj = glm::mat4(1.0f);
	glm::mat4 m_orthoProj = glm::mat4(1.0f);

	unsigned int m_textureProgram = 0;
	unsigned int m_colorProgram = 0;
	unsigned int m_textProgram = 0;

	unsigned int m_textureVAO = 0;
	unsigned int m_textureVBO = 0;
	unsigned int m_colorVAO = 0;
	unsigned int m_colorVBO = 0;

	FT_Library m_FTLibrary;
	FT_Face m_font;

	bool rendr_InvertedX = false;
};
