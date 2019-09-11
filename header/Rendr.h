#ifndef RENDR_H
#define RENDR_H
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include "Note.h"
#include "Player.h"
#include "Generator.h"
#include <vector>
#include <iostream>

class Rendr {
public:
    Rendr();
	void init(GLFWwindow* w);
	void highway(double time);
    void clicker();
    void notes(double time,std::vector<Note> &v);
    void events(double time,std::vector<Note>&ev);
    void lanes(double time, std::vector<Note>&ev);
	void meters();
	void splash();
    void pollState(double time,Player& p,Generator &g);
    virtual ~Rendr();

protected:

private:
	GLFWwindow* m_window = nullptr;
	bool m_red = false, m_green = false, m_blue = false;
    int m_playerCross = 1;
	int m_playerCombo = 0;
	int m_renderCross = 1;
	float m_renderEuValue = 0.0;
	bool m_renderEuActive = false;
	bool m_renderEuZone = false;

	glm::mat4 m_proj;

	unsigned int m_TextureProgram = 0;
	unsigned int m_ColorProgram = 0;
	unsigned int m_HighwayTexture = 0;
	unsigned int m_ObjTexture = 0;
	unsigned int m_MetersTexture = 0;
	unsigned int m_splashTexture = 0;
	unsigned int m_highwayVAO = 0;
	unsigned int m_highwayVBO = 0;
	unsigned int m_lanesVAO = 0;
	unsigned int m_lanesVBO = 0;
	unsigned int m_notesVAO = 0;
	unsigned int m_notesVBO = 0;
	unsigned int m_clickerVAO = 0;
	unsigned int m_clickerVBO = 0;
	unsigned int m_eventVAO = 0;
	unsigned int m_eventVBO = 0;
	unsigned int m_metersVAO = 0;
	unsigned int m_metersVBO = 0;
	/*
	sf::Texture m_tex;

	sf::Sprite m_trayL,m_trayR;
	sf::Sprite m_red_click,m_green_click,m_blue_click;

	sf::Font m_font;
	sf::Text m_time_txt;
	sf::Text m_score_txt;
	sf::Text m_combo_txt;
	sf::Text m_mult_txt;

	sf::Vector2f m_start;
	sf::Vector2f m_end;
	sf::Vector2f m_vel;
	sf::Vector2f m_scl_start;
	sf::Vector2f m_scl_end;
	sf::Vector2f m_scl_vel;

    sf::Vector2f green_center_start = sf::Vector2f(472.0,200.0);
    sf::Vector2f green_center_end = sf::Vector2f(426.0,500.0);
    sf::Vector2f green_left_start = sf::Vector2f(436.0,200.0);
    sf::Vector2f green_left_end = sf::Vector2f(338.0,500.0);

    sf::Vector2f blue_center_start = sf::Vector2f(554.0,200.0);
    sf::Vector2f blue_center_end = sf::Vector2f(600.0,500.0);
    sf::Vector2f blue_right_start = sf::Vector2f(590.0,200.0);
    sf::Vector2f blue_right_end = sf::Vector2f(688.0,500.0);

    sf::Vector2f green_center_vel = (green_center_end-green_center_start)/1.0f;
    sf::Vector2f green_left_vel = (green_left_end-green_left_start)/1.0f;

    sf::Vector2f blue_center_vel = (blue_center_end-blue_center_start)/1.0f;
    sf::Vector2f blue_right_vel = (blue_right_end-blue_right_start)/1.0f;
	*/
};

#endif // RENDR_H
