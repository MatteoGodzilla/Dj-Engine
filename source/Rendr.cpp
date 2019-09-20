#include "Rendr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void checkError() {
	std::cout << "started error checking" << std::endl;
	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		std::cout << "OpenGL Error: " << error << std::endl;
		error = glGetError();
	}
	std::cout << "ended error checking" << std::endl;
}

void pushVertexColor(std::vector<float>& v, float x, float y, float z, float r, float g, float b,float a = 1.0) {
	v.push_back(x);
	v.push_back(y);
	v.push_back(z);
	v.push_back(r);
	v.push_back(g);
	v.push_back(b);
	v.push_back(a);
}

void pushVertexTexture(std::vector<float>& v, float x, float y, float z, float s = 0.0, float t = 0.0) {
	v.push_back(x);
	v.push_back(y);
	v.push_back(z);
	v.push_back(s);
	v.push_back(t);
}

void pushRectangleIndices(std::vector<unsigned int>& v, unsigned int& value) {
	v.push_back(value);
	v.push_back(value + 1);
	v.push_back(value + 2);
	v.push_back(value + 2);
	v.push_back(value + 3);
	v.push_back(value);
	value += 4;
}

Rendr::Rendr() {
	//ctor
}

void Rendr::init(GLFWwindow* w) {
	m_window = w;
	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW INIT ERROR" << std::endl;
		return;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//shader init
	{
		const char* vTextureSource = "\n"
			"#version 330 core\n"
			"layout(location = 0) in vec4 aPos;\n"
			"layout(location = 1) in vec2 tCoords;\n"
			"uniform mat4 u_proj;\n"
			"out vec2 tex_coords;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	gl_Position = u_proj * aPos;\n"
			"	tex_coords = tCoords;\n"
			"}";

		const char* fTextureSource = "\n"
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec2 tex_coords;\n"
			"uniform sampler2D u_t;\n"
			"\n"
			"void main()"
			"{\n"
			"	FragColor = texture(u_t,tex_coords);\n"
			"}\n";


		const char* vColorSource = "\n"
			"#version 330 core\n"
			"layout(location = 0) in vec4 aPos;\n"
			"layout(location = 1) in vec4 c;\n"
			"uniform mat4 u_proj;\n"
			"out vec4 color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	gl_Position = u_proj * aPos;\n"
			"	color = c;\n"
			"}";

		const char* fColorSource = "\n"
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec4 color;\n"
			"\n"
			"void main()"
			"{\n"
			"	FragColor = color;\n"
			"}\n";


		unsigned int vShaderTexture, fShaderTexture;
		unsigned int vShaderColor, fShaderColor;

		vShaderTexture = glCreateShader(GL_VERTEX_SHADER);
		fShaderTexture = glCreateShader(GL_FRAGMENT_SHADER);
		vShaderColor = glCreateShader(GL_VERTEX_SHADER);
		fShaderColor = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vShaderTexture, 1, &vTextureSource, NULL);
		glShaderSource(fShaderTexture, 1, &fTextureSource, NULL);
		glShaderSource(vShaderColor, 1, &vColorSource, NULL);
		glShaderSource(fShaderColor, 1, &fColorSource, NULL);

		glCompileShader(vShaderTexture);
		glCompileShader(fShaderTexture);
		glCompileShader(vShaderColor);
		glCompileShader(fShaderColor);

		m_TextureProgram = glCreateProgram();
		glAttachShader(m_TextureProgram, vShaderTexture);
		glAttachShader(m_TextureProgram, fShaderTexture);
		glLinkProgram(m_TextureProgram);
		glDeleteShader(vShaderTexture);
		glDeleteShader(fShaderTexture);

		m_ColorProgram = glCreateProgram();
		glAttachShader(m_ColorProgram, vShaderColor);
		glAttachShader(m_ColorProgram, fShaderColor);
		glLinkProgram(m_ColorProgram);

		glDeleteShader(vShaderColor);
		glDeleteShader(fShaderColor);
	}

	//texture load
	{
		//variables reused for every load
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* high = stbi_load("res/highway.png", &width, &height, &channels, 0);

		glGenTextures(1, &m_HighwayTexture);
		glBindTexture(GL_TEXTURE_2D, m_HighwayTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, high);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, high);

		unsigned char* obj = stbi_load("res/objects.png", &width, &height, &channels, 0);

		glGenTextures(1, &m_ObjTexture);
		glBindTexture(GL_TEXTURE_2D, m_ObjTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, obj);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, obj);

		unsigned char* meters = stbi_load("res/meters.png", &width, &height, &channels, 0);
		glGenTextures(1, &m_MetersTexture);
		glBindTexture(GL_TEXTURE_2D, m_MetersTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, meters);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, meters);

		unsigned char* splash = stbi_load("res/splash.png", &width, &height, &channels, 0);
		glGenTextures(1, &m_splashTexture);
		glBindTexture(GL_TEXTURE_2D, m_splashTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, splash);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, splash);

		//removing image data
		stbi_image_free(high);
		stbi_image_free(obj);
		stbi_image_free(meters);
		stbi_image_free(splash);


		glBindTexture(GL_TEXTURE_2D, 0);
	}


	//projection init
	{
		//m_proj = main perspective projection
		m_proj = glm::perspective(45.0f, 1024.0f / 600, 1.0f, -2.0f);
		glm::mat4 look = glm::lookAt(glm::vec3(0.0, 2.0, 5.0),
			glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 1.0, 0.0));
		m_proj = m_proj * look;

		//settung up projection uniform
		glUseProgram(m_TextureProgram);
		int location = glGetUniformLocation(m_TextureProgram, "u_proj");
		if (location != -1) {
			glUniformMatrix4fv(location, 1, GL_FALSE, &m_proj[0][0]);
			std::cout << "uniform texture successful" << std::endl;
		}

		glUseProgram(m_ColorProgram);
		location = glGetUniformLocation(m_ColorProgram, "u_proj");
		if (location != -1) {
			glUniformMatrix4fv(location, 1, GL_FALSE, &m_proj[0][0]);
			std::cout << "uniform color successful" << std::endl;
		}

	}

	//vao setup
	{
		glGenVertexArrays(1, &m_textureVAO);
		glGenVertexArrays(1, &m_colorVAO);
		glGenVertexArrays(1, &m_textureVAO);
		glGenVertexArrays(1, &m_textureVAO);
		glGenVertexArrays(1, &m_colorVAO);
		glGenVertexArrays(1, &m_textureVAO);

		//texture vao
		{
			glBindVertexArray(m_textureVAO);

			unsigned int ebo;
			glGenBuffers(1, &m_textureVBO);
			glGenBuffers(1, &ebo);

			glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			//Vertex: x,y,z, u,v
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		//color vao
		{
			glBindVertexArray(m_colorVAO);

			unsigned int index;
			glGenBuffers(1, &m_colorVBO);
			glGenBuffers(1, &index);
			glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

			//Vertex: x,y,z, r,g,b,a
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);


			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	}
}

