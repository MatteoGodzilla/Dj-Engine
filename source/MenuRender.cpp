#include "MenuRender.h"

void MenuRender::init(GLFWwindow* w) {
	Rendr::init(w);

	m_window = w;
	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW INIT ERROR" << std::endl;
		return;
	}
	setTextColor(0.9f, 0.9f, 0.9f, 1.0f);
	std::cout << "MenuRender init: " << glGetString(GL_VERSION) << std::endl;
	loadTexture("res/buttons.png", &m_buttonTexture);
	m_logoDimensions = loadTexture("res/splashArt.png", &m_splashTexture);
	loadTexture("res/calibration.png", &m_calibrationTex);
	loadTexture("res/pgBar-frame.png", &m_pgBarFrame);

	m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/NotoSans-Regular.ttf", 24.0f);
	m_pastTime = glfwGetTime();
}

void MenuRender::tick() {
	double nowTime = glfwGetTime();
	m_dTime = nowTime - m_pastTime;
	m_globalTime += m_dTime;
	m_pastTime = nowTime;
}

void MenuRender::render(MenuNode node, int selected, int vOffset) {
	//enable exit from remapping menu
	m_shouldClose = false;
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//vertices data
	std::vector<float> selVector;
	std::vector<unsigned int> selIndices;
	unsigned int selVertexCount = 0;

	//text scale
	float scale = 0.075f;

	//selection color
	float r = 0.83f;
	float g = 0.35f;
	float b = 0.24f;

	float right = 0.0f;
	int heightIndex = 0;
	float selHeight = 0.0f;

	std::vector<MenuNode> list = node.getChildrens();
	if (node.getChildCount() > 0) {
		MenuNode m = list.at(selected);
		right = getTextWidth(m.getText(), scale);
		heightIndex = selected - vOffset;
		selHeight = getTextHeight(list.at(selected).getText(), scale);
	}
	useOrthoProj();

	//selection rectangle
	if (10.0f - m_selectionDX + right > 1270.0f) {
		m_currentIdleTime += m_dTime;
		if (m_currentIdleTime > m_timeBeforeAnimating) {
			m_selectionDX = 150.0f * (float)(m_currentIdleTime - m_timeBeforeAnimating);
		}
	}

	pushVertexColor(selVector, 10.0f - m_selectionDX, 200.0f + 1000.0f * scale * (float)heightIndex, 0.0f, r, g, b);
	pushVertexColor(selVector, 10.0f - m_selectionDX, 200.0f + 1000.0f * scale * (float)heightIndex + selHeight, 0.0f, r, g, b);
	pushVertexColor(selVector, 10.0f - m_selectionDX + right, 200.0f + 1000.0f * scale * (float)heightIndex + selHeight, 0.0f, r, g, b);
	pushVertexColor(selVector, 10.0f - m_selectionDX + right, 200.0f + 1000.0f * scale * (float)heightIndex, 0.0f, r, g, b);
	pushRectangleIndices(selIndices, selVertexCount);
	renderColor(selVector, selIndices);

	if (node.getChildCount() > 0) {
		//draw every child from node

		if (node.getChildCount() < VISIBLE_ENTRIES) {
			for (size_t i = 0; i < node.getChildCount(); i++) {
				if (m_currentIdleTime > m_timeBeforeAnimating) {
					drawText(list.at(i).getText(), 10.0f - m_selectionDX, 1000.0f * scale * i + 200.0f, scale);
				} else {
					drawText(list.at(i).getText(), 10.0f, 1000.0f * scale * i + 200.0f, scale);
				}
			}
		} else {
			for (size_t i = 0; i < VISIBLE_ENTRIES; i++) {
				if (m_currentIdleTime > m_timeBeforeAnimating && i + vOffset == selected) {
					drawText(list.at(i + vOffset).getText(), 10.0f - m_selectionDX, 1000.0f * scale * i + 200.0f, scale);
				} else {
					drawText(list.at(i + vOffset).getText(), 10.0f, 1000.0f * scale * i + 200.0f, scale);
				}
				//drawText(list.at(i + vOffset).getText(), 10.0f, 100.0f * i + 200.0f, scale);
			}
		}
	}
	drawText(node.getText(), 10.0f, 100.0f, 0.05f);

	if (node.getChildCount() > VISIBLE_ENTRIES) {
		if (vOffset > 0) {
			drawText("^", 10.0f, 150.0f, scale);
		}
		if (vOffset < node.getChildCount() - VISIBLE_ENTRIES) {
			float h = getTextHeight("v", scale);
			drawText("v", 10.0f, 700.0f - h, scale);
		}
	}
}

