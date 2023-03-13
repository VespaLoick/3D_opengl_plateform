#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>

#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "objectLoader.h"
#include "shadeur.h"
#include "laCamera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Model.h"
#include "LeSol.h"

int main()
{		
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // antialiasing 4x (4 fragment dans un pixel)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Version OpenGL 3.3 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Pas de support de l'ancien OpenGL

	// Ouvre une fen�tre et cr�e son contexte OpenGL
	
	GLFWwindow* window = glfwCreateWindow(1024, 768, "C++ Jeux 3D - OpenGL", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialise GLEW
	glewExperimental = true; // N�cessaire pour le profil core
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	// Permet d'utiliser la touche Echap
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Fond d'�cran Bleu-Noir
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Active le test de profondeur
	glEnable(GL_DEPTH_TEST);
	// Accepte le fragment s'il est plus proche de la cam�ra que le pr�c�dent accept�
	glDepthFunc(GL_LESS);

	// Cr�e et compile notre programme GLSL � partir des shaders
	GLuint programID = LoadShaders("resources/VertexShader5.vert", "resources/FragmentShader5.frag");

	// Identifiant pou notre variable uniforme "MVP"
	// Seulement au moment de l'initialisation.
	GLuint matrixID = glGetUniformLocation(programID, "MVP");

	// Texture OpenGL
	GLuint texture;
	// Cr�e une texture OpenGL
	glGenTextures(1, &texture);
	// "Lie" la nouvelle texture cr��e : tous les appels suivants aux fonctions de texture vont modifier cette texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// R�p�te si ne prend pas toute la face.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Lorsque l'on r�tr�cit l'image, utiliser un fondu lin�aire entre deux MIP maps, chacune �tant aussi filtr�e lin�airement
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Lorsque l'on agrandit l'image (aucune MIP map plus grande n'est disponible), utiliser le filtrage LIN�AIRE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::vec3 lumCouleur = glm::vec3(1, 0.5, 1);

	std::vector<glm::vec3> lesvertices;
	std::vector<glm::vec3> lesnormales;
	std::vector<glm::vec2> lesUVs;	

	const char* pathOBJ = "resources/leCube.obj";		
	glm::vec3 posCamera = glm::vec3(5, 0, 5);
	glm::vec3 upcamera = glm::vec3(0, 1, 0);

	std::vector<Model> Tableau_de_model;

	
	Model LeProtagonist("resources/Zuzane.obj", "resources/autrecolor.jpg", programID, glm::vec3(0, 0, 0));

	std::vector<float> GrilleDuSol;
	int largeur_map = 15;


	
	for (int i = 0; i < (largeur_map * largeur_map); i++)
	{
		GrilleDuSol.push_back( static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*2 );
	}
	
	LeSol Plateform(programID, GrilleDuSol , largeur_map) ;




	LaCamera MaCam = LaCamera(programID ,posCamera , 0, 1 , upcamera);

	// Pour une cam�ra orthographique :
	//glm::mat4 projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 view = MaCam.getVue();
	glm::mat4 MVP = projection * view * LeProtagonist.GetCoordoner();
	

	GLuint LumCouleurID = glGetUniformLocation(programID, "LumCouleur");
	glm::vec3 LumCouleur = glm::vec3(1.0f,0,0);
	GLuint LumPosID = glGetUniformLocation(programID, "LumPos");
	glm::vec3 LumPos = glm::vec3(500, 0, 0);
	GLuint VuePosID = glGetUniformLocation(programID, "VuePos");


	double prestemps;
	double actemps = glfwGetTime();
	double diftemps;
	do
	{
		// Nettoie l'�cran
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		prestemps = actemps;
		actemps = glfwGetTime();
		diftemps = actemps - prestemps;

		MaCam.Bouge(window, GrilleDuSol, LeProtagonist , Plateform) ;
		view = MaCam.getVue();

		// Utilise notre shader
		glUseProgram(programID);

		// Envoie notre transformation au shader dans la variable uniforme "MVP"
		// Pour chaque mod�le, la MVP sera diff�rente (au moins la partie M)


		MVP = projection * view * LeProtagonist.GetCoordoner();
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
		LeProtagonist.chargerbuffeur();

		Plateform.RenduDuSol(matrixID, view, projection);

		// Lie texture � GL_TEXTURE0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniform3fv(LumCouleurID, 1, &LumCouleur[0]);
		glUniform3fv(LumPosID, 1, &LumPos[0]);
		glm::vec3 maVue_ici = MaCam.GetVuePos();
		glUniform3fv(VuePosID, 1, &maVue_ici[0]);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// �change les tampons
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // V�rifie si la touche �chap a �t� appuy�e ou si la fen�tre a �t� ferm�e
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	return 0;
}