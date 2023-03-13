#include "LeSol.h"
#include<iostream>
#include<cstdlib>

LeSol::LeSol(GLuint& programme_id , std::vector<float> &SolTableau, unsigned int tailleLine)
{
	SolPhysique = SolTableau;
	M_tailleLine = tailleLine;
	M_programme_id = programme_id;
	int line = 0; 
	int colone = 0;
	
	for (int i = 0; i < SolTableau.size(); i++)
	{
		if (line >= M_tailleLine)
		{
			line = 0;
			colone++;
		}
		SolModel.push_back( 
			Model("resources/leCube.obj",
			"resources/container.jpg",
			programme_id, glm::vec3( (float)colone*2, SolTableau[i] , (float)line*2  ) ) );
		line++;
	}
}

float LeSol::GetHauteur(float coorx, float coory)
{
	std::cout << static_cast<int>(coorx/2 +0.5) << std::endl;
	int tab_value = static_cast<int>(coorx/2 + 0.5) + static_cast<int>(coory/2 + 0.5) * M_tailleLine ;
	if (tab_value >= 0 and tab_value < SolPhysique.size() )
	{
		return SolPhysique[tab_value]+2.0f;
	}
	return 1.0f;
}

void LeSol::RenduDuSol(GLuint& matrice_id, glm::mat4 CamPos , glm::mat4 projection)
{
	for (int i = 0; i < SolModel.size(); i++)
	{
		glm::mat4 MVP = projection * CamPos * SolModel[i].GetCoordoner();
		glUniformMatrix4fv(matrice_id, 1, GL_FALSE, &MVP[0][0]);
		SolModel[i].chargerbuffeur();
	}
}