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
	}
}

void GameRender::highway(double time) {
	//convert from time to texture coords
	float factor = (float)time * 0.875f;

	float plane = 0.0f;

	//vertices data
	std::vector<float> highwayVector;
	std::vector<unsigned int> highwayIndices;
	unsigned int highwayVertexCount = 0;

	//create object and push vertices
	pushVertexTexture(highwayVector, -1.0f, plane, 0.0f, 0.0f, 1.0f + factor);
	pushVertexTexture(highwayVector, -1.0f, plane, 4.0f, 0.0f, 0.0f + factor);
	pushVertexTexture(highwayVector, 1.0f, plane, 4.0f, 1.0f, 0.0f + factor);
	pushVertexTexture(highwayVector, 1.0f, plane, 0.0f, 1.0f, 1.0f + factor);
	pushRectangleIndices(highwayIndices, highwayVertexCount);

	usePersProj();
	renderTexture(highwayVector, highwayIndices, m_highwayTexture);
}

void GameRender::clicker() {
	//difference in size between pressed and not
	float clickedOffset = 0.03f;

	float plane = 0.0f;

	//vertices data
	std::vector<float> clickerVector = {};
	std::vector<unsigned int> clickerIndices = {};
	unsigned int clickerVertexCount = 0;

	m_greenLeft = -0.5f;
	m_greenRight = -0.2f;
	m_greenBack = 3.6f;
	m_greenFront = 3.9f;

	m_blueLeft = 0.2f;
	m_blueRight = 0.5f;
	m_blueBack = 3.6f;
	m_blueFront = 3.9f;

	//left tray object
	pushVertexTexture(clickerVector, -0.85f, plane, 3.6f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.85f, plane, 3.9f, 1.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.2f, plane, 3.9f, 1.0f, 1280.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.2f, plane, 3.6f, 1320.0f / 1760.0f, 1280.0f / 1640.0f);
	pushRectangleIndices(clickerIndices, clickerVertexCount);

	//right tray object
	pushVertexTexture(clickerVector, 0.85f, plane, 3.6f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, 0.85f, plane, 3.9f, 1.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, 0.2f, plane, 3.9f, 1.0f, 1280.0f / 1640.0f);
	pushVertexTexture(clickerVector, 0.2f, plane, 3.6f, 1320.0f / 1760.0f, 1280.0f / 1640.0f);
	pushRectangleIndices(clickerIndices, clickerVertexCount);

	if (m_red) {
		//red pressed clicker
		pushVertexTexture(clickerVector, -0.15f + clickedOffset, plane, 3.6f + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, -0.15f + clickedOffset, plane, 3.9f - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f - clickedOffset, plane, 3.9f - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f - clickedOffset, plane, 3.6f + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
	}
	else {
		//red normal clicker
		pushVertexTexture(clickerVector, -0.15f, plane, 3.6f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, -0.15f, plane, 3.9f, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f, plane, 3.9f, 880.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f, plane, 3.6f, 880.0f / 1760.0f, 840.0f / 1640.0f);
	}
	pushRectangleIndices(clickerIndices, clickerVertexCount);


	if (m_green) {
		//green clicker is pressed
		if (m_playerCross >= 1) {
			//green clicker is on the left
			Animation crossGreen = m_animManager.getAnimById(AN_CROSS_GREEN_TO_CENTER);
			if (crossGreen.isEnabled()) {
				m_greenLeft = -0.85f;
				m_greenRight = -0.55f;

				float d = crossGreen.getPercent() * 0.35;
				m_greenLeft += d;
				m_greenRight += d;
			}
			//green clicker is on the center
			pushVertexTexture(clickerVector, m_greenLeft + clickedOffset, plane, m_greenBack + clickedOffset, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenLeft + clickedOffset, plane, m_greenFront - clickedOffset, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenRight - clickedOffset, plane, m_greenFront - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenRight - clickedOffset, plane, m_greenBack + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			m_greenLeft = -0.85f;
			m_greenRight = -0.55f;

			//green clicker is on the left
			Animation crossGreen = m_animManager.getAnimById(AN_CROSS_GREEN_TO_LEFT);
			if (crossGreen.isEnabled()) {
				m_greenLeft = -0.5;
				m_greenRight = -0.2;
				float d = crossGreen.getPercent() * 0.35;

				m_greenLeft -= d;
				m_greenRight -= d;
			}

			pushVertexTexture(clickerVector, m_greenLeft + clickedOffset, plane, m_greenBack + clickedOffset, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenLeft + clickedOffset, plane, m_greenFront - clickedOffset, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenRight - clickedOffset, plane, m_greenFront - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenRight - clickedOffset, plane, m_greenBack + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	else {
		//default clicker position

		if (m_playerCross >= 1) {
			//green clicker is on the left
			Animation crossGreen = m_animManager.getAnimById(AN_CROSS_GREEN_TO_CENTER);
			if (crossGreen.isEnabled()) {
				m_greenLeft = -0.85f;
				m_greenRight = -0.55f;

				float d = crossGreen.getPercent() * 0.35;
				m_greenLeft += d;
				m_greenRight += d;
			}
			//green clicker is on the center
			pushVertexTexture(clickerVector, m_greenLeft, plane, m_greenBack, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenLeft, plane, m_greenFront, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenRight, plane, m_greenFront, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenRight, plane, m_greenBack, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			m_greenLeft = -0.85f;
			m_greenRight = -0.55f;

			//green clicker is on the left
			Animation crossGreen = m_animManager.getAnimById(AN_CROSS_GREEN_TO_LEFT);
			if (crossGreen.isEnabled()) {
				m_greenLeft = -0.5;
				m_greenRight = -0.2;
				float d = crossGreen.getPercent() * 0.35;

				m_greenLeft -= d;
				m_greenRight -= d;
			}

			pushVertexTexture(clickerVector, m_greenLeft, plane, m_greenBack, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenLeft, plane, m_greenFront, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenRight, plane, m_greenFront, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_greenRight, plane, m_greenBack, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	if (m_blue) {
		//blue clicker is pressed

		if (m_playerCross <= 1) {
			//blue clicker is on the center
			Animation crossBlue = m_animManager.getAnimById(AN_CROSS_BLUE_TO_CENTER);

			if (crossBlue.isEnabled()) {
				m_blueLeft = 0.55;
				m_blueRight = 0.85;

				float d = crossBlue.getPercent() * 0.35;
				m_blueLeft -= d;
				m_blueRight -= d;
			}

			pushVertexTexture(clickerVector, m_blueLeft + clickedOffset, plane, m_blueBack + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueLeft + clickedOffset, plane, m_blueFront - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueRight - clickedOffset, plane, m_blueFront - clickedOffset, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueRight - clickedOffset, plane, m_blueBack + clickedOffset, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			m_blueLeft = 0.55;
			m_blueRight = 0.85;

			Animation crossBlue = m_animManager.getAnimById(AN_CROSS_BLUE_TO_RIGHT);

			if (crossBlue.isEnabled()) {
				m_blueLeft = 0.2;
				m_blueRight = 0.5;

				float d = crossBlue.getPercent() * 0.35;
				m_blueLeft += d;
				m_blueRight += d;
			}

			//blue clicker is on the right
			pushVertexTexture(clickerVector, m_blueLeft + clickedOffset, plane, m_blueBack + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueLeft + clickedOffset, plane, m_blueFront - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueRight - clickedOffset, plane, m_blueFront - clickedOffset, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueRight - clickedOffset, plane, m_blueBack + clickedOffset, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	else {
		if (m_playerCross <= 1) {
			//blue clicker is on the center
			Animation crossBlue = m_animManager.getAnimById(AN_CROSS_BLUE_TO_CENTER);

			if (crossBlue.isEnabled()) {
				m_blueLeft = 0.55;
				m_blueRight = 0.85;

				float d = crossBlue.getPercent() * 0.35;
				m_blueLeft -= d;
				m_blueRight -= d;
			}

			pushVertexTexture(clickerVector, m_blueLeft, plane, m_blueBack, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueLeft, plane, m_blueFront, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueRight, plane, m_blueFront, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueRight, plane, m_blueBack, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			m_blueLeft = 0.55;
			m_blueRight = 0.85;

			Animation crossBlue = m_animManager.getAnimById(AN_CROSS_BLUE_TO_RIGHT);

			if (crossBlue.isEnabled()) {
				m_blueLeft = 0.2;
				m_blueRight = 0.5;

				float d = crossBlue.getPercent() * 0.35;
				m_blueLeft += d;
				m_blueRight += d;
			}

			//blue clicker is on the right
			pushVertexTexture(clickerVector, m_blueLeft, plane, m_blueBack, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueLeft, plane, m_blueFront, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueRight, plane, m_blueFront, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, m_blueRight, plane, m_blueBack, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	usePersProj();
	renderTexture(clickerVector, clickerIndices, m_objTexture);
}

void GameRender::notes(double time, std::vector<Note>& v, std::vector<Note>& cross) {
	float plane = 0.0;

	//vertices data
	std::vector<float> noteVector = {};
	std::vector<unsigned int> noteIndices = {};
	unsigned int noteVertexCount = 0;

	//loop for every note inside vector
	for (size_t i = 0; i < v.size(); i++) {
		double milli = v.at(i).getMilli();
		double hitWindow = v.at(i).hitWindow;
		if (time + m_noteVisibleTime >= milli) {
			//if the note is inside the visible highway

			//calculate 'height' of note
			double dt = v.at(i).getMilli() - time;
			float z = 3.75f - (3.75f * (float)dt);

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

					int numOfSprites = floor((startZ - endZ) / length);

					for (int j = 0; j < numOfSprites; ++j) {
						float spriteZ = startZ - 0.15 - length * j;
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

					int numOfSprites = floor((startZ - endZ) / length);

					for (int j = 0; j < numOfSprites; ++j) {
						float spriteZ = startZ - 0.15 - length * j;
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
		else if (milli > time + m_noteVisibleTime) {
			//if the note is outside the visible area, update lane position
			v.at(i).setLanMod(m_renderCross);
		}
	}
	usePersProj();
	renderTexture(noteVector, noteIndices, m_objTexture);
}

void GameRender::lanes(double time, std::vector<Note>& v, std::vector<Note>& cross) {
	float plane = 0.0;

	//vertices data for each lane
	std::vector<float> greenLaneVector = {};
	std::vector<unsigned int> greenLaneIndices = {};
	unsigned int greenLaneVertexCount = 0;

	std::vector<float> blueLaneVector = {};
	std::vector<unsigned int> blueLaneIndices = {};
	unsigned int blueLaneVertexCount = 0;

	std::vector<float> redLaneVector = {};
	std::vector<unsigned int>redLaneIndices = {};
	unsigned int redLaneVertexCount = 0;

	float r, g, b;

	float offset = 0.04f;

	//if euphoria is active, turn red lane white
	if (m_renderEuActive) {
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
	}
	else {
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}

	pushVertexColor(redLaneVector, -0.02f, plane, 3.75f, r, g, b);
	pushVertexColor(redLaneVector, 0.02f, plane, 3.75f, r, g, b);
	pushVertexColor(redLaneVector, 0.02f, plane, 0.0f, r, g, b);
	pushVertexColor(redLaneVector, -0.02f, plane, 0.0f, r, g, b);
	pushRectangleIndices(redLaneIndices, redLaneVertexCount);


	int start = cross.at(0).getType();
	int middle = start;

	if (start == CROSS_G) {
		//change color if euphoria is active
		if (m_renderEuActive) {
			r = 1.0;
			g = 1.0;
			b = 1.0;
		}
		else {
			r = 0.0;
			g = 1.0;
			b = 0.0;
		}
		//start green left lane
		pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, 3.75f, r, g, b);
		pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, 3.75f, r, g, b);

		//change color if euphoria is active
		if (m_renderEuActive) {
			r = 1.0;
			g = 1.0;
			b = 1.0;
		}
		else {
			r = 0.0;
			g = 0.0;
			b = 1.0;
		}
		//start blue center lane
		pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, 3.75f, r, g, b);
		pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, 3.75f, r, g, b);
		start = 0;
	}
	else if (start == CROSS_B) {
		//change color if euphoria is active
		if (m_renderEuActive) {
			r = 1.0;
			g = 1.0;
			b = 1.0;
		}
		else {
			r = 0.0;
			g = 1.0;
			b = 0.0;
		}
		//start green center lane
		pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, 3.75f, r, g, b);
		pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, 3.75f, r, g, b);

		//change color if euphoria is active
		if (m_renderEuActive) {
			r = 1.0;
			g = 1.0;
			b = 1.0;
		}
		else {
			r = 0.0;
			g = 0.0;
			b = 1.0;
		}
		//start blue right lane
		pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, 3.75f, r, g, b);
		pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, 3.75f, r, g, b);
	}
	else if (start == CROSS_C) {
		//change color if euphoria is active
		if (m_renderEuActive) {
			r = 1.0;
			g = 1.0;
			b = 1.0;
		}
		else {
			r = 0.0;
			g = 1.0;
			b = 0.0;
		}
		//start green center lane
		pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, 3.75f, r, g, b);
		pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, 3.75f, r, g, b);

		//change color if euphoria is active
		if (m_renderEuActive) {
			r = 1.0;
			g = 1.0;
			b = 1.0;
		}
		else {
			r = 0.0;
			g = 0.0;
			b = 1.0;
		}
		//start blue center lane
		pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, 3.75f, r, g, b);
		pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, 3.75f, r, g, b);
	}

	for (size_t i = 1; i < cross.size(); i++) {
		double t = cross.at(i).getMilli();
		double hitWindow = cross.at(i).hitWindow;
		if (time + m_noteVisibleTime >= t && time <= t + hitWindow) {
			double dt = cross.at(i).getMilli() - time;
			if (cross.at(i).getType() == CROSS_G && middle != CROSS_G) {
				middle = CROSS_G;
				float z = 3.75f - 3.75f * (float)dt;

				//change color if euphoria is active
				if (m_renderEuActive) {
					r = 1.0;
					g = 1.0;
					b = 1.0;
				}
				else {
					r = 0.0;
					g = 1.0;
					b = 0.0;
				}

				//end green center lane
				pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z + offset, r, g, b);
				pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, z + offset, 0.0f, 1.0f, 0.0f);
				pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

				//add horizontal line
				pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z + offset, r, g, b);
				pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
				pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z - offset, r, g, b);
				pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);
				pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

				//start green left lane
				pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z - offset, r, g, b);
				pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, z - offset, r, g, b);

				if (cross.at(i - 1).getType() == CROSS_B) {
					//in the case that the crossfade was right before,
					//add horizontal lane to blue lane too

					//change color if euphoria is active
					if (m_renderEuActive) {
						r = 1.0;
						g = 1.0;
						b = 1.0;
					}
					else {
						r = 0.0;
						g = 0.0;
						b = 1.0;
					}

					//end blue right lane
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, z + offset, r, g, b);
					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					//add horizontal line
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z + offset, r, g, b);
					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					//start blue center lane
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, z - offset, r, g, b);
				}
			}
			else if (cross.at(i).getType() == CROSS_B && middle != CROSS_B) {
				middle = CROSS_B;
				float z = 3.75f - 3.75f * (float)dt;

				//change color if euphoria is active
				if (m_renderEuActive) {
					r = 1.0;
					g = 1.0;
					b = 1.0;
				}
				else {
					r = 0.0;
					g = 0.0;
					b = 1.0;
				}
				//end blue center lane
				pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, z + offset, r, g, b);
				pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z + offset, r, g, b);
				pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

				//add horizontal line
				pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z + offset, r, g, b);
				pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
				pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z - offset, r, g, b);
				pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);
				pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

				//start blue right lane
				pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, z - offset, r, g, b);
				pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z - offset, r, g, b);

				if (cross.at(i - 1).getType() == CROSS_G) {
					//in the case that the crossfade was left before,
					//add horizontal lane to green lane too

					//change color if euphoria is active
					if (m_renderEuActive) {
						r = 1.0;
						g = 1.0;
						b = 1.0;
					}
					else {
						r = 0.0;
						g = 1.0;
						b = 0.0;
					}
					//end green left lane
					pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);
					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					//add horizontal line
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);
					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					//start green center lane
					pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
				}
			}
			else if (cross.at(i).getType() == CROSS_C && middle != CROSS_C) {
				middle = CROSS_C;
				//crossfade center event
				float z = 3.75f - 3.75f * (float)dt;

				//if crossfade was from the right
				if (cross.at(i - 1).getType() == CROSS_G) {
					//change color if euphoria is active
					if (m_renderEuActive) {
						r = 1.0;
						g = 1.0;
						b = 1.0;
					}
					else {
						r = 0.0;
						g = 1.0;
						b = 0.0;
					}
					//end green left lane
					pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);
					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					//add horizontal line
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);
					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					//start green center line
					pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
				}
				//if crossfade was from the left
				else if (cross.at(i - 1).getType() == CROSS_B) {
					//change color if euphoria is active
					if (m_renderEuActive) {
						r = 1.0;
						g = 1.0;
						b = 1.0;
					}
					else {
						r = 0.0;
						g = 0.0;
						b = 1.0;
					}
					//end blue right lane
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, z + offset, r, g, b);
					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					//add horizontal line
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z + offset, r, g, b);
					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					//start blue center lane
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, z - offset, r, g, b);
				}
			}
		}
		else if (t > time + m_noteVisibleTime) {
			if (middle == CROSS_G) {
				m_renderCross = 0;
				//change color if euphoria is active
				if (m_renderEuActive) {
					r = 1.0;
					g = 1.0;
					b = 1.0;
				}
				else {
					r = 0.0;
					g = 1.0;
					b = 0.0;
				}
				//end green left lane
				pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, 0.0f, r, g, b);
				pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, 0.0f, r, g, b);

				//change color if euphoria is active
				if (m_renderEuActive) {
					r = 1.0;
					g = 1.0;
					b = 1.0;
				}
				else {
					r = 0.0;
					g = 0.0;
					b = 1.0;
				}
				//end blue center lane
				pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, 0.0f, r, g, b);
				pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, 0.0f, r, g, b);
			}
			else if (middle == CROSS_B) {
				m_renderCross = 2;
				//change color if euphoria is active
				if (m_renderEuActive) {
					r = 1.0;
					g = 1.0;
					b = 1.0;
				}
				else {
					r = 0.0;
					g = 1.0;
					b = 0.0;
				}
				//end green center lane
				pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, 0.0f, r, g, b);
				pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, 0.0f, r, g, b);

				//change color if euphoria is active
				if (m_renderEuActive) {
					r = 1.0;
					g = 1.0;
					b = 1.0;
				}
				else {
					r = 0.0;
					g = 0.0;
					b = 1.0;
				}
				//end blue right lane
				pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, 0.0f, r, g, b);
				pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, 0.0f, r, g, b);
			}
			else if (middle == CROSS_C) {
				m_renderCross = 1;
				//change color if euphoria is active
				if (m_renderEuActive) {
					r = 1.0;
					g = 1.0;
					b = 1.0;
				}
				else {
					r = 0.0;
					g = 1.0;
					b = 0.0;
				}
				//end green center lane
				pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, 0.0f, r, g, b);
				pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, 0.0f, r, g, b);

				//change color if euphoria is active
				if (m_renderEuActive) {
					r = 1.0;
					g = 1.0;
					b = 1.0;
				}
				else {
					r = 0.0;
					g = 0.0;
					b = 1.0;
				}
				//end blue center lane
				pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, 0.0f, r, g, b);
				pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, 0.0f, r, g, b);
			}


			break;
		}
	}
	if (greenLaneVector.size() == 2 * 7) {
		if (m_renderEuActive) {
			r = 1.0;
			g = 1.0;
			b = 1.0;
		}
		else {
			r = 0.0;
			g = 1.0;
			b = 0.0;
		}
		//end green center lane
		pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, 0.0f, r, g, b);
		pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, 0.0f, r, g, b);
	}
	if (blueLaneVector.size() == 2 * 7) {
		//change color if euphoria is active
		if (m_renderEuActive) {
			r = 1.0;
			g = 1.0;
			b = 1.0;
		}
		else {
			r = 0.0;
			g = 0.0;
			b = 1.0;
		}
		//end blue center lane
		pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, 0.0f, r, g, b);
		pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, 0.0f, r, g, b);
	}

	//finally, render each lane 
	pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);
	pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

	usePersProj();
	renderColor(redLaneVector, redLaneIndices);
	renderColor(greenLaneVector, greenLaneIndices);
	renderColor(blueLaneVector, blueLaneIndices);

}

