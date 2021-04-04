// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MenuRender.h"

void MenuRender::init(GLFWwindow* w) {
	Rendr::init(w);

	m_animManager.pushTimer(Timer(AN_SCROLL_UP, 0.1));
	m_animManager.pushTimer(Timer(AN_SCROLL_DOWN, 0.1));

	m_window = w;
	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK) {
		NormalLog << "GLEW INIT ERROR" << ENDL;
		return;
	}
	NormalLog << "MenuRender init: " << glGetString(GL_VERSION) << ENDL;
	loadTexture("res/buttons.png", &m_buttonTexture);
	m_logoDimensions = loadTexture("res/splashArt.png", &m_splashTexture);
	loadTexture("res/calibration.png", &m_calibrationTex);
	loadTexture("res/pgBar-frame.png", &m_pgBarFrame);
	loadTexture("res/menuVynil.png", &m_menuVynil);
	m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/NotoSans-Regular.ttf", 24.0f);
	m_pastTime = glfwGetTime();
}

void MenuRender::tick() {
	double nowTime = glfwGetTime();
	m_dTime = nowTime - m_pastTime;
	m_globalTime += m_dTime;
	m_pastTime = nowTime;

	m_animManager.tick(m_globalTime);
}

void MenuRender::render(MenuNode& node, int selected) {
	//enable exit from remapping menu
	m_shouldClose = false;
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//vertices data
	//std::vector<float> selVector;
	//std::vector<unsigned int> selIndices;
	//unsigned int selVertexCount = 0;

	std::vector<float> mainVector;
	std::vector<unsigned int> mainIndices;
	unsigned int mainVertexCount = 0;

	//text scale
	float scale = 0.075f;
	double padding = 10;
	double textAngle = glm::pi<double>() / 8;

	double radius = 720.0 * 1 / 3;
	glm::vec2 center = {radius / 4, 720.0 / 2};
	Vertex cv(glm::vec3(center.x, center.y, 0.0));
	Vertex before(glm::vec3(center.x, center.y + radius, 0.0));
	cv.tex = glm::vec2(0.5, 0.5);
	before.tex = glm::vec2(1.0, 0.5);

	//selection color
	glm::vec4 firstCol = {0.83, 0.35, 0.24, 1.0}; //yellow {1.0,0.83,0.15,1.0}
	glm::vec4 secondCol = {0.83, 0.35, 0.24, 0.0}; //red {0.83,0.35,0.24,1.0}
		//
	Timer up = m_animManager.getAnimById(AN_SCROLL_UP);
	Timer down = m_animManager.getAnimById(AN_SCROLL_DOWN);

	double angleDiff = 0;

	double scratchSpeed = 2;

	if (up.isEnabled()) {
		angleDiff = (1.0 - up.getPercent()) * textAngle;
		m_timeWarp = m_timeWarpStart + scratchSpeed * up.getTime();
	} else if (down.isEnabled()) {
		angleDiff = (1.0 - down.getPercent()) * textAngle * -1;
		m_timeWarp = m_timeWarpStart - scratchSpeed * down.getTime();
	}

	//vynil on the left
	for (double angle = 0; angle < glm::two_pi<double>(); angle += m_deltaAngle) {
		glm::vec2 p = getCirclePoint(radius, angle);
		Vertex v(glm::vec3(p.x + center.x, -p.y + center.y, 0.0));
		double angleOffset = glm::pi<double>() * 3 / 2 * (m_globalTime + m_timeWarp);
		v.tex = glm::vec2((cos(angle + angleOffset) + 1) / 2, (sin(angle + angleOffset) + 1) / 2);

		pushVertex(mainVector, cv);
		pushVertex(mainVector, v);
		pushVertex(mainVector, before);
		pushTriangleIndices(mainIndices, mainVertexCount);
		before = v;
	}

	useOrthoProj();
	renderTexture(mainVector, mainIndices, m_menuVynil);

	if (selected - 4 >= 0 && selected - 4 < node.getChildCount()) {
		glm::vec2 basePoint = getCirclePoint(radius + padding * 2, glm::pi<double>() / 2);
		std::string s = "^";
		float height = getTextHeight(s, scale);
		drawText(s, basePoint.x + center.x, -basePoint.y - height / 2 + center.y, scale);
	}
	if (selected - 3 >= 0 && selected - 3 < node.getChildCount()) {
		float scl = scale / 2;
		glm::vec2 basePoint = getCirclePoint(radius + padding, textAngle * 3 + angleDiff);
		MenuNode& selectedNode = node.getChildrens().at(selected - 3);
		float height = getTextHeight(selectedNode.getText(), scl);
		drawText(selectedNode.getText(), basePoint.x + center.x, -basePoint.y - height / 2 + center.y, scl);
	}
	if (selected - 2 >= 0 && selected - 2 < node.getChildCount()) {
		float scl = scale / 2;
		glm::vec2 basePoint = getCirclePoint(radius + padding, textAngle * 2 + angleDiff);
		MenuNode& selectedNode = node.getChildrens().at(selected - 2);
		float height = getTextHeight(selectedNode.getText(), scl);
		drawText(selectedNode.getText(), basePoint.x + center.x, -basePoint.y - height / 2 + center.y, scl);
	}
	if (selected - 1 >= 0 && selected - 1 < node.getChildCount()) {
		float scl = scale / 2;
		glm::vec2 basePoint = getCirclePoint(radius + padding, textAngle * 1 + angleDiff);
		MenuNode& selectedNode = node.getChildrens().at(selected - 1);
		float height = getTextHeight(selectedNode.getText(), scl);
		drawText(selectedNode.getText(), basePoint.x + center.x, -basePoint.y - height / 2 + center.y, scl);
	}
	if (selected - 0 >= 0 && selected - 0 < node.getChildCount()) {
		glm::vec2 basePoint = getCirclePoint(radius + padding, textAngle * 0 + angleDiff);
		MenuNode& selectedNode = node.getChildrens().at(selected - 0);
		float height = getTextHeight(selectedNode.getText(), scale);
		drawText(selectedNode.getText(), basePoint.x + center.x, -basePoint.y - height / 2 + center.y, scale);
	}
	if (selected + 1 >= 0 && selected + 1 < node.getChildCount()) {
		float scl = scale / 2;
		glm::vec2 basePoint = getCirclePoint(radius + padding, textAngle * -1 + angleDiff);
		MenuNode& selectedNode = node.getChildrens().at(selected + 1);
		float height = getTextHeight(selectedNode.getText(), scl);
		drawText(selectedNode.getText(), basePoint.x + center.x, -basePoint.y - height / 2 + center.y, scl);
	}
	if (selected + 2 >= 0 && selected + 2 < node.getChildCount()) {
		float scl = scale / 2;
		glm::vec2 basePoint = getCirclePoint(radius + padding, textAngle * -2 + angleDiff);
		MenuNode& selectedNode = node.getChildrens().at(selected + 2);
		float height = getTextHeight(selectedNode.getText(), scl);
		drawText(selectedNode.getText(), basePoint.x + center.x, -basePoint.y - height / 2 + center.y, scl);
	}
	if (selected + 3 >= 0 && selected + 3 < node.getChildCount()) {
		float scl = scale / 2;
		glm::vec2 basePoint = getCirclePoint(radius + padding, textAngle * -3 + angleDiff);
		MenuNode& selectedNode = node.getChildrens().at(selected + 3);
		float height = getTextHeight(selectedNode.getText(), scl);
		drawText(selectedNode.getText(), basePoint.x + center.x, -basePoint.y - height / 2 + center.y, scl);
	}
	if (selected + 4 >= 0 && selected + 4 < node.getChildCount()) {
		glm::vec2 basePoint = getCirclePoint(radius + padding * 2, glm::pi<double>() / 2 * -1);
		std::string s = "v";
		float height = getTextHeight(s, scale);
		drawText(s, basePoint.x + center.x, -basePoint.y - height / 2 + center.y, scale);
	}
}

