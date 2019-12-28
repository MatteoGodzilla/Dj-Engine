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
}

void MenuRender::render(MenuNode menu,int selected,unsigned int vOffset) {
	//enable exit from remapping menu
	m_shouldClose = false;

	//vertices data
	std::vector<float> selVector;
	std::vector<unsigned int> selIndices;
	unsigned int selVertexCount = 0;

	//text scale
	float scale = 0.1f;

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
	pushVertexColor(selVector, 10.0f, 200.0f + 100.0f * heightIndex, 0.0f, r, g, b);
	pushVertexColor(selVector, 10.0f, 200.0f + 100.0f * heightIndex + selHeight, 0.0f, r, g, b);
	pushVertexColor(selVector, 10.0f + right, 200.0f + 100.0f * heightIndex + selHeight, 0.0f, r, g, b);
	pushVertexColor(selVector, 10.0f + right, 200.0f + 100.0f * heightIndex, 0.0f, r, g, b);
	pushRectangleIndices(selIndices, selVertexCount);
	renderColor(selVector, selIndices);

	if (menu.getChildCount() > 0) {
		//draw every child from node
		
		if (menu.getChildCount() < VISIBLE_ENTRIES) {
			for (size_t i = 0; i < menu.getChildCount(); i++) {
				drawText(list.at(i).getText(), 10.0f, 100.0f * i + 200.0f, scale);
			}
		}
		else {
			for (size_t i = 0; i < VISIBLE_ENTRIES; i++) {
				drawText(list.at(i + vOffset).getText(), 10.0f, 100.0f * i + 200.0f, scale);
			}
		}
	}
	drawText(menu.getText(), 10.0f, 100.0f, 0.05f);

	if (menu.getChildCount() > VISIBLE_ENTRIES) {
		if (vOffset > 0) {
			drawText("^", 10.0f, 150.0f, scale);
		}
		if (vOffset < menu.getChildCount() - VISIBLE_ENTRIES) {
			drawText("v", 10.0f, 602.0f, scale);
		}
	}
}

