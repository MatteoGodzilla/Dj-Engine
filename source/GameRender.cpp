#include "GameRender.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

struct Character {
	GLuint TextureID;
	float bx;
	float by;
	float width;
	float height;
	float advance;
};

std::map<char, Character> ChMap;

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

void GameRender::usePersProj() {
	//settung up projection uniform
	glUseProgram(m_textureProgram);
	int location = glGetUniformLocation(m_textureProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_persProj[0][0]);
	}
	else {
		std::cerr << "error setting projection matrix" << std::endl;
	}

	glUseProgram(m_colorProgram);
	location = glGetUniformLocation(m_colorProgram, "u_proj");
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

void GameRender::useOrthoProj() {
	//settung up projection uniform
	glUseProgram(m_textureProgram);
	int location = glGetUniformLocation(m_textureProgram, "u_proj");
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, &m_orthoProj[0][0]);
	}
	else {
		std::cerr << "error setting projection matrix" << std::endl;
	}

	glUseProgram(m_colorProgram);
	location = glGetUniformLocation(m_colorProgram, "u_proj");
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

void GameRender::setTextColor(float r, float g, float b, float a) {
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

void GameRender::renderTexture(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) {
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

void GameRender::renderText(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) {
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

void GameRender::renderColor(std::vector<float>& vertexArr,std::vector<unsigned int>& indexArr) {
	glBindVertexArray(m_colorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);

	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float), vertexArr.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * sizeof(int), indexArr.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_colorProgram);
	glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void GameRender::drawText(const char* text, float x, float y, float scl)
{
	int i = 0;
	char c = text[i];
	while (c != '\0') {
		if (c < 128) {
			Character temp = ChMap[c];
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

GameRender::GameRender() {
	//ctor
}

void GameRender::init(GLFWwindow* w) {
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
		unsigned int vShaderColor, fShaderColor;
		unsigned int fShaderText;

		vShaderTexture = glCreateShader(GL_VERTEX_SHADER);
		fShaderTexture = glCreateShader(GL_FRAGMENT_SHADER);
		vShaderColor = glCreateShader(GL_VERTEX_SHADER);
		fShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
		fShaderText = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vShaderTexture, 1, &vTextureSource, NULL);
		glShaderSource(fShaderTexture, 1, &fTextureSource, NULL);
		glShaderSource(vShaderColor, 1, &vColorSource, NULL);
		glShaderSource(fShaderColor, 1, &fColorSource, NULL);
		glShaderSource(fShaderText, 1, &fTextSource, NULL);

		glCompileShader(vShaderTexture);
		glCompileShader(fShaderTexture);
		glCompileShader(vShaderColor);
		glCompileShader(fShaderColor);
		glCompileShader(fShaderText);

		
		int success;
		char infolog[512];
		glGetShaderiv(fShaderText, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fShaderText, 512, NULL, infolog);
			std::cerr << "error compiling shader" << infolog <<std::endl;
		}
		

		m_textureProgram = glCreateProgram();
		glAttachShader(m_textureProgram, vShaderTexture);
		glAttachShader(m_textureProgram, fShaderTexture);
		glLinkProgram(m_textureProgram);

		m_colorProgram = glCreateProgram();
		glAttachShader(m_colorProgram, vShaderColor);
		glAttachShader(m_colorProgram, fShaderColor);
		glLinkProgram(m_colorProgram);

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

		glDeleteShader(vShaderColor);
		glDeleteShader(fShaderColor);
		glDeleteShader(fShaderText);
	}


	//texture load
	{
		//variables reused for every load
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* high = stbi_load("res/highway.png", &width, &height, &channels, 0);

		glGenTextures(1, &m_highwayTexture);
		glBindTexture(GL_TEXTURE_2D, m_highwayTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, high);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, high);

		unsigned char* obj = stbi_load("res/objects.png", &width, &height, &channels, 0);

		glGenTextures(1, &m_objTexture);
		glBindTexture(GL_TEXTURE_2D, m_objTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, obj);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, obj);

		unsigned char* meters = stbi_load("res/meters.png", &width, &height, &channels, 0);
		glGenTextures(1, &m_metersTexture);
		glBindTexture(GL_TEXTURE_2D, m_metersTexture);
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
		//m_persProj = main perspective projection
		m_persProj = glm::perspective(45.0f, 1024.0f / 600, 1.0f, -2.0f);
		glm::mat4 look = glm::lookAt(glm::vec3(0.0, 2.0, 5.0),
			glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 1.0, 0.0));
		m_persProj = m_persProj * look;

		m_orthoProj = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f,1.0f,-1.0f);

		usePersProj();
	}

	//vao setup
	{
		glGenVertexArrays(1, &m_textureVAO);
		glGenVertexArrays(1, &m_colorVAO);

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
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph:" << c <<std::endl;
				
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RED,m_font->glyph->bitmap.width,m_font->glyph->bitmap.rows,
				0,GL_RED,GL_UNSIGNED_BYTE,m_font->glyph->bitmap.buffer);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				(float)m_font->glyph->bitmap_left,
				(float)m_font->glyph->bitmap_top,
				(float)m_font->glyph->bitmap.width,
				(float)m_font->glyph->bitmap.rows,
				(float)m_font->glyph->advance.x
			};
			ChMap.insert(std::pair<char, Character>(c, character));

		}
		FT_Done_Face(m_font);
		FT_Done_FreeType(m_FTLibrary);


	}

	setTextColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GameRender::highway(double time) {

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

	usePersProj();
	renderTexture(highwayVector, highwayIndices, m_highwayTexture);
}

