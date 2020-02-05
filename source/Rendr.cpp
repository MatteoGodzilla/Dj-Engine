#include "Rendr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

//character struct to store freetype glyph data
struct Character {
	GLuint TextureID;
	float bx;
	float by;
	float width;
	float height;
	float advance;
};

std::map<char, Character> ChMap;

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
void Rendr::pushVertexColor(std::vector<float>& v, float x, float y, float z, float r, float g, float b, float a) {
	if (rendr_InvertedX) v.push_back(-x);
	else v.push_back(x);
	v.push_back(y);
	v.push_back(z);
	v.push_back(r);
	v.push_back(g);
	v.push_back(b);
	v.push_back(a);
}

//utility function
void Rendr::pushVertexTexture(std::vector<float>& v, float x, float y, float z, float s, float t) {
	if (rendr_InvertedX) v.push_back(-x);
	else v.push_back(x);
	v.push_back(y);
	v.push_back(z);
	v.push_back(s);
	v.push_back(t);
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

void Rendr::usePersProj() {
	//setting up projection uniform on all programs
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

void Rendr::useOrthoProj() {
	//setting up projection uniform
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

void Rendr::setTextColor(float r, float g, float b, float a) {
	std::cout << "Rendr msg: Setting up color" << std::endl;
	glUseProgram(m_textProgram);
	int location = glGetUniformLocation(m_textProgram, "u_textColor");
	if (location != -1) {
		glUniform4f(location, r, g, b, a);
	}
	else {
		std::cerr << "Rendr err: error setting text color" << std::endl;
	}
}

void Rendr::renderTexture(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) {
	//bind texture VAO (layout and actual buffers)
	glBindVertexArray(m_textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);
	glBindTexture(GL_TEXTURE_2D, texture);

	//upload data on buffers
	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float), vertexArr.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * sizeof(int), indexArr.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_textureProgram);
	glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Rendr::renderText(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr, unsigned int texture) {
	//bind text VAO (layout and actual buffers)
	glBindVertexArray(m_textureVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);
	glBindTexture(GL_TEXTURE_2D, texture);

	//upload data on buffers
	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float), vertexArr.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * sizeof(int), indexArr.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_textProgram);
	glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Rendr::renderColor(std::vector<float>& vertexArr, std::vector<unsigned int>& indexArr) {
	//bind color VAO (layout and actual buffers)
	glBindVertexArray(m_colorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);

	glBufferData(GL_ARRAY_BUFFER, vertexArr.size() * sizeof(float), vertexArr.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArr.size() * sizeof(int), indexArr.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_colorProgram);
	glDrawElements(GL_TRIANGLES, indexArr.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Rendr::drawText(const std::string& s, float x, float y, float scl) {
	bool old = rendr_InvertedX;
	rendr_InvertedX = false;
	//loop for every char in string
	float maxBearing = 0.0f;
	for (char c : s) {
		Character temp = ChMap[c];
		temp.by *= scl;
		if (temp.by > maxBearing) {
			maxBearing = temp.by;
		}
	}
	y += maxBearing;

	for (char c: s) {
		if (c < 128) {
			Character temp = ChMap[c];
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
		else {
			std::cerr << "Rendr error:Char not supported: " << c << std::endl;
		}
	}
	rendr_InvertedX = old;
}

float Rendr::getTextWidth(const std::string& s,float scale) {
	float x = 0.0f; // return variable
	for (char c:s) {
		Character temp = ChMap[c];
		temp.advance *= scale;
		x += temp.advance / 64;
	}
	return x;
}

float Rendr::getTextHeight(const std::string& s, float scale) {
	float maxBearing = 0;
	float y = 0.0f;
	for (char c : s) {
		Character temp = ChMap[c];
		temp.by *= scale;
		if (temp.by > maxBearing) {
			maxBearing = temp.by;
		}
	}
	//std::cout << maxBearing << std::endl;
	for (char c : s) {
		Character temp = ChMap[c];
		temp.height *= scale;
		temp.by *= scale;
		float bottom = maxBearing - temp.by + temp.height;
		if (bottom > y) {
			y = bottom;
		}
	}
	return y;
}

void Rendr::loadTexture(const std::string& s, unsigned int* destination) {
	int width, height, channels;

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
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		std::cout << "Rendr Msg: successfully loaded texture at " << s << std::endl;
	}
	else {
		std::cerr << "Rendr Err: cannot load texture. Does the file at " << s << " exist?" << std::endl;
	}

}



Rendr::Rendr() {
	//ctor
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
		//shaders source files
		const char* vTextureSource = "\n"
			"#version 330\n"
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
			"#version 330\n"
			"out vec4 FragColor;\n"
			"in vec2 tex_coords;\n"
			"uniform sampler2D u_t;\n"
			"\n"
			"void main()"
			"{\n"
			"	FragColor = texture(u_t,tex_coords);\n"
			"}\n";


		const char* vColorSource = "\n"
			"#version 330\n"
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
			"#version 330\n"
			"out vec4 FragColor;\n"
			"in vec4 color;\n"
			"\n"
			"void main()"
			"{\n"
			"	FragColor = color;\n"
			"}\n";

		const char* fTextSource = "\n"
			"#version 330\n"
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

		//shader ids
		unsigned int vShaderTexture, fShaderTexture;
		unsigned int vShaderColor, fShaderColor;
		unsigned int fShaderText;

		//create shaders
		vShaderTexture = glCreateShader(GL_VERTEX_SHADER);
		fShaderTexture = glCreateShader(GL_FRAGMENT_SHADER);
		vShaderColor = glCreateShader(GL_VERTEX_SHADER);
		fShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
		fShaderText = glCreateShader(GL_FRAGMENT_SHADER);

		//upload shader source data (see above)
		glShaderSource(vShaderTexture, 1, &vTextureSource, NULL);
		glShaderSource(fShaderTexture, 1, &fTextureSource, NULL);
		glShaderSource(vShaderColor, 1, &vColorSource, NULL);
		glShaderSource(fShaderColor, 1, &fColorSource, NULL);
		glShaderSource(fShaderText, 1, &fTextSource, NULL);

		//compile shaders (hoping that they don't fail)
		glCompileShader(vShaderTexture);
		glCompileShader(fShaderTexture);
		glCompileShader(vShaderColor);
		glCompileShader(fShaderColor);
		glCompileShader(fShaderText);

		//example compile status check
		int success;
		char infolog[512];
		glGetShaderiv(vShaderTexture, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vShaderTexture, 512, NULL, infolog);
			std::cerr << "error compiling vertex texture shader:" << infolog << std::endl;
		}
		glGetShaderiv(fShaderTexture, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fShaderTexture, 512, NULL, infolog);
			std::cerr << "error compiling fragment texture shader:" << infolog << std::endl;
		}
		glGetShaderiv(vShaderColor, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vShaderColor, 512, NULL, infolog);
			std::cerr << "error compiling vertex color shader:" << infolog << std::endl;
		}
		glGetShaderiv(fShaderColor, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fShaderColor, 512, NULL, infolog);
			std::cerr << "error compiling fragment color shader:" << infolog << std::endl;
		}
		glGetShaderiv(fShaderText, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fShaderText, 512, NULL, infolog);
			std::cerr << "error compiling fragment text shader:" << infolog << std::endl;
		}
		//create shader programs
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

		//example program linking check
		int linked;
		char log[512];
		glGetProgramiv(m_textProgram, GL_LINK_STATUS, &linked);
		if (!linked) {
			glGetProgramInfoLog(m_textProgram, 512, NULL, log);
			std::cerr << "error linking program:" << log << std::endl;
		}

		//delete shader object files
		glDeleteShader(vShaderTexture);
		glDeleteShader(fShaderTexture);

		glDeleteShader(vShaderColor);
		glDeleteShader(fShaderColor);
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
		//create VAOs
		glGenVertexArrays(1, &m_textureVAO);
		glGenVertexArrays(1, &m_colorVAO);

		//texture vao
		{
			glBindVertexArray(m_textureVAO);

			//indices buffer
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

			//index buffer
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
			std::cerr << "Rendr error:error opening freetype" << std::endl;
		}
		if (FT_New_Face(m_FTLibrary, "res/NotoSans-Regular.ttf", 0, &m_font)) {
			std::cerr << "Rendr error:error loading font" << std::endl;
		}
		FT_Set_Pixel_Sizes(m_font, 0, 1024);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(m_font, c, FT_LOAD_RENDER))
			{
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

GLFWwindow* Rendr::getWindowPtr(){
	return m_window;
}

Rendr::~Rendr() {
}
