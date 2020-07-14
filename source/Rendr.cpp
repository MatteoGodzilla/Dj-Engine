#include "Rendr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//character struct to store freetype glyph data
struct CharTextureData {
	GLuint TextureID;
	float bx;
	float by;
	float width;
	float height;
	float advance;
};

std::map<char, CharTextureData> ChMap;

void Rendr::checkError() {
	std::cout << "started error checking" << std::endl;
	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		std::cout << "OpenGL Error: " << error << std::endl;
		error = glGetError();
	}
	std::cout << "ended error checking" << std::endl;
}

//utility function
void Rendr::pushVertexColor(std::vector<float>& v, float x, float y, float z, float r, float g, float b, float a, float s, float t) const {
	if (rendr_InvertedX) {
		v.push_back(-x);
	} else {
		v.push_back(x);
	}
	v.push_back(y);
	v.push_back(z);
	v.push_back(r);
	v.push_back(g);
	v.push_back(b);
	v.push_back(a);
	v.push_back(s);
	v.push_back(t);
}

//utility function
void Rendr::pushVertexTexture(std::vector<float>& v, float x, float y, float z, float s, float t, float r, float g, float b, float a) const {
	if (rendr_InvertedX) {
		v.push_back(-x);
	} else {
		v.push_back(x);
	}
	v.push_back(y);
	v.push_back(z);
	v.push_back(r);
	v.push_back(g);
	v.push_back(b);
	v.push_back(a);
	v.push_back(s);
	v.push_back(t);
}

void Rendr::pushVertex(std::vector<float>& v, glm::vec3 pos, glm::vec4 color, glm::vec2 texCoords) const {
	if (rendr_InvertedX) {
		v.push_back(-pos.x);
	} else {
		v.push_back(pos.x);
	}
	v.push_back(pos.y);
	v.push_back(pos.z);
	v.push_back(color.r);
	v.push_back(color.g);
	v.push_back(color.b);
	v.push_back(color.a);
	v.push_back(texCoords.x);
	v.push_back(texCoords.y);
}

//utility function
void Rendr::pushRectangleIndices(std::vector<unsigned int>& v, unsigned int& value) {
	v.push_back(value);
	v.push_back(value + 1);
	v.push_back(value + 2);
	v.push_back(value + 2);
	v.push_back(value + 3);
	v.push_back(value);
	value += 4;
}

void Rendr::pushTriangleIndices(std::vector<unsigned int>& v, unsigned int& value) {
	v.push_back(value);
	v.push_back(value + 1);
	v.push_back(value + 2);
	value += 3;
}

void Rendr::usePersProj() {
	//setting up projection uniform on all programs
	glUseProgram(m_mainProgram);
	int location = glGetUniformLocation(m_mainProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_persProj[0][0]);
	} else {
		std::cerr << "error setting projection matrix" << std::endl;
	}

	glUseProgram(m_mainProgram);
	location = glGetUniformLocation(m_mainProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_persProj[0][0]);
	} else {
		std::cerr << "error setting projection matrix" << std::endl;
	}

	glUseProgram(m_textProgram);
	location = glGetUniformLocation(m_textProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_persProj[0][0]);
	} else {
		std::cerr << "error setting projection matrix" << std::endl;
	}
}

void Rendr::useOrthoProj() {
	//setting up projection uniform
	glUseProgram(m_mainProgram);
	int location = glGetUniformLocation(m_mainProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_orthoProj[0][0]);
	} else {
		std::cerr << "error setting projection matrix" << std::endl;
	}

	glUseProgram(m_mainProgram);
	location = glGetUniformLocation(m_mainProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_orthoProj[0][0]);
	} else {
		std::cerr << "error setting projection matrix" << std::endl;
	}

	glUseProgram(m_textProgram);
	location = glGetUniformLocation(m_textProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_orthoProj[0][0]);
	} else {
		std::cerr << "error setting projection matrix" << std::endl;
	}
}

