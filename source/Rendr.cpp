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
		std::cout << "OpenGL Error: "<<error << std::endl;
		error = glGetError();
	}
	std::cout << "ended error checking" << std::endl;
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
			"uniform sampler2D t;\n"
			"\n"
			"void main()"
			"{\n"
			"	FragColor = vec4(0.0,0.0,0.0,1.0)+texture(t,tex_coords);\n"
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
		unsigned char *texture = stbi_load("res/highway.png", &width, &height, &channels, 0);

		glGenTextures(1, &m_gl_texture);
		glBindTexture(GL_TEXTURE_2D, m_gl_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

		stbi_image_free(texture);
	}


	//projection init
	{
		glm::mat4 proj = glm::perspective(45.0f, 1024.0f/600,1.0f,-2.0f);
		glm::mat4 look = glm::lookAt(glm::vec3(0.0, 2.0, 5.0), 
			glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 1.0, 0.0));
		proj = proj * look;
		glUseProgram(m_TextureProgram);		
		int location = glGetUniformLocation(m_TextureProgram, "u_proj");
		if (location != -1) {
			glUniformMatrix4fv(location, 1, GL_FALSE, &proj[0][0]);
			std::cout << "uniform texture successful" << std::endl;
		}
		
		glUseProgram(m_ColorProgram);
		location = glGetUniformLocation(m_ColorProgram, "u_proj");
		if (location != -1) {
			glUniformMatrix4fv(location, 1, GL_FALSE, &proj[0][0]);
			std::cout << "uniform color successful" << std::endl;
		}
		
	}
	
	//vao setup
	{
		glGenVertexArrays(1, &m_HighwayVAO);
		glGenVertexArrays(1, &m_lanesVAO);
		
		
		
		{
			glBindVertexArray(m_HighwayVAO);

			unsigned int ebo;
			glGenBuffers(1, &m_HighwayVBO);
			glGenBuffers(1, &ebo);

			glBindBuffer(GL_ARRAY_BUFFER, m_HighwayVBO);
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
    for(int i = 0; i < resolution; ++i){
        m_lanes.push_back(1);
    }

}

void Rendr::highway(double time) {

	float factor = time / 3;
	float data[] = {
			-1.0f, 0.0f, 0.0f, 0.0f, 1.0f + factor,
			 1.0f, 0.0f, 0.0f, 1.0f, 1.0f + factor,
			-1.0f, 0.0f, 6.0f, 0.0f, 0.0f + factor,
			 1.0f, 0.0f, 6.0f, 1.0f, 0.0f + factor
	};

	unsigned int indices[] = {
			0,2,3,
			3,1,0
	};

	glBindVertexArray(m_HighwayVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_HighwayVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
	
	glUseProgram(m_TextureProgram);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);

}

void Rendr::clicker() {

	/*
    if(m_red)m_red_click.setScale(0.15,0.15);
    else m_red_click.setScale(0.2,0.2);

    if(m_green)m_green_click.setScale(0.15,0.15);
    else m_green_click.setScale(0.2,0.2);

    if(m_blue)m_blue_click.setScale(0.15,0.15);
    else m_blue_click.setScale(0.2,0.2);

    if(m_player_cross == 0) {
        m_green_click.setPosition(338.0,500.0);
        m_blue_click.setPosition(600.0,500.0);
    } else if(m_player_cross == 1) {
        m_green_click.setPosition(426.0,500.0);
        m_blue_click.setPosition(600.0,500.0);
    } else if(m_player_cross == 2) {
        m_green_click.setPosition(426.0,500.0);
        m_blue_click.setPosition(688.0,500.0);
    }

    m_window.draw(m_trayL);
    m_window.draw(m_trayR);
    m_window.draw(m_red_click);
    m_window.draw(m_green_click);
    m_window.draw(m_blue_click);
    m_window.draw(m_time_txt);
    m_window.draw(m_score_txt);
    m_window.draw(m_combo_txt);
    m_window.draw(m_mult_txt);
	*/
}

void Rendr::notes(double time,std::vector<Note> &v) {
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
	
	float redLaneVertexData[] = {
		 -0.02f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		  0.02f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		  -0.02f, 0.0f, 6.0f, 1.0f, 0.0f, 0.0f,
		  0.02f, 0.0f, 6.0f, 1.0f, 0.0f, 0.0
	};

	unsigned int indices[] = {
		0,2,3,
		3,1,0
	};

	glBindVertexArray(m_lanesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lanesVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(redLaneVertexData), redLaneVertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
	glUseProgram(m_ColorProgram);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
	
	float greenLaneVertexData[] = {
		 -0.02f-0.35f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		  0.02f-0.35f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		 -0.02f-0.35f, 0.0f, 6.0f, 0.0f, 1.0f, 0.0f,
		  0.02f-0.35f, 0.0f, 6.0f, 0.0f, 1.0f, 0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(greenLaneVertexData), greenLaneVertexData, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

	float blueLaneVertexData[] = {
		 -0.02f+0.35f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		  0.02f+0.35f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 -0.02f+0.35f, 0.0f, 6.0f, 0.0f, 0.0f, 1.0f,
		  0.02f+0.35f, 0.0f, 6.0f, 0.0f, 0.0f, 1.0f
	};
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(blueLaneVertexData), blueLaneVertexData, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

	float leftGreenLaneVertexData[] = {
		 -0.02f - 0.7f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,
		  0.02f - 0.7f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,
		 -0.02f - 0.7f, 0.0f, 6.0f, 0.0f, 0.5f, 0.0f,
		  0.02f - 0.7f, 0.0f, 6.0f, 0.0f, 0.5f, 0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(leftGreenLaneVertexData), leftGreenLaneVertexData, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

	float rightBlueLaneVertexData[] = {
		 -0.02f + 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f,
		  0.02f + 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f,
		 -0.02f + 0.7f, 0.0f, 6.0f, 0.0f, 0.0f, 0.5f,
		  0.02f + 0.7f, 0.0f, 6.0f, 0.0f, 0.0f, 0.5f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(rightBlueLaneVertexData), rightBlueLaneVertexData, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

	

	/*
    std::vector<sf::Vertex> green_lane;
    std::vector<sf::Vertex> blue_lane;

    for(size_t i = 0; i < ev.size(); ++i){
        if(ev.at(i).getRender()){
            float dt = 1.0f-(ev.at(i).getMilli()-time);
            int s = dt*200;
            if(s >= 200)s = 199;

            if(ev.at(i).getType() == CROSS_L){
                for(int j = s;j >= 0; j--){
                    m_lanes.at(j) = 0;
                    m_ren_cross = 0;
                }
            }if(ev.at(i).getType() == CROSS_C){
                for(int j = s;j >= 0; j--){
                    m_lanes.at(j) = 1;
                    m_ren_cross = 1;
                }
            }
            if(ev.at(i).getType() == CROSS_R){
                for(int j = s;j >= 0; j--){
                    m_lanes.at(j) = 2;
                    m_ren_cross = 2;
                }
            }
        }
    }

    for(size_t i = 0 ; i < m_lanes.size(); ++i){
        float perc = (float)(i+1)/resolution;
        if(m_lanes.at(i)==0){
            sf::Vector2f left = green_left_start+green_left_vel*perc;
            sf::Vector2f right = blue_center_start+blue_center_vel*perc;

            sf::Vertex g(left);
            g.color = sf::Color(145,255,150);
            sf::Vertex b(right);
            b.color = sf::Color(35,70,90);

            green_lane.push_back(g);
            blue_lane.push_back(b);
        }else if(m_lanes.at(i)==1){
            sf::Vector2f left = green_center_start+green_center_vel*perc;
            sf::Vector2f right = blue_center_start+blue_center_vel*perc;

            sf::Vertex g(left);
            g.color = sf::Color(145,255,150);
            sf::Vertex b(right);
            b.color = sf::Color(160,240,255);

            green_lane.push_back(g);
            blue_lane.push_back(b);
        }else if(m_lanes.at(i)==2){
            sf::Vector2f left = green_center_start+green_center_vel*perc;
            sf::Vector2f right = blue_right_start+blue_right_vel*perc;

            sf::Vertex g(left);
            g.color = sf::Color(0,112,25);
            sf::Vertex b(right);
            b.color = sf::Color(160,240,255);

            green_lane.push_back(g);
            blue_lane.push_back(b);
        }
    }

    sf::VertexArray red(sf::Lines, 2);
    red[0].position = sf::Vector2f(512.0,200.0);
    red[0].color = sf::Color(255,60,60);
    red[1].position = sf::Vector2f(512.0,500.0);
    red[1].color = sf::Color(255,60,60);

    m_window.draw(&green_lane[0],green_lane.size(),sf::LineStrip);
    m_window.draw(&blue_lane[0],blue_lane.size(),sf::LineStrip);
    m_window.draw(red);
	*/
}

void Rendr::events(double time,std::vector<Note>&ev) {
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

void Rendr::pollState(double time,Player& p,Generator &g) {
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
