#include "Rendr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Rendr::Rendr() {
	//ctor
}

void checkError() {
	std::cout << "started error checking" << std::endl;
	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		std::cout << "OpenGL Error: " << error << std::endl;
		error = glGetError();
	}
	std::cout << "ended error checking" << std::endl;
}

void pushVertexColor(std::vector<float>& v, float x, float y, float z, float r = 0.0, float g = 0.0, float b = 0.0) {
	v.push_back(x);
	v.push_back(y);
	v.push_back(z);
	v.push_back(r);
	v.push_back(g);
	v.push_back(b);
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
		/*
		int successful;
		glGetShaderiv(vShaderColor, GL_COMPILE_STATUS, &successful);
		std::cout << successful << std::endl;
		*/
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
		/*
		int success;
		glGetProgramiv(m_ColorProgram, GL_LINK_STATUS, &success);
		std::cout << success << std::endl;
		*/

	}

	//texture load
	{
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

		stbi_image_free(high);
		stbi_image_free(obj);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	//projection init
	{
		m_proj = glm::perspective(45.0f, 1024.0f / 600, 1.0f, -2.0f);
		glm::mat4 look = glm::lookAt(glm::vec3(0.0, 2.0, 5.0),
			glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 1.0, 0.0));
		m_proj = m_proj * look;
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
		glGenVertexArrays(1, &m_highwayVAO);
		glGenVertexArrays(1, &m_lanesVAO);
		glGenVertexArrays(1, &m_notesVAO);
		glGenVertexArrays(1, &m_clickerVAO);

		//highway vao
		{
			glBindVertexArray(m_highwayVAO);

			unsigned int ebo;
			glGenBuffers(1, &m_highwayVBO);
			glGenBuffers(1, &ebo);

			glBindBuffer(GL_ARRAY_BUFFER, m_highwayVBO);
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

		//lanes vao
		{
			glBindVertexArray(m_lanesVAO);

			unsigned int index;
			glGenBuffers(1, &m_lanesVBO);
			glGenBuffers(1, &index);
			glBindBuffer(GL_ARRAY_BUFFER, m_lanesVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

			//Vertex: x,y,z, r,g,b,
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);


			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		//notes vao
		{
			glBindVertexArray(m_notesVAO);

			unsigned int ebo;
			glGenBuffers(1, &m_notesVBO);
			glGenBuffers(1, &ebo);

			glBindBuffer(GL_ARRAY_BUFFER, m_notesVBO);
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

		//clicker vao
		{
			glBindVertexArray(m_clickerVAO);

			unsigned int index;
			glGenBuffers(1, &m_clickerVBO);
			glGenBuffers(1, &index);

			glBindBuffer(GL_ARRAY_BUFFER, m_clickerVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

			//vertex: x,y,z, s,t
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		}

	}

	/*
	//scale setup
	m_scl_start = sf::Vector2f(0.1,0.1);
	m_scl_end = sf::Vector2f(0.2,0.2);
	m_scl_vel = (m_scl_end-m_scl_start)/1.0f;

	m_trayL.setTexture(m_tex);
	m_trayL.setTextureRect(sf::IntRect(1320,0,440,880));
	m_trayL.setOrigin(220,440);
	m_trayL.setRotation(90);
	m_trayL.setScale(0.2,0.2);
	m_trayL.setPosition(382.0,500.0);

	m_trayR.setTexture(m_tex);
	m_trayR.setTextureRect(sf::IntRect(1320,0,440,880));
	m_trayR.setOrigin(220,400);
	m_trayR.setRotation(90);
	m_trayR.setScale(0.2,0.2);
	m_trayR.setPosition(652.0,500.0);

	m_red_click.setTexture(m_tex);
	m_red_click.setTextureRect(sf::IntRect(440,400,440,440));
	m_red_click.setOrigin(220,220);
	m_red_click.setScale(0.2,0.2);
	m_red_click.setPosition(512.0,500.0);

	m_green_click.setTexture(m_tex);
	m_green_click.setTextureRect(sf::IntRect(0,400,440,440));
	m_green_click.setOrigin(220,220);
	m_green_click.setScale(0.2,0.2);
	m_green_click.setPosition(424.0,500.0);

	m_blue_click.setTexture(m_tex);
	m_blue_click.setTextureRect(sf::IntRect(880,400,440,440));
	m_blue_click.setOrigin(220,220);
	m_blue_click.setScale(0.2,0.2);
	m_blue_click.setPosition(600.0,500.0);
	*/
	for (int i = 0; i < resolution; ++i) {
		m_lanes.push_back(1);
	}

}

void Rendr::highway(double time) {

	float factor = (float)time * 0.875f;
	float data[] = {
			-1.0f, 0.0f, 0.0f, 0.0f, 1.0f + factor,
			 1.0f, 0.0f, 0.0f, 1.0f, 1.0f + factor,
			-1.0f, 0.0f, 4.0f, 0.0f, 0.0f + factor,
			 1.0f, 0.0f, 4.0f, 1.0f, 0.0f + factor
	};

	unsigned int indices[] = {
			0,2,3,
			3,1,0
	};

	glBindVertexArray(m_highwayVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_highwayVBO);
	glBindTexture(GL_TEXTURE_2D, m_HighwayTexture);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glUseProgram(m_TextureProgram);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);

}

void Rendr::clicker() {
	float clickedOffset = 0.03f;


	std::vector<float> clickerVector = {};
	std::vector<unsigned int> clickerIndices = {};
	unsigned int clickerVertexCount = 0;

	pushVertexTexture(clickerVector, -0.85f, 0.0f, 3.6f, 1320.0f / 1760.0f, 0.0f);
	pushVertexTexture(clickerVector, -0.85f, 0.0f, 3.9f, 1.0f, 0.0f);
	pushVertexTexture(clickerVector, -0.2f, 0.0f, 3.9f, 1.0f, 880.0f / 1640.0f);
	pushVertexTexture(clickerVector, -0.2f, 0.0f, 3.6f, 1320.0f / 1760.0f, 880.0f / 1640.0f);

	pushRectangleIndices(clickerIndices, clickerVertexCount);
	
	pushVertexTexture(clickerVector, 0.85f, 0.0f, 3.6f, 1320.0f / 1760.0f, 0.0f);
	pushVertexTexture(clickerVector, 0.85f, 0.0f, 3.9f, 1.0f, 0.0f);
	pushVertexTexture(clickerVector, 0.2f, 0.0f, 3.9f, 1.0f, 880.0f / 1640.0f );
	pushVertexTexture(clickerVector, 0.2f, 0.0f, 3.6f, 1320.0f / 1760.0f, 880.0f / 1640.0f );

	pushRectangleIndices(clickerIndices, clickerVertexCount);
	if (m_red) {
		pushVertexTexture(clickerVector, -0.15f + clickedOffset, 0.0f, 3.6f + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, -0.15f + clickedOffset, 0.0f, 3.9f - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f - clickedOffset, 0.0f, 3.9f - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f - clickedOffset, 0.0f, 3.6f + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
	}
	else {
		pushVertexTexture(clickerVector, -0.15f, 0.0f, 3.6f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		pushVertexTexture(clickerVector, -0.15f, 0.0f, 3.9f, 440.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f, 0.0f, 3.9f, 880.0f / 1760.0f, 400.0f / 1640.0f);
		pushVertexTexture(clickerVector, 0.15f, 0.0f, 3.6f, 880.0f / 1760.0f, 840.0f / 1640.0f);
	}
	pushRectangleIndices(clickerIndices, clickerVertexCount);
	

	if (m_green) {
		if (m_player_cross >= 1) {
			pushVertexTexture(clickerVector, -0.5f + clickedOffset, 0.0f, 3.6f + clickedOffset, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.5f + clickedOffset, 0.0f, 3.9f - clickedOffset, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f - clickedOffset, 0.0f, 3.9f - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f - clickedOffset, 0.0f, 3.6f + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			pushVertexTexture(clickerVector, -0.85f + clickedOffset, 0.0f, 3.6f + clickedOffset, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.85f + clickedOffset, 0.0f, 3.9f - clickedOffset, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f - clickedOffset, 0.0f, 3.9f - clickedOffset, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f - clickedOffset, 0.0f, 3.6f + clickedOffset, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	else {
		if (m_player_cross >= 1) {
			pushVertexTexture(clickerVector, -0.5f, 0.0f, 3.6f, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.5f, 0.0f, 3.9f, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f, 0.0f, 3.9f, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.2f, 0.0f, 3.6f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			pushVertexTexture(clickerVector, -0.85f, 0.0f, 3.6f, 0.0, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.85f, 0.0f, 3.9f, 0.0, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f, 0.0f, 3.9f, 440.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, -0.55f, 0.0f, 3.6f, 440.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}

	if (m_blue) {
		if (m_player_cross <= 1) {
			pushVertexTexture(clickerVector, 0.5f - clickedOffset, 0.0f, 3.6f + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.5f - clickedOffset, 0.0f, 3.9f - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f + clickedOffset, 0.0f, 3.9f - clickedOffset, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f + clickedOffset, 0.0f, 3.6f + clickedOffset, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			pushVertexTexture(clickerVector, 0.85f - clickedOffset, 0.0f, 3.6f + clickedOffset, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.85f - clickedOffset, 0.0f, 3.9f - clickedOffset, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f + clickedOffset, 0.0f, 3.9f - clickedOffset, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f + clickedOffset, 0.0f, 3.6f + clickedOffset, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}
	else {
		if (m_player_cross <= 1) {
			pushVertexTexture(clickerVector, 0.5f, 0.0f, 3.6f, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.5f, 0.0f, 3.9f, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f, 0.0f, 3.9f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.2f, 0.0f, 3.6f, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		else {
			pushVertexTexture(clickerVector, 0.85f, 0.0f, 3.6f, 880.0f / 1760.0f, 840.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.85f, 0.0f, 3.9f, 880.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f, 0.0f, 3.9f, 1320.0f / 1760.0f, 400.0f / 1640.0f);
			pushVertexTexture(clickerVector, 0.55f, 0.0f, 3.6f, 1320.0f / 1760.0f, 840.0f / 1640.0f);
		}
		pushRectangleIndices(clickerIndices, clickerVertexCount);
	}


	

	
	glBindVertexArray(m_clickerVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_clickerVBO);
	glBufferData(GL_ARRAY_BUFFER, clickerVector.size()*sizeof(float), clickerVector.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, clickerIndices.size()*sizeof(float), clickerIndices.data(), GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, m_ObjTexture);
	glUseProgram(m_TextureProgram);
	glDrawElements(GL_TRIANGLES, clickerIndices.size(), GL_UNSIGNED_INT, 0);

}

void Rendr::notes(double time, std::vector<Note>& v) {
	/*
	float factor = time ;
	float vertices[] = {
		-0.15f, 0.1f, factor    , 400.0f / 1760 ,400.0f / 1640.0f,
		 0.15f, 0.1f, factor    , 800.0f / 1760 ,400.0f / 1640.0f,
		-0.15f, 0.1f, factor-0.3, 400.0f / 1760 ,0.0f,
		 0.15f, 0.1f, factor-0.3, 800.0f / 1760 ,0.0f
	};

	unsigned int indices[] = {
		0,2,3,
		3,1,0
	};

	glBindVertexArray(m_testVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glUseProgram(m_TextureProgram);
	glBindTexture(GL_TEXTURE_2D,m_ObjTexture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	*/
	/*
	for(size_t i = 0 ; i < v.size(); ++i) {
		float dt = v.at(i).getMilli()-time;
		sf::Sprite sprite;
		sprite.setTexture(m_tex);
		int type = v.at(i).getType();
		if(!v.at(i).getRender()){
			if(m_ren_cross == 0){
				v.at(i).setLanMod(0);
			}else if(m_ren_cross == 1){
				v.at(i).setLanMod(1);
			}else if(m_ren_cross == 2){
				v.at(i).setLanMod(2);
			}
		}

		if(type == TAP_R ) {
			sprite.setTextureRect(sf::IntRect(400,0,400,400));
			m_start = sf::Vector2f(512.0,200.0);
			m_end = sf::Vector2f(512.0,500.0);
			m_vel = (m_end-m_start)/1.0f;
		} else if (type == TAP_G || type == SCR_G_UP || type == SCR_G_DOWN || type == SCR_G_ANY) {
			if(type== TAP_G) {
				sprite.setTextureRect(sf::IntRect(0,0,400,400));
			} else if(type == SCR_G_UP) {
				sprite.setTextureRect(sf::IntRect(800,840,400,400));
			} else if(type == SCR_G_DOWN) {
				sprite.setTextureRect(sf::IntRect(400,840,400,400));
			} else if(type == SCR_G_ANY) {
				sprite.setTextureRect(sf::IntRect(0,840,400,400));
			}
			if(v.at(i).getLanMod() == 0){
				//green left
				m_start = green_left_start;
				m_end = green_left_end;
				m_vel = green_left_vel;
			}else if(v.at(i).getLanMod() >= 1){
				//green center
				m_start = green_center_start;
				m_end = green_center_end;
				m_vel = green_center_vel;
			}
		} else if (type == TAP_B || type == SCR_B_UP || type == SCR_B_DOWN || type == SCR_B_ANY) {
			if(type== TAP_B) {
				sprite.setTextureRect(sf::IntRect(800,0,400,400));
			} else if(type == SCR_B_UP) {
				sprite.setTextureRect(sf::IntRect(800,840,400,400));
			} else if(type == SCR_B_DOWN) {
				sprite.setTextureRect(sf::IntRect(400,840,400,400));
			} else if(type == SCR_B_ANY) {
				sprite.setTextureRect(sf::IntRect(0,840,400,400));
			}
			if(v.at(i).getLanMod() <= 1){
			   //blue center
				m_start = blue_center_start;
				m_end = blue_center_end;
				m_vel = blue_center_vel;
			}else if(v.at(i).getLanMod()==2){
				//blue right
				m_start = blue_right_start;
				m_end = blue_right_end;
				m_vel = blue_right_vel;
			}
		} else sprite.setTextureRect(sf::IntRect(0,0,0,0));
		sprite.setOrigin(200.0,200.0);

		if(dt >= -0.2 && dt <= 1.0) {
			//position and scale calculations
			sf::Vector2f pos = m_start + m_vel*(1.0f-dt);
			sf::Vector2f scl = m_scl_start +m_scl_vel*(1.0f-dt);

			//sprite drawn on screen
			if(v.at(i).getRender() == true) {
				sprite.setScale(scl);
				sprite.setPosition(pos);
				m_window.draw(sprite);
			}
		}

	}
	*/
}

void Rendr::lanes(double time, std::vector<Note>& ev) {

	std::vector<float> greenLaneVector = {};
	std::vector<unsigned int> greenLaneIndices = {};
	unsigned int greenLaneVertexCount = 0;

	std::vector<float> blueLaneVector = {};
	std::vector<unsigned int> blueLaneIndices = {};
	unsigned int blueLaneVertexCount = 0;

	std::vector<float> redLaneVector = {};
	std::vector<unsigned int>redLaneIndices = {};
	unsigned int redLaneVertexCount = 0;

	pushVertexColor(redLaneVector, -0.02f, 0.0f, 3.75f, 1.0f, 0.0f, 0.0f);
	pushVertexColor(redLaneVector, 0.02f, 0.0f, 3.75f, 1.0f, 0.0f, 0.0f);
	pushVertexColor(redLaneVector, 0.02f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	pushVertexColor(redLaneVector, -0.02f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	pushRectangleIndices(redLaneIndices, redLaneVertexCount);

	glBindVertexArray(m_lanesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lanesVBO);

	glBufferData(GL_ARRAY_BUFFER, redLaneVector.size() * sizeof(float), redLaneVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, redLaneIndices.size() * sizeof(int), redLaneIndices.data(), GL_DYNAMIC_DRAW);

	glUseProgram(m_ColorProgram);
	glDrawElements(GL_TRIANGLES, redLaneIndices.size(), GL_UNSIGNED_INT, 0);

	int start = 1;
	for (size_t i = 0; i < ev.size(); i++) {
		if (ev.at(i).getMilli() <= time) {
			if (ev.at(i).getType() == CROSS_L) {
				pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, 3.75f, 0.0f, 1.0f, 0.0f);
				pushVertexColor(greenLaneVector, 0.02f - 0.7f, 0.0f, 3.75f, 0.0f, 1.0f, 0.0f);

				pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, 3.75f, 0.0f, 0.0f, 1.0f);
				pushVertexColor(blueLaneVector, 0.02f + 0.35f, 0.0f, 3.75f, 0.0f, 0.0f, 1.0f);
				start = 0;
				break;
			}
			else if (ev.at(i).getType() == CROSS_R) {
				pushVertexColor(greenLaneVector, -0.02f - 0.35f, 0.0f, 3.75f, 0.0f, 1.0f, 0.0f);
				pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, 3.75f, 0.0f, 1.0f, 0.0f);

				pushVertexColor(blueLaneVector, -0.02f + 0.7f, 0.0f, 3.75f, 0.0f, 0.0f, 1.0f);
				pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, 3.75f, 0.0f, 0.0f, 1.0f);
				start = 2;
				break;
			}
			else {
				pushVertexColor(greenLaneVector, -0.02f - 0.35f, 0.0f, 3.75f, 0.0f, 1.0f, 0.0f);
				pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, 3.75f, 0.0f, 1.0f, 0.0f);

				pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, 3.75f, 0.0f, 0.0f, 1.0f);
				pushVertexColor(blueLaneVector, 0.02f + 0.35f, 0.0f, 3.75f, 0.0f, 0.0f, 1.0f);
				start = 1;
				break;
			}
		}
	}

	int middle = start;
	float offset = 0.04f;

	for (size_t i = 0; i < ev.size(); i++) {
		if (ev.at(i).getRender()) {
			if (ev.at(i).getType() == CROSS_L) {
				double dt = ev.at(i).getMilli() - time;
				float z = 3.5f - 3.5f * dt;
				if (middle >= 1) {
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, -0.02f - 0.35f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);
					
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);
					
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, 0.02f - 0.7f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
				}
				if (middle == 2) {
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, -0.02f + 0.7f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);
					
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);
					
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, 0.02f + 0.35f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
				}
				middle = 0;
			}
			else if (ev.at(i).getType() == CROSS_R)
			{
				double dt = ev.at(i).getMilli() - time;
				float z = 3.5f - 3.5f * dt;

				if (middle <= 1) {
					pushVertexColor(blueLaneVector, 0.02f + 0.35f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);
					
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);


					pushVertexColor(blueLaneVector, -0.02f + 0.7f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
				}
				if (middle == 0) {
					pushVertexColor(greenLaneVector, 0.02f - 0.7f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, -0.02f - 0.35f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
				}
				middle = 2;
			}
			else {
				double dt = ev.at(i).getMilli() - time;
				float z = 3.5 - 3.5 * dt;
				
				if (middle == 0) {
					pushVertexColor(greenLaneVector, 0.02f - 0.7f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, z + offset, 0.0f, 1.0f, 0.0f);

					pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

					pushVertexColor(greenLaneVector, -0.02f - 0.35f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
					pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, z - offset, 0.0f, 1.0f, 0.0f);
				}
				else if (middle == 2) {
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, -0.02f + 0.7f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount); 

					pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, z + offset, 0.0f, 0.0f, 1.0f);

					pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

					pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
					pushVertexColor(blueLaneVector, 0.02f + 0.35f, 0.0f, z - offset, 0.0f, 0.0f, 1.0f);
				}
				middle = 1;
			}
		}
	}

	int end = 1;
	for (size_t i = 0; i < ev.size(); i++) {
		if (ev.at(i).getMilli() <= time + 1.0f) {
			if (ev.at(i).getType() == CROSS_L) {
				end = 0;
			}
			else if (ev.at(i).getType() == CROSS_R) {
				end = 2;
			}
			else {
				end = 1;
			}
		}
	}

	if (end == 0) {
		pushVertexColor(greenLaneVector, 0.02f - 0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		pushVertexColor(greenLaneVector, -0.02f - 0.7f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		pushVertexColor(blueLaneVector, 0.02f + 0.35f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (end == 2) {
		pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		pushVertexColor(greenLaneVector, -0.02f - 0.35f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		pushVertexColor(blueLaneVector, 0.02f + 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		pushVertexColor(blueLaneVector, -0.02f + 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (end == 1) {
		pushVertexColor(greenLaneVector, 0.02f - 0.35f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		pushVertexColor(greenLaneVector, -0.02f - 0.35f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		pushVertexColor(blueLaneVector, 0.02f + 0.35f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		pushVertexColor(blueLaneVector, -0.02f + 0.35f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	pushRectangleIndices(greenLaneIndices, greenLaneVertexCount);

	pushRectangleIndices(blueLaneIndices, blueLaneVertexCount);

	glBufferData(GL_ARRAY_BUFFER, greenLaneVector.size() * sizeof(float), greenLaneVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, greenLaneIndices.size() * sizeof(int), greenLaneIndices.data(), GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLES, greenLaneIndices.size(), GL_UNSIGNED_INT, 0);

	glBufferData(GL_ARRAY_BUFFER, blueLaneVector.size() * sizeof(float), blueLaneVector.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, blueLaneIndices.size() * sizeof(int), blueLaneIndices.data(), GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLES, blueLaneIndices.size(), GL_UNSIGNED_INT, 0);
}

void Rendr::events(double time, std::vector<Note>& ev) {
	/*
	for(size_t i = 0; i < ev.size(); ++i) {
		int type = ev.at(i).getType();
		float diff_start = ev.at(i).getMilli()-time;
		if(type == SCR_G_START) {
			float diff_end = -1;
			for(size_t j = 0; j < ev.size(); ++j) {
				if(ev.at(j).getType()==SCR_G_END) {
					diff_end = ev.at(j).getMilli()-time;
					break;
				}
			}
			sf::VertexArray varr(sf::Quads,4);
			for(size_t i = 0; i < varr.getVertexCount(); ++i)varr[i].color = sf::Color(57,172,64);

			sf::Vector2f start_l = green_center_start + sf::Vector2f(-23.0,0.0);
			sf::Vector2f end_l = green_center_end + sf::Vector2f(-40.0,0.0);
			sf::Vector2f start_r = green_center_start + sf::Vector2f(23,0.0);
			sf::Vector2f end_r = green_center_end + sf::Vector2f(40.0,0.0);

			if(m_ren_cross == 0){
				start_l = green_left_start + sf::Vector2f(-23.0,0.0);
				end_l = green_left_end + sf::Vector2f(-40.0,0.0);
				start_r =green_left_start + sf::Vector2f(23,0.0);
				end_r = green_left_end+  sf::Vector2f(40.0,0.0);
			}

			sf::Vector2f vel_l = (end_l-start_l)/1.0f;
			sf::Vector2f vel_r = (end_r-start_r)/1.0f;

			if(diff_end != -1 && diff_end >= 0.0 && diff_start <= 1.0) {
				if(diff_start <= 1.0 && diff_start >= 0.0) {
					varr[2].position = start_r+vel_r*(1.0f-diff_start);
					varr[3].position = start_l+vel_l*(1.0f-diff_start);
				} else {
					varr[2].position = end_r;
					varr[3].position = end_l;
				}
				if(diff_end <= 1.0 && diff_end >= 0.0) {
					varr[0].position = start_l+vel_l*(1.0f-diff_end);
					varr[1].position = start_r+vel_r*(1.0f-diff_end);
				} else {
					varr[0].position = start_l;
					varr[1].position = start_r;
				}
			}

			m_window.draw(varr);
		} else if(type == SCR_B_START) {
			float diff_end = -1;
			for(size_t j = 0; j < ev.size(); ++j) {
				if(ev.at(j).getType()==SCR_B_END) {
					diff_end = ev.at(j).getMilli()-time;
					break;
				}
			}
			sf::VertexArray varr(sf::Quads,4);
			for(size_t i = 0; i < varr.getVertexCount(); ++i)varr[i].color = sf::Color(40,51,177);

			sf::Vector2f start_l = blue_center_start + sf::Vector2f(-23.0,0.0);
			sf::Vector2f end_l = blue_center_end + sf::Vector2f(-40.0,0.0);
			sf::Vector2f start_r = blue_center_start + sf::Vector2f(23,0.0);
			sf::Vector2f end_r = blue_center_end + sf::Vector2f(40.0,0.0);

			if (m_ren_cross == 2){
				start_l = blue_right_start + sf::Vector2f(-23.0,0.0);
				end_l = blue_right_end + sf::Vector2f(-40.0,0.0);
				start_r = blue_right_start + sf::Vector2f(23,0.0);
				end_r = blue_right_end + sf::Vector2f(40.0,0.0);
			}

			sf::Vector2f vel_l = (end_l-start_l)/1.0f;
			sf::Vector2f vel_r = (end_r-start_r)/1.0f;

			if(diff_end != -1 && diff_end >= 0.0 && diff_start <= 1.0) {
				if(diff_start <= 1.0 && diff_start >= 0.0) {
					varr[2].position = start_r+vel_r*(1.0f-diff_start);
					varr[3].position = start_l+vel_l*(1.0f-diff_start);
				} else {
					varr[2].position = end_r;
					varr[3].position = end_l;
				}
				if(diff_end <= 1.0 && diff_end >= 0.0) {
					varr[0].position = start_l+vel_l*(1.0f-diff_end);
					varr[1].position = start_r+vel_r*(1.0f-diff_end);
				} else {
					varr[0].position = start_l;
					varr[1].position = start_r;
				}
			}

			m_window.draw(varr);
		}
	}
	*/
}

void Rendr::pollState(double time, Player& p, Generator& g) {
	/*
	m_time_txt.setString("Time:"+std::to_string(time));
	m_score_txt.setString("Score:"+std::to_string(p.getScore()));
	m_combo_txt.setString("Combo:"+std::to_string(p.getCombo()));
	m_mult_txt.setString("Mult:"+std::to_string(p.getMult()));
	*/
	m_red = p.m_red;
	m_blue = p.m_blue;
	m_green = p.m_green;
	m_player_cross = p.m_cross;
}


Rendr::~Rendr() {
	//dtor
}