void Rendr::renderTexture(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) const {
	//bind texture VAO (layout and actual buffers)
	glBindVertexArray(m_mainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_mainVBO);
	glBindTexture(GL_TEXTURE_2D, texture);

	//upload data on buffers
	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float), vertexArr.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * sizeof(int), indexArr.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_mainProgram);
	glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Rendr::renderText(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) const {
	//bind text VAO (layout and actual buffers)
	glBindVertexArray(m_mainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_mainVBO);
	glBindTexture(GL_TEXTURE_2D, texture);

	//upload data on buffers
	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float), vertexArr.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * sizeof(int), indexArr.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_textProgram);
	glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Rendr::renderColor(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr) const {
	//bind color VAO (layout and actual buffers)
	glBindVertexArray(m_mainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_mainVBO);

	glBindTexture(GL_TEXTURE_2D, m_colorTexture);

	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float), vertexArr.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * sizeof(int), indexArr.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_mainProgram);
	glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void Rendr::drawText(const std::string& s, float x, float y, float scl) {
	bool old = rendr_InvertedX;
	rendr_InvertedX = false;
	//loop for every char in string
	float maxBearing = 0.0f;
	for (char c : s) {
		CharTextureData temp = ChMap[c];
		temp.by *= scl;
		if (temp.by > maxBearing) {
			maxBearing = temp.by;
		}
	}
	y += maxBearing;

	for (char c : s) {
		CharTextureData temp = ChMap[c];
		std::vector<float> textVector;
		std::vector<unsigned int> textIndices;
		unsigned int textVertexCount = 0;

		//scale by scl every value inside temp (Character utility struct)
		temp.bx *= scl;
		temp.by *= scl;
		temp.width *= scl;
		temp.height *= scl;
		temp.advance *= scl;

		pushVertexTexture(textVector, x + temp.bx, y - temp.by, 0.0f, 0.0f, 0.0f);
		pushVertexTexture(textVector, x + temp.bx, y - temp.by + temp.height, 0.0f, 0.0f, 1.0f);
		pushVertexTexture(textVector, x + temp.bx + temp.width, y - temp.by + temp.height, 0.0f, 1.0f, 1.0f);
		pushVertexTexture(textVector, x + temp.bx + temp.width, y - temp.by, 0.0f, 1.0f, 0.0f);
		pushRectangleIndices(textIndices, textVertexCount);

		useOrthoProj();
		renderText(textVector, textIndices, temp.TextureID);
		x += temp.advance / 64;
	}
	rendr_InvertedX = old;
}

float Rendr::getTextWidth(const std::string& s, float scale) {
	float x = 0.0f; // return variable
	for (char c : s) {
		CharTextureData temp = ChMap[c];
		temp.advance *= scale;
		x += temp.advance / 64;
	}
	return x;
}

float Rendr::getTextHeight(const std::string& s, float scale) {
	float maxBearing = 0;
	float y = 0.0f;
	for (char c : s) {
		CharTextureData temp = ChMap[c];
		temp.by *= scale;
		if (temp.by > maxBearing) {
			maxBearing = temp.by;
		}
	}
	//std::cout << maxBearing << std::endl;
	for (char c : s) {
		CharTextureData temp = ChMap[c];
		temp.height *= scale;
		temp.by *= scale;
		float bottom = maxBearing - temp.by + temp.height;
		if (bottom > y) {
			y = bottom;
		}
	}
	return y;
}

glm::vec2 Rendr::loadTexture(const std::string& s, unsigned int* destination) {
	int width;
	int height;
	int channels;

	//using stb_image to actually load textures
	unsigned char* data = stbi_load(s.c_str(), &width, &height, &channels, 0);

	if (data != nullptr) {
		//create texture object on graphics card
		glGenTextures(1, destination);
		glBindTexture(GL_TEXTURE_2D, *destination);
		//set parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//upload texture data
		if (channels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		std::cout << "Rendr Msg: successfully loaded texture at " << s << std::endl;
		stbi_image_free(data);
		return glm::vec2(width, height);
	} else {
		std::cerr << "Rendr Err: cannot load texture. Does the file at " << s << " exist?" << std::endl;
		return glm::vec2(0.0, 0.0);
	}
}

void Rendr::startImGuiFrame(const std::string& winName, ImGuiBackendFlags flags) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(winName.c_str(), nullptr, flags);
}

