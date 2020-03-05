#include "MenuRender.h"

MenuRender::MenuRender()
{
}

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
	loadTexture("res/splashArt.png", &m_splashTexture);
	loadTexture("res/calibration.png", &m_calibrationTex);

	m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/NotoSans-Regular.ttf", 24.0f);
	m_pastTime = glfwGetTime();
}

void MenuRender::tick() {
	double nowTime = glfwGetTime();
	m_dTime = nowTime - m_pastTime;
	m_globalTime += m_dTime;
	m_pastTime = nowTime;
}

void MenuRender::render(MenuNode menu, int selected, unsigned int vOffset) {
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

	std::vector<MenuNode> list = menu.getChildrens();
	if (menu.getChildCount() > 0) {
		MenuNode m = list.at(selected);
		right = getTextWidth(m.getText().c_str(), scale);
		heightIndex = selected - vOffset;
		selHeight = getTextHeight(list.at(selected).getText(), scale);
	}
	useOrthoProj();

	//selection rectangle
	if (10.0f - m_selectionDX + right > 1270.0f) {
		m_currentIdleTime += m_dTime;
		if (m_currentIdleTime > m_timeBeforeAnimating) {
			m_selectionDX = 150.0 * (m_currentIdleTime - m_timeBeforeAnimating);
		}
	}
	
	pushVertexColor(selVector, 10.0f-m_selectionDX, 200.0f + 1000.0 * scale * heightIndex, 0.0f, r, g, b);
	pushVertexColor(selVector, 10.0f-m_selectionDX, 200.0f + 1000.0 * scale * heightIndex + selHeight, 0.0f, r, g, b);
	pushVertexColor(selVector, 10.0f-m_selectionDX + right, 200.0f + 1000.0 * scale * heightIndex + selHeight, 0.0f, r, g, b);
	pushVertexColor(selVector, 10.0f-m_selectionDX + right, 200.0f + 1000.0 * scale * heightIndex, 0.0f, r, g, b);
	pushRectangleIndices(selIndices, selVertexCount);
	renderColor(selVector, selIndices);

	if (menu.getChildCount() > 0) {
		//draw every child from node

		if (menu.getChildCount() < VISIBLE_ENTRIES) {
			for (size_t i = 0; i < menu.getChildCount(); i++) {
				if (m_currentIdleTime > m_timeBeforeAnimating) {
					drawText(list.at(i).getText(), 10.0f- m_selectionDX, 1000.0*scale * i + 200.0f, scale);
				}
				else {
					drawText(list.at(i).getText(), 10.0f, 1000.0 * scale * i + 200.0f, scale);
				}
			}
		}
		else {
			for (size_t i = 0; i < VISIBLE_ENTRIES; i++) {
				if (m_currentIdleTime > m_timeBeforeAnimating && i+vOffset == selected) {
					drawText(list.at(i + vOffset).getText(), 10.0f - m_selectionDX, 1000.0 * scale * i + 200.0f, scale);
				}
				else {
					drawText(list.at(i + vOffset).getText(), 10.0f, 1000.0 * scale * i + 200.0f, scale);
				}
				//drawText(list.at(i + vOffset).getText(), 10.0f, 100.0f * i + 200.0f, scale);
			}
		}
	}
	drawText(menu.getText(), 10.0f, 100.0f, 0.05f);

	if (menu.getChildCount() > VISIBLE_ENTRIES) {
		if (vOffset > 0) {
			drawText("^", 10.0f, 150.0f, scale);
		}
		if (vOffset < menu.getChildCount() - VISIBLE_ENTRIES) {
			float h = getTextHeight("v", scale);
			drawText("v", 10.0f, 700.0 - h, scale);
		}
	}
}

