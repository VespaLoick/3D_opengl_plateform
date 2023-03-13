#pragma once
#include <vector>
#include "Model.h"
class LeSol
{
public:
	LeSol(GLuint& programme_id ,std::vector<float> &SolTableau, unsigned int tailleLine = 5);
	float GetHauteur(float coorx, float coory);
	void RenduDuSol(GLuint& matrice_id , glm::mat4 CamPos, glm::mat4 projection);
	
private:
	std::vector<Model> SolModel;
	std::vector<float> SolPhysique;
	int M_tailleLine;
	GLuint M_programme_id;

};