void MenuRender::remapping(Game* game, menuinputs input) {
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

	ImGui::Text("Using: ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("Choose Input", m_inputSelection.c_str())) {
		if (ImGui::Selectable("Keyboard")) {
			game->getPlayer()->m_useKeyboardInput = true;
			m_input = true;
			m_inputSelection = "Keyboard";
		}
		for (int i = 0; i < 16; ++i) {
			std::string name;
			if (glfwJoystickPresent(i)) {
				name = glfwGetJoystickName(i);
				std::string t = "Id " + std::to_string(i) + ":" + name;
				if (ImGui::Selectable(t.c_str())) {
					game->getPlayer()->m_useKeyboardInput = false;
					game->getPlayer()->m_gamepadId = i;
					m_input = false;
					m_inputSelection = t;
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
				ImGui::Text(std::to_string(i).c_str());
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
				ImGui::Text(std::to_string(i).c_str());
				ImGui::NextColumn();
				ImGui::ProgressBar(values.at(i), ImVec2(0.0f, 0.0f), std::to_string(values.at(i)).c_str());
				ImGui::NextColumn();
			}
			ImGui::Columns();
			ImGui::EndPopup();
		}
	}

	if (game->getPlayer()->m_gpState.size() >= 8 && game->getPlayer()->m_gpMult.size() >= 8 && game->getPlayer()->m_gpDead.size() >= 8) {
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
		if (game->getPlayer()->m_useKeyboardInput) {
			value = game->getPlayer()->getKBMValues(m_window).at(*input.uk);
		} else {
			value = game->getPlayer()->getGamepadValues().at(*input.ug);
		}
		if (value > 0) {
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
			ImGui::Text("Push a button / move an axis for 'Menu Up' ");
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
			ImGui::InputInt("##meup", input.uk);
		} else {
			ImGui::InputInt("##meup", input.ug);
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::NextColumn();
		ImGui::NextColumn();

		//Menu Down
		if (game->getPlayer()->m_useKeyboardInput) {
			value = game->getPlayer()->getKBMValues(m_window).at(*input.dk);
		} else {
			value = game->getPlayer()->getGamepadValues().at(*input.dg);
		}
		if (value > 0) {
			ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "Menu down:");
		} else {
			ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Menu down:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##091");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(MENU_DOWN);
			} else {
				editingAxisController(MENU_DOWN);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Down' ");
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
			ImGui::InputInt("##medown", input.dk);
		} else {
			ImGui::InputInt("##medown", input.dg);
		}
		ImGui::NextColumn();
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::NextColumn();
		ImGui::NextColumn();

		//Menu Select
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

		ImGui::Columns();
	}

	ImGui::Separator();
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

	drawText("on twitter (@MatteoGodzilla) with '#truedj')", x, y, fontsize / 1000.0f);
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
	std::vector<float> vector;
	std::vector<unsigned int> indices;
	unsigned int indexCount = 0;

	float width = 700.0;
	float height = width * (m_logoDimensions.y / m_logoDimensions.x);
	float x = 1280.0f / 5 * 2;
	float y = (720.0f - height) / 2;
	pushVertexTexture(vector, x, y, 0.0, 0.0, 1.0);
	pushVertexTexture(vector, x, y + height, 0.0, 0.0, 0.0);
	pushVertexTexture(vector, x + width, y + height, 0.0, 1.0, 0.0);
	pushVertexTexture(vector, x + width, y, 0.0, 1.0, 1.0);
	pushRectangleIndices(indices, indexCount);

	useOrthoProj();
	renderTexture(vector, indices, m_splashTexture);

	float textScale = 0.02f;
	drawText("*This is alpha v1.4. There are still some bugs left,*", 10.0, 10.0, textScale);
	drawText("*but I think most of them should be fixed by now*", 10.0, 30.0, textScale);
	drawText("*Have Fun! :)*", 10.0, 50.0, textScale);

	std::string discord = "For any questions, ask on the Dj Hero Discord";
	std::string discord2 = "https://discord.gg/HZ82gKR";
	std::string donation = "If you want to support the development, you can donate at";
	std::string donation2 = "paypal.me/MatteoGodzilla";
	drawText(discord, 1270.0f - getTextWidth(discord, textScale), 10.0f, textScale);
	drawText(discord2, 1270.0f - getTextWidth(discord2, textScale), 30.0f, textScale);
	drawText(donation, 1270.0f - getTextWidth(donation, textScale), 50.0f, textScale);
	drawText(donation2, 1270.0f - getTextWidth(donation2, textScale), 70.0f, textScale);

	std::string remap = std::string("Press spacebar to enter Remapping screen");
	drawText(remap, (1280.0f - getTextWidth(remap, 0.03f)) / 2.0f, 680.0f, 0.03f);
}

void MenuRender::scratches(Player* player) {
	useOrthoProj();
	drawText("Here you can test your scatches", 20.0, 20.0, 0.05f);

	/*
	if(player->m_useKeyboardInput){
		player->updateKBMState(m_window);
	}
	else{
		player->updateGamepadState();
	}
	if (player->getFallingZero(SCR_UP_INDEX)) {
		m_testBuffer.push_back('^');
	}
	if (player->getFallingZero(SCR_DOWN_INDEX)) {
		m_testBuffer.push_back('v');
	}
	if (m_testBuffer.size() > 20) {
		m_testBuffer.erase(0, 1);
	}
*/

	drawText(m_testBuffer, 20.0f, 310.0f, 0.1f);
	drawText("Press Menu Back to exit", 20.0, 670.0f, 0.05f);
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
		game->getAudio()->reset();
		m_cbPlayingTime = 0.0;
		m_latencyHits.clear();
		game->getAudio()->load("res/calibration.ogg");
		game->getAudio()->play();
	}
	if (game->getAudio()->isPlaying()) {
		game->getAudio()->buffer(m_cbPlayingTime);
		m_cbPlayingTime += dt;
	} else {
		game->getAudio()->reset();
		m_isCalibrating = false;
		m_cbPlayingTime = 0.0;

		if (!m_latencyHits.empty()) {
			double sum = 0.0;
			for (size_t i = 0; i < m_latencyHits.size(); ++i) {
				sum += m_latencyHits.at(i) - (2.0 + 0.5 * i);
			}
			game->m_audioLatency = (float)(sum / m_latencyHits.size());
			m_latencyHits.clear();
		}
	}
	ImGui::SameLine();
	ImGui::SliderFloat("Current audio latency (in ms.)", &(game->m_audioLatency), 0.0f, 2.0f);
	ImGui::Text("When calibrationg, you are going to hear 4 bass hits, then 8 snare hits and then a ding.");
	ImGui::Text("Press the Spacebar when you hear the snare hits WITHOUT pressing for the ding");
	ImGui::Text("(on zero latency the click should match the lights below on wired headphones)");

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