void Rendr::highway(double time) {

	float factor = (float)time * 0.875f;

	float plane = 0.0f;

	std::vector<float> highwayVector;
	std::vector<unsigned int> highwayIndices;
	unsigned int highwayVertexCount = 0;

	pushVertexTexture(highwayVector, -1.0f, plane, 0.0f, 0.0f, 1.0f + factor);
	pushVertexTexture(highwayVector, -1.0f, plane, 4.0f, 0.0f, 0.0f + factor);
	pushVertexTexture(highwayVector, 1.0f, plane, 4.0f, 1.0f, 0.0f + factor);
	pushVertexTexture(highwayVector, 1.0f, plane, 0.0f, 1.0f, 1.0f + factor);
	pushRectangleIndices(highwayIndices, highwayVertexCount);

	glBindVertexArray(m_textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);
	glBindTexture(GL_TEXTURE_2D, m_HighwayTexture);

	glBufferData(GL_ARRAY_BUFFER, highwayVector.size() * sizeof(float), highwayVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, highwayIndices.size() * sizeof(int), highwayIndices.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_TextureProgram);
	glDrawElements(GL_TRIANGLES, highwayIndices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);

}

void Rendr::clicker() {
	float clickedOffset = 0.03f;

	float plane = 0.0f;

	std::vector<float> clickerVector = {};
	std::vector<unsigned int> clickerIndices = {};
	unsigned int clickerVertexCount = 0;

	pushVertexTexture(clickerVector, -0.85f, plane, 3.6f, 1320.0f / 1760.0f, 400.0/1640.0);
	pushVertexTexture(clickerVector, -0.85f, plane, 3.9f, 1.0f, 400.0 / 1640.0);
	pushVertexTexture(clickerVector, -0.2f, plane, 3.9f, 1.0f, 1280.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.2f, plane, 3.6f, 1320.0f / 1760.0f, 1280.0f / 1640.0f);

	pushRectangleIndices(clickerIndices, clickerVertexCount);

	pushVertexTexture(clickerVector, 0.85f, plane, 3.6f, 1320.0f / 1760.0f, 400.0 / 1640.0);
	pushVertexTexture(clickerVector, 0.85f, plane, 3.9f, 1.0f, 400.0 / 1640.0);
	pushVertexTexture(clickerVector, 0.2f, plane, 3.9f, 1.0f, 1280.0f / 1640.0f);
	pushVertexTexture(clickerVector, 0.2f, plane, 3.6f, 1320.0f / 1760.0f, 1280.0f / 1640.0f);

	pushRectangleIndices(clickerIndices, clickerVertexCount);
	if (m_red) {
		pushVertexTexture(clickerVector, -0.15f + clickedOffset, plane, 3.6f + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, -0.15f + clickedOffset, plane, 3.9f - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f - clickedOffset, plane, 3.9f - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f - clickedOffset, plane, 3.6f + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
	}
	else {
		pushVertexTexture(clickerVector, -0.15f, plane, 3.6f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, -0.15f, plane, 3.9f, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f, plane, 3.9f, 880.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f, plane, 3.6f, 880.0f / 1760.0f, 840.0f / 1640.0f);
	}
	pushRectangleIndices(clickerIndices, clickerVertexCount);


	if (m_green) {
		if (m_playerCross >= 1) {
			pushVertexTexture(clickerVector, -0.5f + clickedOffset, plane, 3.6f + clickedOffset, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.5f + clickedOffset, plane, 3.9f - clickedOffset, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f - clickedOffset, plane, 3.9f - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f - clickedOffset, plane, 3.6f + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			pushVertexTexture(clickerVector, -0.85f + clickedOffset, plane, 3.6f + clickedOffset, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.85f + clickedOffset, plane, 3.9f - clickedOffset, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f - clickedOffset, plane, 3.9f - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f - clickedOffset, plane, 3.6f + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	else {
		if (m_playerCross >= 1) {
			pushVertexTexture(clickerVector, -0.5f, plane, 3.6f, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.5f, plane, 3.9f, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f, plane, 3.9f, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f, plane, 3.6f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			pushVertexTexture(clickerVector, -0.85f, plane, 3.6f, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.85f, plane, 3.9f, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f, plane, 3.9f, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f, plane, 3.6f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	if (m_blue) {
		if (m_playerCross <= 1) {
			pushVertexTexture(clickerVector, 0.5f - clickedOffset, plane, 3.6f + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.5f - clickedOffset, plane, 3.9f - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f + clickedOffset, plane, 3.9f - clickedOffset, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f + clickedOffset, plane, 3.6f + clickedOffset, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			pushVertexTexture(clickerVector, 0.85f - clickedOffset, plane, 3.6f + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.85f - clickedOffset, plane, 3.9f - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f + clickedOffset, plane, 3.9f - clickedOffset, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f + clickedOffset, plane, 3.6f + clickedOffset, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	else {
		if (m_playerCross <= 1) {
			pushVertexTexture(clickerVector, 0.5f, plane, 3.6f, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.5f, plane, 3.9f, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f, plane, 3.9f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f, plane, 3.6f, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			pushVertexTexture(clickerVector, 0.85f, plane, 3.6f, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.85f, plane, 3.9f, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f, plane, 3.9f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f, plane, 3.6f, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	glBindVertexArray(m_textureVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);
	glBufferData(GL_ARRAY_BUFFER, clickerVector.size() * sizeof(float), clickerVector.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, clickerIndices.size() * sizeof(float), clickerIndices.data(), GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, m_ObjTexture);
	glUseProgram(m_TextureProgram);
	glDrawElements(GL_TRIANGLES, clickerIndices.size(), GL_UNSIGNED_INT, 0);

}

void Rendr::notes(double time, std::vector<Note>& v) {

	float plane = 0.0;

	std::vector<float> noteVector = {};
	std::vector<unsigned int> noteIndices = {};
	unsigned int noteVertexCount = 0;

	for (size_t i = 0; i < v.size(); i++) {
		if (v.at(i).getRender()) {
			double dt = v.at(i).getMilli() - time;
			float z = 3.75f - (3.75f * dt);
			int type = v.at(i).getType();

			float s = 0.0;
			float t = 0.0;

			if (type == TAP_R) {
				if (m_renderEuActive) {
					s = 1200.0 / 1760.0;
					t = 0.0;
				}
				else {
					s = 400.0 / 1760.0;
					t = 0.0;
				}
				pushVertexTexture(noteVector, -0.15f, plane, z - 0.15f, s, t + 400.0f / 1760.0f);
				pushVertexTexture(noteVector, -0.15f, plane, z + 0.15f, s, t);
				pushVertexTexture(noteVector, 0.15f, plane, z + 0.15f, s + 400.0/1760.0, t);
				pushVertexTexture(noteVector, 0.15f, plane, z - 0.15f, s + 400.0 / 1760.0, t + 400.0f / 1760.0f);
				pushRectangleIndices(noteIndices, noteVertexCount);
			}
			else if (type == TAP_G) {
				if (m_renderEuActive) {
					s = 1200.0 / 1760.0;
					t = 0.0;
				}
				else {
					s = 0.0;
					t = 0.0;
				}
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
				s = 400.0 / 1760.0;
				t = 840.0 / 1640.0;
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
				s = 800.0 / 1760.0;
				t = 840.0 / 1640.0;
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
				s = 0.0;
				t = 840.0 / 1640.0;
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
				if (m_renderEuActive) {
					s = 1200.0 / 1760.0;
					t = 0.0;
				}
				else {
					s = 800.0/1760.0;
					t = 0.0;
				}
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
			s = 400.0 / 1760.0;
			t = 840.0 / 1640.0;
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
			s = 800.0 / 1760.0;
			t = 840.0 / 1640.0;
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
			s = 0.0;
			t = 840.0 / 1640.0;
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
			v.at(i).setLanMod(m_renderCross);
		}
	}

	glBindVertexArray(m_textureVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);

	glBufferData(GL_ARRAY_BUFFER, noteVector.size() * sizeof(float), noteVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, noteIndices.size() * sizeof(int), noteIndices.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_TextureProgram);
	glBindTexture(GL_TEXTURE_2D, m_ObjTexture);
	glDrawElements(GL_TRIANGLES, noteIndices.size(), GL_UNSIGNED_INT, 0);
	
}

void Rendr::lanes(double time, std::vector<Note>& ev) {

	float plane = 0.0;

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


	if (m_renderEuActive) {
		r = 1.0;
		g = 1.0;
		b = 1.0;
	}
	else {
		r = 1.0;
		g = 0.0;
		b = 0.0;
	}

	pushVertexColor(redLaneVector, -0.02f, plane, 3.75f, r, g, b);
	pushVertexColor(redLaneVector, 0.02f, plane, 3.75f, r, g, b);
	pushVertexColor(redLaneVector, 0.02f, plane, 0.0f, r, g, b);
	pushVertexColor(redLaneVector, -0.02f, plane, 0.0f, r, g, b);
	pushRectangleIndices(redLaneIndices, redLaneVertexCount);

	int start = m_renderCross;
	for (size_t i = 0; i < ev.size(); i++) {
		if (ev.at(i).getMilli() <= time) {
			if (ev.at(i).getType() == CROSS_G) {
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

				pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, 3.75f, r, g, b);
				pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, 3.75f, r, g, b);

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

				pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, 3.75f, r, g, b);
				pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, 3.75f, r, g, b);
				start = 0;
				break;
			}
			else if (ev.at(i).getType() == CROSS_B) {
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

				pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, 3.75f, r, g, b);
				pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, 3.75f, r, g, b);

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

				pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, 3.75f, r, g, b);
				pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, 3.75f, r, g, b);
				start = 2;
				break;
			}
			else {
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
				pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, 3.75f, r, g, b);
				pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, 3.75f, r, g, b);

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
				pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, 3.75f, r, g, b);
				pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, 3.75f, r, g, b);
				start = 1;
				break;
			}
		}
	}

	int middle = start;
	float offset = 0.04f;

	for (size_t i = 0; i < ev.size(); i++) {
		if (ev.at(i).getRender()) {
			double dt = ev.at(i).getMilli() - time;
			if (ev.at(i).getType() == CROSS_G) {
				float z = 3.75f - 3.75f * dt;
				if (middle >= 1) {
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
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, z + offset, 0.0f, 1.0f, 0.0f);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, z - offset, r, g, b);
				}
				if (middle == 2) {
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
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, z + offset, r, g, b);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z + offset, r, g, b);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, z - offset, r, g, b);
				}
				middle = 0;
			}
			else if (ev.at(i).getType() == CROSS_B)
			{
				float z = 3.75f - 3.75f * dt;

				if (middle <= 1) {
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
					pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z + offset, r, g, b);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);


					pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z - offset, r, g, b);
				}
				if (middle == 0) {
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
					pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
				}
				middle = 2;
			}
			else {
				float z = 3.75 - 3.75 * dt;

				if (middle == 0) {
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
					pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z + offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, z + offset, r, g, b);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, z - offset, r, g, b);
				}
				else if (middle == 2) {
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
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, z + offset, r, g, b);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z + offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z + offset, r, g, b);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, z - offset, r, g, b);
					pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, z - offset, r, g, b);
				}
				middle = 1;
			}
		}
	}

	int end = 1;
	for (size_t i = 0; i < ev.size(); i++) {
		if (ev.at(i).getMilli() <= time + 1.0f) {
			if (ev.at(i).getType() == CROSS_G) {
				end = 0;
			}
			else if (ev.at(i).getType() == CROSS_B) {
				end = 2;
			}
			else {
				end = 1;
			}
		}
	}

	if (end == 0) {
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
		pushVertexColor(greenLaneVector, 0.02f - 0.7f, plane, 0.0f, r, g, b);
		pushVertexColor(greenLaneVector, -0.02f - 0.7f, plane, 0.0f, r, g, b);

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
		pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, 0.0f, r, g, b);
		pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, 0.0f, r, g, b);
	}
	else if (end == 2) {
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
		pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, 0.0f, r, g, b);
		pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, 0.0f, r, g, b);

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
		pushVertexColor(blueLaneVector, 0.02f + 0.7f, plane, 0.0f, r, g, b);
		pushVertexColor(blueLaneVector, -0.02f + 0.7f, plane, 0.0f, r, g, b);
	}
	else if (end == 1) {
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
		pushVertexColor(greenLaneVector, 0.02f - 0.35f, plane, 0.0f, r, g, b);
		pushVertexColor(greenLaneVector, -0.02f - 0.35f, plane, 0.0f, r, g, b);

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
		pushVertexColor(blueLaneVector, 0.02f + 0.35f, plane, 0.0f, r, g, b);
		pushVertexColor(blueLaneVector, -0.02f + 0.35f, plane, 0.0f, r, g, b);
	}

	m_renderCross = end;

	pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

	pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

	glBindVertexArray(m_colorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);

	glBufferData(GL_ARRAY_BUFFER, redLaneVector.size() * sizeof(float), redLaneVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, redLaneIndices.size() * sizeof(int), redLaneIndices.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_ColorProgram);
	glDrawElements(GL_TRIANGLES, redLaneIndices.size(), GL_UNSIGNED_INT, 0);

	glBufferData(GL_ARRAY_BUFFER, greenLaneVector.size() * sizeof(float), greenLaneVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, greenLaneIndices.size() * sizeof(int), greenLaneIndices.data(), GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLES, greenLaneIndices.size(), GL_UNSIGNED_INT, 0);

	glBufferData(GL_ARRAY_BUFFER, blueLaneVector.size() * sizeof(float), blueLaneVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, blueLaneIndices.size() * sizeof(int), blueLaneIndices.data(), GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLES, blueLaneIndices.size(), GL_UNSIGNED_INT, 0);
}

