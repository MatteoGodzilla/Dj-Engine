#include "GameRender.h"

void GameRender::init(GLFWwindow* w) {
	Rendr::init(w);
	m_animManager.init();

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
	//setting up object sprite atlas informations
	{
		float colSize = 1.0 / 4.0;
		float rowSize = 1.0 / 6.0;

		m_objectAtlas.insert(std::make_pair(TAP_GREEN_1, glm::vec4(0 * colSize, 0 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_GREEN_2, glm::vec4(1 * colSize, 0 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_GREEN_3, glm::vec4(2 * colSize, 0 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_GREEN_4, glm::vec4(3 * colSize, 0 * rowSize, colSize, rowSize)));

		m_objectAtlas.insert(std::make_pair(TAP_RED_1, glm::vec4(0 * colSize, 1 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_RED_2, glm::vec4(1 * colSize, 1 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_RED_3, glm::vec4(2 * colSize, 1 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_RED_4, glm::vec4(3 * colSize, 1 * rowSize, colSize, rowSize)));

		m_objectAtlas.insert(std::make_pair(TAP_BLUE_1, glm::vec4(0 * colSize, 2 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_BLUE_2, glm::vec4(1 * colSize, 2 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_BLUE_3, glm::vec4(2 * colSize, 2 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_BLUE_4, glm::vec4(3 * colSize, 2 * rowSize, colSize, rowSize)));

		m_objectAtlas.insert(std::make_pair(TAP_EUPHORIA_1, glm::vec4(0 * colSize, 3 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_EUPHORIA_2, glm::vec4(1 * colSize, 3 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_EUPHORIA_3, glm::vec4(2 * colSize, 3 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(TAP_EUPHORIA_4, glm::vec4(3 * colSize, 3 * rowSize, colSize, rowSize)));

		m_objectAtlas.insert(std::make_pair(CLICKER_GREEN, glm::vec4(0 * colSize, 4 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(CLICKER_RED, glm::vec4(1 * colSize, 4 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(CLICKER_BLUE, glm::vec4(2 * colSize, 4 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(CLICKER_TRAY, glm::vec4(4 * colSize, 4 * rowSize, -1 * colSize, 2 * rowSize)));

		m_objectAtlas.insert(std::make_pair(SCRATCH_ANYDIR, glm::vec4(0 * colSize, 5 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(SCRATCH_UP, glm::vec4(1 * colSize, 5 * rowSize, colSize, rowSize)));
		m_objectAtlas.insert(std::make_pair(SCRATCH_DOWN, glm::vec4(2 * colSize, 5 * rowSize, colSize, rowSize)));
	}

	framerate = std::ofstream("log.txt");
	if(framerate.is_open()){
		framerate << "time,fps,note,event,cross," << std::endl;
	}
}

void GameRender::highway(double time) {
	std::vector<float> highwayVector;
	std::vector<unsigned int> highwayIndices;
	unsigned int highwayVertexCount = 0;

	glm::vec2 center = {m_radius, 0.0};

	//value depends from max radius, time, notevisibletime and the highway texture
	float offsetFactor = 1.0f / m_noteVisibleTime * (float)time;

	glm::vec2 beforeOuter = getCirclePoint((double)m_radius + m_deltaAngle, 0.0);
	glm::vec2 BeforeInner = getCirclePoint((double)m_radius - m_deltaAngle, 0.0);

	for (float angle = 0.0; angle < m_maxAngle; angle += m_deltaAngle) {
		glm::vec2 outer = getCirclePoint((double)m_radius + m_deltaRadius, (double)angle);
		glm::vec2 inner = getCirclePoint((double)m_radius - m_deltaRadius, (double)angle);

		pushVertexTexture(highwayVector, -beforeOuter.x + center.x, 0.0, -beforeOuter.y - center.y, 0.0, (angle / m_maxAngle) + offsetFactor);
		pushVertexTexture(highwayVector, -BeforeInner.x + center.x, 0.0, -BeforeInner.y - center.y, 1.0, (angle / m_maxAngle) + offsetFactor);
		pushVertexTexture(highwayVector, -inner.x + center.x, 0.0, -inner.y - center.y, 1.0, (angle / m_maxAngle) + offsetFactor);
		pushVertexTexture(highwayVector, -outer.x + center.x, 0.0, -outer.y - center.y, 0.0, (angle / m_maxAngle) + offsetFactor);
		pushRectangleIndices(highwayIndices, highwayVertexCount);

		beforeOuter = outer;
		BeforeInner = inner;
	}

	usePersProj();
	renderTexture(highwayVector, highwayIndices, m_highwayTexture);
}

void GameRender::clicker() {
	//difference in size between pressed and not
	float deltaSize = 0.05f;
	float PosOffset = 0.025;

	m_front = 0.0;
	m_back = -0.5;

	float plane = 0.0f;

	//vertices data
	std::vector<float> clickerVector = {};
	std::vector<unsigned int> clickerIndices = {};
	unsigned int clickerVertexCount = 0;

	//left tray object
	glm::vec4 clickerTray = m_objectAtlas.at(CLICKER_TRAY);

	pushVertexTexture(clickerVector, -1.25f, plane, m_back, clickerTray.x, 1.0f - clickerTray.y);
	pushVertexTexture(clickerVector, -1.25f, plane, m_front, clickerTray.x + clickerTray.z, 1.0f - clickerTray.y);
	pushVertexTexture(clickerVector, -0.25f, plane, m_front, clickerTray.x + clickerTray.z, 1.0f - clickerTray.y - clickerTray.w);
	pushVertexTexture(clickerVector, -0.25f, plane, m_back, clickerTray.x, 1.0f - clickerTray.y - clickerTray.w);
	pushRectangleIndices(clickerIndices, clickerVertexCount);

	//right tray object

	pushVertexTexture(clickerVector, 0.25f, plane, m_back, clickerTray.x, 1.0f - clickerTray.y);
	pushVertexTexture(clickerVector, 0.25f, plane, m_front, clickerTray.x + clickerTray.z, 1.0f - clickerTray.y);
	pushVertexTexture(clickerVector, 1.25f, plane, m_front, clickerTray.x + clickerTray.z, 1.0f - clickerTray.y - clickerTray.w);
	pushVertexTexture(clickerVector, 1.25f, plane, m_back, clickerTray.x, 1.0f - clickerTray.y - clickerTray.w);
	pushRectangleIndices(clickerIndices, clickerVertexCount);

	glm::vec4 clickerRedInfo = m_objectAtlas.at(CLICKER_RED);

	if (m_red) {
		//red pressed clicker
		pushVertexTexture(clickerVector, -0.25f + deltaSize, plane, m_back + deltaSize + PosOffset, clickerRedInfo.x, 1.0f - clickerRedInfo.y);
		pushVertexTexture(clickerVector, -0.25f + deltaSize, plane, m_front - deltaSize + PosOffset, clickerRedInfo.x, 1.0f - clickerRedInfo.y - clickerRedInfo.w);
		pushVertexTexture(clickerVector, 0.25f - deltaSize, plane, m_front - deltaSize + PosOffset, clickerRedInfo.x + clickerRedInfo.z, 1.0f - clickerRedInfo.y - clickerRedInfo.w);
		pushVertexTexture(clickerVector, 0.25f - deltaSize, plane, m_back + deltaSize + PosOffset, clickerRedInfo.x + clickerRedInfo.z, 1.0f - clickerRedInfo.y);
	} else {
		//red normal clicker
		pushVertexTexture(clickerVector, -0.25f, plane, m_back, clickerRedInfo.x, 1.0f - clickerRedInfo.y);
		pushVertexTexture(clickerVector, -0.25f, plane, m_front, clickerRedInfo.x, 1.0f - clickerRedInfo.y - clickerRedInfo.w);
		pushVertexTexture(clickerVector, 0.25f, plane, m_front, clickerRedInfo.x + clickerRedInfo.z, 1.0f - clickerRedInfo.y - clickerRedInfo.w);
		pushVertexTexture(clickerVector, 0.25f, plane, m_back, clickerRedInfo.x + clickerRedInfo.z, 1.0f - clickerRedInfo.y);
	}
	pushRectangleIndices(clickerIndices, clickerVertexCount);

	if (m_playerCross == 0) {
		m_greenLeft = -1.25f;
		m_greenRight = -0.75f;
		m_blueLeft = 0.25f;
		m_blueRight = 0.75f;

		Animation greenToCenter = m_animManager.getAnimById(AN_CROSS_GREEN_TO_LEFT);
		Animation blueToCenter = m_animManager.getAnimById(AN_CROSS_BLUE_TO_CENTER);

		if (greenToCenter.isEnabled()) {
			float d = (float)greenToCenter.getPercent() * 0.5f;
			m_greenLeft = -0.75f - d;
			m_greenRight = -0.25f - d;
		}

		if (blueToCenter.isEnabled()) {
			float d = (float)blueToCenter.getPercent() * -0.5f;
			m_blueLeft = 0.75f + d;
			m_blueRight = 1.25f + d;
		}

	} else if (m_playerCross == 1) {
		m_greenLeft = -0.75f;
		m_greenRight = -0.25f;
		m_blueLeft = 0.25f;
		m_blueRight = 0.75f;

		Animation greenToCenter = m_animManager.getAnimById(AN_CROSS_GREEN_TO_CENTER);
		Animation blueToCenter = m_animManager.getAnimById(AN_CROSS_BLUE_TO_CENTER);

		if (greenToCenter.isEnabled()) {
			float d = (float)greenToCenter.getPercent() * 0.5f;
			m_greenLeft = -1.25f + d;
			m_greenRight = -0.75f + d;
		}

		if (blueToCenter.isEnabled()) {
			float d = (float)blueToCenter.getPercent() * -0.5f;
			m_blueLeft = 0.75f + d;
			m_blueRight = 1.25f + d;
		}

	} else {
		m_greenLeft = -0.75f;
		m_greenRight = -0.25f;
		m_blueLeft = 0.75f;
		m_blueRight = 1.25f;

		Animation greenToCenter = m_animManager.getAnimById(AN_CROSS_GREEN_TO_CENTER);
		Animation blueToCenter = m_animManager.getAnimById(AN_CROSS_BLUE_TO_RIGHT);

		if (greenToCenter.isEnabled()) {
			float d = (float)greenToCenter.getPercent() * 0.5f;
			m_greenLeft = -1.25f + d;
			m_greenRight = -0.75f + d;
		}

		if (blueToCenter.isEnabled()) {
			float d = (float)blueToCenter.getPercent() * 0.5f;
			m_blueLeft = 0.25f + d;
			m_blueRight = 0.75f + d;
		}
	}

	glm::vec4 clickerGreenInfo = m_objectAtlas.at(CLICKER_GREEN);

	if (m_green) {
		pushVertexTexture(clickerVector, m_greenLeft + deltaSize, plane, m_back + deltaSize + PosOffset, clickerGreenInfo.x, 1.0f - clickerGreenInfo.y);
		pushVertexTexture(clickerVector, m_greenLeft + deltaSize, plane, m_front - deltaSize + PosOffset, clickerGreenInfo.x, 1.0f - clickerGreenInfo.y - clickerGreenInfo.w);
		pushVertexTexture(clickerVector, m_greenRight - deltaSize, plane, m_front - deltaSize + PosOffset, clickerGreenInfo.x + clickerGreenInfo.z, 1.0f - clickerGreenInfo.y - clickerGreenInfo.w);
		pushVertexTexture(clickerVector, m_greenRight - deltaSize, plane, m_back + deltaSize + PosOffset, clickerGreenInfo.x + clickerGreenInfo.z, 1.0f - clickerGreenInfo.y);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	} else {
		pushVertexTexture(clickerVector, m_greenLeft, plane, m_back, clickerGreenInfo.x, 1.0f - clickerGreenInfo.y);
		pushVertexTexture(clickerVector, m_greenLeft, plane, m_front, clickerGreenInfo.x, 1.0f - clickerGreenInfo.y - clickerGreenInfo.w);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_front, clickerGreenInfo.x + clickerGreenInfo.z, 1.0f - clickerGreenInfo.y - clickerGreenInfo.w);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_back, clickerGreenInfo.x + clickerGreenInfo.z, 1.0f - clickerGreenInfo.y);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	glm::vec4 clickerBlueInfo = m_objectAtlas.at(CLICKER_BLUE);

	if (m_blue) {
		pushVertexTexture(clickerVector, m_blueLeft + deltaSize, plane, m_back + deltaSize + PosOffset, clickerBlueInfo.x, 1.0f - clickerBlueInfo.y);
		pushVertexTexture(clickerVector, m_blueLeft + deltaSize, plane, m_front - deltaSize + PosOffset, clickerBlueInfo.x, 1.0f - clickerBlueInfo.y - clickerBlueInfo.w);
		pushVertexTexture(clickerVector, m_blueRight - deltaSize, plane, m_front - deltaSize + PosOffset, clickerBlueInfo.x + clickerBlueInfo.z, 1.0f - clickerBlueInfo.y - clickerBlueInfo.w);
		pushVertexTexture(clickerVector, m_blueRight - deltaSize, plane, m_back + deltaSize + PosOffset, clickerBlueInfo.x + clickerBlueInfo.z, 1.0f - clickerBlueInfo.y);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	} else {
		pushVertexTexture(clickerVector, m_blueLeft, plane, m_back, clickerBlueInfo.x, 1.0f - clickerBlueInfo.y);
		pushVertexTexture(clickerVector, m_blueLeft, plane, m_front, clickerBlueInfo.x, 1.0f - clickerBlueInfo.y - clickerBlueInfo.w);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_front, clickerBlueInfo.x + clickerBlueInfo.z, 1.0f - clickerBlueInfo.y - clickerBlueInfo.w);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_back, clickerBlueInfo.x + clickerBlueInfo.z, 1.0f - clickerBlueInfo.y);
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

	glm::vec2 center = {m_radius, 0.0};
	float height = 0.025;

	for (auto& note : v) {
		double milli = note.getMilli();
		if (time + m_noteVisibleTime > milli) {
			double dt = milli - time;
			double noteAngle = getAngleFromDT(dt);

			int type = note.getType();

			if (type == TAP_R && !note.getDead() && !note.getTouched()) {
				double dAngle = asin(0.5 / m_radius);
				glm::vec2 topLeft = getCirclePoint(m_radius + 0.25, noteAngle + dAngle / 2);
				glm::vec2 bottomLeft = getCirclePoint(m_radius + 0.25, noteAngle - dAngle / 2);
				glm::vec2 topRight = getCirclePoint(m_radius - 0.25, noteAngle + dAngle / 2);
				glm::vec2 bottomRight = getCirclePoint(m_radius - 0.25, noteAngle - dAngle / 2);

				glm::vec4 OneInfo = m_objectAtlas.at(TAP_RED_1);
				glm::vec4 TwoInfo = m_objectAtlas.at(TAP_RED_2);
				glm::vec4 ThreeInfo = m_objectAtlas.at(TAP_RED_3);
				glm::vec4 FourInfo = m_objectAtlas.at(TAP_RED_4);
				if (m_renderEuActive) {
					OneInfo = m_objectAtlas.at(TAP_EUPHORIA_1);
					TwoInfo = m_objectAtlas.at(TAP_EUPHORIA_2);
					ThreeInfo = m_objectAtlas.at(TAP_EUPHORIA_3);
					FourInfo = m_objectAtlas.at(TAP_EUPHORIA_4);
				}

				//layer 1
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 0 * height, -topLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 0 * height, -bottomLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 0 * height, -bottomRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 0 * height, -topRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

				//layer 2
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 1 * height, -topLeft.y + center.y, TwoInfo.x, 1.0f - TwoInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 1 * height, -bottomLeft.y + center.y, TwoInfo.x, 1.0f - TwoInfo.y - TwoInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 1 * height, -bottomRight.y + center.y, TwoInfo.x + TwoInfo.z, 1.0f - TwoInfo.y - TwoInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 1 * height, -topRight.y + center.y, TwoInfo.x + TwoInfo.z, 1.0f - TwoInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

				//layer 3
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 2 * height, -topLeft.y + center.y, ThreeInfo.x, 1.0f - ThreeInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 2 * height, -bottomLeft.y + center.y, ThreeInfo.x, 1.0f - ThreeInfo.y - ThreeInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 2 * height, -bottomRight.y + center.y, ThreeInfo.x + ThreeInfo.z, 1.0f - ThreeInfo.y - ThreeInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 2 * height, -topRight.y + center.y, ThreeInfo.x + ThreeInfo.z, 1.0f - ThreeInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

				//layer 4
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 3 * height, -topLeft.y + center.y, FourInfo.x, 1.0f - FourInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 3 * height, -bottomLeft.y + center.y, FourInfo.x, 1.0f - FourInfo.y - FourInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 3 * height, -bottomRight.y + center.y, FourInfo.x + FourInfo.z, 1.0f - FourInfo.y - FourInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 3 * height, -topRight.y + center.y, FourInfo.x + FourInfo.z, 1.0f - FourInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

			} else if (type == TAP_G && !note.getDead() && !note.getTouched()) {
				double dAngle = asin(0.5 / (m_radius + 0.5));
				glm::vec2 topLeft = getCirclePoint(m_radius + 0.75, noteAngle + dAngle / 2);
				glm::vec2 bottomLeft = getCirclePoint(m_radius + 0.75, noteAngle - dAngle / 2);
				glm::vec2 topRight = getCirclePoint(m_radius + 0.25, noteAngle + dAngle / 2);
				glm::vec2 bottomRight = getCirclePoint(m_radius + 0.25, noteAngle - dAngle / 2);

				if (note.getLanMod() == 0) {
					dAngle = asin(0.5 / (m_radius + 1.0));
					topLeft = getCirclePoint(m_radius + 1.25, noteAngle + dAngle / 2);
					bottomLeft = getCirclePoint(m_radius + 1.25, noteAngle - dAngle / 2);
					topRight = getCirclePoint(m_radius + 0.75, noteAngle + dAngle / 2);
					bottomRight = getCirclePoint(m_radius + 0.75, noteAngle - dAngle / 2);
				}

				glm::vec4 OneInfo = m_objectAtlas.at(TAP_GREEN_1);
				glm::vec4 TwoInfo = m_objectAtlas.at(TAP_GREEN_2);
				glm::vec4 ThreeInfo = m_objectAtlas.at(TAP_GREEN_3);
				glm::vec4 FourInfo = m_objectAtlas.at(TAP_GREEN_4);
				if (m_renderEuActive) {
					OneInfo = m_objectAtlas.at(TAP_EUPHORIA_1);
					TwoInfo = m_objectAtlas.at(TAP_EUPHORIA_2);
					ThreeInfo = m_objectAtlas.at(TAP_EUPHORIA_3);
					FourInfo = m_objectAtlas.at(TAP_EUPHORIA_4);
				}

				//layer 1
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 0 * height, -topLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 0 * height, -bottomLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 0 * height, -bottomRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 0 * height, -topRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

				//layer 2
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 1 * height, -topLeft.y + center.y, TwoInfo.x, 1.0f - TwoInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 1 * height, -bottomLeft.y + center.y, TwoInfo.x, 1.0f - TwoInfo.y - TwoInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 1 * height, -bottomRight.y + center.y, TwoInfo.x + TwoInfo.z, 1.0f - TwoInfo.y - TwoInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 1 * height, -topRight.y + center.y, TwoInfo.x + TwoInfo.z, 1.0f - TwoInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

				//layer 3
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 2 * height, -topLeft.y + center.y, ThreeInfo.x, 1.0f - ThreeInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 2 * height, -bottomLeft.y + center.y, ThreeInfo.x, 1.0f - ThreeInfo.y - ThreeInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 2 * height, -bottomRight.y + center.y, ThreeInfo.x + ThreeInfo.z, 1.0f - ThreeInfo.y - ThreeInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 2 * height, -topRight.y + center.y, ThreeInfo.x + ThreeInfo.z, 1.0f - ThreeInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

				//layer 4
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 3 * height, -topLeft.y + center.y, FourInfo.x, 1.0f - FourInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 3 * height, -bottomLeft.y + center.y, FourInfo.x, 1.0f - FourInfo.y - FourInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 3 * height, -bottomRight.y + center.y, FourInfo.x + FourInfo.z, 1.0f - FourInfo.y - FourInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 3 * height, -topRight.y + center.y, FourInfo.x + FourInfo.z, 1.0f - FourInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

			} else if (type == TAP_B && !note.getDead() && !note.getTouched()) {
				double dAngle = asin(0.5 / (m_radius - 0.5));
				glm::vec2 topLeft = getCirclePoint(m_radius - 0.25, noteAngle + dAngle / 2);
				glm::vec2 bottomLeft = getCirclePoint(m_radius - 0.25, noteAngle - dAngle / 2);
				glm::vec2 topRight = getCirclePoint(m_radius - 0.75, noteAngle + dAngle / 2);
				glm::vec2 bottomRight = getCirclePoint(m_radius - 0.75, noteAngle - dAngle / 2);

				if (note.getLanMod() == 2) {
					dAngle = asin(0.5 / (m_radius - 1.0));
					topLeft = getCirclePoint(m_radius - 0.75, noteAngle + dAngle / 2);
					bottomLeft = getCirclePoint(m_radius - 0.75, noteAngle - dAngle / 2);
					topRight = getCirclePoint(m_radius - 1.25, noteAngle + dAngle / 2);
					bottomRight = getCirclePoint(m_radius - 1.25, noteAngle - dAngle / 2);
				}

				glm::vec4 OneInfo = m_objectAtlas.at(TAP_BLUE_1);
				glm::vec4 TwoInfo = m_objectAtlas.at(TAP_BLUE_2);
				glm::vec4 ThreeInfo = m_objectAtlas.at(TAP_BLUE_3);
				glm::vec4 FourInfo = m_objectAtlas.at(TAP_BLUE_4);
				if (m_renderEuActive) {
					OneInfo = m_objectAtlas.at(TAP_EUPHORIA_1);
					TwoInfo = m_objectAtlas.at(TAP_EUPHORIA_2);
					ThreeInfo = m_objectAtlas.at(TAP_EUPHORIA_3);
					FourInfo = m_objectAtlas.at(TAP_EUPHORIA_4);
				}

				//layer 1
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 0 * height, -topLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 0 * height, -bottomLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 0 * height, -bottomRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 0 * height, -topRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

				//layer 2
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 1 * height, -topLeft.y + center.y, TwoInfo.x, 1.0f - TwoInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 1 * height, -bottomLeft.y + center.y, TwoInfo.x, 1.0f - TwoInfo.y - TwoInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 1 * height, -bottomRight.y + center.y, TwoInfo.x + TwoInfo.z, 1.0f - TwoInfo.y - TwoInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 1 * height, -topRight.y + center.y, TwoInfo.x + TwoInfo.z, 1.0f - TwoInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

				//layer 3
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 2 * height, -topLeft.y + center.y, ThreeInfo.x, 1.0f - ThreeInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 2 * height, -bottomLeft.y + center.y, ThreeInfo.x, 1.0f - ThreeInfo.y - ThreeInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 2 * height, -bottomRight.y + center.y, ThreeInfo.x + ThreeInfo.z, 1.0f - ThreeInfo.y - ThreeInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 2 * height, -topRight.y + center.y, ThreeInfo.x + ThreeInfo.z, 1.0f - ThreeInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

				//layer 4
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 3 * height, -topLeft.y + center.y, FourInfo.x, 1.0f - FourInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 3 * height, -bottomLeft.y + center.y, FourInfo.x, 1.0f - FourInfo.y - FourInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 3 * height, -bottomRight.y + center.y, FourInfo.x + FourInfo.z, 1.0f - FourInfo.y - FourInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 3 * height, -topRight.y + center.y, FourInfo.x + FourInfo.z, 1.0f - FourInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

			} else if (type == SCR_G_UP && !note.getTouched()) {
				double dAngle = asin(0.5 / (m_radius + 0.5));
				glm::vec2 topLeft = getCirclePoint(m_radius + 0.75, noteAngle + dAngle / 2);
				glm::vec2 bottomLeft = getCirclePoint(m_radius + 0.75, noteAngle - dAngle / 2);
				glm::vec2 topRight = getCirclePoint(m_radius + 0.25, noteAngle + dAngle / 2);
				glm::vec2 bottomRight = getCirclePoint(m_radius + 0.25, noteAngle - dAngle / 2);

				if (note.getLanMod() == 0) {
					dAngle = asin(0.5 / (m_radius + 1.0));
					topLeft = getCirclePoint(m_radius + 1.25, noteAngle + dAngle / 2);
					bottomLeft = getCirclePoint(m_radius + 1.25, noteAngle - dAngle / 2);
					topRight = getCirclePoint(m_radius + 0.75, noteAngle + dAngle / 2);
					bottomRight = getCirclePoint(m_radius + 0.75, noteAngle - dAngle / 2);
				}

				glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_UP);
				//layer 1
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 0 * height, -topLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 0 * height, -bottomLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 0 * height, -bottomRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 0 * height, -topRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

			} else if (type == SCR_G_DOWN && !note.getTouched()) {
				double dAngle = asin(0.5 / (m_radius + 0.5));
				glm::vec2 topLeft = getCirclePoint(m_radius + 0.75, noteAngle + dAngle / 2);
				glm::vec2 bottomLeft = getCirclePoint(m_radius + 0.75, noteAngle - dAngle / 2);
				glm::vec2 topRight = getCirclePoint(m_radius + 0.25, noteAngle + dAngle / 2);
				glm::vec2 bottomRight = getCirclePoint(m_radius + 0.25, noteAngle - dAngle / 2);

				if (note.getLanMod() == 0) {
					dAngle = asin(0.5 / (m_radius + 1.0));
					topLeft = getCirclePoint(m_radius + 1.25, noteAngle + dAngle / 2);
					bottomLeft = getCirclePoint(m_radius + 1.25, noteAngle - dAngle / 2);
					topRight = getCirclePoint(m_radius + 0.75, noteAngle + dAngle / 2);
					bottomRight = getCirclePoint(m_radius + 0.75, noteAngle - dAngle / 2);
				}

				glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_DOWN);
				//layer 1
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 0 * height, -topLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 0 * height, -bottomLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 0 * height, -bottomRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 0 * height, -topRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

			} else if (type == SCR_G_ANY) {
				double dAngle = asin(0.75 / (m_radius + 0.5));
				double timeBetween = getDTFromAngle(dAngle);

				for (double j = milli; j < milli + note.getLength(); j += timeBetween) {
					if (j >= time && j <= time + m_noteVisibleTime) {
						double spriteAngle = getAngleFromDT(j - time);

						glm::vec2 topLeft = getCirclePoint(m_radius + 0.75, spriteAngle + dAngle / 2);
						glm::vec2 bottomLeft = getCirclePoint(m_radius + 0.75, spriteAngle - dAngle / 2);
						glm::vec2 topRight = getCirclePoint(m_radius + 0.25, spriteAngle + dAngle / 2);
						glm::vec2 bottomRight = getCirclePoint(m_radius + 0.25, spriteAngle - dAngle / 2);

						if (getCrossAtTime(j, cross) == 0) {
							dAngle = asin(0.5 / (m_radius + 1.0));
							topLeft = getCirclePoint(m_radius + 1.25, spriteAngle + dAngle / 2);
							bottomLeft = getCirclePoint(m_radius + 1.25, spriteAngle - dAngle / 2);
							topRight = getCirclePoint(m_radius + 0.75, spriteAngle + dAngle / 2);
							bottomRight = getCirclePoint(m_radius + 0.75, spriteAngle - dAngle / 2);
						}

						glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_ANYDIR);
						//layer 1
						pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 0 * height, -topLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y);
						pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 0 * height, -bottomLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
						pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 0 * height, -bottomRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
						pushVertexTexture(noteVector, -topRight.x + center.x, plane + 0 * height, -topRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
						pushRectangleIndices(noteIndices, noteVertexCount);
					}
				}
			} else if (type == SCR_B_UP && !note.getTouched()) {
				double dAngle = asin(0.5 / (m_radius - 0.5));
				glm::vec2 topLeft = getCirclePoint(m_radius - 0.25, noteAngle + dAngle / 2);
				glm::vec2 bottomLeft = getCirclePoint(m_radius - 0.25, noteAngle - dAngle / 2);
				glm::vec2 topRight = getCirclePoint(m_radius - 0.75, noteAngle + dAngle / 2);
				glm::vec2 bottomRight = getCirclePoint(m_radius - 0.75, noteAngle - dAngle / 2);

				if (note.getLanMod() == 2) {
					dAngle = asin(0.5 / (m_radius - 1.0));
					topLeft = getCirclePoint(m_radius - 0.75, noteAngle + dAngle / 2);
					bottomLeft = getCirclePoint(m_radius - 0.75, noteAngle - dAngle / 2);
					topRight = getCirclePoint(m_radius - 1.25, noteAngle + dAngle / 2);
					bottomRight = getCirclePoint(m_radius - 1.25, noteAngle - dAngle / 2);
				}

				glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_UP);
				//layer 1
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 0 * height, -topLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 0 * height, -bottomLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 0 * height, -bottomRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 0 * height, -topRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

			} else if (type == SCR_B_DOWN && !note.getTouched()) {
				double dAngle = asin(0.5 / (m_radius - 0.5));
				glm::vec2 topLeft = getCirclePoint(m_radius - 0.25, noteAngle + dAngle / 2);
				glm::vec2 bottomLeft = getCirclePoint(m_radius - 0.25, noteAngle - dAngle / 2);
				glm::vec2 topRight = getCirclePoint(m_radius - 0.75, noteAngle + dAngle / 2);
				glm::vec2 bottomRight = getCirclePoint(m_radius - 0.75, noteAngle - dAngle / 2);

				if (note.getLanMod() == 2) {
					dAngle = asin(0.5 / (m_radius - 1.0));
					topLeft = getCirclePoint(m_radius - 0.75, noteAngle + dAngle / 2);
					bottomLeft = getCirclePoint(m_radius - 0.75, noteAngle - dAngle / 2);
					topRight = getCirclePoint(m_radius - 1.25, noteAngle + dAngle / 2);
					bottomRight = getCirclePoint(m_radius - 1.25, noteAngle - dAngle / 2);
				}

				glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_DOWN);
				//layer 1
				pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 0 * height, -topLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y);
				pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 0 * height, -bottomLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 0 * height, -bottomRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
				pushVertexTexture(noteVector, -topRight.x + center.x, plane + 0 * height, -topRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
				pushRectangleIndices(noteIndices, noteVertexCount);

			} else if (type == SCR_B_ANY) {
				double dAngle = asin(0.75 / (m_radius - 0.5));
				double timeBetween = getDTFromAngle(dAngle);

				for (double j = milli; j < milli + note.getLength(); j += timeBetween) {
					if (j >= time && j <= time + m_noteVisibleTime) {
						double spriteAngle = getAngleFromDT(j - time);

						glm::vec2 topLeft = getCirclePoint(m_radius - 0.25, spriteAngle + dAngle / 2);
						glm::vec2 bottomLeft = getCirclePoint(m_radius - 0.25, spriteAngle - dAngle / 2);
						glm::vec2 topRight = getCirclePoint(m_radius - 0.75, spriteAngle + dAngle / 2);
						glm::vec2 bottomRight = getCirclePoint(m_radius - 0.75, spriteAngle - dAngle / 2);

						if (getCrossAtTime(j, cross) == 2) {
							dAngle = asin(0.5 / (m_radius - 1.0));
							topLeft = getCirclePoint(m_radius - 0.75, spriteAngle + dAngle / 2);
							bottomLeft = getCirclePoint(m_radius - 0.75, spriteAngle - dAngle / 2);
							topRight = getCirclePoint(m_radius - 1.25, spriteAngle + dAngle / 2);
							bottomRight = getCirclePoint(m_radius - 1.25, spriteAngle - dAngle / 2);
						}

						glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_ANYDIR);
						//layer 1
						pushVertexTexture(noteVector, -topLeft.x + center.x, plane + 0 * height, -topLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y);
						pushVertexTexture(noteVector, -bottomLeft.x + center.x, plane + 0 * height, -bottomLeft.y + center.y, OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
						pushVertexTexture(noteVector, -bottomRight.x + center.x, plane + 0 * height, -bottomRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
						pushVertexTexture(noteVector, -topRight.x + center.x, plane + 0 * height, -topRight.y + center.y, OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
						pushRectangleIndices(noteIndices, noteVertexCount);
					}
				}
			}

		} else if (milli >= time + m_noteVisibleTime && note.getLanMod() == -1) {
			//if the note is outside the visible area, update lane position
			int position = 1;
			for (auto& c : cross) {
				if (c.getMilli() <= milli) {
					if (c.getType() == CROSS_G) {
						position = 0;
					} else if (c.getType() == CROSS_C) {
						position = 1;
					} else if (c.getType() == CROSS_B) {
						position = 2;
					}
				}
			}
			note.setLanMod(position);
		}
	}

	usePersProj();
	renderTexture(noteVector, noteIndices, m_objTexture);
}

void GameRender::lanes(double time, std::vector<Note>& v, std::vector<Note>& cross) {
	float plane = 0.0;

	//vertices data for the lanes

	std::vector<float> lanesVector = {};
	std::vector<unsigned int> lanesIndices = {};
	unsigned int lanesVertexCount = 0;

	//copying into local variables for overwite protection
	glm::vec4 greenActiveColor = m_greenLaneActiveColor;
	glm::vec4 greenInactiveColor = m_greenLaneInactiveColor;
	glm::vec4 blueActiveColor = m_blueLaneActiveColor;
	glm::vec4 blueInactiveColor = m_blueLaneInactiveColor;
	glm::vec4 redColor = m_redLaneActiveColor;
	glm::vec4 euphoriaColor = m_euphoriaLaneColor;

	/*
	startImGuiFrame("test",ImGuiBackendFlags_None);
	ImGui::ColorEdit4("Green",greenColor.data());
	ImGui::ColorEdit4("Red",redColor.data());
	ImGui::ColorEdit4("Blue",blueColor.data());
	ImGui::ColorEdit4("Euphoria",euphoriaColor.data());
	renderImGuiFrame();
	*/

	if (m_renderEuActive) {
		greenActiveColor = euphoriaColor;
		redColor = euphoriaColor;
		blueActiveColor = euphoriaColor;
	}

	float size = 0.05;

	float startAngle = asin(0.25f / m_radius);

	glm::vec2 center = {m_radius, 0.0};
	glm::vec2 redBeforeOuter = getCirclePoint((double)m_radius + size, startAngle);
	glm::vec2 redBeforeInner = getCirclePoint((double)m_radius - size, startAngle);
	glm::vec2 greenBeforeOuter = getCirclePoint((double)m_radius + 0.5 + size, startAngle);
	glm::vec2 greenBeforeInner = getCirclePoint((double)m_radius + 0.5 - size, startAngle);
	glm::vec2 blueBeforeOuter = getCirclePoint((double)m_radius - 0.5 + size, startAngle);
	glm::vec2 blueBeforeInner = getCirclePoint((double)m_radius - 0.5 - size, startAngle);

	for (float angle = startAngle; angle < m_maxAngle; angle += m_deltaAngle) {
		auto angleTime = (float)(time + getDTFromAngle(angle));

		glm::vec2 redOuter = getCirclePoint((double)m_radius + size, (double)angle);
		glm::vec2 redInner = getCirclePoint((double)m_radius - size, (double)angle);

		pushVertexColor(lanesVector, -redBeforeOuter.x + center.x, plane, -redBeforeOuter.y - center.y, redColor[0], redColor[1], redColor[2], redColor[3]);
		pushVertexColor(lanesVector, -redBeforeInner.x + center.x, plane, -redBeforeInner.y - center.y, redColor[0], redColor[1], redColor[2], redColor[3]);
		pushVertexColor(lanesVector, -redInner.x + center.x, plane, -redInner.y - center.y, redColor[0], redColor[1], redColor[2], redColor[3]);
		pushVertexColor(lanesVector, -redOuter.x + center.x, plane, -redOuter.y - center.y, redColor[0], redColor[1], redColor[2], redColor[3]);
		pushRectangleIndices(lanesIndices, lanesVertexCount);

		redBeforeOuter = redOuter;
		redBeforeInner = redInner;

		//green

		glm::vec2 greenOuter;
		glm::vec2 greenInner;
		if (getCrossAtTime(angleTime, cross) == 0) {
			greenOuter = getCirclePoint((double)m_radius + 1.0 + size, (double)angle);
			greenInner = getCirclePoint((double)m_radius + 1.0f - size, (double)angle);

			pushVertexColor(lanesVector, -greenBeforeOuter.x + center.x, plane, -greenBeforeOuter.y - center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
			pushVertexColor(lanesVector, -greenBeforeInner.x + center.x, plane, -greenBeforeInner.y - center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
			pushVertexColor(lanesVector, -greenInner.x + center.x, plane, -greenInner.y - center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
			pushVertexColor(lanesVector, -greenOuter.x + center.x, plane, -greenOuter.y - center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
		} else if (getCrossAtTime(angleTime, cross) == 2) {
			greenOuter = getCirclePoint((double)m_radius + 0.5 + size, (double)angle);
			greenInner = getCirclePoint((double)m_radius + 0.5 - size, (double)angle);

			pushVertexColor(lanesVector, -greenBeforeOuter.x + center.x, plane, -greenBeforeOuter.y - center.y, greenInactiveColor.r, greenInactiveColor.g, greenInactiveColor.b, greenInactiveColor.a);
			pushVertexColor(lanesVector, -greenBeforeInner.x + center.x, plane, -greenBeforeInner.y - center.y, greenInactiveColor.r, greenInactiveColor.g, greenInactiveColor.b, greenInactiveColor.a);
			pushVertexColor(lanesVector, -greenInner.x + center.x, plane, -greenInner.y - center.y, greenInactiveColor.r, greenInactiveColor.g, greenInactiveColor.b, greenInactiveColor.a);
			pushVertexColor(lanesVector, -greenOuter.x + center.x, plane, -greenOuter.y - center.y, greenInactiveColor.r, greenInactiveColor.g, greenInactiveColor.b, greenInactiveColor.a);
		} else {
			greenOuter = getCirclePoint((double)m_radius + 0.5 + size, (double)angle);
			greenInner = getCirclePoint((double)m_radius + 0.5 - size, (double)angle);

			pushVertexColor(lanesVector, -greenBeforeOuter.x + center.x, plane, -greenBeforeOuter.y - center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
			pushVertexColor(lanesVector, -greenBeforeInner.x + center.x, plane, -greenBeforeInner.y - center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
			pushVertexColor(lanesVector, -greenInner.x + center.x, plane, -greenInner.y - center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
			pushVertexColor(lanesVector, -greenOuter.x + center.x, plane, -greenOuter.y - center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
		}

		pushRectangleIndices(lanesIndices, lanesVertexCount);

		greenBeforeOuter = greenOuter;
		greenBeforeInner = greenInner;

		//blue

		glm::vec2 blueOuter;
		glm::vec2 blueInner;
		if (getCrossAtTime(angleTime, cross) == 0) {
			blueOuter = getCirclePoint((double)m_radius - 0.5 + size, (double)angle);
			blueInner = getCirclePoint((double)m_radius - 0.5 - size, (double)angle);

			pushVertexColor(lanesVector, -blueBeforeOuter.x + center.x, 0.0, -blueBeforeOuter.y - center.y, blueInactiveColor.r, blueInactiveColor.g, blueInactiveColor.b, blueInactiveColor.a);
			pushVertexColor(lanesVector, -blueBeforeInner.x + center.x, 0.0, -blueBeforeInner.y - center.y, blueInactiveColor.r, blueInactiveColor.g, blueInactiveColor.b, blueInactiveColor.a);
			pushVertexColor(lanesVector, -blueInner.x + center.x, 0.0, -blueInner.y - center.y, blueInactiveColor.r, blueInactiveColor.g, blueInactiveColor.b, blueInactiveColor.a);
			pushVertexColor(lanesVector, -blueOuter.x + center.x, 0.0, -blueOuter.y - center.y, blueInactiveColor.r, blueInactiveColor.g, blueInactiveColor.b, blueInactiveColor.a);
		} else if (getCrossAtTime(angleTime, cross) == 2) {
			blueOuter = getCirclePoint((double)m_radius - 1.0 + size, (double)angle);
			blueInner = getCirclePoint((double)m_radius - 1.0f - size, (double)angle);

			pushVertexColor(lanesVector, -blueBeforeOuter.x + center.x, 0.0, -blueBeforeOuter.y - center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
			pushVertexColor(lanesVector, -blueBeforeInner.x + center.x, 0.0, -blueBeforeInner.y - center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
			pushVertexColor(lanesVector, -blueInner.x + center.x, 0.0, -blueInner.y - center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
			pushVertexColor(lanesVector, -blueOuter.x + center.x, 0.0, -blueOuter.y - center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
		} else {
			blueOuter = getCirclePoint((double)m_radius - 0.5 + size, (double)angle);
			blueInner = getCirclePoint((double)m_radius - 0.5 - size, (double)angle);

			pushVertexColor(lanesVector, -blueBeforeOuter.x + center.x, 0.0, -blueBeforeOuter.y - center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
			pushVertexColor(lanesVector, -blueBeforeInner.x + center.x, 0.0, -blueBeforeInner.y - center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
			pushVertexColor(lanesVector, -blueInner.x + center.x, 0.0, -blueInner.y - center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
			pushVertexColor(lanesVector, -blueOuter.x + center.x, 0.0, -blueOuter.y - center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
		}

		pushRectangleIndices(lanesIndices, lanesVertexCount);

		blueBeforeOuter = blueOuter;
		blueBeforeInner = blueInner;
	}
	for (auto& note : v) {
		double milli = note.getMilli();
		if (time + m_noteVisibleTime > milli) {
			double dt = milli - time;
			double noteAngle = getAngleFromDT(dt);

			int type = note.getType();
			if (type == CF_SPIKE_G) {
				double dAngle = asin(0.25 / (m_radius + 0.5));
				glm::vec2 side = getCirclePoint(m_radius + 1.0, noteAngle);
				glm::vec2 top = getCirclePoint(m_radius + 0.5, noteAngle + dAngle / 2);
				glm::vec2 bottom = getCirclePoint(m_radius + 0.5, noteAngle - dAngle / 2);

				pushVertexColor(lanesVector, -side.x + center.x, plane, -side.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
				pushVertexColor(lanesVector, -top.x + center.x, plane, -top.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
				pushVertexColor(lanesVector, -bottom.x + center.x, plane, -bottom.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
				pushTriangleIndices(lanesIndices, lanesVertexCount);

				if (note.getLanMod() == 2) {
					double dAngle = asin(0.25 / (m_radius - 1.0));
					glm::vec2 side = getCirclePoint(m_radius - 0.5, noteAngle);
					glm::vec2 top = getCirclePoint(m_radius - 1.0, noteAngle + dAngle / 2);
					glm::vec2 bottom = getCirclePoint(m_radius - 1.0, noteAngle - dAngle / 2);

					pushVertexColor(lanesVector, -side.x + center.x, plane, -side.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -top.x + center.x, plane, -top.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -bottom.x + center.x, plane, -bottom.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushTriangleIndices(lanesIndices, lanesVertexCount);
				}
			} else if (type == CF_SPIKE_B) {
				double dAngle = asin(0.25 / (m_radius - 0.5));
				glm::vec2 side = getCirclePoint(m_radius - 1.0, noteAngle);
				glm::vec2 top = getCirclePoint(m_radius - 0.5, noteAngle + dAngle / 2);
				glm::vec2 bottom = getCirclePoint(m_radius - 0.5, noteAngle - dAngle / 2);

				pushVertexColor(lanesVector, -side.x + center.x, plane, -side.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
				pushVertexColor(lanesVector, -top.x + center.x, plane, -top.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
				pushVertexColor(lanesVector, -bottom.x + center.x, plane, -bottom.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
				pushTriangleIndices(lanesIndices, lanesVertexCount);

				if (note.getLanMod() == 0) {
					double dAngle = asin(0.25 / (m_radius + 1.0));
					glm::vec2 side = getCirclePoint(m_radius + 0.5, noteAngle);
					glm::vec2 top = getCirclePoint(m_radius + 1.0, noteAngle + dAngle / 2);
					glm::vec2 bottom = getCirclePoint(m_radius + 1.0, noteAngle - dAngle / 2);

					pushVertexColor(lanesVector, -side.x + center.x, plane, -side.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -top.x + center.x, plane, -top.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -bottom.x + center.x, plane, -bottom.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushTriangleIndices(lanesIndices, lanesVertexCount);
				}
			} else if (type == CF_SPIKE_C) {
				if (note.getLanMod() == 0) {
					double dAngle = asin(0.25 / (m_radius + 1.0));
					glm::vec2 side = getCirclePoint(m_radius + 0.5, noteAngle);
					glm::vec2 top = getCirclePoint(m_radius + 1.0, noteAngle + dAngle / 2);
					glm::vec2 bottom = getCirclePoint(m_radius + 1.0, noteAngle - dAngle / 2);

					pushVertexColor(lanesVector, -side.x + center.x, plane, -side.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -top.x + center.x, plane, -top.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -bottom.x + center.x, plane, -bottom.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushTriangleIndices(lanesIndices, lanesVertexCount);
				} else if (note.getLanMod() == 2) {
					double dAngle = asin(0.25 / (m_radius - 1.0));
					glm::vec2 side = getCirclePoint(m_radius - 0.5, noteAngle);
					glm::vec2 top = getCirclePoint(m_radius - 1.0, noteAngle + dAngle / 2);
					glm::vec2 bottom = getCirclePoint(m_radius - 1.0, noteAngle - dAngle / 2);

					pushVertexColor(lanesVector, -side.x + center.x, plane, -side.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -top.x + center.x, plane, -top.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -bottom.x + center.x, plane, -bottom.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushTriangleIndices(lanesIndices, lanesVertexCount);
				}
			}
		}
	}

	for (size_t i = 1; i < cross.size(); ++i) {
		double milli = cross.at(i).getMilli();
		if (time + m_noteVisibleTime > milli) {
			double dt = milli - time;
			double noteAngle = getAngleFromDT(dt);

			int type = cross.at(i).getType();
			int typeBefore = cross.at(i - 1).getType();
			if (type == CROSS_G && typeBefore != CROSS_G) {
				double dAngle = asin(0.25 / (m_radius + 0.5));

				glm::vec2 topLeft = getCirclePoint(m_radius + 1.0 + size, noteAngle + dAngle / 2);
				glm::vec2 bottomLeft = getCirclePoint(m_radius + 1.0 + size, noteAngle - dAngle / 2);
				glm::vec2 bottomRight = getCirclePoint(m_radius + 0.5 - size, noteAngle - dAngle / 2);
				glm::vec2 topRight = getCirclePoint(m_radius + 0.5 - size, noteAngle + dAngle / 2);

				pushVertexColor(lanesVector, -topLeft.x + center.x, plane, -topLeft.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
				pushVertexColor(lanesVector, -bottomLeft.x + center.x, plane, -bottomLeft.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
				pushVertexColor(lanesVector, -bottomRight.x + center.x, plane, -bottomRight.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
				pushVertexColor(lanesVector, -topRight.x + center.x, plane, -topRight.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
				pushRectangleIndices(lanesIndices, lanesVertexCount);

				if (typeBefore == CROSS_B) {
					dAngle = asin(0.25 / (m_radius - 0.5));

					topLeft = getCirclePoint(m_radius - 0.5 + size, noteAngle + dAngle / 2);
					bottomLeft = getCirclePoint(m_radius - 0.5 + size, noteAngle - dAngle / 2);
					bottomRight = getCirclePoint(m_radius - 1.0f - size, noteAngle - dAngle / 2);
					topRight = getCirclePoint(m_radius - 1.0f - size, noteAngle + dAngle / 2);

					pushVertexColor(lanesVector, -topLeft.x + center.x, plane, -topLeft.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -bottomLeft.x + center.x, plane, -bottomLeft.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -bottomRight.x + center.x, plane, -bottomRight.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -topRight.x + center.x, plane, -topRight.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushRectangleIndices(lanesIndices, lanesVertexCount);
				}

			} else if (type == CROSS_B && typeBefore != CROSS_B) {
				double dAngle = asin(0.25 / (m_radius - 0.5));

				glm::vec2 topLeft = getCirclePoint(m_radius - 0.5 + size, noteAngle + dAngle / 2);
				glm::vec2 bottomLeft = getCirclePoint(m_radius - 0.5 + size, noteAngle - dAngle / 2);
				glm::vec2 bottomRight = getCirclePoint(m_radius - 1.0f - size, noteAngle - dAngle / 2);
				glm::vec2 topRight = getCirclePoint(m_radius - 1.0f - size, noteAngle + dAngle / 2);

				pushVertexColor(lanesVector, -topLeft.x + center.x, plane, -topLeft.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
				pushVertexColor(lanesVector, -bottomLeft.x + center.x, plane, -bottomLeft.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
				pushVertexColor(lanesVector, -bottomRight.x + center.x, plane, -bottomRight.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
				pushVertexColor(lanesVector, -topRight.x + center.x, plane, -topRight.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
				pushRectangleIndices(lanesIndices, lanesVertexCount);

				if (typeBefore == CROSS_G) {
					dAngle = asin(0.25 / (m_radius + 0.5));

					topLeft = getCirclePoint(m_radius + 1.0 + size, noteAngle + dAngle / 2);
					bottomLeft = getCirclePoint(m_radius + 1.0 + size, noteAngle - dAngle / 2);
					bottomRight = getCirclePoint(m_radius + 0.5 - size, noteAngle - dAngle / 2);
					topRight = getCirclePoint(m_radius + 0.5 - size, noteAngle + dAngle / 2);

					pushVertexColor(lanesVector, -topLeft.x + center.x, plane, -topLeft.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -bottomLeft.x + center.x, plane, -bottomLeft.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -bottomRight.x + center.x, plane, -bottomRight.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -topRight.x + center.x, plane, -topRight.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushRectangleIndices(lanesIndices, lanesVertexCount);
				}

			} else if (type == CROSS_C) {
				if (typeBefore == CROSS_G) {
					double dAngle = asin(0.25 / (m_radius + 0.5));

					glm::vec2 topLeft = getCirclePoint(m_radius + 1.0 + size, noteAngle + dAngle / 2);
					glm::vec2 bottomLeft = getCirclePoint(m_radius + 1.0 + size, noteAngle - dAngle / 2);
					glm::vec2 bottomRight = getCirclePoint(m_radius + 0.5 - size, noteAngle - dAngle / 2);
					glm::vec2 topRight = getCirclePoint(m_radius + 0.5 - size, noteAngle + dAngle / 2);

					pushVertexColor(lanesVector, -topLeft.x + center.x, plane, -topLeft.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -bottomLeft.x + center.x, plane, -bottomLeft.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -bottomRight.x + center.x, plane, -bottomRight.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushVertexColor(lanesVector, -topRight.x + center.x, plane, -topRight.y + center.y, greenActiveColor.r, greenActiveColor.g, greenActiveColor.b, greenActiveColor.a);
					pushRectangleIndices(lanesIndices, lanesVertexCount);
				} else if (typeBefore == CROSS_B) {
					double dAngle = asin(0.25 / (m_radius - 0.5));

					glm::vec2 topLeft = getCirclePoint(m_radius - 0.5 + size, noteAngle + dAngle / 2);
					glm::vec2 bottomLeft = getCirclePoint(m_radius - 0.5 + size, noteAngle - dAngle / 2);
					glm::vec2 bottomRight = getCirclePoint(m_radius - 1.0f - size, noteAngle - dAngle / 2);
					glm::vec2 topRight = getCirclePoint(m_radius - 1.0f - size, noteAngle + dAngle / 2);

					pushVertexColor(lanesVector, -topLeft.x + center.x, plane, -topLeft.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -bottomLeft.x + center.x, plane, -bottomLeft.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -bottomRight.x + center.x, plane, -bottomRight.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushVertexColor(lanesVector, -topRight.x + center.x, plane, -topRight.y + center.y, blueActiveColor.r, blueActiveColor.g, blueActiveColor.b, blueActiveColor.a);
					pushRectangleIndices(lanesIndices, lanesVertexCount);
				}
			}
		}
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
	float plane = 0.0;
	float transparency = 0.35f; // euphoria transparency

	//vertices data
	std::vector<float> eventsVector = {};
	std::vector<unsigned int> eventsIndices = {};
	unsigned int eventsVertexCount = 0;

	//loop for every event inside event vector
	for (auto& event : ev) {
		double milli = event.getMilli();
		int type = event.getType();
		double length = event.getLength();

		if (time + m_noteVisibleTime > milli) {
			if (type == SCR_G_ZONE) {
				double startOffset = getDTFromAngle(asin(0.5 / (m_radius + 0.5)));

				double start = std::max(milli - startOffset, time);
				double end = std::min(milli + length, time + m_noteVisibleTime);

				double startAngle = getAngleFromDT(start - time);
				double endAngle = getAngleFromDT(end - time);
				glm::vec2 center = {m_radius, 0.0};

				glm::vec2 beforeOuter = getCirclePoint((double)m_radius + 0.75, startAngle);
				glm::vec2 beforeInner = getCirclePoint((double)m_radius + 0.25, startAngle);

				if (event.getLanMod() == 0) {
					beforeOuter = getCirclePoint((double)m_radius + 1.25, startAngle);
					beforeInner = getCirclePoint((double)m_radius + 0.75, startAngle);
				}
				for (double cycleAngle = startAngle; cycleAngle < endAngle; cycleAngle += m_deltaAngle) {
					double cycleTime = getDTFromAngle(cycleAngle);

					glm::vec2 outer = getCirclePoint(m_radius + 0.75, cycleAngle);
					glm::vec2 inner = getCirclePoint(m_radius + 0.25, cycleAngle);

					if (getCrossAtTime(time + cycleTime, cross) == 0) {
						outer = getCirclePoint(m_radius + 1.25, cycleAngle);
						inner = getCirclePoint(m_radius + 0.75, cycleAngle);
					}

					pushVertexColor(eventsVector, -outer.x + center.x, plane, -outer.y - center.y, m_greenScratchColor.r, m_greenScratchColor.g, m_greenScratchColor.b, m_greenScratchColor.a);
					pushVertexColor(eventsVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, m_greenScratchColor.r, m_greenScratchColor.g, m_greenScratchColor.b, m_greenScratchColor.a);
					pushVertexColor(eventsVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, m_greenScratchColor.r, m_greenScratchColor.g, m_greenScratchColor.b, m_greenScratchColor.a);
					pushVertexColor(eventsVector, -inner.x + center.x, plane, -inner.y - center.y, m_greenScratchColor.r, m_greenScratchColor.g, m_greenScratchColor.b, m_greenScratchColor.a);

					pushRectangleIndices(eventsIndices, eventsVertexCount);

					beforeOuter = outer;
					beforeInner = inner;
				}
			} else if (type == SCR_B_ZONE) {
				double startOffset = getDTFromAngle(asin(0.5 / (m_radius - 0.5)));

				double start = std::max(milli - startOffset, time);
				double end = std::min(milli + length, time + m_noteVisibleTime);

				double startAngle = getAngleFromDT(start - time);
				double endAngle = getAngleFromDT(end - time);
				glm::vec2 center = {m_radius, 0.0};

				glm::vec2 beforeOuter = getCirclePoint((double)m_radius - 0.75, startAngle);
				glm::vec2 beforeInner = getCirclePoint((double)m_radius - 0.25, startAngle);

				if (event.getLanMod() == 2) {
					beforeOuter = getCirclePoint((double)m_radius - 1.25, startAngle);
					beforeInner = getCirclePoint((double)m_radius - 0.75, startAngle);
				}
				for (double cycleAngle = startAngle; cycleAngle < endAngle; cycleAngle += m_deltaAngle) {
					double cycleTime = getDTFromAngle(cycleAngle);

					glm::vec2 outer = getCirclePoint(m_radius - 0.75, cycleAngle);
					glm::vec2 inner = getCirclePoint(m_radius - 0.25, cycleAngle);

					if (getCrossAtTime(time + cycleTime, cross) == 2) {
						outer = getCirclePoint(m_radius - 1.25, cycleAngle);
						inner = getCirclePoint(m_radius - 0.75, cycleAngle);
					}

					pushVertexColor(eventsVector, -outer.x + center.x, plane, -outer.y - center.y, m_blueScratchColor.r, m_blueScratchColor.g, m_blueScratchColor.b, m_blueScratchColor.a);
					pushVertexColor(eventsVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, m_blueScratchColor.r, m_blueScratchColor.g, m_blueScratchColor.b, m_blueScratchColor.a);
					pushVertexColor(eventsVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, m_blueScratchColor.r, m_blueScratchColor.g, m_blueScratchColor.b, m_blueScratchColor.a);
					pushVertexColor(eventsVector, -inner.x + center.x, plane, -inner.y - center.y, m_blueScratchColor.r, m_blueScratchColor.g, m_blueScratchColor.b, m_blueScratchColor.a);

					pushRectangleIndices(eventsIndices, eventsVertexCount);

					beforeOuter = outer;
					beforeInner = inner;
				}
			} else if (type == EU_ZONE) {
				double start = std::max(milli, time);
				double end = std::min(milli + length, time + m_noteVisibleTime);

				double startAngle = getAngleFromDT(start - time);
				double endAngle = getAngleFromDT(end - time);
				glm::vec2 center = {m_radius, 0.0};

				glm::vec2 beforeOuter = getCirclePoint((double)m_radius + m_deltaRadius, startAngle);
				glm::vec2 beforeInner = getCirclePoint((double)m_radius - m_deltaRadius, startAngle);

				for (double cycleAngle = startAngle; cycleAngle < endAngle; cycleAngle += m_deltaAngle) {
					double cycleTime = getDTFromAngle(cycleAngle);

					glm::vec2 outer = getCirclePoint((double)m_radius + m_deltaRadius, cycleAngle);
					glm::vec2 inner = getCirclePoint((double)m_radius - m_deltaRadius, cycleAngle);

					pushVertexColor(eventsVector, -outer.x + center.x, plane, -outer.y - center.y, m_euphoriaZoneColor.r, m_euphoriaZoneColor.g, m_euphoriaZoneColor.b, m_euphoriaZoneColor.a);
					pushVertexColor(eventsVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, m_euphoriaZoneColor.r, m_euphoriaZoneColor.g, m_euphoriaZoneColor.b, m_euphoriaZoneColor.a);
					pushVertexColor(eventsVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, m_euphoriaZoneColor.r, m_euphoriaZoneColor.g, m_euphoriaZoneColor.b, m_euphoriaZoneColor.a);
					pushVertexColor(eventsVector, -inner.x + center.x, plane, -inner.y - center.y, m_euphoriaZoneColor.r, m_euphoriaZoneColor.g, m_euphoriaZoneColor.b, m_euphoriaZoneColor.a);

					pushRectangleIndices(eventsIndices, eventsVertexCount);

					beforeOuter = outer;
					beforeInner = inner;
				}
			}
		}

		else if (milli > time + m_noteVisibleTime && event.getLanMod() == -1) {
			int position = 1;
			for (auto& c : cross) {
				if (c.getMilli() <= milli) {
					if (c.getType() == CROSS_G) {
						position = 0;
					} else if (c.getType() == CROSS_C) {
						position = 1;
					} else if (c.getType() == CROSS_B) {
						position = 2;
					}
				}
			}
			event.setLanMod(position);
		}
	}
	usePersProj();
	renderColor(eventsVector, eventsIndices);
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
	fps.append(std::to_string(1 / deltaTime));
	drawText(fps, 10.0f, 170.0f, 0.05f);

	std::string baseScore;
	baseScore.append(std::to_string((float)m_playerScore / (float)m_genBaseScore));
	baseScore.append("|");
	baseScore.append(std::to_string(m_genBaseScore));
	drawText(baseScore, 940.0f, 370.0f, 0.03f);

	drawText(std::to_string(m_renderEuValue), 200.0, 500.0f, 0.03f);

	if(framerate.is_open()){
		framerate << deltaTime << "," << 1/deltaTime << "," << v.size() << "," << ev.size() << "," << c.size() << "," << std::endl;
	}
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

	bool centerToGreen = p.m_cfCenterToGreen;
	bool centerToBlue = p.m_cfCenterToBlue;
	bool greenToCenter = p.m_cfGreenToCenter;
	bool blueToCenter = p.m_cfBlueToCenter;

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
	if (centerToGreen) {
		m_animManager.triggerAnimation(AN_CROSS_GREEN_TO_LEFT, time);
		p.m_cfCenterToGreen = false;
	}
	if (centerToBlue) {
		m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_RIGHT, time);
		p.m_cfCenterToBlue = false;
	}
	if (greenToCenter) {
		m_animManager.triggerAnimation(AN_CROSS_GREEN_TO_CENTER, time);
		p.m_cfGreenToCenter = false;
	}
	if (blueToCenter) {
		m_animManager.triggerAnimation(AN_CROSS_BLUE_TO_CENTER, time);
		p.m_cfBlueToCenter = false;
	}
	rendr_InvertedX = m_isButtonsRight;
}

void GameRender::clickerAnimation() {
	float plane = 0.0;
	std::vector<float> clickerVector = {};
	std::vector<unsigned int> clickerIndices = {};
	unsigned int clickerVertexCount = 0;

	Animation greenAnim = m_animManager.getAnimById(AN_GREEN_CLICKER);
	Animation redAnim = m_animManager.getAnimById(AN_RED_CLICKER);
	Animation blueAnim = m_animManager.getAnimById(AN_BLUE_CLICKER);

	if (greenAnim.isEnabled()) {
		double x = greenAnim.getPercent();
		x = floor(x * 10) / 10;

		float y = 1.0f;
		if (m_renderEuActive) {
			y = 0.25f;
		}

		pushVertexTexture(clickerVector, m_greenLeft, plane, m_back, (float)x, y);
		pushVertexTexture(clickerVector, m_greenLeft, plane, m_front, (float)x, y - 0.25f);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_front, (float)x + 0.1f, y - 0.25f);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_back, (float)x + 0.1f, y);
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	if (redAnim.isEnabled()) {
		double x = redAnim.getPercent();
		x = floor(x * 10) / 10;

		float y = 0.75f;
		if (m_renderEuActive) {
			y = 0.25f;
		}

		pushVertexTexture(clickerVector, -0.25, plane, m_back, (float)x, y);
		pushVertexTexture(clickerVector, -0.25, plane, m_front, (float)x, y - 0.25f);
		pushVertexTexture(clickerVector, 0.25, plane, m_front, (float)x + 0.1f, y - 0.25f);
		pushVertexTexture(clickerVector, 0.25, plane, m_back, (float)x + 0.1f, y);

		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	if (blueAnim.isEnabled()) {
		double x = blueAnim.getPercent();
		x = floor(x * 10) / 10;

		float y = 0.5f;
		if (m_renderEuActive) {
			y = 0.25f;
		}

		pushVertexTexture(clickerVector, m_blueLeft, plane, m_back, (float)x, y);
		pushVertexTexture(clickerVector, m_blueLeft, plane, m_front, (float)x, y - 0.25f);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_front, (float)x + 0.1f, y - 0.25f);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_back, (float)x + 0.1f, y);
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

	if (note.getLanMod() == 0) {
		lastChange = CROSS_G;
	} else if (note.getLanMod() == 2) {
		lastChange = CROSS_B;
	}

	result.emplace_back(start, lastChange, 0.0f, true);
	for (auto& c : cross) {
		double crossMilli = c.getMilli();
		int crossType = c.getType();
		if (crossMilli <= end && crossMilli >= start) {
			if (crossType == CROSS_G && lastChange != CROSS_G) {
				result.emplace_back(crossMilli, lastChange, 0.0f, true);
				lastChange = CROSS_G;
				result.emplace_back(crossMilli, lastChange, 0.0f, true);
			} else if (crossType == CROSS_C && lastChange != CROSS_C) {
				result.emplace_back(crossMilli, lastChange, 0.0f, true);
				lastChange = CROSS_C;
				result.emplace_back(crossMilli, lastChange, 0.0f, true);
			} else if (crossType == CROSS_B && lastChange != CROSS_B) {
				result.emplace_back(crossMilli, lastChange, 0.0f, true);
				lastChange = CROSS_B;
				result.emplace_back(crossMilli, lastChange, 0.0f, true);
			}
		}
	}
	result.emplace_back(end, lastChange, 0.0f, true);
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
			index = (int)i - 1;
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

double GameRender::getAngleFromDT(double dt) {
	double clickerAngle = asin(0.25 / m_radius);
	return dt * (m_maxAngle - clickerAngle) / m_noteVisibleTime + clickerAngle;
}

double GameRender::getDTFromAngle(double angle) {
	double clickerAngle = asin(0.25 / m_radius);
	return (angle - clickerAngle) * m_noteVisibleTime / (m_maxAngle - clickerAngle);
}

GameRender::~GameRender(){
	framerate.close();
}