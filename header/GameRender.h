#ifndef RENDR_H
#define RENDR_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Note.h"
#include "Player.h"
#include "Generator.h"
#include <vector>
#include <iostream>
#include <string>
#include <map>

class GameRender {
public:
    GameRender();
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
    virtual ~GameRender();
	
protected:

private:
	void renderTexture(std::vector<float>& vArr, std::vector<unsigned int>& iArr, unsigned int t);
	void renderColor(std::vector<float>& vArr, std::vector<unsigned int>& iArr);
	void renderText(std::vector<float>& vArr, std::vector<unsigned int>& iArr,unsigned int t);
	void drawText(const char* text, float x, float y, float scl);
	void usePersProj();
	void useOrthoProj();
	void setTextColor(float r, float g, float b, float a);

	GLFWwindow* m_window = nullptr;
	bool m_red = false, m_green = false, m_blue = false;
    int m_playerCross = 1;
	int m_playerCombo = 0;
	int m_playerMult = 0;
	int m_playerScore = 0;
	int m_renderCross = 1;
	float m_renderEuValue = 0.0;
	bool m_renderEuActive = false;
	bool m_renderEuZone = false;

	glm::mat4 m_persProj;
	glm::mat4 m_orthoProj;

	unsigned int m_textureProgram = 0;
	unsigned int m_colorProgram = 0;
	unsigned int m_textProgram = 0;
	unsigned int m_highwayTexture = 0;
	unsigned int m_objTexture = 0;
	unsigned int m_metersTexture = 0;
	unsigned int m_splashTexture = 0;
	unsigned int m_fontTexture = 0;
	unsigned int m_textureVAO = 0;
	unsigned int m_textureVBO = 0;
	unsigned int m_colorVAO = 0;
	unsigned int m_colorVBO = 0;


	FT_Library m_FTLibrary;
	FT_Face m_font;
	
};



#endif // RENDR_H
