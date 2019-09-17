//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int screenWidth;
int screenHeight;

GLFWwindow * window;

Shader shader;
std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
///Esfera gris
Sphere sphere3(20, 20);
///Esfera Negra
Sphere sphere4(20, 20);

Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder12(20, 20, 0.5, 0.5);
Box box1;
Box box2;

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;
///Brazo 1 (izquierdo)
float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
///Brazo 2 (Derecho)
float rot1d = 0.0, rot2d = 0.0, rot3d = 0.0, rot4d = 0.0;
///Pierna 1 (Derecho)
float rot1pd = 0.0, rot2pd = 0.0, rot3pd = 0.0, rot4pd = 0.0;
///Pierna 2 (Izquierdo)
float rot1pi = 0.0, rot2pi = 0.0, rot3pi = 0.0, rot4pi = 0.0;

///Cabeza 
float cb = 0.0;
///Brazo 1 (Izquierdo)
float rot1_2 = 0.0, rot2_2 = 0.0, rot3_2 = 0.0,  rot4_2 = 0.0;
///Brazo 2 (Derecho)
float rot1_2d = 0.0, rot2_2d = 0.0, rot3_2d = 0.0,  rot4_2d = 0.0;
////////////Giro general
float rot0 = 0.0, dz = 0.0;
bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {
	
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shader);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	// Método setter que colocar el apuntador al shader
	sphere2.setShader(&shader);
	//Setter para poner el color de la geometria
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	////Esfera Gris
	sphere3.init();
	sphere3.setShader(&shader);
	sphere3.setColor(glm::vec4(0.4, 0.4, 0.4,0.0));
