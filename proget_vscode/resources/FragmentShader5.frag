#version 330 core

// Valeurs interpol�es � partir du vertex shader
in vec3 fragmentPosition;
in vec3 fragmentColor;
in vec2 fragmentUV;
in vec3 fragmentNormal;

// Donn�es de sortie
out vec3 color;

// Valeurs qui sont constantes pour l'ensemble du mod�le.
uniform sampler2D maTexture;
uniform bool isTexture;
uniform vec3 LumCouleur;
uniform vec3 LumPos;
uniform vec3 VuePos;

void main()
{
	// Lumi�re Ambiante
	float ambiantFactor = 0.5f;
	vec3 ambiant = LumCouleur * ambiantFactor;

	// Lumi�re Diffuse
	vec3 normal = normalize(fragmentNormal);
	//vec3 lightDir = normalize(LumPos - fragmentPosition);
	vec3 lightDir = normalize(VuePos - fragmentPosition);
	float NDotL = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = LumCouleur * NDotL;

	// Lumi�re Sp�culaire
	float specularFactor = 0.8f;
	float shininess = 32.0f;
	vec3 viewDir = normalize(VuePos - fragmentPosition);
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = LumCouleur * specularFactor * pow(NDotH, shininess);

	vec3 result = ambiant ;//+ diffuse + specular;
	
	if(!isTexture) 
	{
		// Output color = couleur sp�cifi� dans le vertex shader interpol� entre les trois sommets alentours
		color = ( fragmentColor + vec3 (1,1,1) )/2 ;//result *
	}
	else 
	{
		// Output color = couleur de la texture pour les coordonn�es UV sp�cifi�es
		vec3 texel = texture(maTexture, fragmentUV).rgb;
		color =  result * texel;
	}
}