void Rendr::bpmTicks(double time, std::vector<double>& bpmArr)
{
	
	std::vector<float> bpmVector;
	std::vector<unsigned int> bpmIndices;
	unsigned int bpmVertexCount = 0;
	
	float plane = 0.0;
	float size = 0.1;

	float r = 0.3;
	float g = 0.3;
	float b = 0.3;
	
	for (size_t i = 0; i < bpmArr.size(); i++) {
		double tickTime = bpmArr.at(i);
		if (time + 1.0 >= tickTime && time <= tickTime + 0.2) {
			float dt = tickTime - time;
			float z = 3.75 - (3.75 * dt);
			pushVertexColor(bpmVector, -1.0, plane, z - size / 2, r, g, b);
			pushVertexColor(bpmVector, -1.0, plane, z + size / 2, r, g, b);
			pushVertexColor(bpmVector, 1.0, plane, z + size / 2, r, g, b);
			pushVertexColor(bpmVector, 1.0, plane, z - size / 2, r, g, b);
			pushRectangleIndices(bpmIndices, bpmVertexCount);
		}
	}

	glBindVertexArray(m_colorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);

	glBufferData(GL_ARRAY_BUFFER, bpmVector.size() * sizeof(float), bpmVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bpmIndices.size() * sizeof(int), bpmIndices.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_ColorProgram);

	glDrawElements(GL_TRIANGLES, bpmIndices.size(), GL_UNSIGNED_INT, 0);
}

