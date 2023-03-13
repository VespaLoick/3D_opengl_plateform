#include "Model.h"
#include "stb_image.h"
#include "objectLoader.h"

Model::Model(const char* cheminobj, const char* chemintexutre, GLuint& programme_id , glm::vec3 coordonner)
{

	m_matriceID = glGetUniformLocation(programme_id, "MVP");

	std::vector<glm::vec3> lesvertices;
	std::vector<glm::vec3> lesnormales;
	std::vector<glm::vec2> lesUVs;
	m_coordoner = glm::mat4(1.0f); 
	m_coordoner = glm::translate(m_coordoner, coordonner);

	loadOBJ(cheminobj, lesvertices, lesUVs, lesnormales);

	glGenVertexArrays(1, &m_vertexID);
	glBindVertexArray(m_vertexID);

	Nbarray = lesnormales.size() * sizeof(glm::vec3);
	
	glGenBuffers(1, &m_vertex); // G�n�re un tampon et place l'identifiant dans 'vertexbuffer'
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex); // Les commandes suivantes vont parler de notre tampon 'vertexbuffer'
	glBufferData(GL_ARRAY_BUFFER, lesvertices.size() * sizeof(glm::vec3), &lesvertices[0], GL_STATIC_DRAW); // Fournit les sommets � OpenGL.

	glGenBuffers(1, &m_couleur);
	glBindBuffer(GL_ARRAY_BUFFER, m_couleur);
	glBufferData(GL_ARRAY_BUFFER, lesnormales.size() * sizeof(glm::vec3), &lesnormales[0], GL_STATIC_DRAW); // Fournit les sommets � OpenGL.

	glGenBuffers(1, &m_UV);
	glBindBuffer(GL_ARRAY_BUFFER, m_UV);
	glBufferData(GL_ARRAY_BUFFER, lesUVs.size() * sizeof(glm::vec3), &lesUVs[0], GL_STATIC_DRAW); // Fournit les sommets � OpenGL.

	glGenBuffers(1, &m_NormalBuff); // G�n�re un tampon et place l'identifiant dans 'vertexbuffer'
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuff); // Les commandes suivantes vont parler de notre tampon 'vertexbuffer'
	glBufferData(GL_ARRAY_BUFFER, lesnormales.size() * sizeof(glm::vec3), &lesnormales[0], GL_STATIC_DRAW); // Fournit les sommets � OpenGL.


	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuff);
	glVertexAttribPointer(
		2, // attribut
		3, // taille
		GL_FLOAT, // type
		GL_FALSE,
		0, // nombre d’octets séparant deux sommets dans le tampon
		(void*)0 // décalage du tableau de tampons
	);

	glGenTextures(1, &m_laTexture);
	// "Lie" la nouvelle texture cr��e : tous les appels suivants aux fonctions de texture vont modifier cette texture
	glBindTexture(GL_TEXTURE_2D, m_laTexture);

	// R�p�te si ne prend pas toute la face.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Lorsque l'on r�tr�cit l'image, utiliser un fondu lin�aire entre deux MIP maps, chacune �tant aussi filtr�e lin�airement
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Lorsque l'on agrandit l'image (aucune MIP map plus grande n'est disponible), utiliser le filtrage LIN�AIRE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(chemintexutre, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// G�n�rer les MIP maps. 
		glGenerateMipmap(GL_TEXTURE_2D);

		// Initialise "myTextureSampler"
		m_textureID = glGetUniformLocation(programme_id, "maTexture");
	}
	else
	{
		printf("Failed to load texture");
	}
}

Model::~Model()
{}

void Model::chargerbuffeur()
{

	// Lie texture � GL_TEXTURE0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_laTexture);
	// Lie "myTextureSampler" � GL_TEXTURE0
	glUniform1i(m_textureID, 0);

	// Premier tampon d'attributs : les sommets
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex);
	glVertexAttribPointer(
		0,                  // attribut 0. Aucune raison particuli�re pour 0, mais cela doit correspondre au � layout � dans le shader 
		3,                  // taille
		GL_FLOAT,           // type 
		GL_FALSE,           // normalis�
		0,                  // nombre d'octets s�parant deux sommets dans le tampon
		(void*)0            // d�calage du tableau de tampon
	);

	// deuxi�me tampon d'attributs : Couleurs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_couleur);
	glVertexAttribPointer(
		1,
		3,                                // taille
		GL_FLOAT,                         // type
		GL_FALSE,
		0,                                // nombre d'octets s�parant deux sommets dans le tampon
		(void*)0                          // d�calage du tableau de tampons
	);

	// troisi�me tampon d'attributs : UVs
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_UV);
	glVertexAttribPointer(
		2,
		2,                                // taille : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,
		0,                                // nombre d'octets s�parant deux sommets dans le tampon
		(void*)0                          // d�calage du tableau de tampons
	);

	// Dessine le Triangle
	glDrawArrays(GL_TRIANGLES, 0, Nbarray); 
	//glDrawArrays(GL_TRIANGLES, 0, 10);
	glUniform1i(m_textureID, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


void Model::CaBouge(GLFWwindow* fenetre, double diftemps)
{

	if (glfwGetKey(fenetre, GLFW_KEY_M) == GLFW_PRESS)
	{
		m_coordoner = glm::translate(m_coordoner, glm::vec3(diftemps, 0, 0));
	}
	

	if (glfwGetKey(fenetre, GLFW_KEY_P) == GLFW_PRESS && pressp<=0)
	{
		Nbarray -= 1;
		pressp = 10;
	}


	
	if (pressp > 0)
	{
		pressp += diftemps;
	}

}
void Model::SetCoordoner(glm::vec3 coordoner, float angle)
{
	m_coordoner = glm::mat4(1.0f);
	m_coordoner = glm::translate(m_coordoner, coordoner);
	m_coordoner = glm::rotate(m_coordoner, angle , glm::vec3(0,1,0)) ;
}