void GameRender::clicker() {
	float clickedOffset = 0.03f;

	float plane = 0.0f;

	std::vector<float> clickerVector = {};
	std::vector<unsigned int> clickerIndices = {};
	unsigned int clickerVertexCount = 0;

	pushVertexTexture(clickerVector, -0.85f, plane, 3.6f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.85f, plane, 3.9f, 1.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.2f, plane, 3.9f, 1.0f, 1280.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.2f, plane, 3.6f, 1320.0f / 1760.0f, 1280.0f / 1640.0f);

	pushRectangleIndices(clickerIndices, clickerVertexCount);

	pushVertexTexture(clickerVector, 0.85f, plane, 3.6f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
	pushVertexTexture(clickerVector, 0.85f, plane, 3.9f, 1.0f, 400.0f / 1640.0f);
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
	usePersProj();
	renderTexture(clickerVector, clickerIndices, m_objTexture);
}

void GameRender::notes(double time, std::vector<Note>& v) {

	float plane = 0.0;

	std::vector<float> noteVector = {};
	std::vector<unsigned int> noteIndices = {};
	unsigned int noteVertexCount = 0;

	for (size_t i = 0; i < v.size(); i++) {
		if (v.at(i).getRender()) {
			double dt = v.at(i).getMilli() - time;
			float z = 3.75f - (3.75f * (float)dt);
			int type = v.at(i).getType();

			float s = 0.0;
			float t = 0.0;

			if (type == TAP_R) {
				if (m_renderEuActive) {
					s = 1200.0f / 1760.0f;
					t = 0.0f;
				}
				else {
					s = 400.0f / 1760.0f;
					t = 0.0;
				}
				pushVertexTexture(noteVector, -0.15f, plane, z - 0.15f, s, t + 400.0f / 1760.0f);
				pushVertexTexture(noteVector, -0.15f, plane, z + 0.15f, s, t);
				pushVertexTexture(noteVector, 0.15f, plane, z + 0.15f, s + 400.0f / 1760.0f, t);
				pushVertexTexture(noteVector, 0.15f, plane, z - 0.15f, s + 400.0f / 1760.0f, t + 400.0f / 1760.0f);
				pushRectangleIndices(noteIndices, noteVertexCount);
			}
			else if (type == TAP_G) {
				if (m_renderEuActive) {
					s = 1200.0f / 1760.0f;
					t = 0.0f;
				}
				else {
					s = 0.0f;
					t = 0.0f;
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
				s = 400.0f / 1760.0f;
				t = 840.0f / 1640.0f;
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
					s = 1200.0f / 1760.0f;
					t = 0.0f;
				}
				else {
					s = 800.0f / 1760.0f;
					t = 0.0f;
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
			s = 400.0f / 1760.0f;
			t = 840.0f / 1640.0f;
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
	usePersProj();
	renderTexture(noteVector, noteIndices, m_objTexture);
}

void GameRender::lanes(double time, std::vector<Note>& ev) {

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
				float z = 3.75f - 3.75f * (float)dt;
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
				float z = 3.75f - 3.75f * (float)dt;

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
				float z = 3.75f - 3.75f * (float)dt;

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

	usePersProj();
	renderColor(redLaneVector, redLaneIndices);
	renderColor(greenLaneVector, greenLaneIndices);
	renderColor(blueLaneVector, blueLaneIndices);
}

void GameRender::bpmTicks(double time, std::vector<double>& bpmArr)
{
	std::vector<float> bpmVector;
	std::vector<unsigned int> bpmIndices;
	unsigned int bpmVertexCount = 0;
	
	float plane = 0.0f;
	float size = 0.1f;

	float r = 0.3f;
	float g = 0.3f;
	float b = 0.3f;
	
	for (size_t i = 0; i < bpmArr.size(); i++) {
		double tickTime = bpmArr.at(i);
		if (time + 1.0 >= tickTime && time <= tickTime + 0.2) {
			double dt = tickTime - time;
			float z = 3.75f - (3.75f * (float)dt);
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
					float z = 3.75f - (3.75f * (float)dt);
					if (m_renderCross == 0) {
						pushVertexColor(eventsVector, -0.85f, plane, z, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.55f, plane, z, 0.0f, 1.0f, 0.0f);
					}
					else {
						pushVertexColor(eventsVector, -0.5f, plane, z, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.2f, plane, z, 0.0f, 1.0f, 0.0f);
					}
					start_drawn = true;
				}
				else if (dt < 0.0) {
					if (m_renderCross == 0) {
						pushVertexColor(eventsVector, -0.85f, plane, 3.75f, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.55f, plane, 3.75f, 0.0f, 1.0f, 0.0f);
					}
					else {
						pushVertexColor(eventsVector, -0.5f, plane, 3.75f, 0.0f, 1.0f, 0.0f);
						pushVertexColor(eventsVector, -0.2f, plane, 3.75f, 0.0f, 1.0f, 0.0f);
					}
					start_drawn = true;
				}
				//end				
				if (start_drawn) {
					if (dt_end > 0.0 && dt_end <= 1.0) {
						float z = 3.75f - (3.75f * (float)dt_end);
						if (m_renderCross == 0) {
							pushVertexColor(eventsVector, -0.55f, plane, z, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.85f, plane, z, 0.0f, 1.0f, 0.0f);
						}
						else {
							pushVertexColor(eventsVector, -0.2f, plane, z, 0.0f, 1.0f, 0.0f);
							pushVertexColor(eventsVector, -0.5f, plane, z, 0.0f, 1.0f, 0.0f);
						}
						pushRectangleIndices(eventsIndices, eventsVertexCount);
					}
					else if (dt_end > 1.0) {
						if (m_renderCross == 0) {
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
					float z = 3.75f - (3.75f * (float)dt);
					if (m_renderCross == 2) {
						pushVertexColor(eventsVector, 0.55f, plane, z, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.85f, plane, z, 0.0f, 0.0f, 1.0f);
					}
					else {
						pushVertexColor(eventsVector, 0.2f, plane, z, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.5f, plane, z, 0.0f, 0.0f, 1.0f);
					}
					start_drawn = true;
				}
				else if (dt < 0.0) {
					if (m_renderCross == 2) {
						pushVertexColor(eventsVector, 0.55f, plane, 3.75f, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.85f, plane, 3.75f, 0.0f, 0.0f, 1.0f);
					}
					else {
						pushVertexColor(eventsVector, 0.2f, plane, 3.75f, 0.0f, 0.0f, 1.0f);
						pushVertexColor(eventsVector, 0.5f, plane, 3.75f, 0.0f, 0.0f, 1.0f);
					}
					start_drawn = true;
				}
				//end				
				if (start_drawn) {
					if (dt_end > 0.0 && dt_end <= 1.0) {
						float z = 3.75f - (3.75f * (float)dt_end);
						if (m_renderCross == 2) {
							pushVertexColor(eventsVector, 0.85f, plane, z, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.55f, plane, z, 0.0f, 0.0f, 1.0f);
						}
						else {
							pushVertexColor(eventsVector, 0.5f, plane, z, 0.0f, 0.0f, 1.0f);
							pushVertexColor(eventsVector, 0.2f, plane, z, 0.0f, 0.0f, 1.0f);
						}
						pushRectangleIndices(eventsIndices, eventsVertexCount);
					}
					else if (dt_end > 1.0) {
						if (m_renderCross == 2) {
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
				float z = 3.75f - (3.75f * (float)dt);
				pushVertexColor(eventsVector, -1.0f, plane, z, 1.0f, 1.0f, 1.0f, transparency);
				pushVertexColor(eventsVector, 1.0f, plane, z, 1.0f, 1.0f, 1.0f, transparency);
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
					float z = 3.75f - (3.75f * (float)dt_end);
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
	usePersProj();
	renderColor(eventsVector, eventsIndices);
}

void GameRender::meters() {
	float yPlane = 0.1f;

	std::vector<float>metersVector;
	std::vector<unsigned int> metersIndices;
	unsigned int metersVertexCount = 0;

	//combo meter
	if (m_playerMult == 2) {
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 0.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 0.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 3) {
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 400.0f / 1000.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 800.0f / 1000.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 800.0f / 1000.0f, 800.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 4) {
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 0.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 0.0f, 0.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 0.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 400.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 6) {
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 0.0f, 1.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 0.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 400.0f / 1000.0f, 1.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_playerMult == 8) {
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.5f, 400.0f / 1000.0f, 1.0f);
		pushVertexTexture(metersVector, 1.0f, yPlane, 2.7f, 400.0f / 1000.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.7f, 800.0f / 1000.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, 1.2f, yPlane, 2.5f, 800.0f / 1000.0f, 1.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}

	for (int i = 0; i < 8; i++) {
		if (m_playerCombo == 0) {
			pushVertexTexture(metersVector, 1.0f, yPlane, 3.6f - 0.11f * i, 400.0f / 1000.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 3.7f - 0.11f * i, 400.0f / 1000.0f, 200.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 3.7f - 0.11f * i, 800.0f / 1000.0f, 200.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 3.6f - 0.11f * i, 800.0f / 1000.0f, 400.0f / 1200.0f);
			pushRectangleIndices(metersIndices, metersVertexCount);
		}
		else if (m_playerCombo >= 24) {
			pushVertexTexture(metersVector, 1.0f, yPlane, 3.6f - 0.11f * i, 400.0f / 1000.0f, 200.0f / 1200.0f);
			pushVertexTexture(metersVector, 1.0f, yPlane, 3.7f - 0.11f * i, 400.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 3.7f - 0.11f * i, 800.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, 1.2f, yPlane, 3.6f - 0.11f * i, 800.0f / 1000.0f, 200.0f / 1200.0f);
			pushRectangleIndices(metersIndices, metersVertexCount);
		}
		else {
			int limit = m_playerCombo % 8;
			if (limit == 0)limit = 9;
			if (i < limit) {
				pushVertexTexture(metersVector, 1.0f, yPlane, 3.6f - 0.11f * i, 400.0f / 1000.0f, 200.0f / 1200.0f);
				pushVertexTexture(metersVector, 1.0f, yPlane, 3.7f - 0.11f * i, 400.0f / 1000.0f, 0.0f);
				pushVertexTexture(metersVector, 1.2f, yPlane, 3.7f - 0.11f * i, 800.0f / 1000.0f, 0.0f);
				pushVertexTexture(metersVector, 1.2f, yPlane, 3.6f - 0.11f * i, 800.0f / 1000.0f, 200.0f / 1200.0f);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
			else {
				pushVertexTexture(metersVector, 1.0f, yPlane, 3.6f - 0.11f * i, 400.0f / 1000.0f, 400.0f / 1200.0f);
				pushVertexTexture(metersVector, 1.0f, yPlane, 3.7f - 0.11f * i, 400.0f / 1000.0f, 200.0f / 1200.0f);
				pushVertexTexture(metersVector, 1.2f, yPlane, 3.7f - 0.11f * i, 800.0f / 1000.0f, 200.0f / 1200.0f);
				pushVertexTexture(metersVector, 1.2f, yPlane, 3.6f - 0.11f * i, 800.0f / 1000.0f, 400.0f / 1200.0f);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
		}
	}

	//euphoria meter

	for (int i = 0; i < 3; i++) {
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.75f - 0.35f * i, 800.0f / 1000.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.75f - 0.35f * i, 1.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f - 0.35f * i, 1.0f, 800.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f - 0.35f * i, 800.0f / 1000.0f, 800.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}

	if (m_renderEuValue > 0.0 && m_renderEuValue < 1.0) {
		float z = (float)m_renderEuValue;
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.75f, 800.0f / 1000.0f, 0.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.75f, 1.0f, 0.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.75f - 0.35f * z, 1.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.75f - 0.35f * z, 800.0f / 1000.0f, 400.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
	}
	else if (m_renderEuValue >= 1.0) {
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.75f, 800.0f / 1000.0f, 0.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.75f, 1.0f, 0.0f);
		pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f, 1.0f, 400.0f / 1200.0f);
		pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f, 800.0f / 1000.0f, 400.0f / 1200.0f);
		pushRectangleIndices(metersIndices, metersVertexCount);
		if (m_renderEuValue < 2.0) {
			float z = (float)m_renderEuValue - 1.0f;
			pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f, 800.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f, 1.0f, 0.0f);
			pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f - 0.35f*z, 1.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f - 0.35f*z, 800.0f / 1000.0f, 400.0f / 1200.0f);
			pushRectangleIndices(metersIndices, metersVertexCount);
		}
		else if (m_renderEuValue >= 2.0) {
			pushVertexTexture(metersVector, -1.1f, yPlane, 3.4f, 800.0f / 1000.0f, 0.0f);
			pushVertexTexture(metersVector, -1.0f, yPlane, 3.4f, 1.0f, 0.0f);
			pushVertexTexture(metersVector, -1.0f, yPlane, 3.05f, 1.0f, 400.0f / 1200.0f);
			pushVertexTexture(metersVector, -1.1f, yPlane, 3.05f, 800.0f / 1000.0f, 400.0f / 1200.0f);
			pushRectangleIndices(metersIndices, metersVertexCount);
			if (m_renderEuValue < 3.0) {
				float z = (float)m_renderEuValue - 2.0f;
				pushVertexTexture(metersVector, -1.1f, yPlane, 3.05f, 800.0f / 1000.0f, 0.0f);
				pushVertexTexture(metersVector, -1.0f, yPlane, 3.05f, 1.0f, 0.0f);
				pushVertexTexture(metersVector, -1.0f, yPlane, 3.05f - 0.35f * z, 1.0, 400.0f / 1200.0f);
				pushVertexTexture(metersVector, -1.1f, yPlane, 3.05f - 0.35f * z, 800.0f / 1000.0f, 400.0f / 1200.0f);
				pushRectangleIndices(metersIndices, metersVertexCount);
			}
			else {
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
	scoreDisplay.resize(scoreDisplay.length()-s.length());
	scoreDisplay.append(s);
	drawText(scoreDisplay.c_str(), 940.0f, 230.0f, 0.05f);

}

void GameRender::splash() {
	std::vector<float> splashVector;
	std::vector<unsigned int> splashIndices;
	unsigned int splashVertexCount = 0;
	drawText("Press space to start", 10.0f, 380.0f, 0.1f);
}

void GameRender::pollState(double time, Player& p, Generator& g) {
	m_red = p.getRedClicker();
	m_blue = p.getBlueClicker();
	m_green = p.getGreenClicker();
	m_playerCross = p.getCross();
	m_playerCombo = p.getCombo();
	m_playerMult = p.getMult();
	m_renderEuValue = p.getEuValue();
	m_renderEuActive = p.getEuActive();
	m_renderEuZone = p.getEuZoneActive();
	m_playerScore = p.getScore();
}


GameRender::~GameRender() {
	//dtor
}