void Rendr::events(double time, std::vector<Note>& ev) {

	float plane = 0.0;
	float transparency = 0.5;

	std::vector<float> eventsVector = {};
	std::vector<unsigned int> eventsIndices = {};
	unsigned int eventsVertexCount = 0;

	for (size_t i = 0; i < ev.size(); i++) {
		int type = ev.at(i).getType();
		double dt = ev.at(i).getMilli() - time;
		if (type == SCR_G_START) {
			double dt_end = -1;
			for (size_t j = i; j < ev.size(); j++) {
				if (ev.at(j).getType() == SCR_G_END) {
					dt_end = ev.at(j).getMilli() - time;
					break;
				}
			}
			if (dt_end != -1) {
				double ev_time = ev.at(i).getMilli();
				bool start_drawn = false;
				if (dt >= 0.0 && dt < 1.0) {
					float z = 3.75 - (3.75 * dt);
					if (m_renderCross == 0) {
						pushVertexColor(eventsVector, -0.85, plane, z, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.55, plane, z, 0.0f, 1.0f, 0.0f);
					}
					else {
						pushVertexColor(eventsVector, -0.5, plane, z, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.2, plane, z, 0.0f, 1.0f, 0.0f);
					}
					start_drawn = true;
				}
				else if (dt < 0.0) {
					if (m_renderCross == 0) {
						pushVertexColor(eventsVector, -0.85, plane, 3.75f, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.55, plane, 3.75f, 0.0f, 1.0f, 0.0f);
					}
					else {
						pushVertexColor(eventsVector, -0.5, plane, 3.75f, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.2, plane, 3.75f, 0.0f, 1.0f, 0.0f);
					}
					start_drawn = true;
				}
				//end				
				if (start_drawn) {
					if (dt_end > 0.0 && dt_end <= 1.0) {
						float z = 3.75 - (3.75 * dt_end);
						if (m_renderCross == 0) {
							pushVertexColor(eventsVector, -0.55, plane, z, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.85, plane, z, 0.0f, 1.0f, 0.0f);
						}
						else {
							pushVertexColor(eventsVector, -0.2, plane, z, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.5, plane, z, 0.0f, 1.0f, 0.0f);
						}
						pushRectangleIndices(eventsIndices, eventsVertexCount);
					}
					else if (dt_end > 1.0) {
						if (m_renderCross == 0) {
							pushVertexColor(eventsVector, -0.55, plane, 0.0f, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.85, plane, 0.0f, 0.0f, 1.0f, 0.0f);
						}
						else {
							pushVertexColor(eventsVector, -0.2, plane, 0.0f, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.5, plane, 0.0f, 0.0f, 1.0f, 0.0f);
						}
						pushRectangleIndices(eventsIndices, eventsVertexCount);
					}
				}
			}
		}
		else if (type == SCR_B_START) {
			double dt_end = -1;
			for (size_t j = i; j < ev.size(); j++) {
				if (ev.at(j).getType() == SCR_B_END) {
					dt_end = ev.at(j).getMilli() - time;
					break;
				}
			}
			if (dt_end != -1) {
				double ev_time = ev.at(i).getMilli();
				bool start_drawn = false;
				if (dt >= 0.0 && dt < 1.0) {
					float z = 3.75 - (3.75 * dt);
					if (m_renderCross == 2) {
						pushVertexColor(eventsVector, 0.55, plane, z, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.85, plane, z, 0.0f, 0.0f, 1.0f);
					}
					else {
						pushVertexColor(eventsVector, 0.2, plane, z, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.5, plane, z, 0.0f, 0.0f, 1.0f);
					}
					start_drawn = true;
				}
				else if (dt < 0.0) {
					if (m_renderCross == 2) {
						pushVertexColor(eventsVector, 0.55, plane, 3.75f, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.85, plane, 3.75f, 0.0f, 0.0f, 1.0f);
					}
					else {
						pushVertexColor(eventsVector, 0.2, plane, 3.75f, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.5, plane, 3.75f, 0.0f, 0.0f, 1.0f);
					}
					start_drawn = true;
				}
				//end				
				if (start_drawn) {
					if (dt_end > 0.0 && dt_end <= 1.0) {
						float z = 3.75 - (3.75 * dt_end);
						if (m_renderCross == 2) {
							pushVertexColor(eventsVector, 0.85, plane, z, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.55, plane, z, 0.0f, 0.0f, 1.0f);
						}
						else {
							pushVertexColor(eventsVector, 0.5, plane, z, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.2, plane, z, 0.0f, 0.0f, 1.0f);
						}
						pushRectangleIndices(eventsIndices, eventsVertexCount);
					}
					else if (dt_end > 1.0) {
						if (m_renderCross == 2) {
							pushVertexColor(eventsVector, 0.85, plane, 0.0f, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.55, plane, 0.0f, 0.0f, 0.0f, 1.0f);
						}
						else {
							pushVertexColor(eventsVector, 0.5, plane, 0.0f, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.2, plane, 0.0f, 0.0f, 0.0f, 1.0f);
						}
						pushRectangleIndices(eventsIndices, eventsVertexCount);
					}
				}
			}
		}
		else if (type == EU_START) {
		double dt_end = -1;
		for (size_t j = i; j < ev.size(); j++) {
			if (ev.at(j).getType() == EU_END) {
				dt_end = ev.at(j).getMilli() - time;
				break;
			}
		}
		if (dt_end != -1) {
			double ev_time = ev.at(i).getMilli();
			bool start_eu = false;
			if (dt >= 0.0 && dt < 1.0) {
				float z = 3.75 - (3.75 * dt);
				pushVertexColor(eventsVector, -1.0, plane, z, 1.0, 1.0, 1.0, transparency);
				pushVertexColor(eventsVector, 1.0, plane, z, 1.0, 1.0, 1.0, transparency);
				start_eu = true;
			}
			else if(dt < 0.0){
				if (m_renderEuZone) {
					pushVertexColor(eventsVector, -1.0, plane, 3.75, 1.0, 1.0, 1.0, transparency);
					pushVertexColor(eventsVector, 1.0, plane, 3.75, 1.0, 1.0, 1.0, transparency);
					start_eu = true;
				}
			}
			if (start_eu) {
				if (dt_end >= 0.0 && dt_end < 1.0) {
					float z = 3.75 - (3.75 * dt_end);
					pushVertexColor(eventsVector, 1.0, plane, z, 1.0, 1.0, 1.0, transparency);
					pushVertexColor(eventsVector, -1.0, plane, z, 1.0, 1.0, 1.0, transparency);
					
				}
				else {
					pushVertexColor(eventsVector, 1.0, plane, 0.0, 1.0, 1.0, 1.0, transparency);
					pushVertexColor(eventsVector, -1.0, plane, 0.0, 1.0, 1.0, 1.0, transparency);
				}
				pushRectangleIndices(eventsIndices, eventsVertexCount);
			}
		}
}
	}
	glBindVertexArray(m_colorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);

	glBufferData(GL_ARRAY_BUFFER, eventsVector.size() * sizeof(float), eventsVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, eventsIndices.size() * sizeof(int), eventsIndices.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_ColorProgram);
	glDrawElements(GL_TRIANGLES, eventsIndices.size(), GL_UNSIGNED_INT, 0);
}