void MenuRender::remapping(Game* game, menuinputs input) {
	game->getPlayer()->pollInput(m_window);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	int colnum = 5;

	ImGuiBackendFlags flags = 0;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Remapper Window", NULL, flags);
	ImGui::SetWindowPos({ 0.0f,0.0f });

	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	ImGui::SetWindowSize({ (float)width,(float)height });

	ImGui::Text("Using: ");
	ImGui::SameLine();
	if (ImGui::BeginCombo("Choose Input", m_inputSelection.c_str())) {
		if (ImGui::Selectable("Keyboard")) {
			game->getPlayer()->m_useKeyboardInput = true;
			m_input = true;
			m_inputSelection = "Keyboard";
		}
		for (int i = 0; i < 16; ++i) {
			std::string name = "";
			if (glfwJoystickPresent(i)) {
				name = glfwGetJoystickName(i);
			}
			else {
				name = "No Controller detected";
			}
			std::string t = "Id " + std::to_string(i) + ":" + name;
			if (ImGui::Selectable(t.c_str())) {
				game->getPlayer()->m_useKeyboardInput = false;
				game->getPlayer()->m_gamepadId = i;
				m_input = false;
				m_inputSelection = t;
			}
		}
		ImGui::EndCombo();
	}

	if (!game->getPlayer()->m_useKeyboardInput) {
		ImGui::SameLine();
		if (ImGui::Button("Axis viewer")) {
			ImGui::OpenPopup("raw viewer");
			ImGui::SetNextWindowSize(ImVec2(500.0f, 700.0f));
		}
		if (ImGui::BeginPopupModal("raw viewer")) {
			if (ImGui::Button("Close"))ImGui::CloseCurrentPopup();
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
	}
	else if (game->getPlayer()->m_useKeyboardInput) {
		ImGui::SameLine();
		if (ImGui::Button("Axis viewer")) {
			ImGui::OpenPopup("raw viewer");
			ImGui::SetNextWindowSize(ImVec2(500.0f, 700.0f));
		}
		if (ImGui::BeginPopupModal("raw viewer")) {
			if (ImGui::Button("Close"))ImGui::CloseCurrentPopup();
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

	if (game->getPlayer()->m_gpState.size() > 0) {
		ImGui::Columns(colnum, "mycolumns3", false);  // 3-ways, no border

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
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Green Button:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Green Button:");
		}
		ImGui::NextColumn();
		std::string id = std::string("Remap") + std::string("##01");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(GREEN_INDEX);
			}
			else {
				editingAxisController(GREEN_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Green Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##green", &(game->getPlayer()->GREEN_CODE));
		}
		else {
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
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Red Button:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Red Button:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##02k");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(RED_INDEX);
			}
			else {
				editingAxisController(RED_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Red Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##red", &(game->getPlayer()->RED_CODE));
		}
		else {
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
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Blue Button:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Blue Button:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##03k");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(BLUE_INDEX);
			}
			else {
				editingAxisController(BLUE_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Blue Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##blue", &(game->getPlayer()->BLUE_CODE));
		}
		else {
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
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Euphoria Button:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Euphoria Button:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##04");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(EU_INDEX);
			}
			else {
				editingAxisController(EU_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Euphoria Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##eu", &(game->getPlayer()->EUPHORIA));
		}
		else {
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
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Crossfade left:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Crossfade left:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##05");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(CF_LEFT_INDEX);
			}
			else {
				editingAxisController(CF_LEFT_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Crossfade Left' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##left", &(game->getPlayer()->CROSS_L_CODE));
		}
		else {
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
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Crossfade right:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Crossfade right:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##06");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(CF_RIGHT_INDEX);
			}
			else {
				editingAxisController(CF_RIGHT_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Crossfade Right' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##right", &(game->getPlayer()->CROSS_R_CODE));
		}
		else {
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
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Scratch up:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Scratch up:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##07");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(SCR_UP_INDEX);
			}
			else {
				editingAxisController(SCR_UP_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Scratch Up' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##up", &(game->getPlayer()->SCRATCH_UP));
		}
		else {
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
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Scratch down:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Scratch down:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##08");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(SCR_DOWN_INDEX);
			}
			else {
				editingAxisController(SCR_DOWN_INDEX);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Scratch Down' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##down", &(game->getPlayer()->SCRATCH_DOWN));
		}
		else {
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
		}
		else {
			value = game->getPlayer()->getGamepadValues().at(*input.ug);
		}
		if (value > 0) {
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Menu up:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Menu up:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##09");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(MENU_UP);
			}
			else {
				editingAxisController(MENU_UP);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Up' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##meup", input.uk);
		}
		else {
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
		}
		else {
			value = game->getPlayer()->getGamepadValues().at(*input.dg);
		}
		if (value > 0) {
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Menu down:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Menu down:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##091");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(MENU_DOWN);
			}
			else {
				editingAxisController(MENU_DOWN);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Down' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##medown", input.dk);
		}
		else {
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
		}
		else {
			value = game->getPlayer()->getGamepadValues().at(*input.sg);
		}
		if (value > 0) {
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Menu Select:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Menu Select:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##092");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(MENU_SELECT);
			}
			else {
				editingAxisController(MENU_SELECT);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Select' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##meselect", input.sk);
		}
		else {
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
		}
		else {
			value = game->getPlayer()->getGamepadValues().at(*input.bg);
		}
		if (value > 0) {
			ImGui::TextColored({ 0.0f,1.0f,0.0f,1.0f }, "Menu Back:");
		}
		else {
			ImGui::TextColored({ 1.0f,1.0f,1.0f,1.0f }, "Menu Back:");
		}
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##093");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			if (game->getPlayer()->m_useKeyboardInput) {
				editingAxisKBAM(MENU_BACK);
			}
			else {
				editingAxisController(MENU_BACK);
			}
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Back' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey && !m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		if (game->getPlayer()->m_useKeyboardInput) {
			ImGui::InputInt("##meback", input.bk);
		}
		else {
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

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MenuRender::credits() {
	float fontsize = 48.0f;
	float y = 0.0;
	float x = 10.0f;

	drawText("Thanks to MexicanPB for the Logo", x, y, fontsize / 1000.0f);
	y += fontsize;//new line
	y += fontsize;//new line

	drawText("Thanks to everyone in the Dj Hero Fan Server ", x, y, fontsize / 1000.0f);
	y += fontsize;//new line	

	drawText("for supporting me and this game", x, y, fontsize / 1000.0f);
	y += fontsize;//new line
	y += fontsize;//new line

	drawText("A thank you to every programming youtuber", x, y, fontsize / 1000.0f);
	y += fontsize;//new line

	drawText("for giving me the passion to create games", x, y, fontsize / 1000.0f);
	y += fontsize;//new line
	y += fontsize;//new line

	drawText("And finally a thank you to you, player", x, y, fontsize / 1000.0f);
	y += fontsize;//new line
	drawText("You are the reason why I'm doing this", x, y, fontsize / 1000.0f);
	y += fontsize;//new line
	y += fontsize;//new line
	y += fontsize;//new line

	drawText("MatteoGodzilla <3", x, y, fontsize / 1000.0f);
	y += fontsize;//new line

	drawText("(now that you read the credits, DM me", x, y, fontsize / 1000.0f);
	y += fontsize;//new line

	drawText("on twitter (@MatteoGodzilla) with '#truedj')", x, y, fontsize / 1000.0f);
	y += fontsize;//new line
}

void MenuRender::splashArt() {
	std::vector<float> vector;
	std::vector<unsigned int> indices;
	unsigned int indexCount = 0;

	float scale = 1.2f;

	float x = 1280.0f / 2.0f;
	float y = 360.0f - 200.0f * scale / 2;
	pushVertexTexture(vector, x, y, 0.0, 0.0, 1.0);
	pushVertexTexture(vector, x, y + 200.0f * scale, 0.0, 0.0, 0.0);
	pushVertexTexture(vector, x + 500.0f * scale, y + 200.0f * scale, 0.0, 1.0, 0.0);
	pushVertexTexture(vector, x + 500.0f * scale, y, 0.0, 1.0, 1.0);
	pushRectangleIndices(indices, indexCount);

	useOrthoProj();
	renderTexture(vector, indices, m_splashTexture);

	float textScale = 0.02f;
	drawText("*This is alpha v1.2. There are some bugs and you WILL see them*", 10.0, 10.0, textScale);
	drawText("*Remember this when playing*", 10.0, 30.0, textScale);
	drawText("*Have Fun! :)*", 10.0, 50.0, textScale);

	std::string discord = "For any questions, ask on the Dj Hero Discord";
	std::string discord2 = "https://discord.gg/HZ82gKR";
	drawText(discord, 1270.0f - getTextWidth(discord, textScale), 10.0f, textScale);
	drawText(discord2, 1270.0f - getTextWidth(discord2, textScale), 30.0f, textScale);
}

void MenuRender::scratches(Player* player) {
	useOrthoProj();
	drawText("Here you can test your scatches", 20.0, 20.0, 0.05f);

	player->pollInput(m_window);
	if (player->m_isUpPressed && !player->m_wasUpPressed) {
		m_testBuffer.push_back('^');
	}
	if (player->m_isDownPressed && !player->m_wasDownPressed) {
		m_testBuffer.push_back('v');
	}
	if (m_testBuffer.size() > 20)m_testBuffer.erase(0, 1);
	drawText(m_testBuffer, 20.0f, 310.0f, 0.1f);
	drawText("Press Menu Back to exit", 20.0, 670.0f, 0.05f);
}

void MenuRender::calibration(Game* game, double dt) {
	useOrthoProj();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiBackendFlags flags = 0;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Calibration Window", NULL, flags);
	ImGui::SetWindowPos({ 0.0f,0.0f });

	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	ImGui::SetWindowSize({ (float)width,(float)height });

	ImGui::SliderFloat("Audio Latency", &(game->m_audioLatency), 0.0f, 2.0f);
	ImGui::SameLine();
	if (ImGui::Button("Calibrate")) {
		m_isCalibrating = true;
		game->getAudio()->reset();
		m_cbPlayingTime = 0.0;
		m_latencyHits.clear();
		game->getAudio()->load("res/calibration.ogg");
		game->getAudio()->play();
	}
	if (game->getAudio()->isPlaying()) {
		game->getAudio()->buffer();
		m_cbPlayingTime += dt;
	}
	else {
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
	ImGui::Text("When calibrationg, you are going to hear 4 bass hits, then 8 snare hits and then a ding.");
	ImGui::Text("Press the 'Calibrate' Button ONLY when you hear the snare hits for best latency calculations");
	ImGui::Text("(on zero latency the click should match the lights below on wired headphones)");

	ImGui::Separator();

	for (int i = 0; i < 8; ++i) {
		if (m_cbPlayingTime > 2.0 + 0.5 * i) {
			ImGui::SameLine();
			ImGui::Image((ImTextureID)m_calibrationTex, { 100.0f,100.0f }, { 0.5f,0.5f }, { 1.0f,1.0f });
		}
		else {
			ImGui::SameLine();
			ImGui::Image((ImTextureID)m_calibrationTex, { 100.0f,100.0f }, { 0.0f,0.0f }, { 0.5f,0.5f });
		}
	}
	ImGui::Separator();
	//if (ImGui::Button("Tap Me", { 300.0f,250.0f })) {
	//	m_latencyHits.push_back(m_cbPlayingTime);
	//}
	if (ImGui::IsKeyPressed(32)) //space key
	{
		m_latencyHits.push_back(m_cbPlayingTime);
	}
	if (ImGui::Button("Go back to main menu")) {
		m_shouldClose = true;
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MenuRender::setDeckSpeed(Game* game) {
	useOrthoProj();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiBackendFlags flags = 0;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Set Deck Speed", NULL, flags);
	ImGui::Text("Set the deck speed using the slider below.");
	ImGui::Text("The value represents the visible time of a note in the highway");
	ImGui::Text("(i.e. 1.0 is one second in the highway)");
	ImGui::Text("A larger value means slower speed, smaller value means faster");
	ImGui::SliderFloat("Speed", &(game->m_deckSpeed), 0.5f, 3.0f);
	if (ImGui::Button("Close")) {
		m_shouldClose = true;
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MenuRender::editingGameAxis(int action) {
	m_editingKey = false;
	m_editingAxis = true;
	m_gameActionToChange = action;
	m_menuActionToChange = -1;
}

void MenuRender::editingGameKey(int action) {
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_editingKey = true;
	m_editingAxis = false;
	m_gameActionToChange = action;
	m_menuActionToChange = -1;
}

void MenuRender::doneEditing() {
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_editingKey = false;
	m_editingAxis = false;
	m_gameActionToChange = -1;
	m_menuActionToChange = -1;
}

void MenuRender::editingAxisKBAM(int action) {
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_editingKey = true;
	m_editingAxis = false;
	m_gameActionToChange = -1;
	m_menuActionToChange = action;
}

void MenuRender::editingAxisController(int action) {
	m_editingKey = false;
	m_editingAxis = true;
	m_gameActionToChange = -1;
	m_menuActionToChange = action;
}

GLFWwindow* MenuRender::getWindowPtr()
{
	return m_window;
}

MenuRender::~MenuRender() {

}
