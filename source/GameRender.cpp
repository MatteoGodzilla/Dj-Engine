#include "GameRender.h"

GameRender::GameRender() {
	//ctor
}

void GameRender::init(GLFWwindow* w) {
	Rendr::init(w);

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

void GameRender::clicker(){
	//difference in size between pressed and not
	float clickedOffset = 0.03f;

	float plane = 0.0f;

	//vertices data
	std::vector<float> clickerVector = {};
	std::vector<unsigned int> clickerIndices = {};
	unsigned int clickerVertexCount = 0;

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
			//green clicker is on the center
			pushVertexTexture(clickerVector, -0.5f + clickedOffset, plane, 3.6f + clickedOffset, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.5f + clickedOffset, plane, 3.9f - clickedOffset, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f - clickedOffset, plane, 3.9f - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f - clickedOffset, plane, 3.6f + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			//green clicker is on the left
			pushVertexTexture(clickerVector, -0.85f + clickedOffset, plane, 3.6f + clickedOffset, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.85f + clickedOffset, plane, 3.9f - clickedOffset, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f - clickedOffset, plane, 3.9f - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f - clickedOffset, plane, 3.6f + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	else {
		//green clicker is not pressed
		if (m_playerCross >= 1) {
			//green clicker is on the center
			pushVertexTexture(clickerVector, -0.5f, plane, 3.6f, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.5f, plane, 3.9f, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f, plane, 3.9f, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f, plane, 3.6f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			//green clicker is on the left
			pushVertexTexture(clickerVector, -0.85f, plane, 3.6f, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.85f, plane, 3.9f, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f, plane, 3.9f, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f, plane, 3.6f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	if (m_blue) {
		//blue clicker is pressed
		if (m_playerCross <= 1) {
			//blue clicker is on the center
			pushVertexTexture(clickerVector, 0.5f - clickedOffset, plane, 3.6f + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.5f - clickedOffset, plane, 3.9f - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f + clickedOffset, plane, 3.9f - clickedOffset, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f + clickedOffset, plane, 3.6f + clickedOffset, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			//blue clicker is on the right
			pushVertexTexture(clickerVector, 0.85f - clickedOffset, plane, 3.6f + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.85f - clickedOffset, plane, 3.9f - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f + clickedOffset, plane, 3.9f - clickedOffset, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f + clickedOffset, plane, 3.6f + clickedOffset, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	else {
		//blue clicker is not pressed
		if (m_playerCross <= 1) {
			//blue clicker is on the center
			pushVertexTexture(clickerVector, 0.5f, plane, 3.6f, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.5f, plane, 3.9f, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f, plane, 3.9f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f, plane, 3.6f, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			//blue clicker is on the left
			pushVertexTexture(clickerVector, 0.85f, plane, 3.6f, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.85f, plane, 3.9f, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f, plane, 3.9f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f, plane, 3.6f, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	usePersProj();
	renderTexture(clickerVector, clickerIndices, m_objTexture);
}

void GameRender::notes(double time, std::vector<Note>& v){
	float plane = 0.0;

	//vertices data
	std::vector<float> noteVector = {};
	std::vector<unsigned int> noteIndices = {};
	unsigned int noteVertexCount = 0;

	//loop for every note inside vector
	for (size_t i = 0; i < v.size(); i++) {
		double milli = v.at(i).getMilli();
		double hitWindow = v.at(i).hitWindow;
		if (time+m_noteVisibleTime >= milli && time <= milli+hitWindow) {
			//if the note is inside the visible highway

			//calculate 'height' of note
			double dt = v.at(i).getMilli() - time;
			float z = 3.75f - (3.75f * (float)dt);

			int type = v.at(i).getType();

			//texture coordinates (bottom left cause OpenGl)
			float s = 0.0;
			float t = 0.0;

			if (type == TAP_R) {
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
			else if (type == TAP_G) {
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
			else if (type == SCR_G_UP) {
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
			else if (type == SCR_G_DOWN) {
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
			else if (type == TAP_B) {
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
			else if (type == SCR_B_UP) {
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
			else if (type == SCR_B_DOWN) {
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
			
		}
		else {
			//if the note is outside the visible area, update lane position
			v.at(i).setLanMod(m_renderCross);
		}
	}
	usePersProj();
	renderTexture(noteVector, noteIndices, m_objTexture);
}

void GameRender::lanes(double time, std::vector<Note>& v, std::vector<Note>& ev)
{
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

	std::vector<float> spikesVector = {};
	std::vector<unsigned int>spikesIndices = {};
	unsigned int spikesVertexCount = 0;


	float r, g, b;

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

	int start = m_renderCross;

	/*

	the lanes rendering is done in 3 separate loops:
	start -> middle -> end
	(start is down in the highway, end is up)

	*/

	//loop for the start section
	for (size_t i = 0; i < ev.size(); i++) {
		if (ev.at(i).getMilli() <= time) {
			//if event is visible in the highway

			if (ev.at(i).getType() == CROSS_G) {
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
				break;
			}
			else if (ev.at(i).getType() == CROSS_B) {
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
				start = 2;
				break;
			}
			else if(ev.at(i).getType() == CROSS_C){
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
				start = 1;
				break;
			}
		}
	}

	int middle = start;
	float offset = 0.04f;

	//loop for the middle section
	for (size_t i = 0; i < ev.size(); i++) {
		double milli = ev.at(i).getMilli();
		double hitWindow = ev.at(i).hitWindow;
		if (time + m_noteVisibleTime >= milli && time <= milli + hitWindow){
			//if the event is inside the highway

			double dt = ev.at(i).getMilli() - time;
			if (ev.at(i).getType() == CROSS_G) {
				float z = 3.75f - 3.75f * (float)dt;
				if (middle >= 1) {
					//if the crossfade was center or right, 
					//add the crossfade change in the green lane

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
				}
				if (middle == 2) {
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
				middle = 0;
			}
			else if (ev.at(i).getType() == CROSS_B)
			{
				float z = 3.75f - 3.75f * (float)dt;

				if (middle <= 1) {
					//if the crossfade was center or left, 
					//add the crossfade change in the blue lane

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
				}
				if (middle == 0) {
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
				middle = 2;
			}
			else if(ev.at(i).getType() == CROSS_C){
				//crossfade center event
				float z = 3.75f - 3.75f * (float)dt;

				//if crossfade was from the right
				if (middle == 0) {
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
				else if (middle == 2) {
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
				middle = 1;
			}
		}
	}

	int end = 1;
	//loop for the end section
	//find the last visible crossfade change
	for (size_t i = 0; i < ev.size(); i++) {
		if (ev.at(i).getMilli() <= time + 1.0f) {
			if (ev.at(i).getType() == CROSS_G) {
				end = 0;
			}
			else if (ev.at(i).getType() == CROSS_B) {
				end = 2;
			}
			else if (ev.at(i).getType() == CROSS_C){
				end = 1;
			}
		}
	}

	if (end == 0) {
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
	else if (end == 2) {
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
	else if (end == 1) {
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

	//set m_renderCross for other functions
	m_renderCross = end;

	//finally, render each lane 
	pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);
	pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

	for (size_t i = 0; i < v.size(); i++) {
		double milli = v.at(i).getMilli();
		double hitWindow = v.at(i).hitWindow;
		if (time + m_noteVisibleTime >= milli && time <= milli + hitWindow) {
			int type = v.at(i).getType();
			double dt = v.at(i).getMilli() - time;
			float z = 3.75f - (3.75f * (float)dt);

			float s, t;

			if (type == CF_SPIKE_G) {
				if (v.at(i).getLanMod() == 1) {
					s = 1200.0f / 1740.0f;
					t = 1460.0f / 1640.0f;

					pushVertexTexture(spikesVector, -0.7f, plane, z - 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(spikesVector, -0.7f, plane, z + 0.1f, s, t);
					pushVertexTexture(spikesVector, -0.35f, plane, z + 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(spikesVector, -0.35f, plane, z - 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushRectangleIndices(spikesIndices, spikesVertexCount);
				}
			}
			else if (type == CF_SPIKE_B) {
				if (v.at(i).getLanMod() == 1) {
					s = 1200.0f / 1740.0f;
					t = 1280.0f / 1640.0f;

					pushVertexTexture(spikesVector, 0.35f, plane, z - 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(spikesVector, 0.35f, plane, z + 0.1f, s, t);
					pushVertexTexture(spikesVector, 0.7f, plane, z + 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(spikesVector, 0.7f, plane, z - 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushRectangleIndices(spikesIndices, spikesVertexCount);
				}
			}
			else if (type == CF_SPIKE_C) {
				if (v.at(i).getLanMod() == 0) {
					s = 1200.0f / 1740.0f;
					t = 1460.0f / 1640.0f;

					pushVertexTexture(spikesVector, -0.7f, plane, z - 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(spikesVector, -0.7f, plane, z + 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushVertexTexture(spikesVector, -0.35f, plane, z + 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(spikesVector, -0.35f, plane, z - 0.1f, s, t);
					pushRectangleIndices(spikesIndices, spikesVertexCount);

					pushRectangleIndices(spikesIndices, spikesVertexCount);
				}
				else if (v.at(i).getLanMod() == 2) {
					s = 1200.0f / 1740.0f;
					t = 1280.0f / 1640.0f;

					pushVertexTexture(spikesVector, 0.35f, plane, z - 0.1f, s + 560.0f / 1760.0f, t);
					pushVertexTexture(spikesVector, 0.35f, plane, z + 0.1f, s + 560.0f / 1760.0f, t + 180.0f / 1640.0f);
					pushVertexTexture(spikesVector, 0.7f, plane, z + 0.1f, s, t + 180.0f / 1640.0f);
					pushVertexTexture(spikesVector, 0.7f, plane, z - 0.1f, s, t);
					pushRectangleIndices(spikesIndices, spikesVertexCount);


				}
			}
		}
	}


	usePersProj();
	renderColor(redLaneVector, redLaneIndices);
	renderColor(greenLaneVector, greenLaneIndices);
	renderColor(blueLaneVector, blueLaneIndices);

	renderTexture(spikesVector, spikesIndices, m_objTexture);
}

void GameRender::bpmTicks(double time, std::vector<double>& bpmArr){
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

void GameRender::events(double time, std::vector<Note>& ev) {
	float plane = 0.0;
	float transparency = 0.5; // euphoria transparency

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

			double endTime = ev.at(i).getMilli() + ev.at(i).getLength();
			double endDt = endTime - time;
			bool startDrawing = false;

			if (dt >= 0.0 && dt < 1.0) {
				float z = 3.75f - (3.75f * (float)dt);
				if (ev.at(i).getLanMod() == 0) {
					pushVertexColor(eventsVector, -0.85f, plane, z + 0.3f, 0.0f, 1.0f, 0.0f);
					pushVertexColor(eventsVector, -0.55f, plane, z + 0.3f, 0.0f, 1.0f, 0.0f);
				}
				else {
					pushVertexColor(eventsVector, -0.5f, plane, z + 0.3f, 0.0f, 1.0f, 0.0f);
					pushVertexColor(eventsVector, -0.2f, plane, z + 0.3f, 0.0f, 1.0f, 0.0f);
				}
				startDrawing = true;
			}
			//when the start has already passed the clickers
			else if (dt < 0.0) {
				if (ev.at(i).getLanMod() == 0) {
					pushVertexColor(eventsVector, -0.85f, plane, 3.75f, 0.0f, 1.0f, 0.0f);
					pushVertexColor(eventsVector, -0.55f, plane, 3.75f, 0.0f, 1.0f, 0.0f);
				}
				else {
					pushVertexColor(eventsVector, -0.5f, plane, 3.75f, 0.0f, 1.0f, 0.0f);
					pushVertexColor(eventsVector, -0.2f, plane, 3.75f, 0.0f, 1.0f, 0.0f);
				}
				startDrawing = true;
			}
			if (startDrawing) {
				//when the end is in the middle of the highway
				if (endDt > 0.0 && endDt <= 1.0) {
					float z = 3.75f - (3.75f * (float)endDt);
					if (ev.at(i).getLanMod() == 0) {
						pushVertexColor(eventsVector, -0.55f, plane, z - 0.3f, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.85f, plane, z - 0.3f, 0.0f, 1.0f, 0.0f);
					}
					else {
						pushVertexColor(eventsVector, -0.2f, plane, z - 0.3f, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.5f, plane, z - 0.3f, 0.0f, 1.0f, 0.0f);
					}
					pushRectangleIndices(eventsIndices, eventsVertexCount);
				}
				//when the end is beyond what is visible
				else if (endDt > 1.0) {
					if (ev.at(i).getLanMod() == 0) {
						pushVertexColor(eventsVector, -0.55f, plane, 0.0f, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.85f, plane, 0.0f, 0.0f, 1.0f, 0.0f);
					}
					else {
						pushVertexColor(eventsVector, -0.2f, plane, 0.0f, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.5f, plane, 0.0f, 0.0f, 1.0f, 0.0f);
					}
					pushRectangleIndices(eventsIndices, eventsVertexCount);
				}
			}

		}
		if (type == SCR_B_ZONE) {
			double endTime = ev.at(i).getMilli() + ev.at(i).getLength();
			double endDt = endTime - time;
			bool startDrawing = false;

			//when the end is in the middle of the highway
			if (dt >= 0.0 && dt < 1.0) {
				float z = 3.75f - (3.75f * (float)dt);
				if (ev.at(i).getLanMod() == 2) {
					pushVertexColor(eventsVector, 0.55f, plane, z + 0.3f, 0.0f, 0.0f, 1.0f);
					pushVertexColor(eventsVector, 0.85f, plane, z + 0.3f, 0.0f, 0.0f, 1.0f);
				}
				else {
					pushVertexColor(eventsVector, 0.2f, plane, z + 0.3f, 0.0f, 0.0f, 1.0f);
					pushVertexColor(eventsVector, 0.5f, plane, z + 0.3f, 0.0f, 0.0f, 1.0f);
				}
				startDrawing = true;
			}
			//when the start has already passed the clickers
			else if (dt < 0.0) {
				if (ev.at(i).getLanMod() == 2) {
					pushVertexColor(eventsVector, 0.55f, plane, 3.75f, 0.0f, 0.0f, 1.0f);
					pushVertexColor(eventsVector, 0.85f, plane, 3.75f, 0.0f, 0.0f, 1.0f);
				}
				else {
					pushVertexColor(eventsVector, 0.2f, plane, 3.75f, 0.0f, 0.0f, 1.0f);
					pushVertexColor(eventsVector, 0.5f, plane, 3.75f, 0.0f, 0.0f, 1.0f);
				}
				startDrawing = true;
			}
			//if the render successfully added the start
			if (startDrawing) {
				//when the end is in the middle of the highway
				if (endDt > 0.0 && endDt <= 1.0) {
					float z = 3.75f - (3.75f * (float)endDt);
					if (ev.at(i).getLanMod() == 2) {
						pushVertexColor(eventsVector, 0.85f, plane, z + 0.3f, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.55f, plane, z + 0.3f, 0.0f, 0.0f, 1.0f);
					}
					else {
						pushVertexColor(eventsVector, 0.5f, plane, z + 0.3f, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.2f, plane, z + 0.3f, 0.0f, 0.0f, 1.0f);
					}
					pushRectangleIndices(eventsIndices, eventsVertexCount);
				}
				//when the end is beyond what is visible
				else if (endDt > 1.0) {
					if (ev.at(i).getLanMod() == 2) {
						pushVertexColor(eventsVector, 0.85f, plane, 0.0f, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.55f, plane, 0.0f, 0.0f, 0.0f, 1.0f);
					}
					else {
						pushVertexColor(eventsVector, 0.5f, plane, 0.0f, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.2f, plane, 0.0f, 0.0f, 0.0f, 1.0f);
					}
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

		if(milli > time + m_noteVisibleTime){
			ev.at(i).setLanMod(m_renderCross);
		}
	}
	usePersProj();
	renderColor(eventsVector, eventsIndices);
}

void GameRender::meters(){
	float yPlane = 0.1f;

	//vertices data
	std::vector<float>metersVector;
	std::vector<unsigned int> metersIndices;
	unsigned int metersVertexCount = 0;

	//multiplier display
	if (m_playerMult == 2) {
		//render a 'x2'
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 0.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 0.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 3) {
		//render a 'x3'
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 400.0f / 1000.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 800.0f / 1000.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 800.0f / 1000.0f, 800.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 4) {
		//render a 'x4'
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 0.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 0.0f, 0.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 0.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 400.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 6) {
		//render a 'x6'
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 0.0f, 1.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 0.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 1.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 8) {
		//render a 'x8'
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 400.0f / 1000.0f, 1.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 800.0f / 1000.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 800.0f / 1000.0f, 1.0f);
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
	drawText(scoreDisplay.c_str(), 940.0f, 230.0f, 0.05f);

	//combo meter
	if (m_playerCombo >= 15) {
		std::string c = std::to_string(m_playerCombo);
		drawText(c.c_str(), 940.0f, 260.0f, 0.03f);
	}
}

void GameRender::result(Player& player,Generator& generator) {
	useOrthoProj();
	drawText("Result:", 0.0f, 50.0f, 0.05f);

	int all = generator.getNotesTotal();
	int hit = generator.getNotesHit();
	int score = player.getScore();
	int combo = player.getHighCombo();

	std::string hitString = std::string("Number of notes hit:") + std::to_string(hit);
	std::string totalString = std::string("Number of total notes:") + std::to_string(all);
	std::string scoreString = std::string("Score:") + std::to_string(score);
	std::string comboString = std::string("Max Combo:") + std::to_string(combo);


	drawText(hitString.c_str(), 0.0f, 100.0f, 0.05f);
	drawText(totalString.c_str(), 0.0f, 150.0f, 0.05f);
	drawText(scoreString.c_str(), 0.0f, 200.0f, 0.05f);
	drawText(comboString.c_str(), 0.0f, 250.0f, 0.05f);


	if (combo == all) {
		drawText("Wow! a Full Combo", 200.0f, 50.0f, 0.05f);
	}
}

void GameRender::debug(std::vector<Note>& note_arr, std::vector<Note>& ev) {	
	/*
	std::string text = "Notes:";
	for (size_t i = 0; i < note_arr.size()/10; i++) {
		int t = note_arr.at(i).getType();
		text.append(std::to_string(t));
		text.append(",");
	}
	drawText(text.c_str(), 0, 40, 0.05);
	*/
	
	/*
	std::string t2 = "Events:";
	for (size_t i = 0; i < ev.size()/10; i++) {
		int t = ev.at(i).getType();
		t2.append(std::to_string(t));
		t2.append(",");
	}
	drawText(t2.c_str(), 0, 40, 0.05);
	*/
	//std::cout << t2 << std::endl;
	
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
}


GameRender::~GameRender() {
	//dtor
}