void MenuRender::remapping(Game* game, int uk, int dk, int sk, int bk, int ug, int dg, int sg, int bg) {
	game->getPlayer()->pollInput(m_window);
	if (game->getPlayer()->m_gpState.size() > 0 && !game->getPlayer()->m_useKeyboardInput) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		int colnum = 5;

		ImGuiBackendFlags flags = 0;
		flags |= ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoResize;
		flags |= ImGuiWindowFlags_NoCollapse;

		ImGui::Begin("Remapper Window",NULL,flags);
		ImGui::SetWindowPos({ 0.0f,0.0f });
		
		int width, height;
		glfwGetFramebufferSize(m_window, &width, &height);
		ImGui::SetWindowSize({ (float)width,(float)height });

		ImGui::Text("Using Controller:");
		ImGui::SameLine();
		ImGui::Text(glfwGetJoystickName(GLFW_JOYSTICK_1));
		ImGui::Columns(colnum, "mycolumns3",false);  // 3-ways, no border
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

		ImGui::Text("Green Button:");
		ImGui::NextColumn();
		std::string id = std::string("Remap") + std::string("##01");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameAxis(GREEN_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Green Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(game->getPlayer()->GREEN_GAMEPAD).c_str());
		ImGui::NextColumn();
		float value = game->getPlayer()->m_gpState.at(GREEN_INDEX) * game->getPlayer()->m_gpMult.at(GREEN_INDEX);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##1", &(game->getPlayer()->m_gpMult.at(GREEN_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##11", &(game->getPlayer()->m_gpDead.at(GREEN_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		ImGui::Text("Red Button:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##02");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameAxis(RED_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Red Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(game->getPlayer()->RED_GAMEPAD).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->m_gpState.at(RED_INDEX) * game->getPlayer()->m_gpMult.at(RED_INDEX);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##2", &(game->getPlayer()->m_gpMult.at(RED_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##21", &(game->getPlayer()->m_gpDead.at(RED_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		ImGui::Text("Blue Button:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##03");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameAxis(BLUE_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Blue Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(game->getPlayer()->BLUE_GAMEPAD).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->m_gpState.at(BLUE_INDEX) * game->getPlayer()->m_gpMult.at(BLUE_INDEX);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##3", &(game->getPlayer()->m_gpMult.at(BLUE_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##31", &(game->getPlayer()->m_gpDead.at(BLUE_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		ImGui::Text("Euphoria Button:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##04");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameAxis(EU_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Euphoria Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(game->getPlayer()->EU_GAMEPAD).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->m_gpState.at(EU_INDEX) * game->getPlayer()->m_gpMult.at(EU_INDEX);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##4", &(game->getPlayer()->m_gpMult.at(EU_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##41", &(game->getPlayer()->m_gpDead.at(EU_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();
		ImGui::Columns();

		ImGui::Checkbox("Use Single Axis for Crossfade", &(game->getPlayer()->m_useSingleCfAxis));
		
		ImGui::Columns(colnum,"second col",false);
		ImGui::Text("Crossfade left:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##05");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameAxis(CF_LEFT_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Crossfade Left' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(game->getPlayer()->CF_LEFT_GAMEPAD).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->m_gpState.at(CF_LEFT_INDEX) * game->getPlayer()->m_gpMult.at(CF_LEFT_INDEX);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##5", &(game->getPlayer()->m_gpMult.at(CF_LEFT_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##51", &(game->getPlayer()->m_gpDead.at(CF_LEFT_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		ImGui::Text("Crossfade right:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##06");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameAxis(CF_RIGHT_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Crossfade Right' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(game->getPlayer()->CF_RIGHT_GAMEPAD).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->m_gpState.at(CF_RIGHT_INDEX) * game->getPlayer()->m_gpMult.at(CF_RIGHT_INDEX);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##6", &(game->getPlayer()->m_gpMult.at(CF_RIGHT_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##61", &(game->getPlayer()->m_gpDead.at(CF_RIGHT_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();
		ImGui::Columns();

		ImGui::Checkbox("Use Single Axis for Scratch", &(game->getPlayer()->m_useSingleScrAxis));

		ImGui::Columns(colnum, "third col", false);
		ImGui::Text("Scratch up:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##07");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameAxis(SCR_UP_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Scratch Up' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(game->getPlayer()->SCR_UP_GAMEPAD).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->m_gpState.at(SCR_UP_INDEX) * game->getPlayer()->m_gpMult.at(SCR_UP_INDEX);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##7", &(game->getPlayer()->m_gpMult.at(SCR_UP_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##71", &(game->getPlayer()->m_gpDead.at(SCR_UP_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		ImGui::Text("Scratch down:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##08");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameAxis(SCR_DOWN_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Scratch Down' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(game->getPlayer()->SCR_DOWN_GAMEPAD).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->m_gpState.at(SCR_DOWN_INDEX) * game->getPlayer()->m_gpMult.at(SCR_DOWN_INDEX);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::SliderFloat("##8", &(game->getPlayer()->m_gpMult.at(SCR_DOWN_INDEX)), -5.0, 5.0);
		ImGui::NextColumn();
		ImGui::SliderFloat("##81", &(game->getPlayer()->m_gpDead.at(SCR_DOWN_INDEX)), -1.0, 1.0);
		ImGui::NextColumn();

		ImGui::Text("Menu up:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##09");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingMenuAxis(MENU_UP);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Up' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(ug).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->getGamepadValues().at(ug);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::NextColumn();
		ImGui::NextColumn();

		ImGui::Text("Menu down:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##091");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingMenuAxis(MENU_DOWN);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Down' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(dg).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->getGamepadValues().at(dg);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::NextColumn();
		ImGui::NextColumn();
		
		ImGui::Text("Menu Select:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##092");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingMenuAxis(MENU_SELECT);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Select' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(sg).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->getGamepadValues().at(sg);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::NextColumn();
		ImGui::NextColumn();

		ImGui::Text("Menu Back:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##093");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingMenuAxis(MENU_BACK);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button / move an axis for 'Menu Back' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingAxis) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(bg).c_str());
		ImGui::NextColumn();
		value = game->getPlayer()->getGamepadValues().at(bg);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		ImGui::NextColumn();
		ImGui::NextColumn();

		ImGui::Columns();

		ImGui::Separator();
		if (ImGui::Button("Go Back to menu")) {
			m_shouldClose = true;
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	else if (game->getPlayer()->m_useKeyboardInput) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiBackendFlags flags = 0;
		flags |= ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoResize;
		flags |= ImGuiWindowFlags_NoCollapse;

		ImGui::Begin("Remapper Window",NULL,flags);
		ImGui::SetWindowPos({ 0.0f,0.0f });

		int width, height;
		glfwGetFramebufferSize(m_window, &width, &height);
		ImGui::SetWindowSize({ (float)width,(float)height });

		ImGui::Text("Using Keyboard");
		ImGui::Columns(3, "mycolumns3", false);  // 3-ways, no border
		ImGui::Text("Action");
		ImGui::NextColumn();
		ImGui::Text("ID");
		ImGui::NextColumn();
		ImGui::Text("Value");
		ImGui::NextColumn();
		ImGui::Separator();

		ImGui::Text("Green Button:");
		ImGui::NextColumn();
		std::string id = std::string("Remap") + std::string("##101");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameKey(GREEN_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Green Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		int code = game->getPlayer()->GREEN_CODE;
		char c[2] = { code,'\0' };
		ImGui::Text(std::to_string(code).c_str());
		ImGui::SameLine();
		ImGui::Text(c);
		ImGui::NextColumn();
		float value = game->getPlayer()->m_isGreenPressed ? 1.0f : 0.0f;
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		
		ImGui::Text("Red Button:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##102");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameKey(RED_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Red Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		code = game->getPlayer()->RED_CODE;
		c[0] = code;
		ImGui::Text(std::to_string(code).c_str());
		ImGui::SameLine();
		ImGui::Text(c);
		ImGui::NextColumn();
		value = game->getPlayer()->m_isRedPressed ? 1.0f : 0.0f;
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		
		ImGui::Text("Blue Button:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##103");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameKey(BLUE_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Blue Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		code = game->getPlayer()->BLUE_CODE;
		c[0] = code;
		ImGui::Text(std::to_string(code).c_str());
		ImGui::SameLine();
		ImGui::Text(c);
		ImGui::NextColumn();
		value = game->getPlayer()->m_isBluePressed ? 1.0f : 0.0f;
		ImGui::ProgressBar(value);
		ImGui::NextColumn();

		ImGui::Text("Euphoria Button:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##104");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameKey(EU_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Euphoria Button' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		code = game->getPlayer()->EUPHORIA;
		c[0] = code;
		ImGui::Text(std::to_string(code).c_str());
		ImGui::SameLine();
		ImGui::Text(c);
		ImGui::NextColumn();
		value = game->getPlayer()->m_isEuPressed ? 1.0f : 0.0f;
		ImGui::ProgressBar(value);
		ImGui::NextColumn();

		ImGui::Text("Crossfade left:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##105");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameKey(CF_LEFT_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Crossfade Left' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		code = game->getPlayer()->CROSS_L_CODE;
		c[0] = code;
		ImGui::Text(std::to_string(code).c_str());
		ImGui::SameLine();
		ImGui::Text(c);
		ImGui::NextColumn();
		value = game->getPlayer()->m_isCfLeftPressed ? 1.0f : 0.0f;
		ImGui::ProgressBar(value);
		ImGui::NextColumn();

		ImGui::Text("Crossfade right:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##106");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameKey(CF_RIGHT_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Crossfade Right' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		code = game->getPlayer()->CROSS_R_CODE;
		c[0] = code;
		ImGui::Text(std::to_string(code).c_str());
		ImGui::SameLine();
		ImGui::Text(c);
		ImGui::NextColumn();
		value = game->getPlayer()->m_isCfRightPressed ? 1.0f : 0.0f;
		ImGui::ProgressBar(value);
		ImGui::NextColumn();

		ImGui::Text("Scratch up:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##107");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameKey(SCR_UP_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Scratch Up' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		code = game->getPlayer()->SCRATCH_UP;
		c[0] = code;
		ImGui::Text(std::to_string(code).c_str());
		ImGui::SameLine();
		ImGui::Text(c);
		ImGui::NextColumn();
		value = game->getPlayer()->m_isUpPressed ? 1.0f : 0.0f;
		ImGui::ProgressBar(value);
		ImGui::NextColumn();

		ImGui::Text("Scratch down:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##108");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingGameKey(SCR_DOWN_INDEX);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Scratch Down' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		code = game->getPlayer()->SCRATCH_DOWN;
		c[0] = code;
		ImGui::Text(std::to_string(code).c_str());
		ImGui::SameLine();
		ImGui::Text(c);
		ImGui::NextColumn();
		value = game->getPlayer()->m_isDownPressed ? 1.0f : 0.0f;
		ImGui::ProgressBar(value);
		ImGui::NextColumn();

		ImGui::Text("Menu Up:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##109");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingMenuKey(MENU_UP);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Menu Up' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(uk).c_str());
		ImGui::NextColumn();
		value = glfwGetKey(m_window, uk);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();

		ImGui::Text("Menu Down:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##1091");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingMenuKey(MENU_DOWN);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Menu Down' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(dk).c_str());
		ImGui::NextColumn();
		value = glfwGetKey(m_window, dk);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();

		ImGui::Text("Menu Select:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##1092");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingMenuKey(MENU_SELECT);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Menu Select' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(sk).c_str());
		ImGui::NextColumn();
		value = glfwGetKey(m_window, sk);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();
		
		ImGui::Text("Menu Back:");
		ImGui::NextColumn();
		id = std::string("Remap") + std::string("##1093");
		if (ImGui::Button(id.c_str())) {
			ImGui::OpenPopup(id.c_str());
			editingMenuKey(MENU_BACK);
		}
		if (ImGui::BeginPopupModal(id.c_str())) {
			ImGui::Text("Push a button for 'Menu Back' ");
			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!m_editingKey) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(bk).c_str());
		ImGui::NextColumn();
		value = glfwGetKey(m_window, bk);
		ImGui::ProgressBar(value);
		ImGui::NextColumn();

		ImGui::Separator();
		if (ImGui::Button("Go Back to menu")) {
			m_shouldClose = true;
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

void MenuRender::credits(){
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
	pushVertexTexture(vector, x, y + 200.0 * scale, 0.0, 0.0, 0.0);
	pushVertexTexture(vector, x + 500.0 * scale, y + 200.0 * scale, 0.0, 1.0, 0.0);
	pushVertexTexture(vector, x + 500.0 * scale, y, 0.0, 1.0, 1.0);
	pushRectangleIndices(indices, indexCount);

	useOrthoProj();
	renderTexture(vector, indices, m_splashTexture);
}

void MenuRender::editingGameAxis(int action) {
	m_editingKey = false;
	m_editingAxis = true;
	m_gameActionToChange = action;
	m_menuActionToChange = -1;
}

void MenuRender::editingGameKey(int action){
	m_editingKey = true;
	m_editingAxis = false;
	m_gameActionToChange = action;
	m_menuActionToChange = -1;
}

void MenuRender::doneEditing() {
	m_editingKey = false;
	m_editingAxis = false;
	m_gameActionToChange = -1;
	m_menuActionToChange = -1;
}

void MenuRender::editingMenuKey(int action) {
	m_editingKey = true;
	m_editingAxis = false;
	m_gameActionToChange = -1;
	m_menuActionToChange = action;
}

void MenuRender::editingMenuAxis(int action) {
	m_editingKey = false;
	m_editingAxis = true;
	m_gameActionToChange = -1;
	m_menuActionToChange = action;
}

GLFWwindow* MenuRender::getWindowPtr()
{
	return m_window;
}

MenuRender::~MenuRender(){
	
}
