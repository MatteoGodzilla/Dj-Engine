#include "GameRender.h"

void GameRender::init(GLFWwindow* w) {
	Rendr::init(w);

	m_animManager.pushAnimation(Animation(AN_CROSS_GREEN_TO_LEFT, 0.100));
	m_animManager.pushAnimation(Animation(AN_CROSS_GREEN_TO_CENTER, 0.100));
	m_animManager.pushAnimation(Animation(AN_CROSS_BLUE_TO_RIGHT, 0.100));
	m_animManager.pushAnimation(Animation(AN_CROSS_BLUE_TO_CENTER, 0.100));
	m_animManager.pushAnimation(Animation(AN_GREEN_CLICKER, 0.200));
	m_animManager.pushAnimation(Animation(AN_RED_CLICKER, 0.200));
	m_animManager.pushAnimation(Animation(AN_BLUE_CLICKER, 0.200));

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
		const float COLSIZE = 1.0 / 8.0;
		const float ROWSIZE = 1.0 / 6.0;

		m_objectAtlas.insert(std::make_pair(TAP_GREEN_1, glm::vec4(0 * COLSIZE, 0 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_GREEN_2, glm::vec4(1 * COLSIZE, 0 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_GREEN_3, glm::vec4(2 * COLSIZE, 0 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_GREEN_4, glm::vec4(3 * COLSIZE, 0 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(TAP_RED_1, glm::vec4(0 * COLSIZE, 1 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_RED_2, glm::vec4(1 * COLSIZE, 1 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_RED_3, glm::vec4(2 * COLSIZE, 1 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_RED_4, glm::vec4(3 * COLSIZE, 1 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(TAP_BLUE_1, glm::vec4(0 * COLSIZE, 2 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_BLUE_2, glm::vec4(1 * COLSIZE, 2 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_BLUE_3, glm::vec4(2 * COLSIZE, 2 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_BLUE_4, glm::vec4(3 * COLSIZE, 2 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(TAP_EUPHORIA_1, glm::vec4(0 * COLSIZE, 3 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_EUPHORIA_2, glm::vec4(1 * COLSIZE, 3 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_EUPHORIA_3, glm::vec4(2 * COLSIZE, 3 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TAP_EUPHORIA_4, glm::vec4(3 * COLSIZE, 3 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(CLICKER_GREEN, glm::vec4(0 * COLSIZE, 4 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(CLICKER_RED, glm::vec4(1 * COLSIZE, 4 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(CLICKER_BLUE, glm::vec4(2 * COLSIZE, 4 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(CLICKER_TRAY, glm::vec4(4 * COLSIZE, 4 * ROWSIZE, -1 * COLSIZE, 2 * ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(SCRATCH_ANYDIR, glm::vec4(0 * COLSIZE, 5 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(SCRATCH_UP, glm::vec4(1 * COLSIZE, 5 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(SCRATCH_DOWN, glm::vec4(2 * COLSIZE, 5 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(TRAIL_GREEN_MIDDLE, glm::vec4(4 * COLSIZE, 0 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TRAIL_GREEN_END, glm::vec4(5 * COLSIZE, 0 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(TRAIL_RED_MIDDLE, glm::vec4(4 * COLSIZE, 1 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TRAIL_RED_END, glm::vec4(5 * COLSIZE, 1 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(TRAIL_BLUE_MIDDLE, glm::vec4(4 * COLSIZE, 2 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TRAIL_BLUE_END, glm::vec4(5 * COLSIZE, 2 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(TRAIL_EUPHORIA_MIDDLE, glm::vec4(4 * COLSIZE, 3 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(TRAIL_EUPHORIA_END, glm::vec4(5 * COLSIZE, 3 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(FS_CROSS_GREEN_BASE_TOP, glm::vec4(4 * COLSIZE, 4 * ROWSIZE, COLSIZE * 2, ROWSIZE / 2)));
		m_objectAtlas.insert(std::make_pair(FS_CROSS_GREEN_BASE_BOTTOM, glm::vec4(4 * COLSIZE, 4.5 * ROWSIZE, COLSIZE * 2, ROWSIZE / 2)));

		m_objectAtlas.insert(std::make_pair(FS_CROSS_BLUE_BASE_TOP, glm::vec4(4 * COLSIZE, 5 * ROWSIZE, COLSIZE * 2, ROWSIZE / 2)));
		m_objectAtlas.insert(std::make_pair(FS_CROSS_BLUE_BASE_BOTTOM, glm::vec4(4 * COLSIZE, 5.5 * ROWSIZE, COLSIZE * 2, ROWSIZE / 2)));

		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_GREEN_0, glm::vec4(6 * COLSIZE, 4 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_BLUE_0, glm::vec4(7 * COLSIZE, 4 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_GREEN_1, glm::vec4(6 * COLSIZE, 3 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_BLUE_1, glm::vec4(7 * COLSIZE, 3 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_GREEN_2, glm::vec4(6 * COLSIZE, 2 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_BLUE_2, glm::vec4(7 * COLSIZE, 2 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_GREEN_3, glm::vec4(6 * COLSIZE, 1 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_BLUE_3, glm::vec4(7 * COLSIZE, 1 * ROWSIZE, COLSIZE, ROWSIZE)));

		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_GREEN_4, glm::vec4(6 * COLSIZE, 0 * ROWSIZE, COLSIZE, ROWSIZE)));
		m_objectAtlas.insert(std::make_pair(FS_CROSS_SPEAKER_BLUE_4, glm::vec4(7 * COLSIZE, 0 * ROWSIZE, COLSIZE, ROWSIZE)));
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
	float pastAngle = 0.0;

	for (float angle = 0.0; angle < m_maxAngle; angle += m_deltaAngle) {
		glm::vec2 outer = getCirclePoint((double)m_radius + m_deltaRadius, (double)angle);
		glm::vec2 inner = getCirclePoint((double)m_radius - m_deltaRadius, (double)angle);

		pushVertexTexture(highwayVector, -beforeOuter.x + center.x, 0.0, -beforeOuter.y - center.y, 0.0, (pastAngle / m_maxAngle) + offsetFactor);
		pushVertexTexture(highwayVector, -BeforeInner.x + center.x, 0.0, -BeforeInner.y - center.y, 1.0, (pastAngle / m_maxAngle) + offsetFactor);
		pushVertexTexture(highwayVector, -inner.x + center.x, 0.0, -inner.y - center.y, 1.0, (angle / m_maxAngle) + offsetFactor);
		pushVertexTexture(highwayVector, -outer.x + center.x, 0.0, -outer.y - center.y, 0.0, (angle / m_maxAngle) + offsetFactor);
		pushFourIndices(highwayIndices, highwayVertexCount);

		beforeOuter = outer;
		BeforeInner = inner;
		pastAngle = angle;
	}

	usePersProj();
	renderTexture(highwayVector, highwayIndices, m_highwayTexture);
}

void GameRender::clicker() {
	//difference in size between pressed and not
	float deltaSize = 0.05f;

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
	pushFourIndices(clickerIndices, clickerVertexCount);

	//right tray object

	pushVertexTexture(clickerVector, 0.25f, plane, m_back, clickerTray.x, 1.0f - clickerTray.y);
	pushVertexTexture(clickerVector, 0.25f, plane, m_front, clickerTray.x + clickerTray.z, 1.0f - clickerTray.y);
	pushVertexTexture(clickerVector, 1.25f, plane, m_front, clickerTray.x + clickerTray.z, 1.0f - clickerTray.y - clickerTray.w);
	pushVertexTexture(clickerVector, 1.25f, plane, m_back, clickerTray.x, 1.0f - clickerTray.y - clickerTray.w);
	pushFourIndices(clickerIndices, clickerVertexCount);

	glm::vec4 clickerRedInfo = m_objectAtlas.at(CLICKER_RED);

	if (m_red) {
		//red pressed clicker
		double dy = asin(0.25 / m_radius);
		double deltaSizeAngle = asin(deltaSize / m_radius);

		Quad q = createCircleQuad(getAngleFromDT(0.0), m_radius, 0.25 - deltaSize, dy - deltaSizeAngle, plane);
		q.v1.tex = glm::vec2(clickerRedInfo.x, 1.0f - clickerRedInfo.y);
		q.v2.tex = glm::vec2(clickerRedInfo.x, 1.0f - clickerRedInfo.y - clickerRedInfo.w);
		q.v3.tex = glm::vec2(clickerRedInfo.x + clickerRedInfo.z, 1.0f - clickerRedInfo.y - clickerRedInfo.w);
		q.v4.tex = glm::vec2(clickerRedInfo.x + clickerRedInfo.z, 1.0f - clickerRedInfo.y);

		pushQuad(clickerVector, clickerIndices, clickerVertexCount, q);
	} else {
		//red normal clicker
		double dy = asin(0.25 / m_radius);
		Quad q = createCircleQuad(getAngleFromDT(0.0), m_radius, 0.25, dy, plane);
		q.v1.tex = glm::vec2(clickerRedInfo.x, 1.0f - clickerRedInfo.y);
		q.v2.tex = glm::vec2(clickerRedInfo.x, 1.0f - clickerRedInfo.y - clickerRedInfo.w);
		q.v3.tex = glm::vec2(clickerRedInfo.x + clickerRedInfo.z, 1.0f - clickerRedInfo.y - clickerRedInfo.w);
		q.v4.tex = glm::vec2(clickerRedInfo.x + clickerRedInfo.z, 1.0f - clickerRedInfo.y);

		pushQuad(clickerVector, clickerIndices, clickerVertexCount, q);
	}

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
		pushVertexTexture(clickerVector, m_greenLeft + deltaSize, plane, m_back + deltaSize, clickerGreenInfo.x, 1.0f - clickerGreenInfo.y);
		pushVertexTexture(clickerVector, m_greenLeft + deltaSize, plane, m_front - deltaSize, clickerGreenInfo.x, 1.0f - clickerGreenInfo.y - clickerGreenInfo.w);
		pushVertexTexture(clickerVector, m_greenRight - deltaSize, plane, m_front - deltaSize, clickerGreenInfo.x + clickerGreenInfo.z, 1.0f - clickerGreenInfo.y - clickerGreenInfo.w);
		pushVertexTexture(clickerVector, m_greenRight - deltaSize, plane, m_back + deltaSize, clickerGreenInfo.x + clickerGreenInfo.z, 1.0f - clickerGreenInfo.y);
		pushFourIndices(clickerIndices, clickerVertexCount);
	} else {
		pushVertexTexture(clickerVector, m_greenLeft, plane, m_back, clickerGreenInfo.x, 1.0f - clickerGreenInfo.y);
		pushVertexTexture(clickerVector, m_greenLeft, plane, m_front, clickerGreenInfo.x, 1.0f - clickerGreenInfo.y - clickerGreenInfo.w);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_front, clickerGreenInfo.x + clickerGreenInfo.z, 1.0f - clickerGreenInfo.y - clickerGreenInfo.w);
		pushVertexTexture(clickerVector, m_greenRight, plane, m_back, clickerGreenInfo.x + clickerGreenInfo.z, 1.0f - clickerGreenInfo.y);
		pushFourIndices(clickerIndices, clickerVertexCount);
	}

	glm::vec4 clickerBlueInfo = m_objectAtlas.at(CLICKER_BLUE);

	if (m_blue) {
		pushVertexTexture(clickerVector, m_blueLeft + deltaSize, plane, m_back + deltaSize, clickerBlueInfo.x, 1.0f - clickerBlueInfo.y);
		pushVertexTexture(clickerVector, m_blueLeft + deltaSize, plane, m_front - deltaSize, clickerBlueInfo.x, 1.0f - clickerBlueInfo.y - clickerBlueInfo.w);
		pushVertexTexture(clickerVector, m_blueRight - deltaSize, plane, m_front - deltaSize, clickerBlueInfo.x + clickerBlueInfo.z, 1.0f - clickerBlueInfo.y - clickerBlueInfo.w);
		pushVertexTexture(clickerVector, m_blueRight - deltaSize, plane, m_back + deltaSize, clickerBlueInfo.x + clickerBlueInfo.z, 1.0f - clickerBlueInfo.y);
		pushFourIndices(clickerIndices, clickerVertexCount);
	} else {
		pushVertexTexture(clickerVector, m_blueLeft, plane, m_back, clickerBlueInfo.x, 1.0f - clickerBlueInfo.y);
		pushVertexTexture(clickerVector, m_blueLeft, plane, m_front, clickerBlueInfo.x, 1.0f - clickerBlueInfo.y - clickerBlueInfo.w);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_front, clickerBlueInfo.x + clickerBlueInfo.z, 1.0f - clickerBlueInfo.y - clickerBlueInfo.w);
		pushVertexTexture(clickerVector, m_blueRight, plane, m_back, clickerBlueInfo.x + clickerBlueInfo.z, 1.0f - clickerBlueInfo.y);
		pushFourIndices(clickerIndices, clickerVertexCount);
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

	if (!v.empty()) {
		for (size_t i = v.size() - 1; i >= 0 && i < v.size(); i--) {
			Note& note = v.at(i);
			double milli = note.getMilli();
			if (time + m_noteVisibleTime > milli) {
				double dt = milli - time;
				double noteAngle = getAngleFromDT(dt);

				int type = note.getType();

				if (type == TAP_R && !note.getDead() && (!note.getTouched() || note.getLength() > 15 / m_genBPM)) {
					double dAngle = asin(0.5 / m_radius);

					glm::vec4 oneInfo = m_objectAtlas.at(TAP_RED_1);
					glm::vec4 twoInfo = m_objectAtlas.at(TAP_RED_2);
					glm::vec4 threeInfo = m_objectAtlas.at(TAP_RED_3);
					glm::vec4 fourInfo = m_objectAtlas.at(TAP_RED_4);
					glm::vec4 trailMiddle = m_objectAtlas.at(TRAIL_RED_MIDDLE);
					glm::vec4 trailEnd = m_objectAtlas.at(TRAIL_RED_END);

					if (m_renderEuActive) {
						oneInfo = m_objectAtlas.at(TAP_EUPHORIA_1);
						twoInfo = m_objectAtlas.at(TAP_EUPHORIA_2);
						threeInfo = m_objectAtlas.at(TAP_EUPHORIA_3);
						fourInfo = m_objectAtlas.at(TAP_EUPHORIA_4);
						trailMiddle = m_objectAtlas.at(TRAIL_EUPHORIA_MIDDLE);
						trailEnd = m_objectAtlas.at(TRAIL_EUPHORIA_END);
					}

					float startTime = std::max(note.getMilli(), time);
					float endTime = std::min(note.getMilli() + note.getLength(), time + m_noteVisibleTime);

					if (endTime > time && note.getLength() > 15 / m_genBPM) {
						double startAngle = getAngleFromDT(startTime - time);
						double endAngle = getAngleFromDT(endTime - time);

						float trailWidth = 0.15;

						glm::vec2 beforeOuter = getCirclePoint((double)m_radius + trailWidth, startAngle);
						glm::vec2 BeforeInner = getCirclePoint((double)m_radius - trailWidth, startAngle);

						//trail middle part like highway
						for (double angle = startAngle; angle < endAngle; angle += m_deltaAngle) {
							glm::vec2 outer = getCirclePoint((double)m_radius + trailWidth, angle);
							glm::vec2 inner = getCirclePoint((double)m_radius - trailWidth, angle);

							pushVertexTexture(noteVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, trailMiddle.x, 1.0f - (trailMiddle.y + trailMiddle.w));
							pushVertexTexture(noteVector, -BeforeInner.x + center.x, plane, -BeforeInner.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - (trailMiddle.y + trailMiddle.w));
							pushVertexTexture(noteVector, -inner.x + center.x, plane, -inner.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - (trailMiddle.y));
							pushVertexTexture(noteVector, -outer.x + center.x, plane, -outer.y - center.y, trailMiddle.x, 1.0f - (trailMiddle.y));
							pushFourIndices(noteIndices, noteVertexCount);

							beforeOuter = outer;
							BeforeInner = inner;
						}

						//add trail end
						float trailEndAngle = asin(trailWidth * 2 / m_radius);

						glm::vec2 outer = getCirclePoint((double)m_radius + trailWidth, (double)endAngle + trailEndAngle);
						glm::vec2 inner = getCirclePoint((double)m_radius - trailWidth, (double)endAngle + trailEndAngle);

						pushVertexTexture(noteVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, trailEnd.x, 1.0f - (trailEnd.y + trailEnd.w));
						pushVertexTexture(noteVector, -BeforeInner.x + center.x, plane, -BeforeInner.y - center.y, trailEnd.x + trailEnd.z, 1.0f - (trailEnd.y + trailEnd.w));
						pushVertexTexture(noteVector, -inner.x + center.x, plane, -inner.y - center.y, trailEnd.x + trailEnd.z, 1.0f - (trailEnd.y));
						pushVertexTexture(noteVector, -outer.x + center.x, plane, -outer.y - center.y, trailEnd.x, 1.0f - (trailEnd.y));
						pushFourIndices(noteIndices, noteVertexCount);
					}

					Quad layer1 = createCircleQuad(noteAngle, m_radius, 0.25, dAngle / 2, plane + 0 * height);
					Quad layer2 = createCircleQuad(noteAngle, m_radius, 0.25, dAngle / 2, plane + 1 * height);
					Quad layer3 = createCircleQuad(noteAngle, m_radius, 0.25, dAngle / 2, plane + 2 * height);
					Quad layer4 = createCircleQuad(noteAngle, m_radius, 0.25, dAngle / 2, plane + 3 * height);

					layer1.v1.tex = glm::vec2(oneInfo.x, 1.0f - oneInfo.y);
					layer1.v2.tex = glm::vec2(oneInfo.x, 1.0f - oneInfo.y - oneInfo.w);
					layer1.v3.tex = glm::vec2(oneInfo.x + oneInfo.z, 1.0f - oneInfo.y - oneInfo.w);
					layer1.v4.tex = glm::vec2(oneInfo.x + oneInfo.z, 1.0f - oneInfo.y);

					layer2.v1.tex = glm::vec2(twoInfo.x, 1.0f - twoInfo.y);
					layer2.v2.tex = glm::vec2(twoInfo.x, 1.0f - twoInfo.y - twoInfo.w);
					layer2.v3.tex = glm::vec2(twoInfo.x + twoInfo.z, 1.0f - twoInfo.y - twoInfo.w);
					layer2.v4.tex = glm::vec2(twoInfo.x + twoInfo.z, 1.0f - twoInfo.y);

					layer3.v1.tex = glm::vec2(threeInfo.x, 1.0f - threeInfo.y);
					layer3.v2.tex = glm::vec2(threeInfo.x, 1.0f - threeInfo.y - threeInfo.w);
					layer3.v3.tex = glm::vec2(threeInfo.x + threeInfo.z, 1.0f - threeInfo.y - threeInfo.w);
					layer3.v4.tex = glm::vec2(threeInfo.x + threeInfo.z, 1.0f - threeInfo.y);

					layer4.v1.tex = glm::vec2(fourInfo.x, 1.0f - fourInfo.y);
					layer4.v2.tex = glm::vec2(fourInfo.x, 1.0f - fourInfo.y - fourInfo.w);
					layer4.v3.tex = glm::vec2(fourInfo.x + fourInfo.z, 1.0f - fourInfo.y - fourInfo.w);
					layer4.v4.tex = glm::vec2(fourInfo.x + fourInfo.z, 1.0f - fourInfo.y);

					pushQuad(noteVector, noteIndices, noteVertexCount, layer1);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer2);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer3);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer4);
				} else if (type == TAP_G && !note.getDead() && (!note.getTouched() || note.getLength() > 15 / m_genBPM)) {
					double baseRadius = m_radius + 0.5;
					double dAngle = asin(0.5 / baseRadius);

					if (note.getLanMod() == 0) {
						baseRadius = m_radius + 1.0;
						dAngle = asin(0.5 / baseRadius);
					}

					glm::vec4 oneInfo = m_objectAtlas.at(TAP_GREEN_1);
					glm::vec4 twoInfo = m_objectAtlas.at(TAP_GREEN_2);
					glm::vec4 threeInfo = m_objectAtlas.at(TAP_GREEN_3);
					glm::vec4 fourInfo = m_objectAtlas.at(TAP_GREEN_4);
					glm::vec4 trailMiddle = m_objectAtlas.at(TRAIL_GREEN_MIDDLE);
					glm::vec4 trailEnd = m_objectAtlas.at(TRAIL_GREEN_END);

					if (m_renderEuActive) {
						oneInfo = m_objectAtlas.at(TAP_EUPHORIA_1);
						twoInfo = m_objectAtlas.at(TAP_EUPHORIA_2);
						threeInfo = m_objectAtlas.at(TAP_EUPHORIA_3);
						fourInfo = m_objectAtlas.at(TAP_EUPHORIA_4);
						trailMiddle = m_objectAtlas.at(TRAIL_EUPHORIA_MIDDLE);
						trailEnd = m_objectAtlas.at(TRAIL_EUPHORIA_END);
					}

					float startTime = std::max(note.getMilli(), time);
					float endTime = std::min(note.getMilli() + note.getLength(), time + m_noteVisibleTime);

					if (endTime > time && note.getLength() > 15 / m_genBPM) {
						double startAngle = getAngleFromDT(startTime - time);
						double endAngle = getAngleFromDT(endTime - time);

						float trailWidth = 0.15;

						glm::vec2 beforeOuter;
						glm::vec2 beforeInner;

						if (getCrossAtTime(time, cross) == 0) {
							beforeOuter = getCirclePoint((double)m_radius + 1.0f + trailWidth, startAngle);
							beforeInner = getCirclePoint((double)m_radius + 1.0f - trailWidth, startAngle);
						} else {
							beforeOuter = getCirclePoint((double)m_radius + 0.5f + trailWidth, startAngle);
							beforeInner = getCirclePoint((double)m_radius + 0.5f - trailWidth, startAngle);
						}

						//trail middle part like highway

						for (double angle = startAngle; angle < endAngle; angle += m_deltaAngle) {
							glm::vec2 outer;
							glm::vec2 inner;
							if (getCrossAtTime(time + getDTFromAngle(angle), cross) == 0) {
								outer = getCirclePoint((double)m_radius + 1.0f + trailWidth, angle);
								inner = getCirclePoint((double)m_radius + 1.0f - trailWidth, angle);
							} else {
								outer = getCirclePoint((double)m_radius + 0.5f + trailWidth, angle);
								inner = getCirclePoint((double)m_radius + 0.5f - trailWidth, angle);
							}
							pushVertexTexture(noteVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, trailMiddle.x, 1.0f - (trailMiddle.y + trailMiddle.w));
							pushVertexTexture(noteVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - (trailMiddle.y + trailMiddle.w));
							pushVertexTexture(noteVector, -inner.x + center.x, plane, -inner.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - (trailMiddle.y));
							pushVertexTexture(noteVector, -outer.x + center.x, plane, -outer.y - center.y, trailMiddle.x, 1.0f - (trailMiddle.y));
							pushFourIndices(noteIndices, noteVertexCount);

							beforeOuter = outer;
							beforeInner = inner;
						}

						std::vector<Note> cfs = getCrossInsideNote(note, cross);

						for (size_t i = 1; i < cfs.size(); ++i) {
							int prevType = cfs.at(i - 1).getType();
							int nowType = cfs.at(i).getType();

							double nowMilli = cfs.at(i).getMilli();

							if (nowMilli >= time && nowMilli < time + m_noteVisibleTime) {
								double farAngle = getAngleFromDT(nowMilli - time) + asin(trailWidth / (m_radius + 0.75f));
								double nearAngle = getAngleFromDT(nowMilli - time) - asin(trailWidth / (m_radius + 0.75f));

								glm::vec2 mainTopLeft = getCirclePoint(m_radius + 1.0f - trailWidth, farAngle);
								glm::vec2 mainTopRight = getCirclePoint(m_radius + 0.5f + trailWidth, farAngle);
								glm::vec2 mainBottomLeft = getCirclePoint(m_radius + 1.0f - trailWidth, nearAngle);
								glm::vec2 mainBottomRight = getCirclePoint(m_radius + 0.5f + trailWidth, nearAngle);

								glm::vec2 jointTopLeft = getCirclePoint(m_radius + 1.0f + trailWidth, farAngle);
								glm::vec2 jointTopRight = getCirclePoint(m_radius + 0.5f - trailWidth, farAngle);
								glm::vec2 jointBottomLeft = getCirclePoint(m_radius + 1.0f + trailWidth, nearAngle);
								glm::vec2 jointBottomRight = getCirclePoint(m_radius + 0.5f - trailWidth, nearAngle);

								if (prevType == CROSS_C && nowType == CROSS_G) {
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushFourIndices(noteIndices, noteVertexCount);

									// counter-clockwise rotation
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointTopRight.x + center.x, plane, -jointTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -jointBottomRight.x + center.x, plane, -jointBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -jointTopRight.x + center.x, plane, -jointTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);

									//clockwise rotation
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointBottomLeft.x + center.x, plane, -jointBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);
									pushVertexTexture(noteVector, -jointBottomLeft.x + center.x, plane, -jointBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointTopLeft.x + center.x, plane, -jointTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushTriangleIndices(noteIndices, noteVertexCount);
								} else if (prevType == CROSS_G && nowType == CROSS_C) {
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushFourIndices(noteIndices, noteVertexCount);

									// counter-clockwise rotation
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointBottomRight.x + center.x, plane, -jointBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -jointTopRight.x + center.x, plane, -jointTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -jointBottomRight.x + center.x, plane, -jointBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);

									//clockwise rotation
									pushVertexTexture(noteVector, -jointBottomLeft.x + center.x, plane, -jointBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointTopLeft.x + center.x, plane, -jointTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);
									pushVertexTexture(noteVector, -jointTopLeft.x + center.x, plane, -jointTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushTriangleIndices(noteIndices, noteVertexCount);
								}
							}
						}

						//add trail end
						float trailEndAngle = asin(trailWidth * 2 / m_radius);
						glm::vec2 outer;
						glm::vec2 inner;
						if (getCrossAtTime(endTime, cross) == 0) {
							outer = getCirclePoint((double)m_radius + 1.0f + trailWidth, (double)endAngle + trailEndAngle);
							inner = getCirclePoint((double)m_radius + 1.0f - trailWidth, (double)endAngle + trailEndAngle);
						} else {
							outer = getCirclePoint((double)m_radius + 0.5f + trailWidth, (double)endAngle + trailEndAngle);
							inner = getCirclePoint((double)m_radius + 0.5f - trailWidth, (double)endAngle + trailEndAngle);
						}
						pushVertexTexture(noteVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, trailEnd.x, 1.0f - (trailEnd.y + trailEnd.w));
						pushVertexTexture(noteVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, trailEnd.x + trailEnd.z, 1.0f - (trailEnd.y + trailEnd.w));
						pushVertexTexture(noteVector, -inner.x + center.x, plane, -inner.y - center.y, trailEnd.x + trailEnd.z, 1.0f - (trailEnd.y));
						pushVertexTexture(noteVector, -outer.x + center.x, plane, -outer.y - center.y, trailEnd.x, 1.0f - (trailEnd.y));
						pushFourIndices(noteIndices, noteVertexCount);
					}

					Quad layer1 = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle / 2, plane + 0 * height);
					Quad layer2 = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle / 2, plane + 1 * height);
					Quad layer3 = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle / 2, plane + 2 * height);
					Quad layer4 = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle / 2, plane + 3 * height);

					layer1.v1.tex = glm::vec2(oneInfo.x, 1.0f - oneInfo.y);
					layer1.v2.tex = glm::vec2(oneInfo.x, 1.0f - oneInfo.y - oneInfo.w);
					layer1.v3.tex = glm::vec2(oneInfo.x + oneInfo.z, 1.0f - oneInfo.y - oneInfo.w);
					layer1.v4.tex = glm::vec2(oneInfo.x + oneInfo.z, 1.0f - oneInfo.y);

					layer2.v1.tex = glm::vec2(twoInfo.x, 1.0f - twoInfo.y);
					layer2.v2.tex = glm::vec2(twoInfo.x, 1.0f - twoInfo.y - twoInfo.w);
					layer2.v3.tex = glm::vec2(twoInfo.x + twoInfo.z, 1.0f - twoInfo.y - twoInfo.w);
					layer2.v4.tex = glm::vec2(twoInfo.x + twoInfo.z, 1.0f - twoInfo.y);

					layer3.v1.tex = glm::vec2(threeInfo.x, 1.0f - threeInfo.y);
					layer3.v2.tex = glm::vec2(threeInfo.x, 1.0f - threeInfo.y - threeInfo.w);
					layer3.v3.tex = glm::vec2(threeInfo.x + threeInfo.z, 1.0f - threeInfo.y - threeInfo.w);
					layer3.v4.tex = glm::vec2(threeInfo.x + threeInfo.z, 1.0f - threeInfo.y);

					layer4.v1.tex = glm::vec2(fourInfo.x, 1.0f - fourInfo.y);
					layer4.v2.tex = glm::vec2(fourInfo.x, 1.0f - fourInfo.y - fourInfo.w);
					layer4.v3.tex = glm::vec2(fourInfo.x + fourInfo.z, 1.0f - fourInfo.y - fourInfo.w);
					layer4.v4.tex = glm::vec2(fourInfo.x + fourInfo.z, 1.0f - fourInfo.y);

					pushQuad(noteVector, noteIndices, noteVertexCount, layer1);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer2);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer3);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer4);
				} else if (type == TAP_B && !note.getDead() && (!note.getTouched() || note.getLength() > 15 / m_genBPM)) {
					double baseRadius = m_radius - 0.5;
					double dAngle = asin(0.5 / baseRadius);

					if (note.getLanMod() == 2) {
						baseRadius = m_radius - 1.0;
						dAngle = asin(0.5 / baseRadius);
					}

					glm::vec4 oneInfo = m_objectAtlas.at(TAP_BLUE_1);
					glm::vec4 twoInfo = m_objectAtlas.at(TAP_BLUE_2);
					glm::vec4 threeInfo = m_objectAtlas.at(TAP_BLUE_3);
					glm::vec4 fourInfo = m_objectAtlas.at(TAP_BLUE_4);
					glm::vec4 trailMiddle = m_objectAtlas.at(TRAIL_BLUE_MIDDLE);
					glm::vec4 trailEnd = m_objectAtlas.at(TRAIL_BLUE_END);

					if (m_renderEuActive) {
						oneInfo = m_objectAtlas.at(TAP_EUPHORIA_1);
						twoInfo = m_objectAtlas.at(TAP_EUPHORIA_2);
						threeInfo = m_objectAtlas.at(TAP_EUPHORIA_3);
						fourInfo = m_objectAtlas.at(TAP_EUPHORIA_4);
						trailMiddle = m_objectAtlas.at(TRAIL_EUPHORIA_MIDDLE);
						trailEnd = m_objectAtlas.at(TRAIL_EUPHORIA_END);
					}

					float startTime = std::max(note.getMilli(), time);
					float endTime = std::min(note.getMilli() + note.getLength(), time + m_noteVisibleTime);

					if (endTime > time && note.getLength() > 15 / m_genBPM) {
						double startAngle = getAngleFromDT(startTime - time);
						double endAngle = getAngleFromDT(endTime - time);

						float trailWidth = 0.15;

						glm::vec2 beforeOuter;
						glm::vec2 beforeInner;

						if (getCrossAtTime(time, cross) == 2) {
							beforeOuter = getCirclePoint((double)m_radius - 1.0f + trailWidth, startAngle);
							beforeInner = getCirclePoint((double)m_radius - 1.0f - trailWidth, startAngle);
						} else {
							beforeOuter = getCirclePoint((double)m_radius - 0.5f + trailWidth, startAngle);
							beforeInner = getCirclePoint((double)m_radius - 0.5f - trailWidth, startAngle);
						}

						//trail middle part like highway

						for (double angle = startAngle; angle < endAngle; angle += m_deltaAngle) {
							glm::vec2 outer;
							glm::vec2 inner;
							if (getCrossAtTime(time + getDTFromAngle(angle), cross) == 2) {
								outer = getCirclePoint((double)m_radius - 1.0f + trailWidth, angle);
								inner = getCirclePoint((double)m_radius - 1.0f - trailWidth, angle);
							} else {
								outer = getCirclePoint((double)m_radius - 0.5f + trailWidth, angle);
								inner = getCirclePoint((double)m_radius - 0.5f - trailWidth, angle);
							}
							pushVertexTexture(noteVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, trailMiddle.x, 1.0f - (trailMiddle.y + trailMiddle.w));
							pushVertexTexture(noteVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - (trailMiddle.y + trailMiddle.w));
							pushVertexTexture(noteVector, -inner.x + center.x, plane, -inner.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - (trailMiddle.y));
							pushVertexTexture(noteVector, -outer.x + center.x, plane, -outer.y - center.y, trailMiddle.x, 1.0f - (trailMiddle.y));
							pushFourIndices(noteIndices, noteVertexCount);

							beforeOuter = outer;
							beforeInner = inner;
						}

						std::vector<Note> cfs = getCrossInsideNote(note, cross);

						for (size_t i = 1; i < cfs.size(); ++i) {
							int prevType = cfs.at(i - 1).getType();
							int nowType = cfs.at(i).getType();

							double nowMilli = cfs.at(i).getMilli();

							if (nowMilli >= time && nowMilli < time + m_noteVisibleTime) {
								double farAngle = getAngleFromDT(nowMilli - time) + asin(trailWidth / (m_radius - 0.75f));
								double nearAngle = getAngleFromDT(nowMilli - time) - asin(trailWidth / (m_radius - 0.75f));

								glm::vec2 mainTopLeft = getCirclePoint(m_radius - 0.5f - trailWidth, farAngle);
								glm::vec2 mainTopRight = getCirclePoint(m_radius - 1.0f + trailWidth, farAngle);
								glm::vec2 mainBottomLeft = getCirclePoint(m_radius - 0.5f - trailWidth, nearAngle);
								glm::vec2 mainBottomRight = getCirclePoint(m_radius - 1.0f + trailWidth, nearAngle);

								glm::vec2 jointTopLeft = getCirclePoint(m_radius - 0.5f + trailWidth, farAngle);
								glm::vec2 jointTopRight = getCirclePoint(m_radius - 1.0f - trailWidth, farAngle);
								glm::vec2 jointBottomLeft = getCirclePoint(m_radius - 0.5f + trailWidth, nearAngle);
								glm::vec2 jointBottomRight = getCirclePoint(m_radius - 1.0f - trailWidth, nearAngle);

								if (prevType == CROSS_C && nowType == CROSS_B) {
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w); //
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushFourIndices(noteIndices, noteVertexCount);

									// counter-clockwise rotation
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointBottomRight.x + center.x, plane, -jointBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -jointTopRight.x + center.x, plane, -jointTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -jointBottomRight.x + center.x, plane, -jointBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);

									//clockwise rotation
									pushVertexTexture(noteVector, -jointBottomLeft.x + center.x, plane, -jointBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointTopLeft.x + center.x, plane, -jointTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);
									pushVertexTexture(noteVector, -jointTopLeft.x + center.x, plane, -jointTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushTriangleIndices(noteIndices, noteVertexCount);
								} else if (prevType == CROSS_B && nowType == CROSS_C) {
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushFourIndices(noteIndices, noteVertexCount);

									// counter-clockwise rotation
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointTopRight.x + center.x, plane, -jointTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -jointBottomRight.x + center.x, plane, -jointBottomRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);
									pushVertexTexture(noteVector, -mainBottomRight.x + center.x, plane, -mainBottomRight.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainTopRight.x + center.x, plane, -mainTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -jointTopRight.x + center.x, plane, -jointTopRight.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);

									//clockwise rotation
									pushVertexTexture(noteVector, -mainBottomLeft.x + center.x, plane, -mainBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointBottomLeft.x + center.x, plane, -jointBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y - trailMiddle.w);
									pushTriangleIndices(noteIndices, noteVertexCount);
									pushVertexTexture(noteVector, -jointBottomLeft.x + center.x, plane, -jointBottomLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y - trailMiddle.w);
									pushVertexTexture(noteVector, -jointTopLeft.x + center.x, plane, -jointTopLeft.y - center.y, trailMiddle.x, 1.0f - trailMiddle.y);
									pushVertexTexture(noteVector, -mainTopLeft.x + center.x, plane, -mainTopLeft.y - center.y, trailMiddle.x + trailMiddle.z, 1.0f - trailMiddle.y);
									pushTriangleIndices(noteIndices, noteVertexCount);
								}
							}
						}

						//add trail end
						float trailEndAngle = asin(trailWidth * 2 / m_radius);
						glm::vec2 outer;
						glm::vec2 inner;
						if (getCrossAtTime(endTime, cross) == 2) {
							outer = getCirclePoint((double)m_radius - 1.0f + trailWidth, (double)endAngle + trailEndAngle);
							inner = getCirclePoint((double)m_radius - 1.0f - trailWidth, (double)endAngle + trailEndAngle);
						} else {
							outer = getCirclePoint((double)m_radius - 0.5f + trailWidth, (double)endAngle + trailEndAngle);
							inner = getCirclePoint((double)m_radius - 0.5f - trailWidth, (double)endAngle + trailEndAngle);
						}
						pushVertexTexture(noteVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, trailEnd.x, 1.0f - (trailEnd.y + trailEnd.w));
						pushVertexTexture(noteVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, trailEnd.x + trailEnd.z, 1.0f - (trailEnd.y + trailEnd.w));
						pushVertexTexture(noteVector, -inner.x + center.x, plane, -inner.y - center.y, trailEnd.x + trailEnd.z, 1.0f - (trailEnd.y));
						pushVertexTexture(noteVector, -outer.x + center.x, plane, -outer.y - center.y, trailEnd.x, 1.0f - (trailEnd.y));
						pushFourIndices(noteIndices, noteVertexCount);
					}

					Quad layer1 = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle / 2, plane + 0 * height);
					Quad layer2 = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle / 2, plane + 1 * height);
					Quad layer3 = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle / 2, plane + 2 * height);
					Quad layer4 = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle / 2, plane + 3 * height);

					layer1.v1.tex = glm::vec2(oneInfo.x, 1.0f - oneInfo.y);
					layer1.v2.tex = glm::vec2(oneInfo.x, 1.0f - oneInfo.y - oneInfo.w);
					layer1.v3.tex = glm::vec2(oneInfo.x + oneInfo.z, 1.0f - oneInfo.y - oneInfo.w);
					layer1.v4.tex = glm::vec2(oneInfo.x + oneInfo.z, 1.0f - oneInfo.y);

					layer2.v1.tex = glm::vec2(twoInfo.x, 1.0f - twoInfo.y);
					layer2.v2.tex = glm::vec2(twoInfo.x, 1.0f - twoInfo.y - twoInfo.w);
					layer2.v3.tex = glm::vec2(twoInfo.x + twoInfo.z, 1.0f - twoInfo.y - twoInfo.w);
					layer2.v4.tex = glm::vec2(twoInfo.x + twoInfo.z, 1.0f - twoInfo.y);

					layer3.v1.tex = glm::vec2(threeInfo.x, 1.0f - threeInfo.y);
					layer3.v2.tex = glm::vec2(threeInfo.x, 1.0f - threeInfo.y - threeInfo.w);
					layer3.v3.tex = glm::vec2(threeInfo.x + threeInfo.z, 1.0f - threeInfo.y - threeInfo.w);
					layer3.v4.tex = glm::vec2(threeInfo.x + threeInfo.z, 1.0f - threeInfo.y);

					layer4.v1.tex = glm::vec2(fourInfo.x, 1.0f - fourInfo.y);
					layer4.v2.tex = glm::vec2(fourInfo.x, 1.0f - fourInfo.y - fourInfo.w);
					layer4.v3.tex = glm::vec2(fourInfo.x + fourInfo.z, 1.0f - fourInfo.y - fourInfo.w);
					layer4.v4.tex = glm::vec2(fourInfo.x + fourInfo.z, 1.0f - fourInfo.y);

					pushQuad(noteVector, noteIndices, noteVertexCount, layer1);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer2);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer3);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer4);
				} else if (type == SCR_G_UP && !note.getTouched()) {
					double baseRadius = m_radius + 0.5;
					double dAngle = asin(0.25 / baseRadius);

					if (note.getLanMod() == 0) {
						baseRadius = m_radius + 1.0;
						dAngle = asin(0.25 / baseRadius);
					}

					glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_UP);

					Quad layer = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle, plane);
					layer.v1.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y);
					layer.v2.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
					layer.v3.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
					layer.v4.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer);

				} else if (type == SCR_G_DOWN && !note.getTouched()) {
					double baseRadius = m_radius + 0.5;
					double dAngle = asin(0.25 / baseRadius);

					if (note.getLanMod() == 0) {
						baseRadius = m_radius + 1.0;
						dAngle = asin(0.25 / baseRadius);
					}

					glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_DOWN);

					Quad layer = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle, plane);
					layer.v1.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y);
					layer.v2.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
					layer.v3.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
					layer.v4.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer);

				} else if (type == SCR_G_ANY) {
					double baseRadius = m_radius + 0.5;
					double dAngle = asin(0.25 / baseRadius);

					double timeBetween = getDTFromAngle(dAngle);

					for (double j = milli; j < milli + note.getLength(); j += timeBetween) {
						if (j >= time && j <= time + m_noteVisibleTime) {
							double spriteAngle = getAngleFromDT(j - time);

							if (getCrossAtTime(j, cross) == 0) {
								baseRadius = m_radius + 1.0;
								dAngle = asin(0.25 / baseRadius);
							}

							glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_ANYDIR);

							Quad layer = createCircleQuad(spriteAngle, baseRadius, 0.25, dAngle, plane);
							layer.v1.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y);
							layer.v2.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
							layer.v3.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
							layer.v4.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
							pushQuad(noteVector, noteIndices, noteVertexCount, layer);
						}
					}
				} else if (type == SCR_B_UP && !note.getTouched()) {
					double baseRadius = m_radius - 0.5;
					double dAngle = asin(0.25 / baseRadius);

					if (note.getLanMod() == 2) {
						baseRadius = m_radius - 1.0;
						dAngle = asin(0.25 / baseRadius);
					}

					glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_UP);

					Quad layer = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle, plane);
					layer.v1.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y);
					layer.v2.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
					layer.v3.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
					layer.v4.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer);

				} else if (type == SCR_B_DOWN && !note.getTouched()) {
					double baseRadius = m_radius - 0.5;
					double dAngle = asin(0.25 / baseRadius);

					if (note.getLanMod() == 2) {
						baseRadius = m_radius - 1.0;
						dAngle = asin(0.25 / baseRadius);
					}

					glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_DOWN);

					Quad layer = createCircleQuad(noteAngle, baseRadius, 0.25, dAngle, plane);
					layer.v1.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y);
					layer.v2.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
					layer.v3.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
					layer.v4.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
					pushQuad(noteVector, noteIndices, noteVertexCount, layer);

				} else if (type == SCR_B_ANY) {
					double baseRadius = m_radius - 0.5;
					double dAngle = asin(0.25 / baseRadius);

					double timeBetween = getDTFromAngle(dAngle);

					for (double j = milli; j < milli + note.getLength(); j += timeBetween) {
						if (j >= time && j <= time + m_noteVisibleTime) {
							double spriteAngle = getAngleFromDT(j - time);

							if (getCrossAtTime(j, cross) == 2) {
								baseRadius = m_radius - 1.0;
								dAngle = asin(0.25 / baseRadius);
							}

							glm::vec4 OneInfo = m_objectAtlas.at(SCRATCH_ANYDIR);

							Quad layer = createCircleQuad(spriteAngle, baseRadius, 0.25, dAngle, plane);
							layer.v1.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y);
							layer.v2.tex = glm::vec2(OneInfo.x, 1.0f - OneInfo.y - OneInfo.w);
							layer.v3.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y - OneInfo.w);
							layer.v4.tex = glm::vec2(OneInfo.x + OneInfo.z, 1.0f - OneInfo.y);
							pushQuad(noteVector, noteIndices, noteVertexCount, layer);
						}
					}
				}
			} else if (milli >= time + m_noteVisibleTime) {
				//if the note is outside the visible area, update lane position
				note.setLanMod(getCrossAtTime(milli, cross));
			}
		}

		usePersProj();
		renderTexture(noteVector, noteIndices, m_objTexture);
	}
}

void GameRender::lanes(double time, std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& cross) {
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

	if (m_renderEuActive) {
		greenActiveColor = euphoriaColor;
		redColor = euphoriaColor;
		blueActiveColor = euphoriaColor;
	}

	float size = 0.05;

	float greenRadius = m_radius + 0.5f;
	float blueRadius = m_radius - 0.5f;

	float startAngle = asin(0.25f / m_radius);

	bool isPreviousValid = false;

	glm::vec2 center = {m_radius, 0.0};
	glm::vec2 redBeforeOuter = getCirclePoint((double)m_radius + size, startAngle);
	glm::vec2 redBeforeInner = getCirclePoint((double)m_radius - size, startAngle);
	glm::vec2 greenBeforeOuter = getCirclePoint((double)greenRadius + size, startAngle);
	glm::vec2 greenBeforeInner = getCirclePoint((double)greenRadius - size, startAngle);
	glm::vec2 blueBeforeOuter = getCirclePoint((double)blueRadius + size, startAngle);
	glm::vec2 blueBeforeInner = getCirclePoint((double)blueRadius - size, startAngle);

	for (float angle = startAngle; angle < m_maxAngle; angle += m_deltaAngle) {
		auto angleTime = (float)(time + getDTFromAngle(angle));
		int position = getCrossAtTime(angleTime, cross, &ev);

		glm::vec2 redOuter = getCirclePoint((double)m_radius + size, (double)angle);
		glm::vec2 redInner = getCirclePoint((double)m_radius - size, (double)angle);

		Vertex r1(glm::vec3(-redBeforeOuter.x + center.x, plane, -redBeforeOuter.y - center.y), redColor);
		Vertex r2(glm::vec3(-redBeforeInner.x + center.x, plane, -redBeforeInner.y - center.y), redColor);
		Vertex r3(glm::vec3(-redInner.x + center.x, plane, -redInner.y - center.y), redColor);
		Vertex r4(glm::vec3(-redOuter.x + center.x, plane, -redOuter.y - center.y), redColor);

		pushFourVertices(lanesVector, r1, r2, r3, r4);
		pushFourIndices(lanesIndices, lanesVertexCount);

		redBeforeOuter = redOuter;
		redBeforeInner = redInner;

		//green

		glm::vec2 greenOuter;
		glm::vec2 greenInner;

		if (position == 0) {
			greenOuter = getCirclePoint((double)m_radius + 1.0 + size, (double)angle);
			greenInner = getCirclePoint((double)m_radius + 1.0f - size, (double)angle);

			//green active and on the left
			if (isPreviousValid) {
				glm::vec4 tCoords = m_objectAtlas.at(FS_CROSS_GREEN_BASE_BOTTOM);

				Vertex g1(glm::vec3(-greenBeforeOuter.x + center.x, plane, -greenBeforeOuter.y - center.y), greenActiveColor);
				Vertex g2(glm::vec3(-greenBeforeInner.x + center.x, plane, -greenBeforeInner.y - center.y), greenActiveColor);
				Vertex g3(glm::vec3(-greenInner.x + center.x, plane, -greenInner.y - center.y), greenActiveColor);
				Vertex g4(glm::vec3(-greenOuter.x + center.x, plane, -greenOuter.y - center.y), greenActiveColor);
				pushFourVertices(lanesVector, g1, g2, g3, g4);
				pushFourIndices(lanesIndices, lanesVertexCount);
			}
		} else if (position == 1) {
			greenOuter = getCirclePoint((double)m_radius + 0.5 + size, (double)angle);
			greenInner = getCirclePoint((double)m_radius + 0.5 - size, (double)angle);

			//green active and on the center
			if (isPreviousValid) {
				Vertex g1(glm::vec3(-greenBeforeOuter.x + center.x, plane, -greenBeforeOuter.y - center.y), greenActiveColor);
				Vertex g2(glm::vec3(-greenBeforeInner.x + center.x, plane, -greenBeforeInner.y - center.y), greenActiveColor);
				Vertex g3(glm::vec3(-greenInner.x + center.x, plane, -greenInner.y - center.y), greenActiveColor);
				Vertex g4(glm::vec3(-greenOuter.x + center.x, plane, -greenOuter.y - center.y), greenActiveColor);
				pushFourVertices(lanesVector, g1, g2, g3, g4);
				pushFourIndices(lanesIndices, lanesVertexCount);
			}
		} else if (position == 2) {
			greenOuter = getCirclePoint((double)m_radius + 0.5 + size, (double)angle);
			greenInner = getCirclePoint((double)m_radius + 0.5 - size, (double)angle);

			//green inactive and on the center
			if (isPreviousValid) {
				Vertex g1(glm::vec3(-greenBeforeOuter.x + center.x, plane, -greenBeforeOuter.y - center.y), greenInactiveColor);
				Vertex g2(glm::vec3(-greenBeforeInner.x + center.x, plane, -greenBeforeInner.y - center.y), greenInactiveColor);
				Vertex g3(glm::vec3(-greenInner.x + center.x, plane, -greenInner.y - center.y), greenInactiveColor);
				Vertex g4(glm::vec3(-greenOuter.x + center.x, plane, -greenOuter.y - center.y), greenInactiveColor);
				pushFourVertices(lanesVector, g1, g2, g3, g4);
				pushFourIndices(lanesIndices, lanesVertexCount);
			}
		}

		greenBeforeOuter = greenOuter;
		greenBeforeInner = greenInner;

		//blue

		glm::vec2 blueOuter;
		glm::vec2 blueInner;
		if (position == 0) {
			blueOuter = getCirclePoint((double)m_radius - 0.5 + size, (double)angle);
			blueInner = getCirclePoint((double)m_radius - 0.5 - size, (double)angle);

			//blue inactive and on the center

			if (isPreviousValid) {
				Vertex b1(glm::vec3(-blueBeforeOuter.x + center.x, 0.0, -blueBeforeOuter.y - center.y), blueInactiveColor);
				Vertex b2(glm::vec3(-blueBeforeInner.x + center.x, 0.0, -blueBeforeInner.y - center.y), blueInactiveColor);
				Vertex b3(glm::vec3(-blueInner.x + center.x, 0.0, -blueInner.y - center.y), blueInactiveColor);
				Vertex b4(glm::vec3(-blueOuter.x + center.x, 0.0, -blueOuter.y - center.y), blueInactiveColor);
				pushFourVertices(lanesVector, b1, b2, b3, b4);
				pushFourIndices(lanesIndices, lanesVertexCount);
			}
		} else if (position == 1) {
			blueOuter = getCirclePoint((double)m_radius - 0.5 + size, (double)angle);
			blueInner = getCirclePoint((double)m_radius - 0.5 - size, (double)angle);

			//blue active and on the center
			if (isPreviousValid) {
				Vertex b1(glm::vec3(-blueBeforeOuter.x + center.x, 0.0, -blueBeforeOuter.y - center.y), blueActiveColor);
				Vertex b2(glm::vec3(-blueBeforeInner.x + center.x, 0.0, -blueBeforeInner.y - center.y), blueActiveColor);
				Vertex b3(glm::vec3(-blueInner.x + center.x, 0.0, -blueInner.y - center.y), blueActiveColor);
				Vertex b4(glm::vec3(-blueOuter.x + center.x, 0.0, -blueOuter.y - center.y), blueActiveColor);
				pushFourVertices(lanesVector, b1, b2, b3, b4);
				pushFourIndices(lanesIndices, lanesVertexCount);
			}
		} else if (position == 2) {
			blueOuter = getCirclePoint((double)m_radius - 1.0 + size, (double)angle);
			blueInner = getCirclePoint((double)m_radius - 1.0f - size, (double)angle);

			//blue active and on the left
			if (isPreviousValid) {
				Vertex b1(glm::vec3(-blueBeforeOuter.x + center.x, 0.0, -blueBeforeOuter.y - center.y), blueActiveColor);
				Vertex b2(glm::vec3(-blueBeforeInner.x + center.x, 0.0, -blueBeforeInner.y - center.y), blueActiveColor);
				Vertex b3(glm::vec3(-blueInner.x + center.x, 0.0, -blueInner.y - center.y), blueActiveColor);
				Vertex b4(glm::vec3(-blueOuter.x + center.x, 0.0, -blueOuter.y - center.y), blueActiveColor);
				pushFourVertices(lanesVector, b1, b2, b3, b4);
				pushFourIndices(lanesIndices, lanesVertexCount);
			}
		}

		blueBeforeOuter = blueOuter;
		blueBeforeInner = blueInner;

		if (position == -1) {
			//make next one invalid
			isPreviousValid = false;
		} else {
			isPreviousValid = true;
		}
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

			bool drawGreen = false;
			bool drawBlue = false;

			if (type == CROSS_G && typeBefore != CROSS_G) {
				drawGreen = true;
				if (typeBefore == CROSS_B || typeBefore == CROSS_B_TICK) {
					drawBlue = true;
				}
			} else if (type == CROSS_B && typeBefore != CROSS_B) {
				drawBlue = true;
				if (typeBefore == CROSS_G || typeBefore == CROSS_G_TICK) {
					drawGreen = true;
				}
			} else if (type == CROSS_C) {
				if (typeBefore == CROSS_G || typeBefore == CROSS_G_TICK) {
					drawGreen = true;
				} else if (typeBefore == CROSS_B || typeBefore == CROSS_B_TICK) {
					drawBlue = true;
				}
			}
			if (drawGreen) {
				double r = m_radius + 0.75;
				double dAngle = asin(0.125 / r);

				Quad q = createCircleQuad(noteAngle, r, 0.25 + size, dAngle, plane);
				q.v1.col = greenActiveColor;
				q.v2.col = greenActiveColor;
				q.v3.col = greenActiveColor;
				q.v4.col = greenActiveColor;

				pushQuad(lanesVector, lanesIndices, lanesVertexCount, q);
			}
			if (drawBlue) {
				double r = m_radius - 0.75;
				double dAngle = asin(0.125 / r);

				Quad q = createCircleQuad(noteAngle, r, 0.25 + size, dAngle, plane);
				q.v1.col = blueActiveColor;
				q.v2.col = blueActiveColor;
				q.v3.col = blueActiveColor;
				q.v4.col = blueActiveColor;

				pushQuad(lanesVector, lanesIndices, lanesVertexCount, q);
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
	//only color
	std::vector<float> colorVector = {};
	std::vector<unsigned int> colorIndices = {};
	unsigned int colorVertexCount = 0;
	//only textures
	std::vector<float> texVector = {};
	std::vector<unsigned int> texIndices = {};
	unsigned int texVertexCount = 0;

	//loop for every event inside event vector
	if (!ev.empty()) {
		for (size_t i = ev.size() - 1; i >= 0 && i < ev.size(); i--) {
			Note& event = ev.at(i);
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

						pushVertexColor(colorVector, -outer.x + center.x, plane, -outer.y - center.y, m_greenScratchColor.r, m_greenScratchColor.g, m_greenScratchColor.b, m_greenScratchColor.a);
						pushVertexColor(colorVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, m_greenScratchColor.r, m_greenScratchColor.g, m_greenScratchColor.b, m_greenScratchColor.a);
						pushVertexColor(colorVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, m_greenScratchColor.r, m_greenScratchColor.g, m_greenScratchColor.b, m_greenScratchColor.a);
						pushVertexColor(colorVector, -inner.x + center.x, plane, -inner.y - center.y, m_greenScratchColor.r, m_greenScratchColor.g, m_greenScratchColor.b, m_greenScratchColor.a);

						pushFourIndices(colorIndices, colorVertexCount);

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

						pushVertexColor(colorVector, -outer.x + center.x, plane, -outer.y - center.y, m_blueScratchColor.r, m_blueScratchColor.g, m_blueScratchColor.b, m_blueScratchColor.a);
						pushVertexColor(colorVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, m_blueScratchColor.r, m_blueScratchColor.g, m_blueScratchColor.b, m_blueScratchColor.a);
						pushVertexColor(colorVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, m_blueScratchColor.r, m_blueScratchColor.g, m_blueScratchColor.b, m_blueScratchColor.a);
						pushVertexColor(colorVector, -inner.x + center.x, plane, -inner.y - center.y, m_blueScratchColor.r, m_blueScratchColor.g, m_blueScratchColor.b, m_blueScratchColor.a);

						pushFourIndices(colorIndices, colorVertexCount);

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
						glm::vec2 outer = getCirclePoint((double)m_radius + m_deltaRadius, cycleAngle);
						glm::vec2 inner = getCirclePoint((double)m_radius - m_deltaRadius, cycleAngle);

						pushVertexColor(colorVector, -outer.x + center.x, plane, -outer.y - center.y, m_euphoriaZoneColor.r, m_euphoriaZoneColor.g, m_euphoriaZoneColor.b, m_euphoriaZoneColor.a);
						pushVertexColor(colorVector, -beforeOuter.x + center.x, plane, -beforeOuter.y - center.y, m_euphoriaZoneColor.r, m_euphoriaZoneColor.g, m_euphoriaZoneColor.b, m_euphoriaZoneColor.a);
						pushVertexColor(colorVector, -beforeInner.x + center.x, plane, -beforeInner.y - center.y, m_euphoriaZoneColor.r, m_euphoriaZoneColor.g, m_euphoriaZoneColor.b, m_euphoriaZoneColor.a);
						pushVertexColor(colorVector, -inner.x + center.x, plane, -inner.y - center.y, m_euphoriaZoneColor.r, m_euphoriaZoneColor.g, m_euphoriaZoneColor.b, m_euphoriaZoneColor.a);

						pushFourIndices(colorIndices, colorVertexCount);

						beforeOuter = outer;
						beforeInner = inner;
					}
				} else if (type == FS_CROSS_BASE) {
					double start = std::max(milli, time - 0.1);
					double end = std::min(milli + length, time + m_noteVisibleTime);

					double startAngle = getAngleFromDT(start - time);
					double endAngle = getAngleFromDT(end - time);
					glm::vec2 center = {m_radius, 0.0};
					double deltaAngle = asin(0.25 / m_radius);

					double greenOuterRadius = (double)m_radius + 1.25;
					double greenInnerRadius = (double)m_radius + 0.25;
					double blueOuterRadius = (double)m_radius - 1.25;
					double blueInnerRadius = (double)m_radius - 0.25;

					glm::vec2 beforeOuterGreen = getCirclePoint(greenOuterRadius, startAngle);
					glm::vec2 beforeInnerGreen = getCirclePoint(greenInnerRadius, startAngle);

					glm::vec2 beforeOuterBlue = getCirclePoint(blueOuterRadius, startAngle);
					glm::vec2 beforeInnerBlue = getCirclePoint(blueInnerRadius, startAngle);

					glm::vec4 greenTopSprite = m_objectAtlas.at(FS_CROSS_GREEN_BASE_TOP);
					glm::vec4 greenBottomSprite = m_objectAtlas.at(FS_CROSS_GREEN_BASE_BOTTOM);

					glm::vec4 blueTopSprite = m_objectAtlas.at(FS_CROSS_BLUE_BASE_TOP);
					glm::vec4 blueBottomSprite = m_objectAtlas.at(FS_CROSS_BLUE_BASE_BOTTOM);

					if (start > time - 0.1) {
						//draw start zone
						Quad q1 = createCircleQuad(startAngle - deltaAngle / 2, m_radius + 0.75, 0.5, deltaAngle / 2);

						q1.v1.tex = glm::vec2(greenBottomSprite.x, 1.0 - greenBottomSprite.y);
						q1.v2.tex = glm::vec2(greenBottomSprite.x, 1.0 - (greenBottomSprite.y + greenBottomSprite.w));
						q1.v3.tex = glm::vec2(greenBottomSprite.x + greenBottomSprite.z, 1.0 - (greenBottomSprite.y + greenBottomSprite.w));
						q1.v4.tex = glm::vec2(greenBottomSprite.x + greenBottomSprite.z, 1.0 - greenBottomSprite.y);

						pushQuad(texVector, texIndices, texVertexCount, q1);

						Quad q2 = createCircleQuad(startAngle - deltaAngle / 2, m_radius - 0.75, 0.5, deltaAngle / 2);

						q2.v1.tex = glm::vec2(blueBottomSprite.x, 1.0 - blueBottomSprite.y);
						q2.v2.tex = glm::vec2(blueBottomSprite.x, 1.0 - (blueBottomSprite.y + blueBottomSprite.w));
						q2.v3.tex = glm::vec2(blueBottomSprite.x + blueBottomSprite.z, 1.0 - (blueBottomSprite.y + blueBottomSprite.w));
						q2.v4.tex = glm::vec2(blueBottomSprite.x + blueBottomSprite.z, 1.0 - blueBottomSprite.y);

						pushQuad(texVector, texIndices, texVertexCount, q2);

						glm::vec2 bottomOuterGreen = getCirclePoint(greenOuterRadius, startAngle - deltaAngle);
						glm::vec2 bottomInnerGreen = getCirclePoint(greenInnerRadius, startAngle - deltaAngle);
						glm::vec2 bottomOuterBlue = getCirclePoint(blueOuterRadius, startAngle - deltaAngle);
						glm::vec2 bottomInnerBlue = getCirclePoint(blueInnerRadius, startAngle - deltaAngle);
					}
					for (double cycleAngle = startAngle; cycleAngle < endAngle; cycleAngle += m_deltaAngle) {
						glm::vec2 greenOuter = getCirclePoint(greenOuterRadius, cycleAngle);
						glm::vec2 greenInner = getCirclePoint(greenInnerRadius, cycleAngle);

						glm::vec2 blueOuter = getCirclePoint(blueOuterRadius, cycleAngle);
						glm::vec2 blueInner = getCirclePoint(blueInnerRadius, cycleAngle);

						Vertex g1(glm::vec3(-greenOuter.x + center.x, plane, -greenOuter.y - center.y), glm::vec2(greenBottomSprite.x, 1.0 - greenBottomSprite.y));
						Vertex g2(glm::vec3(-beforeOuterGreen.x + center.x, plane, -beforeOuterGreen.y - center.y), glm::vec2(greenBottomSprite.x, 1.0 - greenBottomSprite.y));
						Vertex g3(glm::vec3(-beforeInnerGreen.x + center.x, plane, -beforeInnerGreen.y - center.y), glm::vec2(greenBottomSprite.x + greenBottomSprite.z, 1.0 - greenBottomSprite.y));
						Vertex g4(glm::vec3(-greenInner.x + center.x, plane, -greenInner.y - center.y), glm::vec2(greenBottomSprite.x + greenBottomSprite.z, 1.0 - greenBottomSprite.y));
						pushFourVertices(texVector, g1, g2, g3, g4);
						pushFourIndices(texIndices, texVertexCount);

						Vertex b1(glm::vec3(-blueOuter.x + center.x, plane, -blueOuter.y - center.y), glm::vec2(blueBottomSprite.x, 1.0 - blueBottomSprite.y));
						Vertex b2(glm::vec3(-beforeOuterBlue.x + center.x, plane, -beforeOuterBlue.y - center.y), glm::vec2(blueBottomSprite.x, 1.0 - blueBottomSprite.y));
						Vertex b3(glm::vec3(-beforeInnerBlue.x + center.x, plane, -beforeInnerBlue.y - center.y), glm::vec2(blueBottomSprite.x + blueBottomSprite.z, 1.0 - blueBottomSprite.y));
						Vertex b4(glm::vec3(-blueInner.x + center.x, plane, -blueInner.y - center.y), glm::vec2(blueBottomSprite.x + blueBottomSprite.z, 1.0 - blueBottomSprite.y));
						pushFourVertices(texVector, b1, b2, b3, b4);
						pushFourIndices(texIndices, texVertexCount);

						beforeOuterGreen = greenOuter;
						beforeInnerGreen = greenInner;

						beforeOuterBlue = blueOuter;
						beforeInnerBlue = blueInner;
					}

					if (end < time + m_noteVisibleTime) {
						//draw end zone
						glm::vec2 topOuterGreen = getCirclePoint(greenOuterRadius, endAngle + deltaAngle);
						glm::vec2 topInnerGreen = getCirclePoint(greenInnerRadius, endAngle + deltaAngle);
						glm::vec2 topOuterBlue = getCirclePoint(blueOuterRadius, endAngle + deltaAngle);
						glm::vec2 topInnerblue = getCirclePoint(blueInnerRadius, endAngle + deltaAngle);

						Vertex g1(glm::vec3(-topOuterGreen.x + center.x, plane, -topOuterGreen.y - center.y), glm::vec2(greenTopSprite.x, 1.0 - greenTopSprite.y));
						Vertex g2(glm::vec3(-beforeOuterGreen.x + center.x, plane, -beforeOuterGreen.y - center.y), glm::vec2(greenTopSprite.x, 1.0 - (greenTopSprite.y + greenTopSprite.w)));
						Vertex g3(glm::vec3(-beforeInnerGreen.x + center.x, plane, -beforeInnerGreen.y - center.y), glm::vec2(greenTopSprite.x + greenTopSprite.z, 1.0 - (greenTopSprite.y + greenTopSprite.w)));
						Vertex g4(glm::vec3(-topInnerGreen.x + center.x, plane, -topInnerGreen.y - center.y), glm::vec2(greenTopSprite.x + greenTopSprite.z, 1.0 - greenTopSprite.y));

						pushFourVertices(texVector, g1, g2, g3, g4);
						pushFourIndices(texIndices, texVertexCount);

						Vertex b1(glm::vec3(-topOuterBlue.x + center.x, plane, -topOuterBlue.y - center.y), glm::vec2(blueTopSprite.x, 1.0 - blueTopSprite.y));
						Vertex b2(glm::vec3(-beforeOuterBlue.x + center.x, plane, -beforeOuterBlue.y - center.y), glm::vec2(blueTopSprite.x, 1.0 - (blueTopSprite.y + blueTopSprite.w)));
						Vertex b3(glm::vec3(-beforeInnerBlue.x + center.x, plane, -beforeInnerBlue.y - center.y), glm::vec2(blueTopSprite.x + blueTopSprite.z, 1.0 - (blueTopSprite.y + blueTopSprite.w)));
						Vertex b4(glm::vec3(-topInnerblue.x + center.x, plane, -topInnerblue.y - center.y), glm::vec2(blueTopSprite.x + blueTopSprite.z, 1.0 - blueTopSprite.y));

						pushFourVertices(texVector, b1, b2, b3, b4);
						pushFourIndices(texIndices, texVertexCount);
					}
					//speakers
					double speakerStart = std::max(milli, time);
					double speakerAngle = getAngleFromDT(speakerStart - time);

					glm::vec4 greenSpeaker = m_objectAtlas.at(FS_CROSS_SPEAKER_GREEN_0);
					glm::vec4 blueSpeaker = m_objectAtlas.at(FS_CROSS_SPEAKER_BLUE_0);

					if (speakerStart == time) {
						if (m_playerCross == 0) {
							greenSpeaker = m_objectAtlas.at(FS_CROSS_SPEAKER_GREEN_4);
							blueSpeaker = m_objectAtlas.at(FS_CROSS_SPEAKER_GREEN_0);
						} else if (m_playerCross == 1) {
							greenSpeaker = m_objectAtlas.at(FS_CROSS_SPEAKER_GREEN_2);
							blueSpeaker = m_objectAtlas.at(FS_CROSS_SPEAKER_GREEN_2);
						} else if (m_playerCross == 2) {
							greenSpeaker = m_objectAtlas.at(FS_CROSS_SPEAKER_GREEN_0);
							blueSpeaker = m_objectAtlas.at(FS_CROSS_SPEAKER_GREEN_4);
						}
					}

					if (speakerStart > time) {
						//std::cout << speakerStart << "\t" << speakerStart-time << "\t" << speakerAngle << std::endl;
						Quad q1 = createCircleQuad(speakerAngle, m_radius + 0.5, 0.25, deltaAngle, plane);

						q1.v1.tex = glm::vec2(greenSpeaker.x, 1.0 - greenSpeaker.y);
						q1.v2.tex = glm::vec2(greenSpeaker.x, 1.0 - (greenSpeaker.y + greenSpeaker.w));
						q1.v3.tex = glm::vec2(greenSpeaker.x + greenSpeaker.z, 1.0 - (greenSpeaker.y + greenSpeaker.w));
						q1.v4.tex = glm::vec2(greenSpeaker.x + greenSpeaker.z, 1.0 - greenSpeaker.y);

						pushQuad(texVector, texIndices, texVertexCount, q1);

						Quad q2 = createCircleQuad(speakerAngle, m_radius - 0.5, 0.25, deltaAngle, plane);

						q2.v1.tex = glm::vec2(blueSpeaker.x, 1.0 - blueSpeaker.y);
						q2.v2.tex = glm::vec2(blueSpeaker.x, 1.0 - (blueSpeaker.y + blueSpeaker.w));
						q2.v3.tex = glm::vec2(blueSpeaker.x + blueSpeaker.z, 1.0 - (blueSpeaker.y + blueSpeaker.w));
						q2.v4.tex = glm::vec2(blueSpeaker.x + blueSpeaker.z, 1.0 - blueSpeaker.y);

						pushQuad(texVector, texIndices, texVertexCount, q2);
					} else {
						//WORKS

						Vertex g1(glm::vec3(m_greenLeft, plane, m_back), glm::vec2(greenSpeaker.x, 1.0 - greenSpeaker.y));
						Vertex g2(glm::vec3(m_greenLeft, plane, m_front), glm::vec2(greenSpeaker.x, 1.0 - (greenSpeaker.y + greenSpeaker.w)));
						Vertex g3(glm::vec3(m_greenRight, plane, m_front), glm::vec2(greenSpeaker.x + greenSpeaker.z, 1.0 - (greenSpeaker.y + greenSpeaker.w)));
						Vertex g4(glm::vec3(m_greenRight, plane, m_back), glm::vec2(greenSpeaker.x + greenSpeaker.z, 1.0 - greenSpeaker.y));
						pushFourVertices(texVector, g1, g2, g3, g4);
						pushFourIndices(texIndices, texVertexCount);

						Vertex b1(glm::vec3(m_blueLeft, plane, m_back), glm::vec2(blueSpeaker.x, 1.0 - blueSpeaker.y));
						Vertex b2(glm::vec3(m_blueLeft, plane, m_front), glm::vec2(blueSpeaker.x, 1.0 - (blueSpeaker.y + blueSpeaker.w)));
						Vertex b3(glm::vec3(m_blueRight, plane, m_front), glm::vec2(blueSpeaker.x + blueSpeaker.z, 1.0 - (blueSpeaker.y + blueSpeaker.w)));
						Vertex b4(glm::vec3(m_blueRight, plane, m_back), glm::vec2(blueSpeaker.x + blueSpeaker.z, 1.0 - blueSpeaker.y));
						pushFourVertices(texVector, b1, b2, b3, b4);
						pushFourIndices(texIndices, texVertexCount);
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
		renderColor(colorVector, colorIndices);
		renderTexture(texVector, texIndices, m_objTexture);
	}
}

void GameRender::meters(double time) {
	float yPlane = 0.1f;

	//vertices data
	std::vector<float> metersVector;
	std::vector<unsigned int> metersIndices;
	unsigned int metersVertexCount = 0;

	float radiusTicksDistance = 0.2f;

	glm::vec2 center = {m_radius, 0};
	float meterRadius = m_radius - m_deltaRadius - radiusTicksDistance;

	//combo tickmarks
	double tickWidth = 0.25;
	double tickHeightAngle = asin(tickWidth / (2 * meterRadius));
	double startTicksAngle = tickHeightAngle * 2;
	double endTicksAngle = tickHeightAngle * 10;
	float radiusTicks = m_radius - m_deltaRadius - radiusTicksDistance;

	for (int i = 0; i < 8; i++) {
		int limit = 0;
		if (m_playerCombo > 0 && m_playerCombo < 24) {
			limit = m_playerCombo % 8;
			if (limit == 0) {
				limit = 8;
			}
		} else if (m_playerCombo >= 24) {
			limit = 8;
		}

		glm::vec2 topLeft;
		glm::vec2 topRight;
		glm::vec2 bottomLeft;
		glm::vec2 bottomRight;

		topLeft = getCirclePoint(radiusTicks + tickWidth / 2, startTicksAngle + tickHeightAngle * (i + 1));
		topRight = getCirclePoint(radiusTicks - tickWidth / 2, startTicksAngle + tickHeightAngle * (i + 1));
		bottomLeft = getCirclePoint(radiusTicks + tickWidth / 2, startTicksAngle + tickHeightAngle * i);
		bottomRight = getCirclePoint(radiusTicks - tickWidth / 2, startTicksAngle + tickHeightAngle * i);

		glm::vec4 spriteRect;

		if (i < limit) {
			//active at position i
			spriteRect = {400.0f / 1000.0f, 200.0f / 1200.0f, 400.0f / 1000.0f, 200.0f / 1200.0f};
		} else {
			//inactive at position i
			spriteRect = {400.0f / 1000.0f, 400.0f / 1200.0f, 400.0f / 1000.0f, 200.0f / 1200.0f};
		}

		pushVertexTexture(metersVector, center.x - topLeft.x, yPlane, -center.y - topLeft.y, spriteRect.x, spriteRect.y);
		pushVertexTexture(metersVector, center.x - bottomLeft.x, yPlane, -center.y - bottomLeft.y, spriteRect.x, spriteRect.y - spriteRect.w);
		pushVertexTexture(metersVector, center.x - bottomRight.x, yPlane, -center.y - bottomRight.y, spriteRect.x + spriteRect.z, spriteRect.y - spriteRect.w);
		pushVertexTexture(metersVector, center.x - topRight.x, yPlane, -center.y - topRight.y, spriteRect.x + spriteRect.z, spriteRect.y);
		pushFourIndices(metersIndices, metersVertexCount);
	}

	//multiplier display
	glm::vec4 multiplierSprite;
	if (m_playerMult == 2) {
		multiplierSprite = {0.0f / 1000.0f, 400.0f / 1200.0f, 400.0f / 1000.0f, 400.0f / 1200.0f};
	} else if (m_playerMult == 3) {
		multiplierSprite = {400.0f / 1000.0f, 400.0f / 1200.0f, 400.0f / 1000.0f, 400.0f / 1200.0f};
	} else if (m_playerMult == 4) {
		multiplierSprite = {0.0f / 1000.0f, 800.0f / 1200.0f, 400.0f / 1000.0f, 400.0f / 1200.0f};
	} else if (m_playerMult == 6) {
		multiplierSprite = {0.0f / 1000.0f, 0.0f / 1200.0f, 400.0f / 1000.0f, 400.0f / 1200.0f};
	} else if (m_playerMult == 8) {
		multiplierSprite = {400.0f / 1000.0f, 0.0f / 1200.0f, 400.0f / 1000.0f, 400.0f / 1200.0f};
	} else {
		multiplierSprite = {0.0f / 1000.0f, 0.0f / 1200.0f, 0.0f / 1000.0f, 0.0f / 1200.0f};
	}

	if (m_isButtonsRight) {
		multiplierSprite = {multiplierSprite.x + multiplierSprite.z, multiplierSprite.y, -multiplierSprite.z, multiplierSprite.w};
	}

	glm::vec2 size = {0.3, 0.3};
	double displayAngle = asin(size.y / radiusTicks);

	glm::vec2 topLeft = getCirclePoint(radiusTicks + size.x / 2, endTicksAngle + displayAngle);
	glm::vec2 bottomLeft = getCirclePoint(radiusTicks + size.x / 2, endTicksAngle);
	glm::vec2 bottomRight = getCirclePoint(radiusTicks - size.x / 2, endTicksAngle);
	glm::vec2 topRight = getCirclePoint(radiusTicks - size.x / 2, endTicksAngle + displayAngle);

	//multiplier number
	pushVertexTexture(metersVector, center.x - topLeft.x, yPlane, -center.y - topLeft.y, multiplierSprite.x, 1.0f - multiplierSprite.y);
	pushVertexTexture(metersVector, center.x - bottomLeft.x, yPlane, -center.y - bottomLeft.y, multiplierSprite.x, 1.0f - multiplierSprite.y - multiplierSprite.w);
	pushVertexTexture(metersVector, center.x - bottomRight.x, yPlane, -center.y - bottomRight.y, multiplierSprite.x + multiplierSprite.z, 1.0f - multiplierSprite.y - multiplierSprite.w);
	pushVertexTexture(metersVector, center.x - topRight.x, yPlane, -center.y - topRight.y, multiplierSprite.x + multiplierSprite.z, 1.0f - multiplierSprite.y);
	pushFourIndices(metersIndices, metersVertexCount);

	//euphoria meter
	//empty indicator

	glm::vec4 euEmptySprite = {800.0f / 1000.0f, 400.0f / 1200.0f, 200.0f / 1000.0f, 400.0f / 1200.0f};
	glm::vec4 euFullSprite = {800.0f / 1000.0f, 800.0f / 1200.0f, 200.0f / 1000.0f, 400.0f / 1200.0f};

	float radiusEuphoria = m_radius - m_deltaRadius - 0.4f;
	glm::vec2 slotSize = {0.05, 0.3333};
	double heightAngle = asin(slotSize.y / radiusEuphoria);
	double startEuphoriaAngle = getAngleHorizontal(startTicksAngle, radiusTicks, radiusEuphoria);

	for (int i = 0; i < 3; i++) {
		glm::vec2 topLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * (i + 1));
		glm::vec2 bottomLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * i);
		glm::vec2 bottomRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * i);
		glm::vec2 topRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * (i + 1));

		pushVertexTexture(metersVector, center.x - topLeft.x, yPlane, -center.y - topLeft.y, euEmptySprite.x, 1.0f - euEmptySprite.y);
		pushVertexTexture(metersVector, center.x - bottomLeft.x, yPlane, -center.y - bottomLeft.y, euEmptySprite.x, 1.0f - euEmptySprite.y - euEmptySprite.w);
		pushVertexTexture(metersVector, center.x - bottomRight.x, yPlane, -center.y - bottomRight.y, euEmptySprite.x + euEmptySprite.z, 1.0f - euEmptySprite.y - euEmptySprite.w);
		pushVertexTexture(metersVector, center.x - topRight.x, yPlane, -center.y - topRight.y, euEmptySprite.x + euEmptySprite.z, 1.0f - euEmptySprite.y);
		pushFourIndices(metersIndices, metersVertexCount);
	}

	//if only the first indicator needs to change
	if (m_renderEuValue > 0.0 && m_renderEuValue < 1.0) {
		glm::vec2 topLeftTarget = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * (0 + 1));
		glm::vec2 bottomLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * 0);
		glm::vec2 bottomRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * 0);
		glm::vec2 topRightTarget = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * (0 + 1));

		glm::vec2 leftSide = topLeftTarget - bottomLeft;
		glm::vec2 rightSide = topRightTarget - bottomRight;

		float euValueoffset = 0.0f;

		glm::vec2 topLeft = {bottomLeft.x + leftSide.x * (m_renderEuValue - euValueoffset), bottomLeft.y + leftSide.y * (m_renderEuValue - euValueoffset)};
		glm::vec2 topRight = {bottomRight.x + rightSide.x * (m_renderEuValue - euValueoffset), bottomRight.y + rightSide.y * (m_renderEuValue - euValueoffset)};

		pushVertexTexture(metersVector, center.x - topLeft.x, yPlane, -center.y - topLeft.y, euFullSprite.x, 1.0f - euFullSprite.y);
		pushVertexTexture(metersVector, center.x - bottomLeft.x, yPlane, -center.y - bottomLeft.y, euFullSprite.x, 1.0f - euFullSprite.y - euFullSprite.w);
		pushVertexTexture(metersVector, center.x - bottomRight.x, yPlane, -center.y - bottomRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y - euFullSprite.w);
		pushVertexTexture(metersVector, center.x - topRight.x, yPlane, -center.y - topRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y);
		pushFourIndices(metersIndices, metersVertexCount);
	}
	//if more indicators need to change (two or three)
	else if (m_renderEuValue >= 1.0) {
		//add first full indicator
		glm::vec2 topLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * (0 + 1));
		glm::vec2 bottomLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * 0);
		glm::vec2 bottomRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * 0);
		glm::vec2 topRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * (0 + 1));

		pushVertexTexture(metersVector, center.x - topLeft.x, yPlane, -center.y - topLeft.y, euFullSprite.x, 1.0f - euFullSprite.y);
		pushVertexTexture(metersVector, center.x - bottomLeft.x, yPlane, -center.y - bottomLeft.y, euFullSprite.x, 1.0f - euFullSprite.y - euFullSprite.w);
		pushVertexTexture(metersVector, center.x - bottomRight.x, yPlane, -center.y - bottomRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y - euFullSprite.w);
		pushVertexTexture(metersVector, center.x - topRight.x, yPlane, -center.y - topRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y);
		pushFourIndices(metersIndices, metersVertexCount);

		//if only the second indicator needs to change
		if (m_renderEuValue < 2.0) {
			glm::vec2 topLeftTarget = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * (1 + 1));
			glm::vec2 bottomLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * 1);
			glm::vec2 bottomRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * 1);
			glm::vec2 topRightTarget = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * (1 + 1));

			glm::vec2 leftSide = topLeftTarget - bottomLeft;
			glm::vec2 rightSide = topRightTarget - bottomRight;

			float euValueoffset = 1.0f;

			glm::vec2 topLeft = {bottomLeft.x + leftSide.x * (m_renderEuValue - euValueoffset), bottomLeft.y + leftSide.y * (m_renderEuValue - euValueoffset)};
			glm::vec2 topRight = {bottomRight.x + rightSide.x * (m_renderEuValue - euValueoffset), bottomRight.y + rightSide.y * (m_renderEuValue - euValueoffset)};

			pushVertexTexture(metersVector, center.x - topLeft.x, yPlane, -center.y - topLeft.y, euFullSprite.x, 1.0f - euFullSprite.y);
			pushVertexTexture(metersVector, center.x - bottomLeft.x, yPlane, -center.y - bottomLeft.y, euFullSprite.x, 1.0f - euFullSprite.y - euFullSprite.w);
			pushVertexTexture(metersVector, center.x - bottomRight.x, yPlane, -center.y - bottomRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y - euFullSprite.w);
			pushVertexTexture(metersVector, center.x - topRight.x, yPlane, -center.y - topRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y);
			pushFourIndices(metersIndices, metersVertexCount);
		}
		//if more indicators need to change (all three)
		else if (m_renderEuValue >= 2.0) {
			//add full second indicator
			glm::vec2 topLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * (1 + 1));
			glm::vec2 bottomLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * 1);
			glm::vec2 bottomRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * 1);
			glm::vec2 topRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * (1 + 1));

			pushVertexTexture(metersVector, center.x - topLeft.x, yPlane, -center.y - topLeft.y, euFullSprite.x, 1.0f - euFullSprite.y);
			pushVertexTexture(metersVector, center.x - bottomLeft.x, yPlane, -center.y - bottomLeft.y, euFullSprite.x, 1.0f - euFullSprite.y - euFullSprite.w);
			pushVertexTexture(metersVector, center.x - bottomRight.x, yPlane, -center.y - bottomRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y - euFullSprite.w);
			pushVertexTexture(metersVector, center.x - topRight.x, yPlane, -center.y - topRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y);
			pushFourIndices(metersIndices, metersVertexCount);

			//if the euphoria value is below full
			if (m_renderEuValue < 3.0) {
				glm::vec2 topLeftTarget = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * (2 + 1));
				glm::vec2 bottomLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * 2);
				glm::vec2 bottomRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * 2);
				glm::vec2 topRightTarget = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * (2 + 1));

				glm::vec2 leftSide = topLeftTarget - bottomLeft;
				glm::vec2 rightSide = topRightTarget - bottomRight;

				float euValueoffset = 2.0f;

				glm::vec2 topLeft = {bottomLeft.x + leftSide.x * (m_renderEuValue - euValueoffset), bottomLeft.y + leftSide.y * (m_renderEuValue - euValueoffset)};
				glm::vec2 topRight = {bottomRight.x + rightSide.x * (m_renderEuValue - euValueoffset), bottomRight.y + rightSide.y * (m_renderEuValue - euValueoffset)};

				pushVertexTexture(metersVector, center.x - topLeft.x, yPlane, -center.y - topLeft.y, euFullSprite.x, 1.0f - euFullSprite.y);
				pushVertexTexture(metersVector, center.x - bottomLeft.x, yPlane, -center.y - bottomLeft.y, euFullSprite.x, 1.0f - euFullSprite.y - euFullSprite.w);
				pushVertexTexture(metersVector, center.x - bottomRight.x, yPlane, -center.y - bottomRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y - euFullSprite.w);
				pushVertexTexture(metersVector, center.x - topRight.x, yPlane, -center.y - topRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y);
				pushFourIndices(metersIndices, metersVertexCount);
			} else {
				//add full third indicator

				glm::vec2 topLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * (2 + 1));
				glm::vec2 bottomLeft = getCirclePoint(radiusEuphoria + slotSize.x, startEuphoriaAngle + heightAngle * 2);
				glm::vec2 bottomRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * 2);
				glm::vec2 topRight = getCirclePoint(radiusEuphoria - slotSize.x, startEuphoriaAngle + heightAngle * (2 + 1));

				pushVertexTexture(metersVector, center.x - topLeft.x, yPlane, -center.y - topLeft.y, euFullSprite.x, 1.0f - euFullSprite.y);
				pushVertexTexture(metersVector, center.x - bottomLeft.x, yPlane, -center.y - bottomLeft.y, euFullSprite.x, 1.0f - euFullSprite.y - euFullSprite.w);
				pushVertexTexture(metersVector, center.x - bottomRight.x, yPlane, -center.y - bottomRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y - euFullSprite.w);
				pushVertexTexture(metersVector, center.x - topRight.x, yPlane, -center.y - topRight.y, euFullSprite.x + euFullSprite.z, 1.0f - euFullSprite.y);
				pushFourIndices(metersIndices, metersVertexCount);
			}
		}
	}

	usePersProj();
	renderTexture(metersVector, metersIndices, m_metersTexture);
	metersVector.clear();
	metersIndices.clear();
	metersVertexCount = 0;

	//push the old state for later
	bool oldButtonsPos = rendr_InvertedX;
	rendr_InvertedX = false;

	//progress bar
	std::string s = std::to_string(m_playerScore);
	std::string scoreDisplay("00000000");
	scoreDisplay.resize(scoreDisplay.length() - s.length());
	scoreDisplay.append(s);

	glm::vec2 scorePosition {140.0f, 310.0f};
	glm::vec2 scoreSize {200.0f, 20.0f};
	glm::vec2 scoreOuterPadding {10.0f, 10.0f};
	float starSideLength = scoreSize.y + 6 * scoreOuterPadding.y;

	if (oldButtonsPos) {
		//if m_isButtonRight was true move meter to the right
		scorePosition.x = 1280.0f - scorePosition.x - scoreSize.x;
	}

	//base meter
	pushVertexTexture(metersVector, scorePosition.x - scoreOuterPadding.x, scorePosition.y - scoreOuterPadding.y, 0.0f, 0.0f, 0.5f);
	pushVertexTexture(metersVector, scorePosition.x - scoreOuterPadding.x, scorePosition.y + scoreOuterPadding.y + scoreSize.y, 0.0f, 0.0f, 0.0f);
	pushVertexTexture(metersVector, scorePosition.x + scoreOuterPadding.x + scoreSize.x, scorePosition.y + scoreOuterPadding.y + scoreSize.y, 0.0f, 220.0f / 300.0f, 0.0f);
	pushVertexTexture(metersVector, scorePosition.x + scoreOuterPadding.x + scoreSize.x, scorePosition.y - scoreOuterPadding.y, 0.0f, 220.0f / 300.0f, 0.5f);
	pushFourIndices(metersIndices, metersVertexCount);
	//star
	pushVertexTexture(metersVector, scorePosition.x + scoreOuterPadding.x + scoreSize.x, scorePosition.y + scoreSize.y / 2 - starSideLength / 2, 0.0f, 221.0f / 300.0f, 1.0f);
	pushVertexTexture(metersVector, scorePosition.x + scoreOuterPadding.x + scoreSize.x, scorePosition.y + scoreSize.y / 2 + starSideLength / 2, 0.0f, 221.0f / 300.0f, 0.0f);
	pushVertexTexture(metersVector, scorePosition.x + scoreOuterPadding.x + scoreSize.x + starSideLength, scorePosition.y + scoreSize.y / 2 + starSideLength / 2, 0.0f, 1.0f, 0.0f);
	pushVertexTexture(metersVector, scorePosition.x + scoreOuterPadding.x + scoreSize.x + starSideLength, scorePosition.y + scoreSize.y / 2 - starSideLength / 2, 0.0f, 1.0f, 1.0f);
	pushFourIndices(metersIndices, metersVertexCount);

	useOrthoProj();
	renderTexture(metersVector, metersIndices, m_pgBarFrame);
	metersVector.clear();
	metersIndices.clear();
	metersVertexCount = 0;

	float scale = 0.05f;
	float x = 0.0;
	x = (float)m_playerScore / (float)m_genBaseScore;

	float textX = scorePosition.x + scoreSize.x + 3.5f * scoreOuterPadding.x;
	float textY = scorePosition.y - scoreOuterPadding.y;

	if (x < 0.1f) {
		x = x / 0.1f;
		x *= scoreSize.x;
	} else if (x < 0.2f) {
		drawText("1", textX, textY, scale);
		x = (x - 0.1f) / 0.1f;
		x *= scoreSize.x;
	} else if (x < 0.3f) {
		drawText("2", textX, textY, scale);
		x = (x - 0.2f) / 0.1f;
		x *= scoreSize.x;
	} else if (x < 0.4f) {
		drawText("3", textX, textY, scale);
		x = (x - 0.3f) / 0.1f;
		x *= scoreSize.x;
	} else if (x < 0.5f) {
		drawText("4", textX - 2.0f, textY, scale);
		x = (x - 0.40f) / 0.10f;
		x *= scoreSize.x;
	} else if (x < 0.7f) {
		drawText("5", textX, textY, scale);
		x = (x - 0.50f) / 0.20f;
		x *= scoreSize.x;
	} else {
		drawText("5", textX, textY, scale);
		x = scoreSize.x;
	}

	float ratio = x / scoreSize.y;
	float toffset = (float)time * -1.0f;

	//inner meter thing
	pushVertexTexture(metersVector, scorePosition.x, scorePosition.y, 0.0f, 0.0f + toffset, 1.0f);
	pushVertexTexture(metersVector, scorePosition.x, scorePosition.y + scoreSize.y, 0.0f, 0.0f + toffset, 0.0f);
	pushVertexTexture(metersVector, scorePosition.x + x, scorePosition.y + scoreSize.y, 0.0f, 0.0f + ratio + toffset, 0.0f);
	pushVertexTexture(metersVector, scorePosition.x + x, scorePosition.y, 0.0f, 0.0f + ratio + toffset, 1.0f);
	pushFourIndices(metersIndices, metersVertexCount);

	useOrthoProj();
	renderTexture(metersVector, metersIndices, m_pgBarInside);

	//score meter
	drawText(scoreDisplay, scorePosition.x, scorePosition.y - scoreOuterPadding.y - 80.0f, scale);

	//combo meter
	if (m_playerCombo >= 15) {
		std::string c = std::to_string(m_playerCombo);
		drawText(c, scorePosition.x, scorePosition.y - scoreOuterPadding.y - 30.0f, 0.03f);
	}
	//pop back the old inverted state
	rendr_InvertedX = oldButtonsPos;
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
	bool brokeOnce = player.getBrokeOnce();

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
			pushFourIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.2) {
			pushVertexTexture(resultVector, x + 30.0f + scale * 1000.0f, y, 0.0f, 221.0f / 300.0f, 1.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 1000.0f, y + scale * 1000.0f, 0.0f, 221.0f / 300.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 2000.0f, y + scale * 1000.0f, 0.0f, 1.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 2000.0f, y, 0.0f, 1.0f, 1.0f);
			pushFourIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.3) {
			pushVertexTexture(resultVector, x + 30.0f + scale * 2000.0f, y, 0.0f, 221.0f / 300.0f, 1.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 2000.0f, y + scale * 1000.0f, 0.0f, 221.0f / 300.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 3000.0f, y + scale * 1000.0f, 0.0f, 1.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 3000.0f, y, 0.0f, 1.0f, 1.0f);
			pushFourIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.4) {
			pushVertexTexture(resultVector, x + 30.0f + scale * 3000.0f, y, 0.0f, 221.0f / 300.0f, 1.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 3000.0f, y + scale * 1000.0f, 0.0f, 221.0f / 300.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 4000.0f, y + scale * 1000.0f, 0.0f, 1.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 4000.0f, y, 0.0f, 1.0f, 1.0f);
			pushFourIndices(resultIndices, resultVertexCount);
		}
		if (stars >= 0.5) {
			pushVertexTexture(resultVector, x + 30.0f + scale * 4000.0f, y, 0.0f, 221.0f / 300.0f, 1.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 4000.0f, y + scale * 1000.0f, 0.0f, 221.0f / 300.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 5000.0f, y + scale * 1000.0f, 0.0f, 1.0f, 0.0f);
			pushVertexTexture(resultVector, x + 30.0f + scale * 5000.0f, y, 0.0f, 1.0f, 1.0f);
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
	std::string fps = "Delta Time:";
	fps.append(std::to_string(deltaTime));
	fps.append(std::string(" | FPS:"));
	fps.append(std::to_string(1 / deltaTime));
	drawText(fps, 10.0f, 170.0f, 0.05f);

	std::string baseScore;
	baseScore.append(std::to_string((float)m_playerScore / (float)m_genBaseScore));
	baseScore.append("|");
	baseScore.append(std::to_string(m_genBaseScore));
	if (m_isButtonsRight) {
		drawText(baseScore, 940.0f, 370.0f, 0.03f);
	} else {
		drawText(baseScore, 140.0f, 370.0f, 0.03f);
	}

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
	m_genBPM = g.m_bpm;

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
		pushFourIndices(clickerIndices, clickerVertexCount);
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

		pushFourIndices(clickerIndices, clickerVertexCount);
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
		pushFourIndices(clickerIndices, clickerVertexCount);
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

std::vector<Note> GameRender::getCrossInsideNote(Note& note, std::vector<Note>& cross) {
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

int GameRender::getCrossAtTime(double time, std::vector<Note>& crossArr, std::vector<Note>* eventArr) {
	std::vector<Note> crossCopy;
	std::copy(crossArr.begin(), crossArr.end(), std::back_inserter(crossCopy));
	if (eventArr != nullptr) {
		//0.5 to be considered 0.25 away from the clicker
		double timeOffset = getDTFromAngle(0.5 / m_radius);
		//check for freestyle crossfade at that time
		for (auto& n : *eventArr) {
			if (n.getType() == FS_CROSS_BASE && n.getMilli() - timeOffset <= time && n.getMilli() + n.getLength() + timeOffset > time) {
				return -1; // no position there
			}
		}
	}
	if (!crossCopy.empty() && time >= 0) {
		int index = -1;
		for (size_t i = 0; i < crossCopy.size(); ++i) {
			if (crossCopy.at(i).getMilli() <= time) {
				index = i;
			} else if (crossCopy.at(i).getMilli() > time) {
				break;
			}
		}
		//std::cerr << index << std::endl;
		int type = crossCopy.at(index).getType();
		if (type == CROSS_G || type == CROSS_G_TICK) {
			return 0;
		} else if (type == CROSS_B || type == CROSS_B_TICK) {
			return 2;
		}
		return 1;
	} else {
		return 1; // CENTER BY DEFAULT
	}
}

double GameRender::getAngleFromDT(double dt) const {
	double clickerAngle = asin(0.25 / m_radius);
	return dt * (m_maxAngle - clickerAngle) / m_noteVisibleTime + clickerAngle;
}

double GameRender::getDTFromAngle(double angle) const {
	double clickerAngle = asin(0.25 / m_radius);
	return (angle - clickerAngle) * m_noteVisibleTime / (m_maxAngle - clickerAngle);
}

double GameRender::getAngleHorizontal(double innerAngle, double innerRadius, double outerRadius) {
	return asin(sin(innerAngle) * innerRadius / outerRadius);
}

Quad GameRender::createCircleQuad(double baseAngle, double baseRadius, double deltaRadius, double deltaAngle, float plane) const {
	Quad res;
	/*
		1---4
		| p |
		2---3

		p is at baseRadius,angle

		angle must be calculated with asin() before calling func
	*/
	glm::vec2 center = {m_radius, 0.0};

	glm::vec2 topLeft = getCirclePoint(baseRadius - deltaRadius, baseAngle + deltaAngle);
	glm::vec2 topRight = getCirclePoint(baseRadius + deltaRadius, baseAngle + deltaAngle);
	glm::vec2 bottomLeft = getCirclePoint(baseRadius - deltaRadius, baseAngle - deltaAngle);
	glm::vec2 bottomRight = getCirclePoint(baseRadius + deltaRadius, baseAngle - deltaAngle);

	res.v1.pos = {-topLeft.x + center.x, plane, -topLeft.y - center.y};
	res.v2.pos = {-bottomLeft.x + center.x, plane, -bottomLeft.y - center.y};
	res.v3.pos = {-bottomRight.x + center.x, plane, -bottomRight.y - center.y};
	res.v4.pos = {-topRight.x + center.x, plane, -topRight.y - center.y};

	return res;
}