///Esfera Negra
	sphere4.init();
	sphere4.setShader(&shader);
	sphere4.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));

	cylinder1.init();
	cylinder1.setShader(&shader);
	cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));
	////Segundo cilindro
	cylinder12.init();
	cylinder12.setShader(&shader);
	cylinder12.setColor(glm::vec4(255.0, 255.0, 255.0, 1.0));
	///
	box1.init();
	box1.setShader(&shader);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
	///Caja Pies
	box2.init();
	box2.setShader(&shader);
	box2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	camera->setPosition(glm::vec3(0.0, 0.0, 4.0));
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Destrucción de los VAO, EBO, VBO
	sphere1.destroy();
	cylinder1.destroy();
	cylinder12.destroy();
	box1.destroy();
	box2.destroy();
	sphere3.destroy();
	sphere4.destroy();
	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication){
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, 0.01);
	offsetX = 0;
	offsetY = 0;
	///rotacion Izquierda
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && sentido)
		rot1_2 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !sentido)
		rot1_2 -= 0.01;
	if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && sentido)
		rot2_2 += 0.01;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && sentido)
		rot3_2 += 0.01;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && sentido)
		 rot4_2 += 0.01;
	///rotacion Brazo derecho
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && sentido)
		rot1_2d += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !sentido)
		rot1_2d -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && sentido)
		rot2_2d += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && !sentido)
		rot2_2d -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && sentido)
		rot3_2d += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && !sentido)
		rot3_2d -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && sentido)
		 rot4_2d += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !sentido)
		 rot4_2d -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && sentido)
		cb += 0.01;
	sentido = true;
	////////////////////////////// Gire todo el Cilindro
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		rot0 = 0.01;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		rot0 = -0.01;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		dz = 0.01;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		dz = -0.01;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///rotacion Brazo Izquierdo
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && sentido)
		rot1 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !sentido)
		rot1 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && sentido)
		rot2 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !sentido)
		rot2 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && sentido)
		rot3 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !sentido)
		rot3 -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && sentido)
		rot4 += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !sentido)
		rot4 -= 0.01;
	sentido = true;

	///rotacion Brazo derecho
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && sentido)
		rot1d += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !sentido)
		rot1d -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && sentido)
		rot2d += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && !sentido)
		rot2d -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && sentido)
		rot3d += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && !sentido)
		rot3d -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && sentido)
		rot4d += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && !sentido)
		rot4d -= 0.01;
	sentido = true;

	///rotacion Pierna Derecha
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && sentido)
		rot1pd += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !sentido)
		rot1pd -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && sentido)
		rot2pd += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !sentido)
		rot2pd -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && sentido)
		rot3pd += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !sentido)
		rot3pd -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && sentido)
		rot4pd += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && !sentido)
		rot4pd -= 0.01;
	sentido = true;

	///rotacion Pierna Izquierda
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		sentido = false;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && sentido)
		rot1pi += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !sentido)
		rot1pi -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && sentido)
		rot2pi += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !sentido)
		rot2pi -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && sentido)
		rot3pi += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !sentido)
		rot3pi -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && sentido)
		rot4pi += 0.001;
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !sentido)
		rot4pi -= 0.01;
	sentido = true;

	////////////////////////////// Gire todo el box
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.01;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.01;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.01;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.01;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)//se agregan estas dos lineas para mover la camara, hacia enfrente
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)//se agregan estas dos lineas para mover la camara, hacia atras
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)//se agregan estas dos lineas para mover la camara, hacia la derecha
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)//se agregan estas dos lineas para mover la camara, hacia la izquierda
		camera->moveRightCamera(true, deltaTime);
	sentido = true;
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	///Quitar esta linea fuera del While
	///Figura 1
	glm::mat4 model = glm::mat4(1.0f);
	/// Figura 2
	glm::mat4 model2 = glm::mat4(1.0f);
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////// Figura 1 ///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		/*
		//box1.enableWireMode();
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));

		//articulacion
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));//se agrego esta linea para empezar a modelar a la esponja
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));
		j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
		j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));

		//hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0f, 0.0f));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion 2
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5f, 0.0f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));
		j2 = glm::rotate(j2, rot3, glm::vec3(0, 0, 1));
		j2 = glm::rotate(j2, rot4, glm::vec3(0, 1, 0));

		//hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));

		//ojo 1
		glm::mat4 o1 = glm::translate(model, glm::vec3(0.25f, 0.25f, 0.05f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(o1, glm::vec3(0.20f, 0.20f, 0.10f)));

		//ojo 2
		glm::mat4 o2 = glm::translate(model, glm::vec3(-0.25f, 0.25f, 0.05f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(o2, glm::vec3(0.20f, 0.20f, 0.10f)));

		//Nariz
		glm::mat4 n1 = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.05f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(n1, glm::vec3(0.20f, 0.20f, 0.10f)));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////Parte izquierda//////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//articulacion
		glm::mat4 ji1 = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));//se agrego esta linea para empezar a modelar a la esponja
		sphere1.enableWireMode();
		sphere1.render(glm::scale(ji1, glm::vec3(0.1, 0.1, 0.1)));
		ji1 = glm::rotate(ji1, rot1d, glm::vec3(0, 0, 1));
		ji1 = glm::rotate(ji1, rot2d, glm::vec3(0, 1, 0));

		//hueso 1
		glm::mat4 li1 = glm::translate(ji1, glm::vec3(-0.25f, 0.0f, 0.0f));
		li1 = glm::rotate(li1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(li1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion 2
		glm::mat4 ji2 = glm::translate(ji1, glm::vec3(-0.5f, 0.0f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(ji2, glm::vec3(-0.1, 0.1, 0.1)));
		ji2 = glm::rotate(ji2, rot3d, glm::vec3(0, 0, 1));
		ji2 = glm::rotate(ji2, rot4d, glm::vec3(0, 1, 0));

		//hueso 2
		glm::mat4 li2 = glm::translate(ji2, glm::vec3(-0.25, 0.0, 0.0));
		li2 = glm::rotate(li2, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(li2, glm::vec3(-0.1, 0.5, 0.1)));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////Parte Abajo///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

		//articulacion
		glm::mat4 ja1 = glm::translate(model, glm::vec3(-0.25f, -0.5f, 0.0f));//se agrego esta linea para empezar a modelar a la esponja
		sphere1.enableWireMode();
		sphere1.render(glm::scale(ja1, glm::vec3(-0.1, 0.1, 0.1)));
		ja1 = glm::rotate(ja1, rot1pd, glm::vec3(0, 0, 1));
		ja1 = glm::rotate(ja1, rot2pd, glm::vec3(0, 1, 0));

		//hueso 1
		glm::mat4 la1 = glm::translate(ja1, glm::vec3(-0.015f, -0.25f, 0.0f));
		la1 = glm::rotate(la1, glm::radians(180.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(la1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion 2
		glm::mat4 ja2 = glm::translate(ja1, glm::vec3(-0.015f, -0.5f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(ja2, glm::vec3(0.1, 0.1, 0.1)));
		ja2 = glm::rotate(ja2, rot3pd, glm::vec3(0, 0, 1));
		ja2 = glm::rotate(ja2, rot4pd, glm::vec3(0, 1, 0));
		//hueso 2
		glm::mat4 la2 = glm::translate(ja2, glm::vec3(-0.015f, -0.25f, 0.0f));
		la2 = glm::rotate(la2, glm::radians(180.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(la2, glm::vec3(0.1, 0.5, 0.1)));
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////77
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//articulacion 2
		glm::mat4 jaa1 = glm::translate(model, glm::vec3(0.25f, -0.5f, 0.0f));//se agrego esta linea para empezar a modelar a la esponja
		sphere1.enableWireMode();
		sphere1.render(glm::scale(jaa1, glm::vec3(-0.1, 0.1, 0.1)));
		jaa1 = glm::rotate(jaa1, rot1pi, glm::vec3(0, 0, 1));
		jaa1 = glm::rotate(jaa1, rot2pi, glm::vec3(0, 1, 0));

		//hueso 12
		glm::mat4 laa1 = glm::translate(jaa1, glm::vec3(0.015f, -0.25f, 0.0f));
		laa1 = glm::rotate(laa1, glm::radians(180.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(laa1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion 22
		glm::mat4 jaa2 = glm::translate(jaa1, glm::vec3(0.015f, -0.5f, 0.0f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(jaa2, glm::vec3(0.1, 0.1, 0.1)));
		jaa2 = glm::rotate(jaa2, rot3pi, glm::vec3(0, 0, 1));
		jaa2 = glm::rotate(jaa2, rot4pi, glm::vec3(0, 1, 0));

		//hueso 22
		glm::mat4 laa2 = glm::translate(jaa2, glm::vec3(0.015f, -0.25f, 0.0f));
		laa2 = glm::rotate(laa2, glm::radians(180.0f), glm::vec3(0, 0, 1.0));
		cylinder1.enableWireMode();
		cylinder1.render(glm::scale(laa2, glm::vec3(0.1, 0.5, 0.1)));

		shader.turnOff();
		dz = 0;
		rot0 = 0;
		*/
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////// Figura 2 ///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4 projection2 = glm::perspective(glm::radians(120.0f), (float)screenWidth / (float)screenHeight, 0.01f, 1000.0f);
		glm::mat4 view2 = camera->getViewMatrix();

		shader.setMatrix4("projection2", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view2", 1, false, glm::value_ptr(view));

		model2 = glm::translate(model, glm::vec3(0, 0, dz));
		model2 = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		///Cuerpo principal
		//cylinder12.enableWireMode();
		cylinder12.render(glm::scale(model2, glm::vec3(0.5, 0.5, 0.5)));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///Cabeza
		glm::mat4 c1 = glm::translate(model2, glm::vec3(0.0f, 0.3f, 0.0f));
		//sphere3.enableWireMode();
		sphere3.render(glm::scale(c1, glm::vec3(0.5, 0.5, 0.5)));
		c1 = glm::rotate(c1, cb, glm::vec3(0, 0, 1));
		///ojo
		glm::mat4 oj1 = glm::translate(c1, glm::vec3(0.00f, 0.1f, 0.18f));
		//sphere1.enableWireMode();
		sphere4.render(glm::scale(oj1, glm::vec3(0.1, 0.1, 0.1)));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///Pies
		glm::mat4 p1 = glm::translate(model2, glm::vec3(0.0f, -0.35, 0.0));
		p1 = glm::rotate(p1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//box1.enableWireMode();
		box2.render(glm::scale(p1, glm::vec3(0.3, 0.2, 0.2)));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Articulacion derecha alta
		glm::mat4 j111 = glm::translate(model2, glm::vec3(0.3f, 0.15f, 0.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j111, glm::vec3(0.1, 0.1, 0.1)));
		j111 = glm::rotate(j111, rot1_2d, glm::vec3(0, 0, 1));
		j111 = glm::rotate(j111, rot2_2d, glm::vec3(0, 1, 0));
		j111 = glm::rotate(j111, rot3_2d, glm::vec3(0, 0, 1));
		j111 = glm::rotate(j111,  rot4_2d, glm::vec3(0, 1, 0));
		///Brazo - Pata Derecha 
		glm::mat4 l111 = glm::translate(j111, glm::vec3(0.0f, -0.20, 0.0));
		l111 = glm::rotate(l111, glm::radians(0.0f), glm::vec3(0, 0, 3.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l111, glm::vec3(0.15, 0.5, 0.15)));
		/// Pata-Brazo Derecha
		glm::mat4 b1 = glm::translate(l111, glm::vec3(0.0f, -0.35, 0.0));
		b1 = glm::rotate(b1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//box1.enableWireMode();
		box2.render(glm::scale(b1, glm::vec3(0.2, 0.3, 0.2)));
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Articulacion Izquierda Alta
		glm::mat4 j112 = glm::translate(model2, glm::vec3(-0.3f, 0.15f, 0.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j112, glm::vec3(0.1, 0.1, 0.1)));
		j112 = glm::rotate(j112, rot1_2, glm::vec3(0, 0, 1));
		j112 = glm::rotate(j112, rot2_2, glm::vec3(0, 1, 0));
		j112 = glm::rotate(j112, rot3_2, glm::vec3(0, 0, 1));
		j112 = glm::rotate(j112,  rot4_2, glm::vec3(0, 1, 0));
		///Brazo - Pata Izquierda
		glm::mat4 l112 = glm::translate(j112, glm::vec3(0.0f, -0.20, 0.0));
		l112 = glm::rotate(l112, glm::radians(180.0f), glm::vec3(0, 0, 3.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l112, glm::vec3(0.15, 0.5, 0.15)));
		/// Pata-Brazo Derecha
		glm::mat4 b12 = glm::translate(l112, glm::vec3(0.0f, 0.35, 0.0));
		b12 = glm::rotate(b12, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//box1.enableWireMode();
		box2.render(glm::scale(b12, glm::vec3(0.2, 0.3, 0.2)));

		shader.turnOff();
		dz = 0;
		rot0 = 0;
		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
