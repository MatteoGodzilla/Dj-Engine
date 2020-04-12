#include "GameRender.h"

GameRender::GameRender() {
	//ctor
}

void GameRender::init(GLFWwindow* w) {
	Rendr::init(w);
	m_animManager.init(-1000.0);

	m_window = w;
	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW INIT ERROR" << std::endl;
		return;
	}
	std::cout << "GameRender Init: " << glGetString(GL_VERSION) << std::endl;

	//texture load
	{
		loadTexture("res/highway.png", &m_highwayTexture);
		loadTexture("res/objects.png", &m_objTexture);
		loadTexture("res/meters.png", &m_metersTexture);
		loadTexture("res/clickerAnim.png", &m_clickerAnimation);
		loadTexture("res/pgBar-frame.png", &m_pgBarFrame);
		loadTexture("res/pgBar-inside.png", &m_pgBarInside);
	}
}

void GameRender::highway(double time) {
	std::vector<float> highwayVector;
	std::vector<unsigned int> highwayIndices;
	unsigned int highwayVertexCount = 0;

	glm::vec2 center = {m_radius, 0.0};

	//value depends from max radius, time, notevisibletime and the highway texture
	float offsetFactor = 1 / m_noteVisibleTime * time;

	glm::vec2 beforeOuter = getCirclePoint((double)m_radius + m_deltaAngle, 0.0);
	glm::vec2 greenBeforeInner = getCirclePoint((double)m_radius - m_deltaAngle, 0.0);

	for (float angle = 0.0; angle < m_maxAngle; angle += m_deltaAngle) {
		glm::vec2 outer = getCirclePoint((double)m_radius + m_deltaRadius, (double)angle);
		glm::vec2 inner = getCirclePoint((double)m_radius - m_deltaRadius, (double)angle);

		pushVertexTexture(highwayVector, -beforeOuter.x + center.x, 0.0, -beforeOuter.y - center.y, 0.0, (angle / m_maxAngle) + offsetFactor);
		pushVertexTexture(highwayVector, -greenBeforeInner.x + center.x, 0.0, -greenBeforeInner.y - center.y, 1.0, (angle / m_maxAngle) + offsetFactor);
		pushVertexTexture(highwayVector, -inner.x + center.x, 0.0, -inner.y - center.y, 1.0, (angle / m_maxAngle) + offsetFactor);
		pushVertexTexture(highwayVector, -outer.x + center.x, 0.0, -outer.y - center.y, 0.0, (angle / m_maxAngle) + offsetFactor);
		pushRectangleIndices(highwayIndices, highwayVertexCount);

		beforeOuter = outer;
		greenBeforeInner = inner;
	}

	usePersProj();
	renderTexture(highwayVector, highwayIndices, m_highwayTexture);
}