void Rendr::meters() {

	float plane = 0.1f;

	std::vector<float>metersVector;
	std::vector<unsigned int> metersIndices;
	unsigned int metersVertexCount = 0;

	//combo meter
	if (m_playerMult == 2) {
		pushVertexTexture(metersVector, 1.0, plane, 2.5, 0.0, 800.0/1200.0);
		pushVertexTexture(metersVector, 1.0, plane, 2.7, 0.0, 400.0/1200.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.7, 400.0 / 1000.0, 400.0/1200.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.5, 400.0 / 1000.0, 800.0/1200.0);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 3) {
		pushVertexTexture(metersVector, 1.0, plane, 2.5, 400.0/1000.0, 800.0 / 1200.0);
		pushVertexTexture(metersVector, 1.0, plane, 2.7, 400.0/1000.0, 400.0 / 1200.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.7, 800.0/1000.0, 400.0 / 1200.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.5, 800.0/1000.0, 800.0 / 1200.0);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 4) {
		pushVertexTexture(metersVector, 1.0, plane, 2.5, 0.0, 400.0 / 1200.0);
		pushVertexTexture(metersVector, 1.0, plane, 2.7, 0.0, 0.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.7, 400.0 / 1000.0, 0.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.5, 400.0 / 1000.0, 400.0 / 1200.0);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 6) {
		pushVertexTexture(metersVector, 1.0, plane, 2.5, 0.0, 1.0);
		pushVertexTexture(metersVector, 1.0, plane, 2.7, 0.0, 800.0 / 1200.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.7, 400.0 / 1000.0, 800.0 / 1200.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.5, 400.0 / 1000.0, 1.0);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 8) {
		pushVertexTexture(metersVector, 1.0, plane, 2.5, 400.0 / 1000.0, 1.0);
		pushVertexTexture(metersVector, 1.0, plane, 2.7, 400.0 / 1000.0, 800.0 / 1200.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.7, 800.0 / 1000.0, 800.0 / 1200.0);
		pushVertexTexture(metersVector, 1.2, plane, 2.5, 800.0 / 1000.0, 1.0);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}

	for (int i = 0; i < 8; i++) {
		if (m_playerCombo == 0) {
			pushVertexTexture(metersVector, 1.0, plane, 3.6f - 0.11 * i, 400.0 / 1000.0, 400.0/1200.0);
			pushVertexTexture(metersVector, 1.0, plane, 3.7f - 0.11 * i, 400.0 / 1000.0, 200.0/1200.0);
			pushVertexTexture(metersVector, 1.2, plane, 3.7f - 0.11 * i, 800.0 / 1000.0, 200.0/1200.0);
			pushVertexTexture(metersVector, 1.2, plane, 3.6f - 0.11 * i, 800.0 / 1000.0, 400.0/1200.0);
			pushRectangleIndices(metersIndices, metersVertexCount);
		}
		else if (m_playerCombo >= 24) {
			pushVertexTexture(metersVector, 1.0, plane, 3.6f - 0.11 * i, 400.0 / 1000.0, 200.0/1200.0);
			pushVertexTexture(metersVector, 1.0, plane, 3.7f - 0.11 * i, 400.0 / 1000.0, 0.0);
			pushVertexTexture(metersVector, 1.2, plane, 3.7f - 0.11 * i, 800.0 / 1000.0, 0.0);
			pushVertexTexture(metersVector, 1.2, plane, 3.6f - 0.11 * i, 800.0 / 1000.0, 200.0/1200.0);
			pushRectangleIndices(metersIndices, metersVertexCount);
		}
		else {
			int limit = m_playerCombo % 8;
			if (limit == 0)limit = 9;
			if (i < limit) {
				pushVertexTexture(metersVector, 1.0, plane, 3.6f - 0.11 * i, 400.0 / 1000.0, 200.0/1200.0);
				pushVertexTexture(metersVector, 1.0, plane, 3.7f - 0.11 * i, 400.0 / 1000.0, 0.0);
				pushVertexTexture(metersVector, 1.2, plane, 3.7f - 0.11 * i, 800.0 / 1000.0, 0.0);
				pushVertexTexture(metersVector, 1.2, plane, 3.6f - 0.11 * i, 800.0 / 1000.0, 200.0/1200.0);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
			else {
				pushVertexTexture(metersVector, 1.0, plane, 3.6f - 0.11 * i, 400.0 / 1000.0, 400.0/1200.0);
				pushVertexTexture(metersVector, 1.0, plane, 3.7f - 0.11 * i, 400.0 / 1000.0, 200.0/1200.0);
				pushVertexTexture(metersVector, 1.2, plane, 3.7f - 0.11 * i, 800.0 / 1000.0, 200.0/1200.0);
				pushVertexTexture(metersVector, 1.2, plane, 3.6f - 0.11 * i, 800.0 / 1000.0, 400.0/1200.0);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
		}
	}

	//euphoria meter

	for (int i = 0; i < 3; i++) {
		pushVertexTexture(metersVector, -1.1, plane, 3.75-0.35*i, 800.0 / 1000.0, 400.0/1200.0);
		pushVertexTexture(metersVector, -1.0, plane, 3.75-0.35*i, 1.0, 400.0/1200.0);
		pushVertexTexture(metersVector, -1.0, plane, 3.4-0.35*i, 1.0, 800.0/1200.0);
		pushVertexTexture(metersVector, -1.1, plane, 3.4-0.35*i, 800.0 / 1000.0, 800.0 / 1200.0);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}

	if (m_renderEuValue > 0.0 && m_renderEuValue < 1.0) {
		float z = m_renderEuValue;
		pushVertexTexture(metersVector, -1.1, plane, 3.75, 800.0 / 1000.0, 0.0);
		pushVertexTexture(metersVector, -1.0, plane, 3.75, 1.0, 0.0);
		pushVertexTexture(metersVector, -1.0, plane, 3.75 - 0.35 * z, 1.0, 400.0/1200.0);
		pushVertexTexture(metersVector, -1.1, plane, 3.75 - 0.35 * z, 800.0 / 1000.0, 400.0/1200.0);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_renderEuValue >= 1.0) {
		pushVertexTexture(metersVector, -1.1, plane, 3.75, 800.0 / 1000.0, 0.0);
		pushVertexTexture(metersVector, -1.0, plane, 3.75, 1.0, 0.0);
		pushVertexTexture(metersVector, -1.0, plane, 3.4, 1.0, 400.0/1200.0);
		pushVertexTexture(metersVector, -1.1, plane, 3.4, 800.0 / 1000.0, 400.0/1200.0);
		pushRectangleIndices(metersIndices, metersVertexCount);
		if (m_renderEuValue < 2.0) {
			float z = m_renderEuValue - 1.0;
			pushVertexTexture(metersVector, -1.1, plane, 3.4, 800.0 / 1000.0, 0.0);
			pushVertexTexture(metersVector, -1.0, plane, 3.4, 1.0, 0.0);
			pushVertexTexture(metersVector, -1.0, plane, 3.4-0.35*z, 1.0, 400.0/1200.0);
			pushVertexTexture(metersVector, -1.1, plane, 3.4-0.35*z, 800.0 / 1000.0, 400.0/1200.0);
			pushRectangleIndices(metersIndices, metersVertexCount);
		}
		else if (m_renderEuValue >= 2.0) {
			pushVertexTexture(metersVector, -1.1, plane, 3.4, 800.0 / 1000.0, 0.0);
			pushVertexTexture(metersVector, -1.0, plane, 3.4, 1.0, 0.0);
			pushVertexTexture(metersVector, -1.0, plane, 3.05, 1.0, 400.0/1200.0);
			pushVertexTexture(metersVector, -1.1, plane, 3.05, 800.0 / 1000.0, 400.0/1200.0);
			pushRectangleIndices(metersIndices, metersVertexCount);
			if (m_renderEuValue < 3.0) {
				float z = m_renderEuValue - 2.0;
				pushVertexTexture(metersVector, -1.1, plane, 3.05, 800.0 / 1000.0, 0.0);
				pushVertexTexture(metersVector, -1.0, plane, 3.05, 1.0, 0.0);
				pushVertexTexture(metersVector, -1.0, plane, 3.05 - 0.35 * z, 1.0, 400.0/1200.0);
				pushVertexTexture(metersVector, -1.1, plane, 3.05 - 0.35 * z, 800.0 / 1000.0, 400.0/1200.0);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
			else {
				pushVertexTexture(metersVector, -1.1, plane, 3.05, 800.0 / 1000.0, 0.0);
				pushVertexTexture(metersVector, -1.0, plane, 3.05, 1.0, 0.0);
				pushVertexTexture(metersVector, -1.0, plane, 2.7, 1.0, 400.0/1200.0);
				pushVertexTexture(metersVector, -1.1, plane, 2.7, 800.0 / 1000.0, 400.0/1200.0);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
		}
	}
	

	glBindVertexArray(m_textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);

	glBufferData(GL_ARRAY_BUFFER, metersVector.size() * sizeof(float), metersVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, metersIndices.size() * sizeof(int), metersIndices.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_TextureProgram);

	glBindTexture(GL_TEXTURE_2D, m_MetersTexture);
	glDrawElements(GL_TRIANGLES, metersIndices.size(), GL_UNSIGNED_INT, 0);



}

void Rendr::splash() {
	std::vector<float> splashVector;
	std::vector<unsigned int> splashIndices;
	unsigned int splashVertexCount = 0;

	pushVertexTexture(splashVector, -0.5, 0.5, 3.5, 0.0, 0.0);
	pushVertexTexture(splashVector, -0.5, 1.5, 3.5, 0.0, 1.0);
	pushVertexTexture(splashVector, 0.5, 1.5, 3.5, 1.0, 1.0);
	pushVertexTexture(splashVector, 0.5, 0.5, 3.5, 1.0, 0.0);
	pushRectangleIndices(splashIndices, splashVertexCount);

	glBindVertexArray(m_textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);

	glBufferData(GL_ARRAY_BUFFER, splashVector.size() * sizeof(float), splashVector.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, splashIndices.size() * sizeof(int), splashIndices.data(), GL_STATIC_DRAW);

	glUseProgram(m_TextureProgram);
	glBindTexture(GL_TEXTURE_2D, m_splashTexture);
	glDrawElements(GL_TRIANGLES, splashIndices.size(), GL_UNSIGNED_INT, 0);
}

void Rendr::pollState(double time, Player& p, Generator& g) {
	m_red = p.getRedClicker();
	m_blue = p.getBlueClicker();
	m_green = p.getGreenClicker();
	m_playerCross = p.getCross();
	m_playerCombo = p.getCombo();
	m_playerMult = p.getMult();
	m_renderEuValue = p.getEuValue();
	m_renderEuActive = p.getEuActive();
	m_renderEuZone = p.getEuZoneActive();
}


Rendr::~Rendr() {
	//dtor
}