void MenuRender::play(std::vector<SongEntry>& list, int selected) {
	float textSize = 0.040;
	if (!list.empty()) {
		std::vector<float> mainVector;
		std::vector<unsigned int> mainIndices;
		unsigned int mainVertexCount = 0;

		int otherCount = 10;

		//Main Selection
		float listWidth = 1280.0f / 5.0f * 4.0f;
		float infoWidth = 1280.0f - listWidth;

		std::string album1Path = list.at(selected).path + std::string("/album1.png");
		std::string album2Path = list.at(selected).path + std::string("/album2.png");
		std::ifstream checkOne = std::ifstream(album1Path);
		std::ifstream checkTwo = std::ifstream(album2Path);

		Quad q;
		q.v1 = Vertex(glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 1.0));
		q.v2 = Vertex(glm::vec3(0.0, infoWidth, 0.0), glm::vec2(0.0, 0.0));
		q.v3 = Vertex(glm::vec3(infoWidth, infoWidth, 0.0), glm::vec2(1.0, 0.0));
		q.v4 = Vertex(glm::vec3(infoWidth, 0.0, 0.0), glm::vec2(1.0, 1.0));

		useOrthoProj();
		pushQuad(mainVector, mainIndices, mainVertexCount, q);

		if (checkOne.is_open()) {
			//available album
			loadTexture(album1Path, &m_album1);
			renderTexture(mainVector, mainIndices, m_album1);
		} else {
			//album not available
			renderTexture(mainVector, mainIndices, m_menuVynil);
		}
		checkOne.close();

		mainVector.clear();
		mainIndices.clear();
		mainVertexCount = 0;

		Quad q2;
		q2.v1 = Vertex(glm::vec3(0.0, infoWidth, 0.0), glm::vec2(0.0, 1.0));
		q2.v2 = Vertex(glm::vec3(0.0, infoWidth * 2, 0.0), glm::vec2(0.0, 0.0));
		q2.v3 = Vertex(glm::vec3(infoWidth, infoWidth * 2, 0.0), glm::vec2(1.0, 0.0));
		q2.v4 = Vertex(glm::vec3(infoWidth, infoWidth, 0.0), glm::vec2(1.0, 1.0));

		useOrthoProj();
		pushQuad(mainVector, mainIndices, mainVertexCount, q2);

		if (checkTwo.is_open()) {
			//available album
			loadTexture(album2Path, &m_album2);
			renderTexture(mainVector, mainIndices, m_album2);
		} else {
			//album not available
			renderTexture(mainVector, mainIndices, m_menuVynil);
		}
		checkTwo.close();

		mainVector.clear();
		mainIndices.clear();
		mainVertexCount = 0;

		float firstBaseX = infoWidth + listWidth / 4;
		float secondBaseX = 1280.0f - listWidth / 4;
		float singleTrackX = infoWidth + listWidth / 2;
		float selectedY = 720.0f / 2.0;

		float difficultyHeight = 720.0f - infoWidth * 2;

		std::string s1 = list.at(selected).s1;
		std::string s2 = list.at(selected).s2;

		if (s2.empty()) {
			std::string rendered1 = std::string("");
			for (size_t i = 0; i < s1.size() && getTextWidth(rendered1, textSize) < listWidth - 30; ++i) {
				rendered1 += s1.at(i);
			}

			float w = getTextWidth(rendered1, textSize);
			float h = getTextHeight(rendered1, textSize);
			drawText(rendered1, singleTrackX - w / 2, selectedY - h / 2, textSize);
		} else {
			std::string rendered1 = std::string("");
			std::string rendered2 = std::string("");
			for (size_t i = 0; i < s1.size() && getTextWidth(rendered1, textSize) < listWidth / 2 - 30; ++i) {
				rendered1 += s1.at(i);
			}
			for (size_t i = 0; i < s2.size() && getTextWidth(rendered2, textSize) < listWidth / 2 - 30; ++i) {
				rendered2 += s2.at(i);
			}

			float w = getTextWidth(rendered1, textSize);
			float h = getTextHeight(rendered1, textSize);
			drawText(rendered1, firstBaseX - w / 2, selectedY - h / 2, textSize);

			w = getTextWidth(rendered2, textSize);
			h = getTextHeight(rendered2, textSize);
			drawText(rendered2, secondBaseX - w / 2, selectedY - h / 2, textSize);
		}

		for (size_t i = 1; i < otherCount; ++i) {
			if (selected - i >= 0 && selected - i < list.size()) {
				std::string s1 = list.at(selected - i).s1;
				std::string s2 = list.at(selected - i).s2;

				if (s2.empty()) {
					std::string rendered1 = std::string("");
					for (size_t i = 0; i < s1.size() && getTextWidth(rendered1, textSize / 2) < listWidth - 30; ++i) {
						rendered1 += s1.at(i);
					}

					float w = getTextWidth(rendered1, textSize / 2);
					drawText(rendered1, singleTrackX - w / 2, selectedY - textSize * 1000 - (textSize * 750) * i, textSize / 2);
				} else {
					std::string rendered1 = std::string("");
					std::string rendered2 = std::string("");
					for (size_t i = 0; i < s1.size() && getTextWidth(rendered1, textSize / 2) < listWidth / 2 - 30; ++i) {
						rendered1 += s1.at(i);
					}
					for (size_t i = 0; i < s2.size() && getTextWidth(rendered2, textSize / 2) < listWidth / 2 - 30; ++i) {
						rendered2 += s2.at(i);
					}

					float w = getTextWidth(rendered1, textSize / 2);
					drawText(rendered1, firstBaseX - w / 2, selectedY - textSize * 1000 - (textSize * 750) * i, textSize / 2);

					w = getTextWidth(rendered2, textSize / 2);
					drawText(rendered2, secondBaseX - w / 2, selectedY - textSize * 1000 - (textSize * 750) * i, textSize / 2);
				}
			} else {
				break;
			}
		}

		for (size_t i = 1; i < otherCount; ++i) {
			if (selected + i >= 0 && selected + i < list.size()) {
				std::string s1 = list.at(selected + i).s1;
				std::string s2 = list.at(selected + i).s2;

				if (s2.empty()) {
					std::string rendered1 = std::string("");
					for (size_t i = 0; i < s1.size() && getTextWidth(rendered1, textSize / 2) < listWidth - 30; ++i) {
						rendered1 += s1.at(i);
					}

					float w = getTextWidth(rendered1, textSize / 2);
					drawText(rendered1, singleTrackX - w / 2, selectedY + textSize * 1000 + (textSize * 750) * i, textSize / 2);
				} else {
					std::string rendered1 = std::string("");
					std::string rendered2 = std::string("");
					for (size_t i = 0; i < s1.size() && getTextWidth(rendered1, textSize / 2) < listWidth / 2 - 30; ++i) {
						rendered1 += s1.at(i);
					}
					for (size_t i = 0; i < s2.size() && getTextWidth(rendered2, textSize / 2) < listWidth / 2 - 30; ++i) {
						rendered2 += s2.at(i);
					}

					float w = getTextWidth(rendered1, textSize / 2);
					drawText(rendered1, firstBaseX - w / 2, selectedY + textSize * 1000 + (textSize * 750) * i, textSize / 2);

					w = getTextWidth(rendered2, textSize / 2);
					drawText(rendered2, secondBaseX - w / 2, selectedY + textSize * 1000 + (textSize * 750) * i, textSize / 2);
				}
			} else {
				break;
			}
		}

		//info rendering

		mainVector.clear();
		mainIndices.clear();
		mainVertexCount = 0;

		float deltaText = difficultyHeight / 8;

		glm::vec4 grayBaseColor = glm::vec4(0.2, 0.2, 0.2, 1.0);
		glm::vec4 greenHighlightColor = glm::vec4(0.133, 0.875, 0.180, 1.0);

		//progress bar gray base
		q.v1 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 3.5, 0.0), grayBaseColor);
		q.v2 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 4.5, 0.0), grayBaseColor);
		q.v3 = Vertex(glm::vec3(infoWidth - 10.0, 720.0 - difficultyHeight + deltaText * 4.5, 0.0), grayBaseColor);
		q.v4 = Vertex(glm::vec3(infoWidth - 10.0, 720.0 - difficultyHeight + deltaText * 3.5, 0.0), grayBaseColor);
		pushQuad(mainVector, mainIndices, mainVertexCount, q);

		q.v1 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 4.5, 0.0), grayBaseColor);
		q.v2 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 5.5, 0.0), grayBaseColor);
		q.v3 = Vertex(glm::vec3(infoWidth - 10.0, 720.0 - difficultyHeight + deltaText * 5.5, 0.0), grayBaseColor);
		q.v4 = Vertex(glm::vec3(infoWidth - 10.0, 720.0 - difficultyHeight + deltaText * 4.5, 0.0), grayBaseColor);
		pushQuad(mainVector, mainIndices, mainVertexCount, q);

		q.v1 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 5.5, 0.0), grayBaseColor);
		q.v2 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 6.5, 0.0), grayBaseColor);
		q.v3 = Vertex(glm::vec3(infoWidth - 10.0, 720.0 - difficultyHeight + deltaText * 6.5, 0.0), grayBaseColor);
		q.v4 = Vertex(glm::vec3(infoWidth - 10.0, 720.0 - difficultyHeight + deltaText * 5.5, 0.0), grayBaseColor);
		pushQuad(mainVector, mainIndices, mainVertexCount, q);

		q.v1 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 6.5, 0.0), grayBaseColor);
		q.v2 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 7.5, 0.0), grayBaseColor);
		q.v3 = Vertex(glm::vec3(infoWidth - 10.0, 720.0 - difficultyHeight + deltaText * 7.5, 0.0), grayBaseColor);
		q.v4 = Vertex(glm::vec3(infoWidth - 10.0, 720.0 - difficultyHeight + deltaText * 6.5, 0.0), grayBaseColor);
		pushQuad(mainVector, mainIndices, mainVertexCount, q);

		//General complexity
		int v = list.at(selected).dTrack;
		q.v1 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 3.5, 0.0), greenHighlightColor);
		q.v2 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 4.5, 0.0), greenHighlightColor);
		q.v3 = Vertex(glm::vec3(10.0 + (infoWidth - 20.0) * v / 100, 720.0 - difficultyHeight + deltaText * 4.5, 0.0), greenHighlightColor);
		q.v4 = Vertex(glm::vec3(10.0 + (infoWidth - 20.0) * v / 100, 720.0 - difficultyHeight + deltaText * 3.5, 0.0), greenHighlightColor);
		pushQuad(mainVector, mainIndices, mainVertexCount, q);

		//Tap complexity
		v = list.at(selected).dTap;
		q.v1 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 4.5, 0.0), greenHighlightColor);
		q.v2 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 5.5, 0.0), greenHighlightColor);
		q.v3 = Vertex(glm::vec3(10.0 + (infoWidth - 20.0) * v / 100, 720.0 - difficultyHeight + deltaText * 5.5, 0.0), greenHighlightColor);
		q.v4 = Vertex(glm::vec3(10.0 + (infoWidth - 20.0) * v / 100, 720.0 - difficultyHeight + deltaText * 4.5, 0.0), greenHighlightColor);
		pushQuad(mainVector, mainIndices, mainVertexCount, q);

		//Crossfade complexity
		v = list.at(selected).dCrossfade;
		q.v1 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 5.5, 0.0), greenHighlightColor);
		q.v2 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 6.5, 0.0), greenHighlightColor);
		q.v3 = Vertex(glm::vec3(10.0 + (infoWidth - 20.0) * v / 100, 720.0 - difficultyHeight + deltaText * 6.5, 0.0), greenHighlightColor);
		q.v4 = Vertex(glm::vec3(10.0 + (infoWidth - 20.0) * v / 100, 720.0 - difficultyHeight + deltaText * 5.5, 0.0), greenHighlightColor);
		pushQuad(mainVector, mainIndices, mainVertexCount, q);

		//Scratch complexity
		v = list.at(selected).dScratch;
		q.v1 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 6.5, 0.0), greenHighlightColor);
		q.v2 = Vertex(glm::vec3(10.0, 720.0 - difficultyHeight + deltaText * 7.5, 0.0), greenHighlightColor);
		q.v3 = Vertex(glm::vec3(10.0 + (infoWidth - 20.0) * v / 100, 720.0 - difficultyHeight + deltaText * 7.5, 0.0), greenHighlightColor);
		q.v4 = Vertex(glm::vec3(10.0 + (infoWidth - 20.0) * v / 100, 720.0 - difficultyHeight + deltaText * 6.5, 0.0), greenHighlightColor);
		pushQuad(mainVector, mainIndices, mainVertexCount, q);

		renderColor(mainVector, mainIndices);

		std::string rendered = std::string("");
		std::string origin = list.at(selected).a1;
		for (size_t i = 0; i < origin.size() && getTextWidth(rendered, textSize / 2) < infoWidth - 30; ++i) {
			rendered += origin.at(i);
		}
		drawText(rendered, 10.0f, 720.0f - difficultyHeight + deltaText * 1 - getTextHeight(rendered, textSize / 2) / 2, textSize / 2);

		rendered = std::string("");
		origin = list.at(selected).a2;
		for (size_t i = 0; i < origin.size() && getTextWidth(rendered, textSize / 2) < infoWidth - 30; ++i) {
			rendered += origin.at(i);
		}
		drawText(rendered, 10.0f, 720.0f - difficultyHeight + deltaText * 2 - getTextHeight(rendered, textSize / 2) / 2, textSize / 2);

		rendered = std::string("");
		origin = std::string("BPM:") + std::to_string(list.at(selected).bpm);
		for (size_t i = 0; i < origin.size() && getTextWidth(rendered, textSize / 2) < infoWidth - 30; ++i) {
			rendered += origin.at(i);
		}
		drawText(rendered, 10.0f, 720.0f - difficultyHeight + deltaText * 3 - getTextHeight(rendered, textSize / 2) / 2, textSize / 2);

		rendered = std::string("");
		origin = std::string("Overall:") + std::to_string(list.at(selected).dTrack);
		for (size_t i = 0; i < origin.size() && getTextWidth(rendered, textSize / 2) < infoWidth - 30; ++i) {
			rendered += origin.at(i);
		}
		drawText(rendered, 10.0f, 720.0f - difficultyHeight + deltaText * 4 - getTextHeight(rendered, textSize / 2) / 2, textSize / 2);

		rendered = std::string("");
		origin = std::string("Taps:") + std::to_string(list.at(selected).dTap);
		for (size_t i = 0; i < origin.size() && getTextWidth(rendered, textSize / 2) < infoWidth - 30; ++i) {
			rendered += origin.at(i);
		}
		drawText(rendered, 10.0f, 720.0f - difficultyHeight + deltaText * 5 - getTextHeight(rendered, textSize / 2) / 2, textSize / 2);

		rendered = std::string("");
		origin = std::string("Crossfades:") + std::to_string(list.at(selected).dCrossfade);
		for (size_t i = 0; i < origin.size() && getTextWidth(rendered, textSize / 2) < infoWidth - 30; ++i) {
			rendered += origin.at(i);
		}
		drawText(rendered, 10.0f, 720.0f - difficultyHeight + deltaText * 6 - getTextHeight(rendered, textSize / 2) / 2, textSize / 2);

		rendered = std::string("");
		origin = std::string("Scratches:") + std::to_string(list.at(selected).dScratch);
		for (size_t i = 0; i < origin.size() && getTextWidth(rendered, textSize / 2) < infoWidth - 30; ++i) {
			rendered += origin.at(i);
		}
		drawText(rendered, 10.0f, 720.0f - difficultyHeight + deltaText * 7 - getTextHeight(rendered, textSize / 2) / 2, textSize / 2);

	} else {
		std::string rendered1 = std::string("No songs found in the song Cache");
		std::string rendered2 = std::string("Press Green to scan now");

		float w = getTextWidth(rendered1, textSize);
		float h = getTextHeight(rendered1, textSize) + getTextHeight(rendered2, textSize);
		drawText(rendered1, 1280.0f / 2 - w / 2, 720.0f / 2 - h / 2, textSize);
		w = getTextWidth(rendered2, textSize);
		drawText(rendered2, 1280.0f / 2 - w / 2, 720.0f / 2, textSize);
	}
}