void Rendr::renderImGuiFrame() {
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Rendr::init(GLFWwindow* w) {
	stbi_set_flip_vertically_on_load(true);

	m_window = w;
	glfwMakeContextCurrent(m_window);
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW INIT ERROR" << std::endl;
		return;
	}
	std::cout << "Rendr Init: " << glGetString(GL_VERSION) << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//shader init
	{
		const char* vMainSource =
			"\n"
			"#version 330\n"
			"layout(location = 0) in vec4 aPos;\n"
			"layout(location = 1) in vec4 c;\n"
			"layout(location = 2) in vec2 texCoords;\n"
			"uniform mat4 u_proj;\n"
			"out vec4 color;\n"
			"out vec2 tCoord;\n"
			"\n"
			"void main()\n"
			"{\n"
			"	gl_Position = u_proj * aPos;\n"
			"	color = c;\n"
			"	tCoord = texCoords;\n"
			"}";

		const char* fMainSource =
			"\n"
			"#version 330\n"
			"out vec4 FragColor;\n"
			"in vec4 color;\n"
			"in vec2 tCoord;\n"
			"\n"
			"uniform sampler2D u_t;\n"
			"\n"
			"void main()"
			"{\n"
			"	FragColor = texture(u_t,tCoord) * color;\n"
			"}\n";

		const char* fTextSource =
			"\n"
			"#version 330\n"
			"out vec4 FragColor;\n"
			"in vec4 color;\n"
			"in vec2 tCoord;\n"
			"\n"
			"uniform sampler2D u_t;\n"
			"\n"
			"void main()"
			"{\n"
			"	vec4 sample = vec4(1.0f,1.0f,1.0f,texture(u_t, tCoord).r);\n"
			"	FragColor = color * sample;\n"
			"}\n";

		//shader ids
		unsigned int vMainShader;
		unsigned int fMainShader;
		unsigned int fTextShader;

		//create shaders
		vMainShader = glCreateShader(GL_VERTEX_SHADER);
		fMainShader = glCreateShader(GL_FRAGMENT_SHADER);
		fTextShader = glCreateShader(GL_FRAGMENT_SHADER);

		//upload shader source data (see above)
		glShaderSource(vMainShader, 1, &vMainSource, nullptr);
		glShaderSource(fMainShader, 1, &fMainSource, nullptr);
		glShaderSource(fTextShader, 1, &fTextSource, nullptr);

		//compile shaders (hoping that they don't fail)
		glCompileShader(vMainShader);
		glCompileShader(fMainShader);
		glCompileShader(fTextShader);

		//example compile status check
		int success;
		std::array<char, 512> infolog;
		glGetShaderiv(vMainShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vMainShader, 512, nullptr, infolog.data());
			std::cerr << "error compiling vertex main shader:" << infolog.data() << std::endl;
		}
		glGetShaderiv(fMainShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fMainShader, 512, nullptr, infolog.data());
			std::cerr << "error compiling fragment main shader:" << infolog.data() << std::endl;
		}
		glGetShaderiv(fTextShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fTextShader, 512, nullptr, infolog.data());
			std::cerr << "error compiling fragment text shader:" << infolog.data() << std::endl;
		}
		//create shader programs
		m_mainProgram = glCreateProgram();
		glAttachShader(m_mainProgram, vMainShader);
		glAttachShader(m_mainProgram, fMainShader);
		glLinkProgram(m_mainProgram);

		m_textProgram = glCreateProgram();
		glAttachShader(m_textProgram, vMainShader);
		glAttachShader(m_textProgram, fTextShader);
		glLinkProgram(m_textProgram);

		//example program linking check
		int linked;
		std::array<char, 512> log;
		glGetProgramiv(m_mainProgram, GL_LINK_STATUS, &linked);
		if (!linked) {
			glGetProgramInfoLog(m_textProgram, 512, nullptr, log.data());
			std::cerr << "error linking program:" << log.data() << std::endl;
		}

		//delete shader object files
		glDeleteShader(vMainShader);
		glDeleteShader(fMainShader);
		glDeleteShader(fTextShader);
	}

	//projection init
	{
		//m_persProj = main perspective projection
		auto angle = glm::quarter_pi<float>();

		float zOffset = -1.160f;

		m_persProj = glm::perspective(45.0f, 1024.0f / 600, 1.0f, -2.0f);
		glm::mat4 look = glm::lookAt(
			glm::vec3(0.0, sin(angle) * 4.0, cos(angle) * 4.0 + zOffset),
			glm::vec3(0.0, 0.0, -2.0 + zOffset),
			glm::vec3(0.0, 1.0, 0.0));
		m_persProj = m_persProj * look;

		m_orthoProj = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 1.0f, -1.0f);

		usePersProj();
	}

	//vao setup
	{
		//create VAO
		glGenVertexArrays(1, &m_mainVAO);

		//color vao
		{
			glBindVertexArray(m_mainVAO);

			//index buffer
			unsigned int index;
			glGenBuffers(1, &m_mainVBO);
			glGenBuffers(1, &index);

			glBindBuffer(GL_ARRAY_BUFFER, m_mainVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

			//Vertex: x,y,z, r,g,b,a, s,t
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}

	//freetype init
	{
		if (FT_Init_FreeType(&m_FTLibrary)) {
			std::cerr << "Rendr error:error opening freetype" << std::endl;
		}
		if (FT_New_Face(m_FTLibrary, "res/NotoSans-Regular.ttf", 0, &m_font)) {
			std::cerr << "Rendr error:error loading font" << std::endl;
		}
		FT_Set_Pixel_Sizes(m_font, 0, 1024);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 128; c++) {
			// Load character glyph
			if (FT_Load_Char(m_font, c, FT_LOAD_RENDER)) {
				std::cout << "Rendr error::FREETYTPE: Failed to load Glyph:" << c << std::endl;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_font->glyph->bitmap.width, m_font->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE, m_font->glyph->bitmap.buffer);
			// set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// store Character data for later use
			CharTextureData character = {
				texture,
				(float)m_font->glyph->bitmap_left,
				(float)m_font->glyph->bitmap_top,
				(float)m_font->glyph->bitmap.width,
				(float)m_font->glyph->bitmap.rows,
				(float)m_font->glyph->advance.x};
			ChMap.insert(std::pair<char, CharTextureData>(c, character));
		}
		FT_Done_Face(m_font);
		FT_Done_FreeType(m_FTLibrary);
	}

	//basic white texture creation
	{
		std::array<char, 3> data = {char(0xff), char(0xff), char(0xff)};

		glGenTextures(1, &m_colorTexture);
		glBindTexture(GL_TEXTURE_2D, m_colorTexture);
		//set parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	}
}

GLFWwindow* Rendr::getWindowPtr() {
	return m_window;
}
