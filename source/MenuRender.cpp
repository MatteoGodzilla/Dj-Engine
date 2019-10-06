#include "MenuRender.h"

struct Glyph {
	GLuint TextureID;
	float bx;
	float by;
	float width;
	float height;
	float advance;
};

std::map<char, Glyph> ChMap;

void checkErr() {
	std::cout << "started error checking" << std::endl;
	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		std::cout << "OpenGL Error: " << error << std::endl;
		error = glGetError();
	}
	std::cout << "ended error checking" << std::endl;
}

void MenuRender::pushVertexTexture(std::vector<float>& v, float x, float y, float z, float s, float t) {
	v.push_back(x);
	v.push_back(y);
	v.push_back(z);
	v.push_back(s);
	v.push_back(t);
}

void MenuRender::pushRectangleIndices(std::vector<unsigned int>& v, unsigned int& value) {
	v.push_back(value);
	v.push_back(value + 1);
	v.push_back(value + 2);
	v.push_back(value + 2);
	v.push_back(value + 3);
	v.push_back(value);
	value += 4;
}

void MenuRender::renderTexture(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) {
	glBindVertexArray(m_textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float), vertexArr.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * sizeof(int), indexArr.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_textureProgram);
	glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MenuRender::renderText(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) {
	glBindVertexArray(m_textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float), vertexArr.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * sizeof(int), indexArr.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_textProgram);
	glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MenuRender::usePersProj() {
	//settung up projection uniform
	glUseProgram(m_textureProgram);
	int location = glGetUniformLocation(m_textureProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_persProj[0][0]);
	}
	else {
		std::cerr << "error setting projection matrix" << std::endl;
	}

	glUseProgram(m_textProgram);
	location = glGetUniformLocation(m_textProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_persProj[0][0]);
	}
	else {
		std::cerr << "error setting projection matrix" << std::endl;
	}
}

void MenuRender::useOrthoProj() {
	//settung up projection uniform
	glUseProgram(m_textureProgram);
	int location = glGetUniformLocation(m_textureProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_orthoProj[0][0]);
	}
	else {
		std::cerr << "error setting projection matrix" << std::endl;
	}

	glUseProgram(m_textProgram);
	location = glGetUniformLocation(m_textProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_orthoProj[0][0]);
	}
	else {
		std::cerr << "error setting projection matrix" << std::endl;
	}
}

void MenuRender::drawText(const char* text, float x, float y, float scl)
{
	int i = 0;
	char c = text[i];
	while (c != '\0') {
		if (c < 128) {
			Glyph temp = ChMap[c];
			std::vector<float> textVector;
			std::vector<unsigned int> textIndices;
			unsigned int textVertexCount = 0;

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
		else {
			std::cerr << "ERROR:Char not supported" << std::endl;
		}
		i++;
		c = text[i];
	}
}

void MenuRender::setTextColor(float r, float g, float b, float a) {
	std::cout << "Setting up color" << std::endl;
	glUseProgram(m_textProgram);
	int location = glGetUniformLocation(m_textProgram, "u_textColor");
	if (location != -1) {
		glUniform4f(location, r, g, b, a);
	}
	else {
		std::cerr << "error setting text color" << std::endl;
	}
}

MenuRender::MenuRender()
{
}

void MenuRender::init(GLFWwindow* w)
{
	std::cout << "MenuRender init:" << std::endl;

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

		const char* fTextSource = "\n"
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec2 tex_coords;\n"
			"\n"
			"uniform sampler2D u_t;\n"
			"uniform vec4 u_textColor;\n"
			"\n"
			"void main()"
			"{\n"
			"	vec4 sample = vec4(1.0f,1.0f,1.0f,texture(u_t, tex_coords).r);\n"
			"	FragColor = u_textColor * sample;\n"
			"}\n";


		unsigned int vShaderTexture, fShaderTexture;
		unsigned int fShaderText;

		vShaderTexture = glCreateShader(GL_VERTEX_SHADER);
		fShaderTexture = glCreateShader(GL_FRAGMENT_SHADER);
		fShaderText = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vShaderTexture, 1, &vTextureSource, NULL);
		glShaderSource(fShaderTexture, 1, &fTextureSource, NULL);
		glShaderSource(fShaderText, 1, &fTextSource, NULL);

		glCompileShader(vShaderTexture);
		glCompileShader(fShaderTexture);
		glCompileShader(fShaderText);


		int success;
		char infolog[512];
		glGetShaderiv(fShaderText, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fShaderText, 512, NULL, infolog);
			std::cerr << "error compiling shader" << infolog << std::endl;
		}

		m_textureProgram = glCreateProgram();
		glAttachShader(m_textureProgram, vShaderTexture);
		glAttachShader(m_textureProgram, fShaderTexture);
		glLinkProgram(m_textureProgram);

		m_textProgram = glCreateProgram();
		glAttachShader(m_textProgram, vShaderTexture);
		glAttachShader(m_textProgram, fShaderText);
		glLinkProgram(m_textProgram);


		int linked;
		char log[512];
		glGetProgramiv(m_textProgram, GL_LINK_STATUS, &linked);
		if (!linked) {
			glGetProgramInfoLog(m_textProgram, 512, NULL, log);
			std::cerr << "error linking program:" << log << std::endl;
		}


		glDeleteShader(vShaderTexture);
		glDeleteShader(fShaderTexture);
		glDeleteShader(fShaderText);
	}



	//projection init
	{
		//m_persProj = main perspective projection
		m_persProj = glm::perspective(45.0f, 1024.0f / 600, 1.0f, -2.0f);
		glm::mat4 look = glm::lookAt(glm::vec3(0.0, 2.0, 5.0),
			glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 1.0, 0.0));
		m_persProj = m_persProj * look;

		m_orthoProj = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 1.0f, -1.0f);

		usePersProj();
	}

	//vao setup
	{
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

	}

	//freetype init
	{
		if (FT_Init_FreeType(&m_FTLibrary)) {
			std::cerr << "error opening freetype" << std::endl;
		}
		if (FT_New_Face(m_FTLibrary, "res/NotoSans-Regular.ttf", 0, &m_font)) {
			std::cerr << "error loading font" << std::endl;
		}
		FT_Set_Pixel_Sizes(m_font, 0, 1024);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(m_font, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph:" << c << std::endl;

			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_font->glyph->bitmap.width, m_font->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE, m_font->glyph->bitmap.buffer);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Glyph character = {
				texture,
				(float)m_font->glyph->bitmap_left,
				(float)m_font->glyph->bitmap_top,
				(float)m_font->glyph->bitmap.width,
				(float)m_font->glyph->bitmap.rows,
				(float)m_font->glyph->advance.x
			};
			ChMap.insert(std::pair<char, Glyph>(c, character));

		}
		FT_Done_Face(m_font);
		FT_Done_FreeType(m_FTLibrary);


	}

	setTextColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MenuRender::render()
{
	useOrthoProj();
	drawText("Hello World!", 10.0, 400.0f, 0.1);
}

MenuRender::~MenuRender()
{
}