void MenuRender::remapping(Game* game) {
	int colnum = 5;

	ImGuiBackendFlags flags = 0;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	startImGuiFrame("Remapper Window", flags);
	ImGui::SetWindowPos({0.0f, 0.0f});

	int width;
	int height;
	glfwGetFramebufferSize(m_window, &width, &height);
	ImGui::SetWindowSize({(float)width, (float)height});

	int id = game->getPlayer()->m_useKeyboardInput ? -1 : game->getPlayer()->m_gamepadId;
	if (id < 0) {
		m_inputSelection = "Keyboard";
	} else {
		std::string name = glfwGetJoystickName(id);
		std::string t = "Id " + std::to_string(id) + ":" + name;
		m_inputSelection = t;
	}

	ImGui::Text("Using: ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("Choose Input", m_inputSelection.c_str())) {
		if (ImGui::Selectable("Keyboard")) {
			game->getPlayer()->m_useKeyboardInput = true;
		}
		for (int i = 0; i < 16; ++i) {
			std::string name;
			if (glfwJoystickPresent(i)) {
				name = glfwGetJoystickName(i);
				std::string t = "Id " + std::to_string(id) + ":" + name;
				if (ImGui::Selectable(t.c_str())) {
					game->getPlayer()->m_useKeyboardInput = false;
					game->getPlayer()->m_gamepadId = i;
				}
			}
		}
		ImGui::EndCombo();
	}

	if (!game->getPlayer()->m_useKeyboardInput) {
		game->getPlayer()->updateGamepadState();
		ImGui::SameLine();
		if (ImGui::Button("Axis viewer")) {
			ImGui::OpenPopup("raw viewer");
			ImGui::SetNextWindowSize(ImVec2(500.0f, 700.0f));
		}
		if (ImGui::BeginPopupModal("raw viewer")) {
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::Columns(2, "axes");
			std::vector<float> values = game->getPlayer()->getGamepadValues();
			for (size_t i = 0; i < values.size(); ++i) {
				ImGui::Text("%s", std::to_string(i).c_str());
				ImGui::NextColumn();
				ImGui::ProgressBar(values.at(i), ImVec2(0.0f, 0.0f), std::to_string(values.at(i)).c_str());
				ImGui::NextColumn();
			}
			ImGui::Columns();
			ImGui::EndPopup();
		}
	} else {
		game->getPlayer()->updateKBMState(game->getGameRender()->getWindowPtr());
		ImGui::SameLine();
		if (ImGui::Button("Axis viewer")) {
			ImGui::OpenPopup("raw viewer");
			ImGui::SetNextWindowSize(ImVec2(500.0f, 700.0f));
		}
		if (ImGui::BeginPopupModal("raw viewer")) {
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::Columns(2, "axes");
			std::vector<float> values = game->getPlayer()->getKBMValues(m_window);
			for (size_t i = 0; i < values.size(); ++i) {
				ImGui::Text("%s", std::to_string(i).c_str());
				ImGui::NextColumn();
				ImGui::ProgressBar(values.at(i), ImVec2(0.0f, 0.0f), std::to_string(values.at(i)).c_str());
				ImGui::NextColumn();
			}
			ImGui::Columns();
			ImGui::EndPopup();
		}
	}

	if (game->getPlayer()->m_gpState.size() >= 10 && game->getPlayer()->m_gpMult.size() >= 10 && game->getPlayer()->m_gpDead.size() >= 10) {
		ImGui::Columns(colnum, "mycolumns3", false); // 3-ways, no border

		ImGui::Text("Action");
		ImGui::NextColumn();
		ImGui::Text("ID");
		ImGui::NextColumn();
		ImGui::Text("Value");
		ImGui::NextColumn();
		ImGui::Text("Sensitivity");
		ImGui::NextColumn();
		ImGui::Text("Deadzone");
		ImGui::NextColumn();
		ImGui::Separator();

		//Green button
		float value = game->getPlayer()->m_gpState.at(GREEN_INDEX) * game->getPlayer()->m_gpMult.at(GREEN_INDEX);
		float dead = game->getPlayer()->m_gpDead.at(GREEN_INDEX);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Green Button:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Green Button:");
		}
		ImGui::NextColumn();
		std::string id = std::string("Remap") + std::string("##01");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(GREEN_INDEX);
			} else {
				editingAxisController(GREEN_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Green Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##green", &(game->getPlayer()->GREEN_CODE));
		} else {
			ImGui::InputInt("##green", &(game->getPlayer()->GREEN_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##1", &(game->getPlayer()->m_gpMult.at(GREEN_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##11", &(game->getPlayer()->m_gpDead.at(GREEN_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		//Red Button
		value = game->getPlayer()->m_gpState.at(RED_INDEX) * game->getPlayer()->m_gpMult.at(RED_INDEX);
		dead = game->getPlayer()->m_gpDead.at(RED_INDEX);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Red Button:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Red Button:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##02k");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(RED_INDEX);
			} else {
				editingAxisController(RED_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Red Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##red", &(game->getPlayer()->RED_CODE));
		} else {
			ImGui::InputInt("##red", &(game->getPlayer()->RED_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##2", &(game->getPlayer()->m_gpMult.at(RED_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##21", &(game->getPlayer()->m_gpDead.at(RED_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		//Blue Button
		value = game->getPlayer()->m_gpState.at(BLUE_INDEX) * game->getPlayer()->m_gpMult.at(BLUE_INDEX);
		dead = game->getPlayer()->m_gpDead.at(BLUE_INDEX);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Blue Button:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Blue Button:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##03k");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(BLUE_INDEX);
			} else {
				editingAxisController(BLUE_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Blue Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##blue", &(game->getPlayer()->BLUE_CODE));
		} else {
			ImGui::InputInt("##blue", &(game->getPlayer()->BLUE_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##3", &(game->getPlayer()->m_gpMult.at(BLUE_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##31", &(game->getPlayer()->m_gpDead.at(BLUE_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		//Euphoria button
		value = game->getPlayer()->m_gpState.at(EU_INDEX) * game->getPlayer()->m_gpMult.at(EU_INDEX);
		dead = game->getPlayer()->m_gpDead.at(EU_INDEX);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Euphoria Button:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Euphoria Button:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##04");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(EU_INDEX);
			} else {
				editingAxisController(EU_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Euphoria Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##eu", &(game->getPlayer()->EUPHORIA));
		} else {
			ImGui::InputInt("##eu", &(game->getPlayer()->EU_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##4", &(game->getPlayer()->m_gpMult.at(EU_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##41", &(game->getPlayer()->m_gpDead.at(EU_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();
		ImGui::Columns();

		ImGui::Checkbox("Use Single Axis for Crossfade", &(game->getPlayer()->m_useSingleCfAxis));
		ImGui::Columns(colnum, "second col", false);

		//Crossfade Left
		value = game->getPlayer()->m_gpState.at(CF_LEFT_INDEX) * game->getPlayer()->m_gpMult.at(CF_LEFT_INDEX);
		dead = game->getPlayer()->m_gpDead.at(CF_LEFT_INDEX);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Crossfade left:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Crossfade left:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##05");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(CF_LEFT_INDEX);
			} else {
				editingAxisController(CF_LEFT_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Crossfade Left' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##left", &(game->getPlayer()->CROSS_L_CODE));
		} else {
			ImGui::InputInt("##left", &(game->getPlayer()->CF_LEFT_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##5", &(game->getPlayer()->m_gpMult.at(CF_LEFT_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##51", &(game->getPlayer()->m_gpDead.at(CF_LEFT_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		//Crossfade Right
		value = game->getPlayer()->m_gpState.at(CF_RIGHT_INDEX) * game->getPlayer()->m_gpMult.at(CF_RIGHT_INDEX);
		dead = game->getPlayer()->m_gpDead.at(CF_RIGHT_INDEX);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Crossfade right:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Crossfade right:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##06");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(CF_RIGHT_INDEX);
			} else {
				editingAxisController(CF_RIGHT_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Crossfade Right' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##right", &(game->getPlayer()->CROSS_R_CODE));
		} else {
			ImGui::InputInt("##right", &(game->getPlayer()->CF_RIGHT_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##6", &(game->getPlayer()->m_gpMult.at(CF_RIGHT_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##61", &(game->getPlayer()->m_gpDead.at(CF_RIGHT_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();
		ImGui::Columns();

		ImGui::Checkbox("Use Single Axis for Scratch", &(game->getPlayer()->m_useSingleScrAxis));
		ImGui::SameLine();
		ImGui::Checkbox("Treat Scratch Axes as digital inputs", &(game->getPlayer()->m_digitalScratch));
		ImGui::Columns(colnum, "third col", false);

		//Scratch Up
		value = game->getPlayer()->m_gpState.at(SCR_UP_INDEX) * game->getPlayer()->m_gpMult.at(SCR_UP_INDEX);
		dead = game->getPlayer()->m_gpDead.at(SCR_UP_INDEX);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Scratch up:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Scratch up:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##07");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(SCR_UP_INDEX);
			} else {
				editingAxisController(SCR_UP_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Scratch Up' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##up", &(game->getPlayer()->SCRATCH_UP));
		} else {
			ImGui::InputInt("##up", &(game->getPlayer()->SCR_UP_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##7", &(game->getPlayer()->m_gpMult.at(SCR_UP_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##71", &(game->getPlayer()->m_gpDead.at(SCR_UP_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		//Scratch Down
		value = game->getPlayer()->m_gpState.at(SCR_DOWN_INDEX) * game->getPlayer()->m_gpMult.at(SCR_DOWN_INDEX);
		dead = game->getPlayer()->m_gpDead.at(SCR_DOWN_INDEX);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Scratch down:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Scratch down:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##08");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(SCR_DOWN_INDEX);
			} else {
				editingAxisController(SCR_DOWN_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Scratch Down' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##down", &(game->getPlayer()->SCRATCH_DOWN));
		} else {
			ImGui::InputInt("##down", &(game->getPlayer()->SCR_DOWN_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##8", &(game->getPlayer()->m_gpMult.at(SCR_DOWN_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##81", &(game->getPlayer()->m_gpDead.at(SCR_DOWN_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		//Menu Up
		value = game->getPlayer()->m_gpState.at(MENU_UP) * game->getPlayer()->m_gpMult.at(MENU_UP);
		dead = game->getPlayer()->m_gpDead.at(MENU_UP);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Menu up:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Menu up:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##09");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(MENU_UP);
			} else {
				editingAxisController(MENU_UP);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu up' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##menuup", &(game->getPlayer()->UP_CODE));
		} else {
			ImGui::InputInt("##menuup", &(game->getPlayer()->UP_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##9", &(game->getPlayer()->m_gpMult.at(MENU_UP)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##91", &(game->getPlayer()->m_gpDead.at(MENU_UP)), -1.0, 1.0);
		ImGui::NextColumn();

		//Menu Down
		value = game->getPlayer()->m_gpState.at(MENU_DOWN) * game->getPlayer()->m_gpMult.at(MENU_UP);
		dead = game->getPlayer()->m_gpDead.at(MENU_DOWN);
		if (value > dead) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Menu down:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Menu down:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##100");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(MENU_DOWN);
			} else {
				editingAxisController(MENU_DOWN);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu down' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##menudown", &(game->getPlayer()->DOWN_CODE));
		} else {
			ImGui::InputInt("##menudown", &(game->getPlayer()->DOWN_GAMEPAD));
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##100", &(game->getPlayer()->m_gpMult.at(MENU_DOWN)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##101", &(game->getPlayer()->m_gpDead.at(MENU_DOWN)), -1.0, 1.0);
		ImGui::NextColumn();

		//Menu Select
		/*
		if (game->getPlayer()->m_useKeyboardInput) {
			value = game->getPlayer()->getKBMValues(m_window).at(*input.sk);
		} else {
			value = game->getPlayer()->getGamepadValues().at(*input.sg);
		}
		if (value > 0) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Menu Select:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Menu Select:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##092");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(MENU_SELECT);
			} else {
				editingAxisController(MENU_SELECT);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Select' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##meselect", input.sk);
		} else {
			ImGui::InputInt("##meselect", input.sg);
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::NextColumn();
		ImGui::NextColumn();

		//Menu Back
		if (game->getPlayer()->m_useKeyboardInput) {
			value = game->getPlayer()->getKBMValues(m_window).at(*input.bk);
		} else {
			value = game->getPlayer()->getGamepadValues().at(*input.bg);
		}
		if (value > 0) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Menu Back:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Menu Back:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##093");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(MENU_BACK);
			} else {
				editingAxisController(MENU_BACK);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Back' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##meback", input.bk);
		} else {
			ImGui::InputInt("##meback", input.bg);
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::NextColumn();
		ImGui::NextColumn();
		*/
		ImGui::Columns();
	}

	ImGui::Separator();
	ImGui::Text("Tip: you can hold CTRL and click on a slider to set the raw value. The values are not clamped");
	ImGui::Text("Tip #2: you can click on the numbers near 'Remap' buttons and write the axis id manually. For a reference on the axis, click 'Axis Viewer' ");
	if (ImGui::Button("Go Back to menu")) {
		m_shouldClose = true;
	}
	renderImGuiFrame();
}

void MenuRender::credits() {
	float fontsize = 48.0f;
	float y = 0.0;
	float x = 10.0f;

	drawText("Thanks to everyone in the Dj Hero Fan Server ", x, y, fontsize / 1000.0f);
	y += fontsize; //new line

	drawText("for supporting me and this game", x, y, fontsize / 1000.0f);
	y += fontsize; //new line
	y += fontsize; //new line

	drawText("A thank you to every programming youtuber", x, y, fontsize / 1000.0f);
	y += fontsize; //new line

	drawText("for giving me the passion to create games", x, y, fontsize / 1000.0f);
	y += fontsize; //new line
	y += fontsize; //new line

	drawText("And finally a thank you to you, player", x, y, fontsize / 1000.0f);
	y += fontsize; //new line
	drawText("You are the reason why I'm doing this", x, y, fontsize / 1000.0f);
	y += fontsize; //new line
	y += fontsize; //new line
	y += fontsize; //new line

	drawText("MatteoGodzilla <3", x, y, fontsize / 1000.0f);
	y += fontsize; //new line

	drawText("(now that you read the credits, DM me", x, y, fontsize / 1000.0f);
	y += fontsize; //new line

	drawText("on discord (@MatteoGodzilla#6709) with '#truedj')", x, y, fontsize / 1000.0f);
}

void MenuRender::result(Game* game) {
	std::vector<float> resultVector;
	std::vector<unsigned int> resultIndices;
	unsigned int resultVertexCount = 0;

	useOrthoProj();
	float y = 50.0f;
	float scale = 0.05f;

	int all = game->getGenerator()->getNotesTotal();
	int hit = game->getGenerator()->getNotesHit();
	int score = game->getPlayer()->getScore();
	int combo = game->getPlayer()->getHighCombo();
	bool brokeOnce = game->getPlayer()->getBrokeOnce();

	std::string firstSong = std::string(game->getGenerator()->getSongEntry().s1);
	std::string secondSong = std::string(game->getGenerator()->getSongEntry().s2);
	std::string completeName = firstSong;
	if (!secondSong.empty()) {
		completeName += " vs ";
		completeName += secondSong;
	}
	std::string hitString = std::string("Number of notes hit:") + std::to_string(hit);
	std::string totalString = std::string("Number of total notes:") + std::to_string(all);
	std::string scoreString = std::string("Score:") + std::to_string(score);
	std::string comboString = std::string("Max Combo:") + std::to_string(combo);

	drawText(completeName, 10.0f, y, scale);
	y += scale * 1000.0f;
	drawText("Result:", 10.0f, y, scale);
	float x = getTextWidth("Result:", scale);
	if (game->getPlayer()->m_botEnabled) {
		drawText("!BOT ACTIVE!", x + 30.0f, y, scale);
	} else {
		float stars = (float)game->getPlayer()->getScore() / (float)game->getGenerator()->m_baseScore;

		Vertex topLeft = Vertex({x + 30.0f + scale * 0000.0f, y, 0.0}, {221.0f / 300.0f, 1.0f});
		Vertex bottomLeft = Vertex({x + 30.0f + scale * 0000.0f, y + scale, 0.0}, {221.0f / 300.0f, 0.0f});
		Vertex bottomRight = Vertex({x + 30.0f + scale * 1000.0f, y + scale, 0.0}, {1.0f, 1.0f});
		Vertex topRight = Vertex({x + 30.0f + scale * 1000.0f, y, 0.0}, {1.0f, 1.0f});

		if (stars >= 0.1) {
			pushFourVertices(resultVector, topLeft, bottomLeft, bottomRight, topRight);
			pushFourIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.2) {
			topLeft.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			bottomLeft.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			bottomRight.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			topRight.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);

			pushFourVertices(resultVector, topLeft, bottomLeft, bottomRight, topRight);
			pushFourIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.3) {
			topLeft.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			bottomLeft.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			bottomRight.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			topRight.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);

			pushFourVertices(resultVector, topLeft, bottomLeft, bottomRight, topRight);
			pushFourIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.4) {
			topLeft.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			bottomLeft.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			bottomRight.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			topRight.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);

			pushFourVertices(resultVector, topLeft, bottomLeft, bottomRight, topRight);
			pushFourIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.5) {
			topLeft.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			bottomLeft.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			bottomRight.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);
			topRight.pos += glm::vec3(scale * 1000.0f, 0.0, 0.0);

			pushFourVertices(resultVector, topLeft, bottomLeft, bottomRight, topRight);
			pushFourIndices(resultIndices, resultVertexCount);
		}
		if (!resultVector.empty()) {
			useOrthoProj();
			renderTexture(resultVector, resultIndices, m_pgBarFrame);
		}
	}
	y += scale * 1000.0f;
	drawText(hitString, 10.0f, y, scale);
	y += scale * 1000.0f;
	drawText(totalString, 10.0f, y, scale);
	y += scale * 1000.0f;
	drawText(comboString, 10.0f, y, scale);
	y += scale * 1000.0f;
	drawText(scoreString, 10.0f, y, scale);
	y += scale * 1000.0f;

	if (all > 0) {
		std::string percent = std::string("Percent:") + std::to_string((float)hit / (float)all * 100.0f) + std::string("%");
		drawText(percent, 10.0f, y, scale);
	}
	if (!brokeOnce) {
		std::string t = "Wow! a Full Combo";
		float x = 1270.0f - getTextWidth(t, scale);
		drawText(t, x, 100.0f, scale);
	}

	std::string exit("Press menu Back to Exit");
	drawText(exit, 10.0f, 710 - getTextHeight(exit, scale), scale);
}

void MenuRender::splashArt() {
	float textScale = 0.02f;
	std::string firstLine = std::string("*This is ") + VERSION + std::string(". There are still some bugs left.*");
	drawText(firstLine, 10.0, 10.0, textScale);
	drawText("*I hope you like the new Menu*", 10.0, 30.0, textScale);
	drawText("*Have Fun! :)*", 10.0, 50.0, textScale);

	std::string discord = "For any questions, ask on the Dj Hero Discord";
	std::string discord2 = "https://discord.gg/HZ82gKR";
	//std::string donation = "If you want to support the development, you can donate at";
	//std::string donation2 = "paypal.me/MatteoGodzilla";
	drawText(discord, 1270.0f - getTextWidth(discord, textScale), 10.0f, textScale);
	drawText(discord2, 1270.0f - getTextWidth(discord2, textScale), 30.0f, textScale);
	//drawText(donation, 1270.0f - getTextWidth(donation, textScale), 50.0f, textScale);
	//drawText(donation2, 1270.0f - getTextWidth(donation2, textScale), 70.0f, textScale);

	float hintTextScale = 0.03f;
	std::string controls = std::string("Move with Up/Down Arrow. Select with Green (Enter). Go back with Red (Escape)");
	std::string remap = std::string("Press spacebar to enter Remapping screen");
	drawText(controls, (1280.0f - getTextWidth(controls, hintTextScale)) / 2.0f, 720.0f - getTextHeight(controls, hintTextScale) - getTextHeight(remap, hintTextScale), hintTextScale);
	drawText(remap, (1280.0f - getTextWidth(remap, hintTextScale)) / 2.0f, 720.0f - getTextHeight(remap, hintTextScale), hintTextScale);
}

void MenuRender::calibration(Game* game, double dt) {
	useOrthoProj();

	ImGuiBackendFlags flags = 0;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	startImGuiFrame("Calibration Window", flags);
	ImGui::SetWindowPos({0.0f, 0.0f});

	int width;
	int height;
	glfwGetFramebufferSize(m_window, &width, &height);
	ImGui::SetWindowSize({(float)width, (float)height});

	if (ImGui::Button("Calibrate")) {
		m_isCalibrating = true;
		m_cbPlayingTime = 0.0;
		m_latencyHits.clear();
		SongEntry temp;
		temp.path = "res";
		temp.streams = 1;
		game->getAudio()->init();
		game->getAudio()->load(temp);
		game->getAudio()->play();
	}
	if (game->getAudio()->isPlaying()) {
		m_cbPlayingTime += dt;
	} else {
		game->getAudio()->destroy();
		m_isCalibrating = false;
		m_cbPlayingTime = 0.0;

		if (!m_latencyHits.empty()) {
			double sum = 0.0;
			for (size_t i = 0; i < m_latencyHits.size(); ++i) {
				sum += m_latencyHits.at(i) - (2.0 + 0.5 * i);
			}
			NormalLog << (float)(sum / m_latencyHits.size());
			game->m_audioLatency = (float)(sum / m_latencyHits.size());
			m_latencyHits.clear();
		}
	}
	ImGui::SameLine();
	ImGui::SliderFloat("Current audio latency (in ms.)", &(game->m_audioLatency), 0.0f, 2.0f);
	ImGui::Text("When calibrating, you are going to hear 4 bass hits, then 8 snare hits and then a ding.");
	ImGui::Text("Press the Spacebar when you hear the snare hits");
	ImGui::Text("(Note: there's latency in the audio engine. Even with wired headphones it will be above 0)");

	ImGui::Separator();

	for (int i = 0; i < 8; ++i) {
		if (m_cbPlayingTime > 2.0 + 0.5 * i) {
			ImGui::SameLine();
			ImGui::Image((ImTextureID*)m_calibrationTex, {100.0f, 100.0f}, {0.5f, 0.5f}, {1.0f, 1.0f});
		} else {
			ImGui::SameLine();
			ImGui::Image((ImTextureID*)m_calibrationTex, {100.0f, 100.0f}, {0.0f, 0.0f}, {0.5f, 0.5f});
		}
	}
	ImGui::Separator();
	//if (ImGui::Button("Tap Me", { 300.0f,250.0f })) {
	//	m_latencyHits.push_back(m_cbPlayingTime);
	//}
	if (ImGui::IsKeyPressed(32)) { //space key
		m_latencyHits.push_back(m_cbPlayingTime);
	}
	if (ImGui::Button("Go back to main menu")) {
		m_shouldClose = true;
	}

	renderImGuiFrame();
}

void MenuRender::setDeckSpeed(Game* game) {
	useOrthoProj();

	ImGuiBackendFlags flags = 0;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;

	startImGuiFrame("Set Deck Speed", flags);
	ImGui::Text("Set the deck speed using the slider below.");
	ImGui::Text("The value represents the visible time of a note in the highway");
	ImGui::Text("(i.e. 1.0 is one second in the highway)");
	ImGui::Text("A larger value means slower speed, smaller value means faster");
	ImGui::SliderFloat("Speed", &(game->m_deckSpeed), 0.5f, 3.0f);
	if (ImGui::Button("Close")) {
		m_shouldClose = true;
	}

	renderImGuiFrame();
}

void MenuRender::setLaneColors(Game* game) {
	useOrthoProj();

	ImGuiBackendFlags flags = 0;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;

	glm::vec4& greenActiveVector = game->getGameRender()->m_greenLaneActiveColor;
	glm::vec4& greenInactiveVector = game->getGameRender()->m_greenLaneInactiveColor;
	glm::vec4& blueActiveVector = game->getGameRender()->m_blueLaneActiveColor;
	glm::vec4& blueInactiveVector = game->getGameRender()->m_blueLaneInactiveColor;
	glm::vec4& redActiveVector = game->getGameRender()->m_redLaneActiveColor;
	glm::vec4& euphoriaActiveVector = game->getGameRender()->m_euphoriaLaneColor;
	glm::vec4& greenScratchVector = game->getGameRender()->m_greenScratchColor;
	glm::vec4& blueScratchVector = game->getGameRender()->m_blueScratchColor;
	glm::vec4& euphoriaZoneVector = game->getGameRender()->m_euphoriaZoneColor;

	static std::array<float, 4> greenActiveArray = {greenActiveVector.r, greenActiveVector.g, greenActiveVector.b, greenActiveVector.a};
	static std::array<float, 4> greenInactiveArray = {greenInactiveVector.r, greenInactiveVector.g, greenInactiveVector.b, greenInactiveVector.a};
	static std::array<float, 4> blueActiveArray = {blueActiveVector.r, blueActiveVector.g, blueActiveVector.b, blueActiveVector.a};
	static std::array<float, 4> blueInactiveArray = {blueInactiveVector.r, blueInactiveVector.g, blueInactiveVector.b, blueInactiveVector.a};
	static std::array<float, 4> redActiveArray = {redActiveVector.r, redActiveVector.g, redActiveVector.b, redActiveVector.a};
	static std::array<float, 4> euphoriaActiveArray = {euphoriaActiveVector.r, euphoriaActiveVector.g, euphoriaActiveVector.b, euphoriaActiveVector.a};
	static std::array<float, 4> greenScratchArray = {greenScratchVector.r, greenScratchVector.g, greenScratchVector.b, greenScratchVector.a};
	static std::array<float, 4> blueScratchArray = {blueScratchVector.r, blueScratchVector.g, blueScratchVector.b, blueScratchVector.a};
	static std::array<float, 4> euphoriaZoneArray = {euphoriaZoneVector.r, euphoriaZoneVector.g, euphoriaZoneVector.b, euphoriaZoneVector.a};

	startImGuiFrame("Set Deck Speed", flags);
	ImGui::Text("Here you can edit for each lane what color it is");
	ImGui::Text("For green and blue lane you can also pick active and inactive colors");
	ImGui::ColorEdit4("Green Lane when active", greenActiveArray.data());
	ImGui::ColorEdit4("Green Lane when inactive", greenInactiveArray.data());
	ImGui::ColorEdit4("Green Scratch Zone color", greenScratchArray.data());
	ImGui::Text(" ");
	ImGui::ColorEdit4("Blue Lane when active", blueActiveArray.data());
	ImGui::ColorEdit4("Blue Lane when inactive", blueInactiveArray.data());
	ImGui::ColorEdit4("Blue Scratch Zone color", blueScratchArray.data());
	ImGui::Text(" ");
	ImGui::ColorEdit4("Red Lane color", redActiveArray.data());
	ImGui::ColorEdit4("Euphoria Lane color", euphoriaActiveArray.data());
	ImGui::ColorEdit4("Euphoria Zone color", euphoriaZoneArray.data());
	if (ImGui::Button("Close popup")) {
		m_shouldClose = true;
	}
	renderImGuiFrame();

	game->getGameRender()->m_greenLaneActiveColor = glm::vec4(greenActiveArray[0], greenActiveArray[1], greenActiveArray[2], greenActiveArray[3]);
	game->getGameRender()->m_greenLaneInactiveColor = glm::vec4(greenInactiveArray[0], greenInactiveArray[1], greenInactiveArray[2], greenInactiveArray[3]);
	game->getGameRender()->m_blueLaneActiveColor = glm::vec4(blueActiveArray[0], blueActiveArray[1], blueActiveArray[2], blueActiveArray[3]);
	game->getGameRender()->m_blueLaneInactiveColor = glm::vec4(blueInactiveArray[0], blueInactiveArray[1], blueInactiveArray[2], blueInactiveArray[3]);
	game->getGameRender()->m_redLaneActiveColor = glm::vec4(redActiveArray[0], redActiveArray[1], redActiveArray[2], redActiveArray[3]);
	game->getGameRender()->m_euphoriaLaneColor = glm::vec4(euphoriaActiveArray[0], euphoriaActiveArray[1], euphoriaActiveArray[2], euphoriaActiveArray[3]);
	game->getGameRender()->m_greenScratchColor = glm::vec4(greenScratchArray[0], greenScratchArray[1], greenScratchArray[2], greenScratchArray[3]);
	game->getGameRender()->m_blueScratchColor = glm::vec4(blueScratchArray[0], blueScratchArray[1], blueScratchArray[2], blueScratchArray[3]);
	game->getGameRender()->m_euphoriaZoneColor = glm::vec4(euphoriaZoneArray[0], euphoriaZoneArray[1], euphoriaZoneArray[2], euphoriaZoneArray[3]);
}

void MenuRender::setPollRate(Game* game) {
	useOrthoProj();

	ImGuiBackendFlags flags = 0;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;

	startImGuiFrame("Set Poll Rate", flags);
	ImGui::Text("Set the poll rate using the slider below.");
	ImGui::Text("The value represents how many times the inputs are read in Hz.");
	ImGui::Text("For a smooth experience it is recommended to have it at least x2 the framerate");
	ImGui::Text("Note: increasing too much WILL increase the cpu usage A LOT");
	ImGui::Text("Note: you have been warned");
	ImGui::SliderInt("HZ", &(game->m_inputThreadPollRate), 128, 1000);
	if (ImGui::Button("Close")) {
		m_shouldClose = true;
	}

	renderImGuiFrame();
}

void MenuRender::editingAxisController(int axis) {
	m_editingKey = false;
	m_editingAxis = true;
	m_ActionToChange = axis;
}

void MenuRender::editingAxisKBAM(int axis) {
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_editingKey = true;
	m_editingAxis = false;
	m_ActionToChange = axis;
}

void MenuRender::doneEditing() {
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_editingKey = false;
	m_editingAxis = false;
	m_ActionToChange = -1;
}

GLFWwindow* MenuRender::getWindowPtr() {
	return m_window;
}
