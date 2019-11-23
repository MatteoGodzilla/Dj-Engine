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
}

void MenuRender::render(MenuNode menu,int selected,unsigned int vOffset) {
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

void MenuRender::remapping() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowTestWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MenuRender::credits()
{
	drawText("Credits scene", 0.0f, 0.0f, 0.1f);
	drawText("Nothing to see here", 0.0f, 100.0f, 0.1f);
	drawText(":)", 0.0f, 200.0f, 0.1f);
}

GLFWwindow* MenuRender::getWindowPtr()
{
	return m_window;
}

MenuRender::~MenuRender(){
	
}
