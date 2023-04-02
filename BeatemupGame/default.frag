#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;





// Gets the Texture Unit from the main function
uniform sampler2D diffuse0;

uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;



//The vec4 used for creating a light that acts like a lamp or other general light source
vec4 pointLight()
{
	vec3 lightVec = lightPos - crntPos;
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	float ambient = 0.20f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);

	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse + inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}


//The vec4 use to basically recreate the sun and provide blanket light in one general direction
vec4 directLight()
{
	float ambient = 0.20f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));

	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}


//The vec4 used to create a flashlight or other small circle of light.
//For a retro flashlight look, you could rewrite this with only one cone annd it will look different.
vec4 spotLight()
{
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	float ambient = 0.20f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);

	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;

}



void main()
{
	//The line for if I want to use the only current light source as point lighting.
	FragColor = pointLight();

	//The line for if I want to use the only current light source as direct lighting.
	//FragColor = directLight();

	//The line for if I want to use the only current light source as spotlighting.
	//FragColor = spotLight();
}


