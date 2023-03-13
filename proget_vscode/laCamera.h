#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/matrix_decompose.hpp"

#include "objectLoader.h"
#include "shadeur.h"
#include "Model.h"
#include "LeSol.h"

#pragma once
class LaCamera
{
public:

	LaCamera(GLuint& programme_id ,glm::vec3 pos, float angeVer , float angleHor, glm::vec3 axerot = glm::vec3(0, 1, 0));
	LaCamera(glm::mat4 vue);
	~LaCamera();

	inline glm::mat4 getVue() { return m_vue; }
	void Bouge(GLFWwindow* fenetre, std::vector<float>& tabSol , Model& Protagonist , LeSol& Platform); // return la matrice du protagonist
	glm::vec3 GetVuePos();
	inline GLuint GetVuePosID() { return VuePosID; }
	
private:
	glm::vec3 m_pos, m_dir, m_axerot , m_direction ,  m_Protagonsit_pos;
	glm::mat4 m_vue;

	float saut;
	float force_saut = 5;
	float vitesse_chute = -5;
	float Gravity = 9.8;
	float hitbox = 0.4;

	double prestemps, actemps;
	float m_angeVer, m_angleHor , m_angleHor_protagonist ;
	GLuint VuePosID;
};


