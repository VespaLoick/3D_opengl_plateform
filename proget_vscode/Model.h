#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>

#include <chrono>
#include <ctime>

#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"



#pragma once


class Model
{
public:
	Model(const char* cheminobj, const char* chemintexutre, GLuint& programme_id, glm::vec3 coordonner = glm::vec3(-1, 0,-1));
	//Model(int graine, const char* chemintexutre, GLuint& programme_id, glm::vec3 coordonner = glm::vec3(0, 0, 0));
	~Model();
	inline glm::mat4 GetCoordoner() {return m_coordoner; }
	void CaBouge(GLFWwindow* fenetre , double diftemps);
	void chargerbuffeur();
	void SetCoordoner(glm::vec3 coordoner, float angle);
private:
	GLuint m_vertexID , m_vertex, m_couleur, m_UV, m_textureID , m_laTexture, m_matriceID , m_NormalBuff;
	int Nbarray;
	glm::mat4 m_coordoner;
	
	double pressp =0;
};