void GameRender::bpmTicks(double time, std::vector<double>& bpmArr) {
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

		/*
		if the bar is visible on the highway
		exact same as Note.getRender(), but done manually
		because bpm ticks are not Notes/Events
		*/
		if (time + 1.0 >= tickTime && time <= tickTime + 0.2) {
			double dt = tickTime - time;
			float z = 3.75f - (3.75f * (float)dt);

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
}

void GameRender::events(double time, std::vector<Note>& ev, std::vector<Note>& cross) {
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
		double dt = ev.at(i).getMilli() - time;

		if (type == SCR_G_ZONE) {
			std::vector<Note> cfChanges = getCrossInsideNote(ev.at(i), cross);
			int num = 0;
			int lastVisible = -1;
			float z;
			for (size_t i = 0; i < cfChanges.size(); ++i) {
				double cfMilli = cfChanges.at(i).getMilli();
				double cfDt = cfMilli - time;
				int cfType = cfChanges.at(i).getType();
				if (cfMilli <= time + m_noteVisibleTime) {
					z = 3.75f - (3.75f * (float)cfDt);
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
				double cfDt = cfMilli - time;
				int cfType = cfChanges.at(i).getType();
				if (cfMilli <= time + m_noteVisibleTime) {
					z = 3.75f - (3.75f * (float)cfDt);
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
			double endDt = endTime - time;
			bool start_eu = false;
			//if euphoria start is in the middle of the highway
			if (dt >= 0.0 && dt < 1.0) {
				float z = 3.75f - (3.75f * (float)dt);
				pushVertexColor(eventsVector, -1.0f, plane, z, 1.0f, 1.0f, 1.0f, transparency);
				pushVertexColor(eventsVector, 1.0f, plane, z, 1.0f, 1.0f, 1.0f, transparency);
				start_eu = true;
			}
			//if euphoria start has already passed the clicker
			else if (dt < 0.0) {
				//if (m_renderEuZone) {
				pushVertexColor(eventsVector, -1.0, plane, 3.75, 1.0, 1.0, 1.0, transparency);
				pushVertexColor(eventsVector, 1.0, plane, 3.75, 1.0, 1.0, 1.0, transparency);
				start_eu = true;
				//}
			}
			//if the render has successfully added the start
			if (start_eu) {
				//if euphoria end is in the middle of the highway
				if (endDt >= 0.0 && endDt < 1.0) {
					float z = 3.75f - (3.75f * (float)endDt);
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
}

void GameRender::meters() {
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

	//score meter
	std::string s = std::to_string(m_playerScore);
	std::string scoreDisplay("00000000");
	scoreDisplay.resize(scoreDisplay.length() - s.length());
	scoreDisplay.append(s);
	drawText(scoreDisplay, 940.0f, 230.0f, 0.05f);

	//combo meter
	if (m_playerCombo >= 15) {
		std::string c = std::to_string(m_playerCombo);
		drawText(c, 940.0f, 280.0f, 0.03f);
	}
}

void GameRender::result(Player& player, Generator& generator) {
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
		float x = 1270.0f - getTextWidth(t,scale);
		drawText(t, x, 100.0f, scale);
	}
	else if (hit > all) {
		std::string t = "Hold on a sec. How did you...";
		float x = 1270.0f - getTextWidth(t, scale);
		drawText(t, x, 100.0f, scale);
	}
}

void GameRender::debug(std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& c) {
	/*
	std::string text = "Notes:";
	for (size_t i = 0; i < note_arr.size(); i++) {
		if (i > 15)break;
		int t = note_arr.at(i).getType();
		double m = note_arr.at(i).getMilli();
		text += std::to_string(t);
		text += "(";
		text += std::to_string(m);
		text += ")";
		text += ",";
	}
	drawText(text, 0.0f, 0.0f, 0.05f);
	*/
	
	/*
	std::string t2 = "Events:";
	for (size_t i = 0; i < ev.size(); i++) {
		int t = ev.at(i).getType();
		std::string text = std::to_string(t);

		t2.append(text);
		t2.append(",");

		float x = getTextWidth(t2.c_str(), 0.05f);
		if (x > 960.0f)break;
	}
	//drawText(t2, 0.0f, 40.0f, 0.05f);
	*/
	/*
	std::string cs = "Cross:";
	for (size_t i = 0; i < c.size(); i++) {
		if (i > 15)break;
		int t = (int)c.at(i).getMilli();
		std::string text = std::to_string(t);

		cs.append(text);
		cs.append(",");
	}

	drawText(cs, 0.0f, 0.0f, 0.05f);
	//std::cout << t2 << std::endl;
	*/

	std::string sizes;
	sizes += std::string("Notes:") + std::to_string(v.size()) + std::string("|");
	sizes += std::string("Events:") + std::to_string(ev.size()) + std::string("|");
	sizes += std::string("Cross:") + std::to_string(c.size());
	drawText(sizes, 0.0f, 0.0f, 0.05f);

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

	bool greenAnimEnabled = p.m_greenAnimation;
	bool redAnimEnabled = p.m_redAnimation;
	bool blueAnimEnabled = p.m_blueAnimation;

	if (p.m_pastCross >= 1 && p.m_cross == 0) {
		m_animManager.triggerAnimation(AN_CROSS_GREEN_TO_LEFT, time);
		if (p.m_pastCross == 2)m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_CENTER, time);
	}
	else if (p.m_pastCross <= 1 && p.m_cross == 2) {
		m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_RIGHT, time);
		if (p.m_pastCross == 0) m_animManager.triggerAnimation(AN_CROSS_GREEN_TO_CENTER, time);
	}

	if (p.m_cross == 1) {
		if (p.m_pastCross == 0)m_animManager.triggerAnimation(AN_CROSS_GREEN_TO_CENTER, time);
		else if (p.m_pastCross == 2)m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_CENTER, time);
	}

	if (greenAnimEnabled) {
		m_animManager.triggerAnimation(AN_GREEN_CLICKER, time);
		p.m_greenAnimation = false;
	}
	if (redAnimEnabled) {
		m_animManager.triggerAnimation(AN_RED_CLICKER, time);
		p.m_redAnimation = false;
	}if (blueAnimEnabled) {
		m_animManager.triggerAnimation(AN_BLUE_CLICKER, time);
		p.m_blueAnimation = false;
	}
	rendr_InvertedX = m_isButtonsRight;
}

void GameRender::clickerAnimation() {
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

		pushVertexTexture(clickerVector, m_greenLeft, plane, m_greenBack, x, y);
		pushVertexTexture(clickerVector, m_greenLeft, plane, m_greenFront, x, y - 0.25f);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_greenFront, x + 0.1, y - 0.25f);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_greenBack, x + 0.1, y);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	if (redAnim.isEnabled()) {
		double x = redAnim.getPercent();
		x = floor(x * 10) / 10;

		float y = 0.75f;
		if (m_renderEuActive)y = 0.25f;

		pushVertexTexture(clickerVector, -0.15f, plane, 3.6f, x, y);
		pushVertexTexture(clickerVector, -0.15f, plane, 3.9f, x, y - 0.25f);
		pushVertexTexture(clickerVector, 0.15f, plane, 3.9f, x + 0.1f, y - 0.25f);
		pushVertexTexture(clickerVector, 0.15f, plane, 3.6f, x + 0.1f, y);

		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	if (blueAnim.isEnabled()) {
		double x = blueAnim.getPercent();
		x = floor(x * 10) / 10;

		float y = 0.5f;
		if (m_renderEuActive)y = 0.25f;

		pushVertexTexture(clickerVector, m_blueLeft, plane, m_blueBack, x, y);
		pushVertexTexture(clickerVector, m_blueLeft, plane, m_blueFront, x, y - 0.25f);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_blueFront, x + 0.1f, y - 0.25f);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_blueBack, x + 0.1f, y);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	usePersProj();
	renderTexture(clickerVector, clickerIndices, m_clickerAnimation);
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

	if (note.getLanMod() == 0)lastChange = CROSS_G;
	else if (note.getLanMod() == 2)lastChange = CROSS_B;

	result.push_back(Note(start, lastChange, 0.0f, true));
	for (size_t i = 0; i < cross.size(); ++i) {
		double crossMilli = cross.at(i).getMilli();
		int crossType = cross.at(i).getType();
		if (crossMilli <= end && crossMilli >= start) {
			if (crossType == CROSS_G && lastChange != CROSS_G) {
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
				lastChange = CROSS_G;
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
			}
			else if (crossType == CROSS_C && lastChange != CROSS_C) {
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
				lastChange = CROSS_C;
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
			}
			else if (crossType == CROSS_B && lastChange != CROSS_B) {
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
				lastChange = CROSS_B;
				result.push_back(Note(crossMilli, lastChange, 0.0f, true));
			}
		}
	}
	result.push_back(Note(end, lastChange, 0.0f, true));
	return result;
}



GameRender::~GameRender() {
}
