	//The guy just deleted all these out of here because I guess they run through Mesh.h now.
	//I don't wanna have to retype all of these.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <io/keyboard.h>
#include <io/mouse.h>
#include <io/joystick.h>
#include <json/json.h>


#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

#include "Mesh.h"
#include "Model.h"



void processInput(GLFWwindow* window);

const unsigned int width = 800;
const unsigned int height = 800;


glm::mat4 mouseTransform = glm::mat4(1.0f);
Joystick mainJ(0);





int main()
{
	//Initializes glfw
	glfwInit();

	//tells glfw what version of opengl we're using'
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	//the actual window (5 inputs; width, height, name of window, toggle Fullscreen, and something they said wasn't important
	GLFWwindow* window = glfwCreateWindow(width,height,"My Own Opengl Window!", NULL, NULL);
	
	//Error checking if the window didn't properly load
	if (window == NULL)
	{
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//glad will actually let us do things within the window, I guess.  just typing out what the guy says
	gladLoadGL();

	//The section of the window that tells glad what to change.  I believe can go outside the window area too.
	glViewport(0,0,width,height);


	//This is about processing inputs from the mouse and keyboard
	glfwSetKeyCallback(window, Keyboard::keyCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);


	


	Shader shaderProgram("default.vert", "default.frag");


	//Lighting lines
	//There's point light, directional light, and spot light
	//point light is similar to a lamp
	//directional light is similar to sunlight
	//Spot light is similar to a flashlight or spotlight at a stageshow



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	//end of lighting lines



	//Texture
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("own_image.png", &widthImg, &heightImg, &numColCh, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	//something about a texture unit.  Don't quite understand it yet.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//next part involves scaling textures up & down. 
		//NEAREST keeps pixel ratio same so good for pixel art
		//LINEAR ends up with a blurrier image, so better for regular artwork where you don't want to see pixels.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");


	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);

	

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	//The line for choosing which model to import
	Model model("Models/Sword/scene.gltf");



	/*mainJ.update();
	if (mainJ.isPresent())
	{
		std::cout << mainJ.getName() << " is present" << std::endl;
	}
	else
	{
		std::cout << " not present" << std::endl;
	}
	*/

	//while loop that actually keeps the window open for longer than a split second.
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		



		//Me trying to implement a model matrix that was pointlessly removed.  Also partly in the vertex shader.
		//Currently this model matrix affects lighting as well, I think because of where I have this code placed.
		/*glm::vec3 testTranslation(0.0f, 0.0f, 0.0f);
		glm::vec3 testTranslationA(0.2f, 0.2f, 0.2f);
		glm::vec3 testTranslationB(-0.2f, -0.2f, -0.2f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, testTranslation);
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		*/
		



		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		//line that draws the chosen model of import
		model.Draw(shaderProgram, camera);


		glfwSwapBuffers(window);

		//makes glfw process polled events such as resizing
		glfwPollEvents();
	}


	//delete all objects we've created
	shaderProgram.Delete();

	//shuts down glfw
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow* window)
{
	if (Keyboard::key(GLFW_KEY_ESCAPE) || mainJ.buttonState(GLFW_JOYSTICK_BTN_RIGHT))
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (Keyboard::key(GLFW_KEY_UP))
	{

	}

	if (Keyboard::key(GLFW_KEY_DOWN))
	{

	}

	if (Keyboard::key(GLFW_KEY_W))
	{
		
	}
	if (Keyboard::key(GLFW_KEY_A))
	{

	}
	if (Keyboard::key(GLFW_KEY_S))
	{

	}
	if (Keyboard::key(GLFW_KEY_D))
	{

	}

	/*mainJ.update();


	//Joystick Inputs

	float rt = mainJ.axesState(GLFW_JOYSTICK_AXES_RIGHT_TRIGGER) / 2 + 0.05f;
	if (rt > 0.05f)
	{

	}

	float lt = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_TRIGGER) / 2 + 0.05f;
	if (lt > 0.05f)
	{

	}

	float lx = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_X);
	float ly = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_Y);

	if (std::abs(lx) > 0.05f)
	{

	}
	if (std::abs(ly) > 0.05f)
	{

	}

	if (mainJ.buttonState(GLFW_JOYSTICK_BTN_DOWN) == GLFW_PRESS)
	{

	}
	if (mainJ.buttonState(GLFW_JOYSTICK_BTN_RIGHT) == GLFW_PRESS)
	{

	}
	*/

}