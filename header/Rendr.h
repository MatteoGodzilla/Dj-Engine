// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "DJEUtils.h"
#include "Graphics.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Vertex {
	Vertex(glm::vec3 pos_, glm::vec4 col_, glm::vec2 tex_);
	Vertex(glm::vec3 pos_, glm::vec2 tex_);
	Vertex(glm::vec3 pos_, glm::vec4 col_);
	Vertex(glm::vec3 pos_);
	Vertex();
	Vertex(const Vertex& other);

	glm::vec3 pos;
	glm::vec4 col;
	glm::vec2 tex;
};

struct Quad {
	Vertex v1, v2, v3, v4;
};

class Rendr {
public:
	void init(GLFWwindow* w);
	GLFWwindow* getWindowPtr();

protected:
	void renderTexture(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) const;
	void renderColor(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr) const;
	void renderText(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) const;
	void drawText(const std::string& s, float x, float y, float scl);

	void usePersProj();
	void useOrthoProj();
	static void checkError();

	void pushVertexColor(std::vector<float>& v, float x, float y, float z, float r, float g, float b, float a = 1.0, float s = 0.0, float t = 0.0) const;
	void pushVertexTexture(std::vector<float>& v, float x, float y, float z, float s, float t, float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0) const;
	void pushVertex(std::vector<float>& v, Vertex& vertex) const;
	void pushFourVertices(std::vector<float>& v, Vertex& ver1, Vertex& ver2, Vertex& ver3, Vertex& ver4) const;
	static void pushFourIndices(std::vector<unsigned int>& v, unsigned int& value);
	static void pushTriangleIndices(std::vector<unsigned int>& v, unsigned int& value);
	void pushQuad(std::vector<float>& vArr, std::vector<unsigned int>& indices, unsigned int& indexCount, Quad& quad) const;

	static glm::vec2 getCirclePoint(double radius, double angle);
	static glm::vec2 loadTexture(const std::string& s, unsigned int* destination);

	float getTextWidth(const std::string& s, float scale) const;
	float getTextHeight(const std::string& s, float scale) const;

	static void startImGuiFrame(const std::string& winName, ImGuiBackendFlags flags);
	static void renderImGuiFrame();

	GLFWwindow* m_window = nullptr;

	glm::mat4 m_persProj = glm::mat4(1.0f);
	glm::mat4 m_orthoProj = glm::mat4(1.0f);

	unsigned int m_mainProgram = 0;
	unsigned int m_textProgram = 0;

	unsigned int m_colorTexture = 0;

	unsigned int m_mainVAO = 0;
	unsigned int m_mainVBO = 0;

	FT_Library m_FTLibrary;
	FT_Face m_font;

	bool rendr_InvertedX = false;

	float textFactor = 0.2;
};
