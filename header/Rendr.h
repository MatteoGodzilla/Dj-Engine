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
	void bpmTicks(double time, std::vector<double>& bpm_arr);
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
	int m_playerMult = 0;
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
	unsigned int m_textureVAO = 0;
	unsigned int m_textureVBO = 0;
	unsigned int m_colorVAO = 0;
	unsigned int m_colorVBO = 0;
};

#endif // RENDR_H