void GameRender::clicker() {
	//difference in size between pressed and not
	float clickedOffset = 0.05f;

	float plane = 0.0f;

	//vertices data
	std::vector<float> clickerVector = {};
	std::vector<unsigned int> clickerIndices = {};
	unsigned int clickerVertexCount = 0;

	//left tray object
	pushVertexTexture(clickerVector, -1.25f, plane, -0.5f, 1.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, -1.25f, plane, 0.0f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.25f, plane, 0.0f, 1320.0f / 1760.0f, 1280.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.25f, plane, -0.5f, 1.0f, 1280.0f / 1640.0f);
	pushRectangleIndices(clickerIndices, clickerVertexCount);

	//right tray object
	pushVertexTexture(clickerVector, 0.25f, plane, -0.5f, 1.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, 0.25f, plane, 0.0f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, 1.25f, plane, 0.0f, 1320.0f / 1760.0f, 1280.0f / 1640.0f);
	pushVertexTexture(clickerVector, 1.25f, plane, -0.5f, 1.0f, 1280.0f / 1640.0f);
	pushRectangleIndices(clickerIndices, clickerVertexCount);

	if (m_red) {
		//red pressed clicker
		pushVertexTexture(clickerVector, -0.25f + clickedOffset, plane, -0.5f + clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, -0.25f + clickedOffset, plane, 0.0f - clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.25f - clickedOffset, plane, 0.0f - clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.25f - clickedOffset, plane, -0.5f + clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
	} else {
		//red normal clicker
		pushVertexTexture(clickerVector, -0.25f, plane, -0.5f, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, -0.25f, plane, 0.0f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.25f, plane, 0.0f, 880.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.25f, plane, -0.5f, 880.0f / 1760.0f, 400.0f / 1640.0f);
	}
	pushRectangleIndices(clickerIndices, clickerVertexCount);

	float greenLeft = 0.0;
	float greenRight = 0.0;
	float blueLeft = 0.0;
	float blueRight = 0.0;

	if (m_playerCross == 0) {
		greenLeft = -1.25;
		greenRight = -0.75;
		blueLeft = 0.25;
		blueRight = 0.75;

		Animation greenToCenter = m_animManager.getAnimById(AN_CROSS_GREEN_TO_LEFT);
		Animation blueToCenter = m_animManager.getAnimById(AN_CROSS_BLUE_TO_CENTER);

		if (greenToCenter.isEnabled()) {
			float d = greenToCenter.getPercent() * 0.5f;
			greenLeft = -0.75 - d;
			greenRight = -0.25 - d;
		}

		if (blueToCenter.isEnabled()) {
			float d = blueToCenter.getPercent() * -0.5f;
			blueLeft = 0.75 + d;
			blueRight = 1.25 + d;
		}

	} else if (m_playerCross == 1) {
		greenLeft = -0.75;
		greenRight = -0.25;
		blueLeft = 0.25;
		blueRight = 0.75;

		Animation greenToCenter = m_animManager.getAnimById(AN_CROSS_GREEN_TO_CENTER);
		Animation blueToCenter = m_animManager.getAnimById(AN_CROSS_BLUE_TO_CENTER);

		if (greenToCenter.isEnabled()) {
			float d = greenToCenter.getPercent() * 0.5f;
			greenLeft = -1.25 + d;
			greenRight = -0.75 + d;
		}

		if (blueToCenter.isEnabled()) {
			float d = blueToCenter.getPercent() * -0.5f;
			blueLeft = 0.75 + d;
			blueRight = 1.25 + d;
		}

	} else {
		greenLeft = -0.75;
		greenRight = -0.25;
		blueLeft = 0.75;
		blueRight = 1.25;

		Animation greenToCenter = m_animManager.getAnimById(AN_CROSS_GREEN_TO_CENTER);
		Animation blueToCenter = m_animManager.getAnimById(AN_CROSS_BLUE_TO_RIGHT);

		if (greenToCenter.isEnabled()) {
			float d = greenToCenter.getPercent() * 0.5f;
			greenLeft = -1.25 + d;
			greenRight = -0.75 + d;
		}

		if (blueToCenter.isEnabled()) {
			float d = blueToCenter.getPercent() * 0.5f;
			blueLeft = 0.25 + d;
			blueRight = 0.75 + d;
		}
	}

	if (m_green) {
		pushVertexTexture(clickerVector, greenLeft + clickedOffset, plane, -0.5 + clickedOffset, 0.0, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, greenLeft + clickedOffset, plane, 0.0 - clickedOffset, 0.0, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, greenRight - clickedOffset, plane, 0.0 - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, greenRight - clickedOffset, plane, -0.5 + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	} else {
		pushVertexTexture(clickerVector, greenLeft, plane, -0.5, 0.0, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, greenLeft, plane, 0.0, 0.0, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, greenRight, plane, 0.0, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, greenRight, plane, -0.5, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	if (m_blue) {
		pushVertexTexture(clickerVector, blueLeft + clickedOffset, plane, -0.5 + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, blueLeft + clickedOffset, plane, 0.0 - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, blueRight - clickedOffset, plane, 0.0 - clickedOffset, 1320.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, blueRight - clickedOffset, plane, -0.5 + clickedOffset, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	} else {
		pushVertexTexture(clickerVector, blueLeft, plane, -0.5, 880.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, blueLeft, plane, 0.0, 880.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, blueRight, plane, 0.0, 1320.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, blueRight, plane, -0.5, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	usePersProj();
	renderTexture(clickerVector, clickerIndices, m_objTexture);
}

void GameRender::notes(double time, std::vector<Note>& v, std::vector<Note>& cross) {
	/*
	float plane = 0.0;

	//vertices data
	std::vector<float> noteVector = {};
	std::vector<unsigned int> noteIndices = {};
	unsigned int noteVertexCount = 0;

	//loop for every note inside vector
	for (size_t i = 0; i < v.size(); i++) {
		double milli = v.at(i).getMilli();
		double hitWindow = v.at(i).hitWindow;
		if (time + m_noteVisibleTime > milli) {
			//if the note is inside the visible highway

			//calculate 'height' of note
			double percent = (v.at(i).getMilli() - time) / m_noteVisibleTime;
			float z = 3.75f - (3.75f * (float)percent);

			int type = v.at(i).getType();

			//texture coordinates (bottom left cause OpenGl)
			float s = 0.0;
			float t = 0.0;

			if (type == TAP_R && !v.at(i).getTouched()) {
				if (!v.at(i).getDead()) {
					//change texture if euphoria is active
					if (m_renderEuActive) {
						s = 1200.0f / 1760.0f;
						t = 0.0f;
					}
					else {
						s = 400.0f / 1760.0f;
						t = 0.0;
					}

					//actual note vertices
					pushVertexTexture(noteVector, -0.15f, plane, z - 0.15f, s, t + 400.0f / 1760.0f);
					pushVertexTexture(noteVector, -0.15f, plane, z + 0.15f, s, t);
					pushVertexTexture(noteVector, 0.15f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
					pushVertexTexture(noteVector, 0.15f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1760.0f);
					pushRectangleIndices(noteIndices, noteVertexCount);
				}
			}
			else if (type == TAP_G && !v.at(i).getTouched()) {
				if (!v.at(i).getDead()) {
					//change texture if euphoria is active
					if (m_renderEuActive) {
						s = 1200.0f / 1760.0f;
						t = 0.0f;
					}
					else {
						s = 0.0f;
						t = 0.0f;
					}
					//if the highway at the note's time is on the left
					if (v.at(i).getLanMod() == 0) {
						pushVertexTexture(noteVector, -0.85f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
						pushVertexTexture(noteVector, -0.85f, plane, z + 0.15f, s, t);
						pushVertexTexture(noteVector, -0.55f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
						pushVertexTexture(noteVector, -0.55f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
					}
					else {
						pushVertexTexture(noteVector, -0.5f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
						pushVertexTexture(noteVector, -0.5f, plane, z + 0.15f, s, t);
						pushVertexTexture(noteVector, -0.2f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
						pushVertexTexture(noteVector, -0.2f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
					}
					pushRectangleIndices(noteIndices, noteVertexCount);
				}
			}
			else if (type == SCR_G_UP && !v.at(i).getTouched()) {
				s = 400.0f / 1760.0f;
				t = 840.0f / 1640.0f;

				//if the highway at the note's time is on the left
				if (v.at(i).getLanMod() == 0) {
					pushVertexTexture(noteVector, -0.85f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
					pushVertexTexture(noteVector, -0.85f, plane, z + 0.15f, s, t);
					pushVertexTexture(noteVector, -0.55f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
					pushVertexTexture(noteVector, -0.55f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
				}
				else {
					pushVertexTexture(noteVector, -0.5f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
					pushVertexTexture(noteVector, -0.5f, plane, z + 0.15f, s, t);
					pushVertexTexture(noteVector, -0.2f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
					pushVertexTexture(noteVector, -0.2f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
				}
				pushRectangleIndices(noteIndices, noteVertexCount);
			}
			else if (type == SCR_G_DOWN && !v.at(i).getTouched()) {
				s = 800.0f / 1760.0f;
				t = 840.0f / 1640.0f;

				//if the highway at the note's time is on the left
				if (v.at(i).getLanMod() == 0) {
					pushVertexTexture(noteVector, -0.85f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
					pushVertexTexture(noteVector, -0.85f, plane, z + 0.15f, s, t);
					pushVertexTexture(noteVector, -0.55f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
					pushVertexTexture(noteVector, -0.55f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
				}
				else {
					pushVertexTexture(noteVector, -0.5f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
					pushVertexTexture(noteVector, -0.5f, plane, z + 0.15f, s, t);
					pushVertexTexture(noteVector, -0.2f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
					pushVertexTexture(noteVector, -0.2f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
				}
				pushRectangleIndices(noteIndices, noteVertexCount);
			}
			else if (type == SCR_G_ANY) {
				s = 0.0f;
				t = 840.0f / 1640.0f;

				double length = 0.30;

				std::vector<Note> changes = getCrossInsideNote(v.at(i), cross);

				int segments = changes.size() / 2;

				for (int i = 0; i < segments; ++i) {
					Note start = changes.at(i * 2);
					Note end = changes.at(i * 2 + 1);

					double startDt = start.getMilli() - time;
					double startZ = 3.75 - (3.75 * (float)startDt);
					double endDt = end.getMilli() - time;
					double endZ = 3.75 - (3.75 * (float)endDt);

					int numOfSprites = (int)floor((startZ - endZ) / length);

					for (int j = 0; j < numOfSprites; ++j) {
						float spriteZ = (float)(startZ - 0.15 - length * j);
						if (spriteZ >= 0.0f && spriteZ <= 3.75f) {
							if (start.getType() == CROSS_G) {
								pushVertexTexture(noteVector, -0.85f, plane, spriteZ - 0.15f, s, t + 400.0f / 1640.0f);
								pushVertexTexture(noteVector, -0.85f, plane, spriteZ + 0.15f, s, t);
								pushVertexTexture(noteVector, -0.55f, plane, spriteZ + 0.15f, s + 400.0f / 1760.0f, t);
								pushVertexTexture(noteVector, -0.55f, plane, spriteZ - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
							}
							else {
								pushVertexTexture(noteVector, -0.5f, plane, spriteZ - 0.15f, s, t + 400.0f / 1640.0f);
								pushVertexTexture(noteVector, -0.5f, plane, spriteZ + 0.15f, s, t);
								pushVertexTexture(noteVector, -0.2f, plane, spriteZ + 0.15f, s + 400.0f / 1760.0f, t);
								pushVertexTexture(noteVector, -0.2f, plane, spriteZ - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
							}
							pushRectangleIndices(noteIndices, noteVertexCount);
						}
					}
				}
			}
			else if (type == TAP_B && !v.at(i).getTouched()) {
				if (!v.at(i).getDead()) {
					//change texture if euphoria is active
					if (m_renderEuActive) {
						s = 1200.0f / 1760.0f;
						t = 0.0f;
					}
					else {
						s = 800.0f / 1760.0f;
						t = 0.0f;
					}

					//if the highway at the note's time is on the left
					if (v.at(i).getLanMod() == 2) {
						pushVertexTexture(noteVector, 0.55f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
						pushVertexTexture(noteVector, 0.55f, plane, z + 0.15f, s, t);
						pushVertexTexture(noteVector, 0.85f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
						pushVertexTexture(noteVector, 0.85f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
					}
					else {
						pushVertexTexture(noteVector, 0.2f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
						pushVertexTexture(noteVector, 0.2f, plane, z + 0.15f, s, t);
						pushVertexTexture(noteVector, 0.5f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
						pushVertexTexture(noteVector, 0.5f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
					}
					pushRectangleIndices(noteIndices, noteVertexCount);
				}
			}
			else if (type == SCR_B_UP && !v.at(i).getTouched()) {
				s = 400.0f / 1760.0f;
				t = 840.0f / 1640.0f;

				//if the highway at the note's time is on the right
				if (v.at(i).getLanMod() == 2) {
					pushVertexTexture(noteVector, 0.55f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
					pushVertexTexture(noteVector, 0.55f, plane, z + 0.15f, s, t);
					pushVertexTexture(noteVector, 0.85f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
					pushVertexTexture(noteVector, 0.85f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
				}
				else {
					pushVertexTexture(noteVector, 0.2f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
					pushVertexTexture(noteVector, 0.2f, plane, z + 0.15f, s, t);
					pushVertexTexture(noteVector, 0.5f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
					pushVertexTexture(noteVector, 0.5f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
				}
				pushRectangleIndices(noteIndices, noteVertexCount);
			}
			else if (type == SCR_B_DOWN && !v.at(i).getTouched()) {
				s = 800.0f / 1760.0f;
				t = 840.0f / 1640.0f;

				//if the highway at the note's time is on the right
				if (v.at(i).getLanMod() == 2) {
					pushVertexTexture(noteVector, 0.55f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
					pushVertexTexture(noteVector, 0.55f, plane, z + 0.15f, s, t);
					pushVertexTexture(noteVector, 0.85f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
					pushVertexTexture(noteVector, 0.85f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
				}
				else {
					pushVertexTexture(noteVector, 0.2f, plane, z - 0.15f, s, t + 400.0f / 1640.0f);
					pushVertexTexture(noteVector, 0.2f, plane, z + 0.15f, s, t);
					pushVertexTexture(noteVector, 0.5f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
					pushVertexTexture(noteVector, 0.5f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
				}
				pushRectangleIndices(noteIndices, noteVertexCount);
			}
			else if (type == SCR_B_ANY) {
				s = 0.0f;
				t = 840.0f / 1640.0f;

				double length = 0.30;

				std::vector<Note> changes = getCrossInsideNote(v.at(i), cross);

				int segments = changes.size() / 2;

				for (int i = 0; i < segments; ++i) {
					Note start = changes.at(i * 2);
					Note end = changes.at(i * 2 + 1);

					double startDt = start.getMilli() - time;
					double startZ = 3.75 - (3.75 * startDt);
					double endDt = end.getMilli() - time;
					double endZ = 3.75 - (3.75 * endDt);

					int numOfSprites = (int)floor((startZ - endZ) / length);

					for (int j = 0; j < numOfSprites; ++j) {
						float spriteZ = (float)(startZ - 0.15 - length * j);
						if (spriteZ >= 0.0f && spriteZ <= 3.75f) {
							if (start.getType() == CROSS_B) {
								pushVertexTexture(noteVector, 0.55f, plane, spriteZ - 0.15f, s, t + 400.0f / 1640.0f);
								pushVertexTexture(noteVector, 0.55f, plane, spriteZ + 0.15f, s, t);
								pushVertexTexture(noteVector, 0.85f, plane, spriteZ + 0.15f, s + 400.0f / 1760.0f, t);
								pushVertexTexture(noteVector, 0.85f, plane, spriteZ - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
							}
							else {
								pushVertexTexture(noteVector, 0.2f, plane, spriteZ - 0.15f, s, t + 400.0f / 1640.0f);
								pushVertexTexture(noteVector, 0.2f, plane, spriteZ + 0.15f, s, t);
								pushVertexTexture(noteVector, 0.5f, plane, spriteZ + 0.15f, s + 400.0f / 1760.0f, t);
								pushVertexTexture(noteVector, 0.5f, plane, spriteZ - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1640.0f);
							}
							pushRectangleIndices(noteIndices, noteVertexCount);
						}
					}
				}
			}
			if (type == CF_SPIKE_G && !v.at(i).getTouched()) {
				if (v.at(i).getLanMod() >= 1) {
					s = 1200.0f / 1740.0f;
					t = 1460.0f / 1640.0f;

					pushVertexTexture(noteVector, -0.7f, plane, z - 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, -0.7f, plane, z + 0.1f, s, t);
					pushVertexTexture(noteVector, -0.35f, plane, z + 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(noteVector, -0.35f, plane, z - 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushRectangleIndices(noteIndices, noteVertexCount);
				}if (v.at(i).getLanMod() == 2) {
					s = 1200.0f / 1740.0f;
					t = 1280.0f / 1640.0f;

					pushVertexTexture(noteVector, 0.35f, plane, z - 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(noteVector, 0.35f, plane, z + 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, 0.7f, plane, z + 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, 0.7f, plane, z - 0.1f, s, t);
					pushRectangleIndices(noteIndices, noteVertexCount);

				}
			}
			else if (type == CF_SPIKE_B && !v.at(i).getTouched()) {
				if (v.at(i).getLanMod() <= 1) {
					s = 1200.0f / 1740.0f;
					t = 1280.0f / 1640.0f;

					pushVertexTexture(noteVector, 0.35f, plane, z - 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, 0.35f, plane, z + 0.1f, s, t);
					pushVertexTexture(noteVector, 0.7f, plane, z + 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(noteVector, 0.7f, plane, z - 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushRectangleIndices(noteIndices, noteVertexCount);
				}
				if (v.at(i).getLanMod() == 0) {
					s = 1200.0f / 1740.0f;
					t = 1460.0f / 1640.0f;

					pushVertexTexture(noteVector, -0.7f, plane, z - 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(noteVector, -0.7f, plane, z + 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, -0.35f, plane, z + 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, -0.35f, plane, z - 0.1f, s, t);
					pushRectangleIndices(noteIndices, noteVertexCount);
				}
			}
			else if (type == CF_SPIKE_C && !v.at(i).getTouched()) {
				if (v.at(i).getLanMod() == 0) {
					s = 1200.0f / 1740.0f;
					t = 1460.0f / 1640.0f;

					pushVertexTexture(noteVector, -0.7f, plane, z - 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(noteVector, -0.7f, plane, z + 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, -0.35f, plane, z + 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, -0.35f, plane, z - 0.1f, s, t);
					pushRectangleIndices(noteIndices, noteVertexCount);

				}
				else if (v.at(i).getLanMod() == 2) {
					s = 1200.0f / 1740.0f;
					t = 1280.0f / 1640.0f;

					pushVertexTexture(noteVector, 0.35f, plane, z - 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(noteVector, 0.35f, plane, z + 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, 0.7f, plane, z + 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(noteVector, 0.7f, plane, z - 0.1f, s, t);
					pushRectangleIndices(noteIndices, noteVertexCount);

				}
			}

		}
		else if (milli >= time + m_noteVisibleTime && v.at(i).getLanMod() == -1) {
			//if the note is outside the visible area, update lane position
			int position = 1;
			for (size_t j = 0; j < cross.size(); ++j) {
				if (cross.at(j).getMilli() <= milli) {
					if (cross.at(j).getType() == CROSS_G) position = 0;
					else if (cross.at(j).getType() == CROSS_C) position = 1;
					else if (cross.at(j).getType() == CROSS_B) position = 2;
				}
			}
			v.at(i).setLanMod(position);
		}
	}
	usePersProj();
	renderTexture(noteVector, noteIndices, m_objTexture);
	*/
}

void GameRender::lanes(double time, std::vector<Note>& v, std::vector<Note>& cross) {
	float plane = 0.0;

	//vertices data for the lanes

	std::vector<float> lanesVector = {};
	std::vector<unsigned int> lanesIndices = {};
	unsigned int lanesVertexCount = 0;

	std::vector<float> greenColor = {0.0, 1.0, 0.0, 1.0};
	std::vector<float> redColor = {1.0, 0.0, 0.0, 1.0};
	std::vector<float> blueColor = {0.0, 0.0, 1.0, 1.0};
	std::vector<float> euphoriaColor = {1.0, 1.0, 1.0, 1.0};

	/*
	startImGuiFrame("test",ImGuiBackendFlags_None);
	ImGui::ColorEdit4("Green",greenColor.data());
	ImGui::ColorEdit4("Red",redColor.data());
	ImGui::ColorEdit4("Blue",blueColor.data());
	ImGui::ColorEdit4("Euphoria",euphoriaColor.data());
	renderImGuiFrame();
	*/

	if (m_renderEuActive) {
		greenColor = euphoriaColor;
		redColor = euphoriaColor;
		blueColor = euphoriaColor;
	}

	float size = 0.05;

	float startAngle = asin(0.25 / m_radius);

	glm::vec2 center = {m_radius, 0.0};
	glm::vec2 redBeforeOuter = getCirclePoint((double)m_radius + size, startAngle);
	glm::vec2 redBeforeInner = getCirclePoint((double)m_radius - size, startAngle);
	glm::vec2 greenBeforeOuter = getCirclePoint((double)m_radius + 0.5 + size, startAngle);
	glm::vec2 greenBeforeInner = getCirclePoint((double)m_radius + 0.5 - size, startAngle);
	glm::vec2 blueBeforeOuter = getCirclePoint((double)m_radius - 0.5 + size, startAngle);
	glm::vec2 blueBeforeInner = getCirclePoint((double)m_radius - 0.5 - size, startAngle);

	for (float angle = startAngle; angle < m_maxAngle; angle += m_deltaAngle) {
		float angleTime = time + angle * m_noteVisibleTime / m_maxAngle;

		glm::vec2 redOuter = getCirclePoint((double)m_radius + size, (double)angle);
		glm::vec2 redInner = getCirclePoint((double)m_radius - size, (double)angle);

		pushVertexColor(lanesVector, -redBeforeOuter.x + center.x, 0.0, -redBeforeOuter.y - center.y, redColor[0], redColor[1], redColor[2], redColor[3]);
		pushVertexColor(lanesVector, -redBeforeInner.x + center.x, 0.0, -redBeforeInner.y - center.y, redColor[0], redColor[1], redColor[2], redColor[3]);
		pushVertexColor(lanesVector, -redInner.x + center.x, 0.0, -redInner.y - center.y, redColor[0], redColor[1], redColor[2], redColor[3]);
		pushVertexColor(lanesVector, -redOuter.x + center.x, 0.0, -redOuter.y - center.y, redColor[0], redColor[1], redColor[2], redColor[3]);
		pushRectangleIndices(lanesIndices, lanesVertexCount);

		redBeforeOuter = redOuter;
		redBeforeInner = redInner;

		//green

		glm::vec2 greenOuter;
		glm::vec2 greenInner;
		if (getCrossAtTime(angleTime, cross) >= 1) {
			greenOuter = getCirclePoint((double)m_radius + 0.5 + size, (double)angle);
			greenInner = getCirclePoint((double)m_radius + 0.5 - size, (double)angle);
		} else {
			greenOuter = getCirclePoint((double)m_radius + 1.0 + size, (double)angle);
			greenInner = getCirclePoint((double)m_radius + 1.0 - size, (double)angle);
		}

		pushVertexColor(lanesVector, -greenBeforeOuter.x + center.x, 0.0, -greenBeforeOuter.y - center.y, greenColor[0], greenColor[1], greenColor[2], greenColor[3]);
		pushVertexColor(lanesVector, -greenBeforeInner.x + center.x, 0.0, -greenBeforeInner.y - center.y, greenColor[0], greenColor[1], greenColor[2], greenColor[3]);
		pushVertexColor(lanesVector, -greenInner.x + center.x, 0.0, -greenInner.y - center.y, greenColor[0], greenColor[1], greenColor[2], greenColor[3]);
		pushVertexColor(lanesVector, -greenOuter.x + center.x, 0.0, -greenOuter.y - center.y, greenColor[0], greenColor[1], greenColor[2], greenColor[3]);
		pushRectangleIndices(lanesIndices, lanesVertexCount);

		greenBeforeOuter = greenOuter;
		greenBeforeInner = greenInner;

		//blue

		glm::vec2 blueOuter;
		glm::vec2 blueInner;
		if (getCrossAtTime(angleTime, cross) <= 1) {
			blueOuter = getCirclePoint((double)m_radius - 0.5 + size, (double)angle);
			blueInner = getCirclePoint((double)m_radius - 0.5 - size, (double)angle);
		} else {
			blueOuter = getCirclePoint((double)m_radius - 1.0 + size, (double)angle);
			blueInner = getCirclePoint((double)m_radius - 1.0 - size, (double)angle);
		}

		pushVertexColor(lanesVector, -blueBeforeOuter.x + center.x, 0.0, -blueBeforeOuter.y - center.y, blueColor[0], blueColor[1], blueColor[2], blueColor[3]);
		pushVertexColor(lanesVector, -blueBeforeInner.x + center.x, 0.0, -blueBeforeInner.y - center.y, blueColor[0], blueColor[1], blueColor[2], blueColor[3]);
		pushVertexColor(lanesVector, -blueInner.x + center.x, 0.0, -blueInner.y - center.y, blueColor[0], blueColor[1], blueColor[2], blueColor[3]);
		pushVertexColor(lanesVector, -blueOuter.x + center.x, 0.0, -blueOuter.y - center.y, blueColor[0], blueColor[1], blueColor[2], blueColor[3]);
		pushRectangleIndices(lanesIndices, lanesVertexCount);

		blueBeforeOuter = blueOuter;
		blueBeforeInner = blueInner;
	}
	usePersProj();
	renderColor(lanesVector, lanesIndices);
}

void GameRender::bpmTicks(double time, std::vector<double>& bpmArr) {
	/*
	//vertices data
	std::vector<float> bpmVector;
	std::vector<unsigned int> bpmIndices;
	unsigned int bpmVertexCount = 0;

	float plane = 0.0f;
	float size = 0.1f;

	//color of bpm bar. Set to a dark grey
	float r = 0.3f;
	float g = 0.3f;
	float b = 0.3f;

	//for each bpm bar in the vector
	for (size_t i = 0; i < bpmArr.size(); i++) {
		double tickTime = bpmArr.at(i);

		if (time + m_noteVisibleTime >= tickTime && time <= tickTime + 0.2) {
			double percent = (tickTime - time) / m_noteVisibleTime;
			float z = (float)(3.75 - 3.75 * percent);

			//add tick
			pushVertexColor(bpmVector, -1.0f, plane, z - size / 2, r, g, b);
			pushVertexColor(bpmVector, -1.0f, plane, z + size / 2, r, g, b);
			pushVertexColor(bpmVector, 1.0f, plane, z + size / 2, r, g, b);
			pushVertexColor(bpmVector, 1.0f, plane, z - size / 2, r, g, b);
			pushRectangleIndices(bpmIndices, bpmVertexCount);
		}
	}
	usePersProj();
	renderColor(bpmVector, bpmIndices);
	*/
}

void GameRender::events(double time, std::vector<Note>& ev, std::vector<Note>& cross) {
	/*
	float plane = 0.0;
	float transparency = 0.35f; // euphoria transparency

	//vertices data
	std::vector<float> eventsVector = {};
	std::vector<unsigned int> eventsIndices = {};
	unsigned int eventsVertexCount = 0;

	//loop for every event inside event vector
	for (size_t i = 0; i < ev.size(); i++) {
		double milli = ev.at(i).getMilli();
		double hitWindow = ev.at(i).hitWindow;
		int type = ev.at(i).getType();
		double percent = (ev.at(i).getMilli() - time) / m_noteVisibleTime;

		if (type == SCR_G_ZONE) {
			std::vector<Note> cfChanges = getCrossInsideNote(ev.at(i), cross);
			int num = 0;
			int lastVisible = -1;
			float z;
			for (size_t i = 0; i < cfChanges.size(); ++i) {
				double cfMilli = cfChanges.at(i).getMilli();
				double cfPercent = (cfMilli - time) / m_noteVisibleTime;
				int cfType = cfChanges.at(i).getType();
				if (cfMilli <= time + m_noteVisibleTime) {
					z = (float)(3.75 - 3.75 * cfPercent);
					if (cfType == CROSS_G) {
						if (num % 4 != 0) {
							pushVertexColor(eventsVector, -0.55f, plane, z, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.85f, plane, z, 0.0f, 1.0f, 0.0f);
							num += 2;
						}
						else {
							pushVertexColor(eventsVector, -0.85f, plane, z, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.55f, plane, z, 0.0f, 1.0f, 0.0f);
							num += 2;
						}
						lastVisible = CROSS_G;
					}
					else if (cfType == CROSS_C) {
						if (num % 4 != 0) {
							pushVertexColor(eventsVector, -0.2f, plane, z, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.5f, plane, z, 0.0f, 1.0f, 0.0f);
							num += 2;
						}
						else {
							pushVertexColor(eventsVector, -0.5f, plane, z, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.2f, plane, z, 0.0f, 1.0f, 0.0f);
							num += 2;
						}
						lastVisible = CROSS_C;
					}
					if (num > 0 && num % 4 == 0) {
						pushRectangleIndices(eventsIndices, eventsVertexCount);
					}
				}
			}
			if (num % 4 != 0) {
				if (lastVisible == CROSS_G) {
					pushVertexColor(eventsVector, -0.55f, plane, 0.0f, 0.0f, 1.0f, 0.0f);
					pushVertexColor(eventsVector, -0.85f, plane, 0.0f, 0.0f, 1.0f, 0.0f);
					pushRectangleIndices(eventsIndices, eventsVertexCount);
				}
				else if (lastVisible == CROSS_C) {
					pushVertexColor(eventsVector, -0.2f, plane, 0.0f, 0.0f, 1.0f, 0.0f);
					pushVertexColor(eventsVector, -0.5f, plane, 0.0f, 0.0f, 1.0f, 0.0f);
					pushRectangleIndices(eventsIndices, eventsVertexCount);
				}
			}
		}
		else if (type == SCR_B_ZONE) {
			std::vector<Note> cfChanges = getCrossInsideNote(ev.at(i), cross);
			int num = 0;
			int lastVisible = -1;
			float z;
			for (size_t i = 0; i < cfChanges.size(); ++i) {
				double cfMilli = cfChanges.at(i).getMilli();
				double cfPercent = (cfMilli - time) / m_noteVisibleTime;
				int cfType = cfChanges.at(i).getType();
				if (cfMilli <= time + m_noteVisibleTime) {
					z = (float)(3.75 - 3.75 * cfPercent);
					if (cfType == CROSS_B) {
						if (num % 4 != 0) {
							pushVertexColor(eventsVector, 0.85f, plane, z, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.55f, plane, z, 0.0f, 0.0f, 1.0f);
							num += 2;
						}
						else {
							pushVertexColor(eventsVector, 0.55f, plane, z, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.85f, plane, z, 0.0f, 0.0f, 1.0f);
							num += 2;
						}
						lastVisible = CROSS_B;
					}
					else if (cfType == CROSS_C) {
						if (num % 4 != 0) {
							pushVertexColor(eventsVector, 0.5f, plane, z, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.2f, plane, z, 0.0f, 0.0f, 1.0f);
							num += 2;
						}
						else {
							pushVertexColor(eventsVector, 0.2f, plane, z, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.5f, plane, z, 0.0f, 0.0f, 1.0f);
							num += 2;
						}
						lastVisible = CROSS_C;
					}
					if (num > 0 && num % 4 == 0) {
						pushRectangleIndices(eventsIndices, eventsVertexCount);
					}
				}
			}
			if (num % 4 != 0) {
				if (lastVisible == CROSS_B) {
					pushVertexColor(eventsVector, 0.85f, plane, 0.0f, 0.0f, 0.0f, 1.0f);
					pushVertexColor(eventsVector, 0.55f, plane, 0.0f, 0.0f, 0.0f, 1.0f);
					pushRectangleIndices(eventsIndices, eventsVertexCount);
				}
				else if (lastVisible == CROSS_C) {
					pushVertexColor(eventsVector, 0.5f, plane, 0.0f, 0.0f, 0.0f, 1.0f);
					pushVertexColor(eventsVector, 0.2f, plane, 0.0f, 0.0f, 0.0f, 1.0f);
					pushRectangleIndices(eventsIndices, eventsVertexCount);
				}
			}
		}
		else if (type == EU_ZONE) {
			double endTime = ev.at(i).getMilli() + ev.at(i).getLength();
			double endPercent = (endTime - time) / m_noteVisibleTime;
			bool start_eu = false;
			//if euphoria start is in the middle of the highway
			if (percent >= 0.0 && percent < 1.0) {
				float z = (float)(3.75 - 3.75 * percent);
				pushVertexColor(eventsVector, -1.0f, plane, z, 1.0f, 1.0f, 1.0f, transparency);
				pushVertexColor(eventsVector, 1.0f, plane, z, 1.0f, 1.0f, 1.0f, transparency);
				start_eu = true;
			}
			//if euphoria start has already passed the clicker
			else if (percent < 0.0) {
				//if (m_renderEuZone) {
				pushVertexColor(eventsVector, -1.0, plane, 3.75, 1.0, 1.0, 1.0, transparency);
				pushVertexColor(eventsVector, 1.0, plane, 3.75, 1.0, 1.0, 1.0, transparency);
				start_eu = true;
				//}
			}
			//if the render has successfully added the start
			if (start_eu) {
				//if euphoria end is in the middle of the highway
				if (endPercent >= 0.0 && endPercent < 1.0) {
					float z = 3.75f - (3.75f * (float)endPercent);
					pushVertexColor(eventsVector, 1.0, plane, z, 1.0, 1.0, 1.0, transparency);
					pushVertexColor(eventsVector, -1.0, plane, z, 1.0, 1.0, 1.0, transparency);

				}
				//if euphoria end is beyond the visible highway
				else {
					pushVertexColor(eventsVector, 1.0, plane, 0.0, 1.0, 1.0, 1.0, transparency);
					pushVertexColor(eventsVector, -1.0, plane, 0.0, 1.0, 1.0, 1.0, transparency);
				}
				pushRectangleIndices(eventsIndices, eventsVertexCount);
			}
		}

		if (milli > time + m_noteVisibleTime) {
			ev.at(i).setLanMod(m_renderCross);
		}
	}
	usePersProj();
	renderColor(eventsVector, eventsIndices);
	*/
}

void GameRender::meters(double time) {
	/*
	float yPlane = 0.1f;

	//vertices data
	std::vector<float>metersVector;
	std::vector<unsigned int> metersIndices;
	unsigned int metersVertexCount = 0;

	//multiplier display
	if (m_playerMult == 2) {
		//render a 'x2'
		if (m_isButtonsRight) {
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 400.0f / 1000.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 400.0f / 1000.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 0.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 0.0f, 800.0f / 1200.0f);
		}
		else {
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 0.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 0.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		}
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 3) {
		//render a 'x3'
		if (m_isButtonsRight) {
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 800.0f / 1000.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 800.0f / 1000.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		}
		else {
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 400.0f / 1000.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 400.0f / 1000.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 800.0f / 1000.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 800.0f / 1000.0f, 800.0f / 1200.0f);
		}
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 4) {
		//render a 'x4'
		if (m_isButtonsRight) {
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 400.0f / 1000.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 400.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 0.0f, 0.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 0.0f, 400.0f / 1200.0f);
		}
		else {
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 0.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 0.0f, 0.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 400.0f / 1200.0f);
		}
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 6) {
		//render a 'x6'
		if (m_isButtonsRight) {
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 0.0f, 1.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 0.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 400.0f / 1000.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 400.0f / 1000.0f, 1.0f);
		}
		else {
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 0.0f, 1.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 0.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 1.0f);
		}
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 8) {
		//render a 'x8'
		if (m_isButtonsRight) {
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 1.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 800.0f / 1000.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 800.0f / 1000.0f, 1.0f);
		}
		else {
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 400.0f / 1000.0f, 1.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 400.0f / 1000.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 800.0f / 1000.0f, 800.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 800.0f / 1000.0f, 1.0f);
		}

		pushRectangleIndices(metersIndices, metersVertexCount);
	}

	//combo tickmarks
	for (int i = 0; i < 8; i++) {
		if (m_playerCombo == 0) {
			//add inactive tick
			pushVertexTexture(metersVector, 1.0f, yPlane, 3.6f - 0.11f * i, 400.0f / 1000.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 3.7f - 0.11f * i, 400.0f / 1000.0f, 200.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 3.7f - 0.11f * i, 800.0f / 1000.0f, 200.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 3.6f - 0.11f * i, 800.0f / 1000.0f, 400.0f / 1200.0f);
			pushRectangleIndices(metersIndices, metersVertexCount);
		}
		else if (m_playerCombo >= 24) {
			//add active tick
			pushVertexTexture(metersVector, 1.0f, yPlane, 3.6f - 0.11f * i, 400.0f / 1000.0f, 200.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 3.7f - 0.11f * i, 400.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 3.7f - 0.11f * i, 800.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 3.6f - 0.11f * i, 800.0f / 1000.0f, 200.0f / 1200.0f);
			pushRectangleIndices(metersIndices, metersVertexCount);
		}
		//if the multiplier is growing
		else {
			//find the last tick to be active
			int limit = m_playerCombo % 8;
			if (limit == 0)limit = 9;

			if (i < limit) {
				//add active tick
				pushVertexTexture(metersVector, 1.0f, yPlane, 3.6f - 0.11f * i, 400.0f / 1000.0f, 200.0f / 1200.0f);
				pushVertexTexture(metersVector, 1.0f, yPlane, 3.7f - 0.11f * i, 400.0f / 1000.0f, 0.0f);
				pushVertexTexture(metersVector, 1.2f, yPlane, 3.7f - 0.11f * i, 800.0f / 1000.0f, 0.0f);
				pushVertexTexture(metersVector, 1.2f, yPlane, 3.6f - 0.11f * i, 800.0f / 1000.0f, 200.0f / 1200.0f);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
			else {
				//add inactive tick
				pushVertexTexture(metersVector, 1.0f, yPlane, 3.6f - 0.11f * i, 400.0f / 1000.0f, 400.0f / 1200.0f);
				pushVertexTexture(metersVector, 1.0f, yPlane, 3.7f - 0.11f * i, 400.0f / 1000.0f, 200.0f / 1200.0f);
				pushVertexTexture(metersVector, 1.2f, yPlane, 3.7f - 0.11f * i, 800.0f / 1000.0f, 200.0f / 1200.0f);
				pushVertexTexture(metersVector, 1.2f, yPlane, 3.6f - 0.11f * i, 800.0f / 1000.0f, 400.0f / 1200.0f);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
		}
	}

	//euphoria meter
	//empty indicator
	for (int i = 0; i < 3; i++) {
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.75f - 0.35f * i, 800.0f / 1000.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.75f - 0.35f * i, 1.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f - 0.35f * i, 1.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f - 0.35f * i, 800.0f / 1000.0f, 800.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}

	//if only the first indicator needs to change
	if (m_renderEuValue > 0.0 && m_renderEuValue < 1.0) {
		float z = (float)m_renderEuValue;
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.75f, 800.0f / 1000.0f, 0.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.75f, 1.0f, 0.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.75f - 0.35f * z, 1.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.75f - 0.35f * z, 800.0f / 1000.0f, 400.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	//if more indicators need to change (two or three)
	else if (m_renderEuValue >= 1.0) {
		//add first full indicator
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.75f, 800.0f / 1000.0f, 0.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.75f, 1.0f, 0.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f, 1.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f, 800.0f / 1000.0f, 400.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
		//if only the second indicator needs to change
		if (m_renderEuValue < 2.0) {
			float z = (float)m_renderEuValue - 1.0f;
			pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f, 800.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f, 1.0f, 0.0f);
			pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f - 0.35f * z, 1.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f - 0.35f * z, 800.0f / 1000.0f, 400.0f / 1200.0f);
			pushRectangleIndices(metersIndices, metersVertexCount);
		}
		//if more indicators need to change (all three)
		else if (m_renderEuValue >= 2.0) {
			//add full second indicator
			pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f, 800.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f, 1.0f, 0.0f);
			pushVertexTexture(metersVector, -1.0f, yPlane, 3.05f, 1.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, -1.1f, yPlane, 3.05f, 800.0f / 1000.0f, 400.0f / 1200.0f);
			pushRectangleIndices(metersIndices, metersVertexCount);
			//if the euphoria value is below full
			if (m_renderEuValue < 3.0) {
				float z = (float)m_renderEuValue - 2.0f;
				pushVertexTexture(metersVector, -1.1f, yPlane, 3.05f, 800.0f / 1000.0f, 0.0f);
				pushVertexTexture(metersVector, -1.0f, yPlane, 3.05f, 1.0f, 0.0f);
				pushVertexTexture(metersVector, -1.0f, yPlane, 3.05f - 0.35f * z, 1.0, 400.0f / 1200.0f);
				pushVertexTexture(metersVector, -1.1f, yPlane, 3.05f - 0.35f * z, 800.0f / 1000.0f, 400.0f / 1200.0f);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
			else {
				//add full third indicator
				pushVertexTexture(metersVector, -1.1f, yPlane, 3.05f, 800.0f / 1000.0f, 0.0f);
				pushVertexTexture(metersVector, -1.0f, yPlane, 3.05f, 1.0f, 0.0f);
				pushVertexTexture(metersVector, -1.0f, yPlane, 2.7f, 1.0f, 400.0f / 1200.0f);
				pushVertexTexture(metersVector, -1.1f, yPlane, 2.7f, 800.0f / 1000.0f, 400.0f / 1200.0f);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
		}
	}
	usePersProj();
	renderTexture(metersVector, metersIndices, m_metersTexture);

	metersVector.clear();
	metersIndices.clear();
	metersVertexCount = 0;
	//progress bar
	std::string s = std::to_string(m_playerScore);
	std::string scoreDisplay("00000000");
	scoreDisplay.resize(scoreDisplay.length() - s.length());
	scoreDisplay.append(s);

	float scale = 0.05f;
	float x = 0.0;
	float width = 200.0f;
	x = (float)m_playerScore / (float)m_genBaseScore;
	float y = 20.0f;

	if (m_isButtonsRight) {
		pushVertexTexture(metersVector, -940.0f, 310.0f, 0.0f, 0.0f, 0.5f);
		pushVertexTexture(metersVector, -940.0f, 310.0f + y + 20.0, 0.0f, 0.0f, 0.0f);
		pushVertexTexture(metersVector, -940.0f - width - 20.0, 310.0f + y + 20.0, 0.0f, 220.0f / 300.0f, 0.0f);
		pushVertexTexture(metersVector, -940.0f - width - 20.0, 310.0f, 0.0f, 220.0f / 300.0f, 0.5f);
		pushRectangleIndices(metersIndices, metersVertexCount);

		pushVertexTexture(metersVector, -940.0f - width, 280.0f, 0.0f, 221.0f / 300.0f, 1.0f);
		pushVertexTexture(metersVector, -940.0f - width, 280.0f + 80.0, 0.0f, 221.0f / 300.0f, 0.0f);
		pushVertexTexture(metersVector, -940.0f - width - 80.0, 280.0f + 80.0, 0.0f, 1.0f, 0.0f);
		pushVertexTexture(metersVector, -940.0f - width - 80.0, 280.0f, 0.0f, 1.0f, 1.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else {
		pushVertexTexture(metersVector, 940.0f, 310.0f, 0.0f, 0.0f, 0.5f);
		pushVertexTexture(metersVector, 940.0f, 310.0f + y + 20.0, 0.0f, 0.0f, 0.0f);
		pushVertexTexture(metersVector, 940.0f + width + 20.0, 310.0f + y + 20.0, 0.0f, 220.0f / 300.0f, 0.0f);
		pushVertexTexture(metersVector, 940.0f + width + 20.0, 310.0f, 0.0f, 220.0f / 300.0f, 0.5f);
		pushRectangleIndices(metersIndices, metersVertexCount);

		pushVertexTexture(metersVector, 940.0f + width, 280.0f, 0.0f, 221.0f / 300.0f, 1.0f);
		pushVertexTexture(metersVector, 940.0f + width, 280.0f + 80.0, 0.0f, 221.0f / 300.0f, 0.0f);
		pushVertexTexture(metersVector, 940.0f + width + 80.0, 280.0f + 80.0, 0.0f, 1.0f, 0.0f);
		pushVertexTexture(metersVector, 940.0f + width + 80.0, 280.0f, 0.0f, 1.0f, 1.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);

	}




	useOrthoProj();
	renderTexture(metersVector, metersIndices, m_pgBarFrame);
	metersVector.clear();
	metersIndices.clear();
	metersVertexCount = 0;

	float textX = 960.0f + width + 3.0f;
	float textY = 305.0f;

	if (x < 0.1) {
		x = x / 0.1;
		x *= width;
	}
	else if (x < 0.2) {
		drawText("1", textX, textY, scale);
		x = (x - 0.1) / 0.1;
		x *= width;
	}
	else if (x < 0.3) {
		drawText("2", textX, textY, scale);
		x = (x - 0.2) / 0.1;
		x *= width;
	}
	else if (x < 0.4) {
		drawText("3", textX, textY, scale);
		x = (x - 0.3) / 0.1;
		x *= width;
	}
	else if (x < 0.5) {
		drawText("4", textX - 2.0f, textY, scale);
		x = (x - 0.40) / 0.10;
		x *= width;
	}
	else if (x < 0.7) {
		drawText("5", textX, textY, scale);
		x = (x - 0.50) / 0.20;
		x *= width;
	}
	else {
		drawText("5", textX, textY, scale);
		x = width;
	}

	float ratio = x / y;
	float toffset = time * -1;

	if (m_isButtonsRight) {
		pushVertexTexture(metersVector, -950.0f, 320.0f, 0.0f, 0.0f + toffset, 1.0f);
		pushVertexTexture(metersVector, -950.0f, 320.0f + y, 0.0f, 0.0f + toffset, 0.0f);
		pushVertexTexture(metersVector, -950.0f - x, 320.0f + y, 0.0f, 0.0f + ratio + toffset, 0.0f);
		pushVertexTexture(metersVector, -950.0f - x, 320.0f, 0.0f, 0.0f + ratio + toffset, 1.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else {
		pushVertexTexture(metersVector, 950.0f, 320.0f, 0.0f, 0.0f + toffset, 1.0f);
		pushVertexTexture(metersVector, 950.0f, 320.0f + y, 0.0f, 0.0f + toffset, 0.0f);
		pushVertexTexture(metersVector, 950.0f + x, 320.0f + y, 0.0f, 0.0f + ratio + toffset, 0.0f);
		pushVertexTexture(metersVector, 950.0f + x, 320.0f, 0.0f, 0.0f + ratio + toffset, 1.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}

	useOrthoProj();
	renderTexture(metersVector, metersIndices, m_pgBarInside);

	//score meter

	drawText(scoreDisplay, 940.0f, 230.0f, scale);

	//combo meter
	if (m_playerCombo >= 15) {
		std::string c = std::to_string(m_playerCombo);
		drawText(c, 940.0f, 280.0f, 0.03f);
	}
	*/
}

void GameRender::result(Player& player, Generator& generator) {
	std::vector<float> resultVector;
	std::vector<unsigned int> resultIndices;
	unsigned int resultVertexCount = 0;

	useOrthoProj();
	float y = 50.0f;
	float scale = 0.05f;

	int all = generator.getNotesTotal();
	int hit = generator.getNotesHit();
	int score = player.getScore();
	int combo = player.getHighCombo();

	std::string firstSong = std::string(generator.getSongEntry().s1);
	std::string secondSong = std::string(generator.getSongEntry().s2);
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
	if (player.m_botEnabled) {
		drawText("!BOT ACTIVE!", x + 30.0f, y, scale);
	} else {
		float stars = (float)player.getScore() / (float)generator.m_baseScore;
		if (stars >= 0.1) {
			pushVertexTexture(resultVector, x + 30.0f + scale * 0000.0f, y, 0.0f, 221.0f / 300.0f, 1.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 0000.0f, y + scale * 1000.0f, 0.0f, 221.0f / 300.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 1000.0f, y + scale * 1000.0f, 0.0f, 1.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 1000.0f, y, 0.0f, 1.0f, 1.0f);
			pushRectangleIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.2) {
			pushVertexTexture(resultVector, x + 30.0f + scale * 1000.0f, y, 0.0f, 221.0f / 300.0f, 1.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 1000.0f, y + scale * 1000.0f, 0.0f, 221.0f / 300.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 2000.0f, y + scale * 1000.0f, 0.0f, 1.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 2000.0f, y, 0.0f, 1.0f, 1.0f);
			pushRectangleIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.3) {
			pushVertexTexture(resultVector, x + 30.0f + scale * 2000.0f, y, 0.0f, 221.0f / 300.0f, 1.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 2000.0f, y + scale * 1000.0f, 0.0f, 221.0f / 300.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 3000.0f, y + scale * 1000.0f, 0.0f, 1.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 3000.0f, y, 0.0f, 1.0f, 1.0f);
			pushRectangleIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.4) {
			pushVertexTexture(resultVector, x + 30.0f + scale * 3000.0f, y, 0.0f, 221.0f / 300.0f, 1.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 3000.0f, y + scale * 1000.0f, 0.0f, 221.0f / 300.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 4000.0f, y + scale * 1000.0f, 0.0f, 1.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 4000.0f, y, 0.0f, 1.0f, 1.0f);
			pushRectangleIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.5) {
			pushVertexTexture(resultVector, x + 30.0f + scale * 4000.0f, y, 0.0f, 221.0f / 300.0f, 1.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 4000.0f, y + scale * 1000.0f, 0.0f, 221.0f / 300.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 5000.0f, y + scale * 1000.0f, 0.0f, 1.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 5000.0f, y, 0.0f, 1.0f, 1.0f);
			pushRectangleIndices(resultIndices, resultVertexCount);
		}
		if (resultVector.size() > 0) {
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
	if (combo == all && all != 0) {
		std::string t = "Wow! a Full Combo";
		float x = 1270.0f - getTextWidth(t, scale);
		drawText(t, x, 100.0f, scale);
	} else if (hit > all) {
		std::string t = "Hold on a sec. How did you...";
		float x = 1270.0f - getTextWidth(t, scale);
		drawText(t, x, 100.0f, scale);
	}
}

void GameRender::debug(double deltaTime, std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& c) {
	std::string text = "Notes";
	text.append("(");
	text.append(std::to_string(v.size()));
	text.append("):");
	for (size_t i = 0; i < v.size() && i < 15; i++) {
		text.append(std::to_string(v.at(i).getType()));
		text.append(",");
	}
	drawText(text, 10.0f, 10.0f, 0.05f);

	std::string t2 = "Events";
	t2.append("(");
	t2.append(std::to_string(ev.size()));
	t2.append("):");
	for (size_t i = 0; i < ev.size() && i < 15; i++) {
		t2.append(std::to_string(ev.at(i).getType()));
		t2.append(",");
	}
	drawText(t2, 10.0f, 70.0f, 0.05f);

	std::string cs = "Cross";
	cs.append("(");
	cs.append(std::to_string(c.size()));
	cs.append("):");
	for (size_t i = 0; i < c.size() && i < 15; i++) {
		cs.append(std::to_string(c.at(i).getType()));
		cs.append(",");
	}
	drawText(cs, 10.0f, 120.0f, 0.05f);
	std::string fps = "FPS:";
	fps.append(std::to_string(1/deltaTime));
	drawText(fps,10.0f,170.0f,0.05f);
	
	std::string baseScore;
	baseScore.append(std::to_string((float)m_playerScore / (float)m_genBaseScore));
	baseScore.append("|");
	baseScore.append(std::to_string(m_genBaseScore));
	drawText(baseScore, 940.0f, 370.0f, 0.03f);

	drawText(std::to_string(m_renderEuValue), 200.0, 500.0f, 0.03f);
}

void GameRender::pollState(double time, Player& p, Generator& g) {
	//update local render variables to match player and generator
	m_red = p.getRedClicker();
	m_blue = p.getBlueClicker();
	m_green = p.getGreenClicker();
	m_playerCross = p.getCross();
	m_playerCombo = p.getCombo();
	m_playerMult = p.getMult();
	m_playerScore = p.getScore();
	m_renderEuValue = p.getEuValue();
	m_renderEuActive = p.getEuActive();
	m_renderEuZone = p.getEuZoneActive();
	m_genBaseScore = g.m_baseScore;

	bool greenAnimEnabled = p.m_greenAnimation;
	bool redAnimEnabled = p.m_redAnimation;
	bool blueAnimEnabled = p.m_blueAnimation;

	if (p.m_pastCross >= 1 && p.m_cross == 0) {
		m_animManager.triggerAnimation(AN_CROSS_GREEN_TO_LEFT, time);
		if (p.m_pastCross == 2) {
			m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_CENTER, time);
		}
	} else if (p.m_pastCross <= 1 && p.m_cross == 2) {
		m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_RIGHT, time);
		if (p.m_pastCross == 0) {
			m_animManager.triggerAnimation(AN_CROSS_GREEN_TO_CENTER, time);
		}
	}

	if (p.m_cross == 1) {
		if (p.m_pastCross == 0) {
			m_animManager.triggerAnimation(AN_CROSS_GREEN_TO_CENTER, time);
		} else if (p.m_pastCross == 2) {
			m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_CENTER, time);
		}
	}

	if (greenAnimEnabled) {
		m_animManager.triggerAnimation(AN_GREEN_CLICKER, time);
		p.m_greenAnimation = false;
	}
	if (redAnimEnabled) {
		m_animManager.triggerAnimation(AN_RED_CLICKER, time);
		p.m_redAnimation = false;
	}
	if (blueAnimEnabled) {
		m_animManager.triggerAnimation(AN_BLUE_CLICKER, time);
		p.m_blueAnimation = false;
	}
	/*if (centerToGreen) {
		m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_RIGHT, time);
		p.m_cfCenterToGreen = false;
	}if (centerToBlue) {
		m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_RIGHT, time);
		p.m_cfCenterToBlue = false;
	}if (greenToCenter) {
		m_animManager.triggerAnimation(AN_CROSS_GREEN_TO_CENTER, time);
		p.m_cfGreenToCenter = false;
	}if (centerToGreen) {
		m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_CENTER, time);
		p.m_cfBlueToCenter = false;
	}*/
	rendr_InvertedX = m_isButtonsRight;
}

void GameRender::clickerAnimation() {
	/*
	float plane = 0.0;
	std::vector<float>clickerVector = {};
	std::vector<unsigned int>clickerIndices = {};
	unsigned int clickerVertexCount = 0;

	Animation greenAnim = m_animManager.getAnimById(AN_GREEN_CLICKER);
	Animation redAnim = m_animManager.getAnimById(AN_RED_CLICKER);
	Animation blueAnim = m_animManager.getAnimById(AN_BLUE_CLICKER);

	if (greenAnim.isEnabled()) {
		double x = greenAnim.getPercent();
		x = floor(x * 10) / 10;

		float y = 1.0f;
		if (m_renderEuActive)y = 0.25f;

		pushVertexTexture(clickerVector, m_greenLeft, plane, m_greenBack, (float)x, y);
		pushVertexTexture(clickerVector, m_greenLeft, plane, m_greenFront, (float)x, y - 0.25f);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_greenFront, (float)x + 0.1f, y - 0.25f);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_greenBack, (float)x + 0.1f, y);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	if (redAnim.isEnabled()) {
		double x = redAnim.getPercent();
		x = floor(x * 10) / 10;

		float y = 0.75f;
		if (m_renderEuActive)y = 0.25f;

		pushVertexTexture(clickerVector, -0.15f, plane, 3.6f, (float)x, y);
		pushVertexTexture(clickerVector, -0.15f, plane, 3.9f, (float)x, y - 0.25f);
		pushVertexTexture(clickerVector, 0.15f, plane, 3.9f, (float)x + 0.1f, y - 0.25f);
		pushVertexTexture(clickerVector, 0.15f, plane, 3.6f, (float)x + 0.1f, y);

		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	if (blueAnim.isEnabled()) {
		double x = blueAnim.getPercent();
		x = floor(x * 10) / 10;

		float y = 0.5f;
		if (m_renderEuActive)y = 0.25f;

		pushVertexTexture(clickerVector, m_blueLeft, plane, m_blueBack, (float)x, y);
		pushVertexTexture(clickerVector, m_blueLeft, plane, m_blueFront, (float)x, y - 0.25f);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_blueFront, (float)x + 0.1f, y - 0.25f);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_blueBack, (float)x + 0.1f, y);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	usePersProj();
	renderTexture(clickerVector, clickerIndices, m_clickerAnimation);
	*/
}

void GameRender::reset() {
	m_noteVisibleTime = 1.0;
	m_red = false;
	m_green = false;
	m_blue = false;
	m_playerCross = 1;
	m_playerCombo = 0;
	m_playerMult = 0;
	m_playerScore = 0;
	m_renderCross = 1;
	m_renderEuValue = 0.0;
	m_renderEuActive = false;
	m_renderEuZone = false;
}

void GameRender::updateAnimations(double time) {
	m_animManager.tick(time);
}

std::vector<Note> GameRender::getCrossInsideNote(Note& note, std::vector<Note> cross) {
	std::vector<Note> result = {};
	double start = note.getMilli();
	double end = start + note.getLength();
	int lastChange = CROSS_C;

	if (note.getLanMod() == 0) {
		lastChange = CROSS_G;
	} else if (note.getLanMod() == 2) {
		lastChange = CROSS_B;
	}

	result.push_back(Note(start, lastChange, 0.0f, true));
	for (size_t i = 0; i < cross.size(); ++i) {
		double crossMilli = cross.at(i).getMilli();
		int crossType = cross.at(i).getType();
		if (crossMilli <= end && crossMilli >= start) {
			if (crossType == CROSS_G && lastChange != CROSS_G) {
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
				lastChange = CROSS_G;
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
			} else if (crossType == CROSS_C && lastChange != CROSS_C) {
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
				lastChange = CROSS_C;
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
			} else if (crossType == CROSS_B && lastChange != CROSS_B) {
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
				lastChange = CROSS_B;
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
			}
		}
	}
	result.push_back(Note(end, lastChange, 0.0f, true));
	return result;
}

glm::vec2 GameRender::getCirclePoint(double radius, double angle) {
	double x = radius * cos(angle);
	double y = radius * sin(angle);
	return {x, y};
}

int GameRender::getCrossAtTime(double time, std::vector<Note> crossArr) {
	int index = 0;
	for (size_t i = 0; i < crossArr.size(); ++i) {
		if (crossArr.at(i).getMilli() > time) {
			index = i - 1;
			break;
		}
	}
	int type = crossArr.at(index).getType();
	if (type == CROSS_G) {
		return 0;
	} else if (type == CROSS_C) {
		return 1;
	} else {
		return 2;
	}
}

GameRender::~GameRender() {
}
