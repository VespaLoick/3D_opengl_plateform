
#include "laCamera.h"
#include "gtx/string_cast.hpp"

LaCamera::LaCamera(GLuint& programme_id , glm::vec3 pos, float angeVer, float angleHor, glm::vec3 axerot )
{
	m_pos = pos;
	m_angeVer = angeVer;
	m_angleHor = angleHor;
	m_axerot = axerot;
		
	actemps = glfwGetTime();
	VuePosID  = glGetUniformLocation(programme_id, "VuePosition");
}

LaCamera::LaCamera(glm::mat4 vue)
{
	m_vue = vue;
	actemps = glfwGetTime();
}

LaCamera::~LaCamera()
{}

void LaCamera::Bouge(GLFWwindow* fenetre , std::vector<float>& tabSol , Model& Protagonist , LeSol& Platform)
{
	prestemps = actemps;
	actemps=glfwGetTime();
	double diftemps = actemps - prestemps;
		
	if (glfwGetKey(fenetre, GLFW_KEY_UP) == GLFW_PRESS)
	{
		m_angeVer = m_angeVer + diftemps;
	}
	else if (glfwGetKey(fenetre, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		m_angeVer = m_angeVer - diftemps;
	}
	if (glfwGetKey(fenetre, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_angleHor = m_angleHor + diftemps;
	}
	else if (glfwGetKey(fenetre, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_angleHor = m_angleHor - diftemps;
	}
	glm::vec3 direction = glm::vec3(
		 sin(m_angleHor),
		 0,
		 cos(m_angleHor)
	);
	glm::vec3 m_cible = glm::vec3(
		cos(m_angeVer) * sin(m_angleHor),
		sin(m_angeVer),
		cos(m_angeVer) * cos(m_angleHor)
	);
	glm::vec3 m_droit = glm::vec3(
		sin(m_angleHor - 3.14f / 2.0f),
		0,
		cos(m_angleHor - 3.14f / 2.0f)
	);


	m_pos += glm::vec3(0, diftemps*vitesse_vertical, 0);//gravity lineaire
	

	//saut
	if (glfwGetKey(fenetre, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (m_pos.y <= Platform.GetHauteur(m_pos.z, m_pos.x)+0.01 )
		{
			vitesse_vertical = force_saut;
		}	
	}
	if (vitesse_vertical > vitesse_terminal) //chute
	{
		vitesse_vertical -= diftemps*Gravity;
	}	
	//correction si encré dans le sol
	if (m_pos.y < Platform.GetHauteur(m_pos.z, m_pos.x) )
	{
		m_pos = glm::vec3(m_pos.x, Platform.GetHauteur(m_pos.z, m_pos.x) , m_pos.z);
		vitesse_vertical = 0;
	}

	glm::vec3 next_position;
	
	if (glfwGetKey(fenetre, GLFW_KEY_Z) == GLFW_PRESS)
	{
		next_position = m_pos + direction* hitbox;
		if (m_pos.y >= Platform.GetHauteur(next_position.z, next_position.x))//check collision
		{
			m_pos += direction * (float)diftemps;
		}
	}
	else if (glfwGetKey(fenetre, GLFW_KEY_S) == GLFW_PRESS)
	{
		next_position = m_pos - direction * hitbox;
		if (m_pos.y >= Platform.GetHauteur(next_position.z, next_position.x))
		{
			m_pos -= direction * (float)diftemps;
		}
	}
	if (glfwGetKey(fenetre, GLFW_KEY_Q) == GLFW_PRESS)
	{
		next_position = m_pos - m_droit * hitbox;
		if (m_pos.y >= Platform.GetHauteur(next_position.z, next_position.x))
		{
			m_pos -= m_droit * (float)diftemps;
		}
	}
	else if (glfwGetKey(fenetre, GLFW_KEY_D) == GLFW_PRESS)
	{
		next_position = m_pos + m_droit * hitbox;
		if (m_pos.y >= Platform.GetHauteur(next_position.z, next_position.x))
		{
			m_pos += m_droit * (float)diftemps;
		}
	}

	

	glm::vec3 decal_up = glm::vec3(0, 1.5, 0); //decalage vers le haut 

	
	m_vue = glm::lookAt(
		m_pos + decal_up - (m_cible * 8.0f),		// Position de la cam�ra (4,3,3) dans l'espace monde
		m_pos + decal_up,	// Direction de la cam�ra (ce qu'elle regarde)
		m_axerot	// Vecteur Up (utilisez 0,-1,0 pour regarder � l'envers)
	);

	Protagonist.SetCoordoner(m_pos, m_angleHor);

}

glm::vec3 LaCamera::GetVuePos()
{
	return m_vue[3];
}

