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

//Descomentar
#include "Headers/Texture.h"

int screenWidth;
int screenHeight;

GLFWwindow * window;

Shader shader;
//Descomentar
Shader shaderTexture;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Box box1, box2;

// Descomentar
GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, textureID8;

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

float rot0 = 0.0, dz = 0.0;

///Brazo 1 (izquierdo)
float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
///Brazo 2 (Derecho)
float rot1d = 0.0, rot2d = 0.0, rot3d = 0.0, rot4d = 0.0;
///Pierna 1 (Derecho)
float rot1pd = 0.0, rot2pd = 0.0, rot3pd = 0.0, rot4pd = 0.0;
///Pierna 2 (Izquierdo)
float rot1pi = 0.0, rot2pi = 0.0, rot3pi = 0.0, rot4pi = 0.0;
//bool sentido = true;
bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		int mode);
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
	//Descomentar
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderTexture);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	// Método setter que colocar el apuntador al shader
	sphere2.setShader(&shaderTexture);
	//Setter para poner el color de la geometria
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinder1.init();
	cylinder1.setShader(&shaderTexture);
	//cylinder1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shaderTexture);
	

	box1.init();
	//Settear ek shader
	box1.setShader(&shaderTexture);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
	////
	box2.init();
	box2.setShader(&shaderTexture);
	camera->setPosition(glm::vec3(0.0, 0.0, 4.0));
	//////
	sphere3.init();
	sphere3.setShader(&shaderTexture);

	//Descomentar
	int imageWidth, imageHeight;
	Texture texture1("../Textures/sponge.jpg");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	FIBITMAP *bitmap = texture1.loadImage();
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	unsigned char *data = texture1.convertToData(bitmap, imageWidth,
			imageHeight);
	//creando la textura con id 1
	glGenTextures(1, &textureID1);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters coloca los parametros del wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Aqui simplemente verifica si se pudo abrir la textura
	if (data) {
		//tranformar los datos de la imagen a memoria
		//Tipo de textura, formato interno OpenGL, ancho, alto, Mimaps
		//formato inter de la libreria, el tipo de dato y el papuntador a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		//generan los nuveles del mipmap(OpenGl es el encargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	texture1.freeImage(bitmap);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Texture texture2("../Textures/water.jpg");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	bitmap = texture2.loadImage();
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	data = texture2.convertToData(bitmap, imageWidth,
		imageHeight);
	//creando la textura con id 1
	glGenTextures(1, &textureID2);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID2);
	// set the texture wrapping parameters coloca los parametros del wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Aqui simplemente verifica si se pudo abrir la textura
	if (data) {
		//tranformar los datos de la imagen a memoria
		//Tipo de textura, formato interno OpenGL, ancho, alto, Mimaps
		//formato inter de la libreria, el tipo de dato y el papuntador a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		//generan los nuveles del mipmap(OpenGl es el encargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture2.freeImage(bitmap);

	///////////////////////////////////////////////////textura 3//////////////////////////////////////////////////////////////////////////////////////

	Texture texture3("../Textures/goku.png");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	///Esto para voltear mi imagen
	bitmap = texture3.loadImage(true);
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	data = texture3.convertToData(bitmap, imageWidth,
		imageHeight);
	//creando la textura con id 1
	glGenTextures(1, &textureID3);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters coloca los parametros del wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Aqui simplemente verifica si se pudo abrir la textura
	if (data) {
		//tranformar los datos de la imagen a memoria
		//Tipo de textura, formato interno OpenGL, ancho, alto, Mimaps
		//formato inter de la libreria, el tipo de dato y el papuntador a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		//generan los nuveles del mipmap(OpenGl es el encargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture3.freeImage(bitmap);

	////////////////////////////////////////////////////Textura lata Cuerpo//////////////////////////////////////////////
	Texture texture4("../Textures/Coca.jpg");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	///Esto para voltear mi imagen
	bitmap = texture4.loadImage(true);
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	data = texture4.convertToData(bitmap, imageWidth,
		imageHeight);
	//creando la textura con id 4
	glGenTextures(1, &textureID4);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters coloca los parametros del wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Aqui simplemente verifica si se pudo abrir la textura
	if (data) {
		//tranformar los datos de la imagen a memoria
		//Tipo de textura, formato interno OpenGL, ancho, alto, Mimaps
		//formato inter de la libreria, el tipo de dato y el papuntador a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		//generan los nuveles del mipmap(OpenGl es el encargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture4.freeImage(bitmap);

	////////////////////////////////////////////////////Textura lata Tapa arriba//////////////////////////////////////////////
	Texture texture5("../Textures/tapa.jpg");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	///Esto para voltear mi imagen
	bitmap = texture5.loadImage(true);
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	data = texture5.convertToData(bitmap, imageWidth,
		imageHeight);
	//creando la textura con id 5
	glGenTextures(1, &textureID5);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID5);
	// set the texture wrapping parameters coloca los parametros del wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Aqui simplemente verifica si se pudo abrir la textura
	if (data) {
		//tranformar los datos de la imagen a memoria
		//Tipo de textura, formato interno OpenGL, ancho, alto, Mimaps
		//formato inter de la libreria, el tipo de dato y el papuntador a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		//generan los nuveles del mipmap(OpenGl es el encargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture5.freeImage(bitmap);

	////////////////////////////////////////////////////Textura lata Tapa abajo//////////////////////////////////////////////
	Texture texture7("../Textures/ojobob.png");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	///Esto para voltear mi imagen
	bitmap = texture7.loadImage(true);
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	data = texture7.convertToData(bitmap, imageWidth,
		imageHeight);
	//creando la textura con id 5
	glGenTextures(1, &textureID7);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID7);
	// set the texture wrapping parameters coloca los parametros del wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Aqui simplemente verifica si se pudo abrir la textura
	if (data) {
		//tranformar los datos de la imagen a memoria
		//Tipo de textura, formato interno OpenGL, ancho, alto, Mimaps
		//formato inter de la libreria, el tipo de dato y el papuntador a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		//generan los nuveles del mipmap(OpenGl es el encargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture7.freeImage(bitmap);
	////////////////////////////////////////////////////Textura lata Tapa abajo//////////////////////////////////////////////
	Texture texture6("../Textures/Tapa_inferior.jpg");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	///Esto para voltear mi imagen
	bitmap = texture6.loadImage(true);
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	data = texture6.convertToData(bitmap, imageWidth,
		imageHeight);
	//creando la textura con id 5
	glGenTextures(1, &textureID6);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID6);
	// set the texture wrapping parameters coloca los parametros del wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Aqui simplemente verifica si se pudo abrir la textura
	if (data) {
		//tranformar los datos de la imagen a memoria
		//Tipo de textura, formato interno OpenGL, ancho, alto, Mimaps
		//formato inter de la libreria, el tipo de dato y el papuntador a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		//generan los nuveles del mipmap(OpenGl es el encargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture6.freeImage(bitmap);
	////////////////////////////////////////////////////Textura pantalones//////////////////////////////////////////////
	Texture texture8("../Textures/pantalon.jpg");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	///Esto para voltear mi imagen
	bitmap = texture8.loadImage(true);
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	data = texture6.convertToData(bitmap, imageWidth,
		imageHeight);
	//creando la textura con id 5
	glGenTextures(1, &textureID8);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID8);
	// set the texture wrapping parameters coloca los parametros del wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Aqui simplemente verifica si se pudo abrir la textura
	if (data) {
		//tranformar los datos de la imagen a memoria
		//Tipo de textura, formato interno OpenGL, ancho, alto, Mimaps
		//formato inter de la libreria, el tipo de dato y el papuntador a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		//generan los nuveles del mipmap(OpenGl es el encargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture8.freeImage(bitmap);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Destrucción de los VAO, EBO, VBO
	sphere1.destroy();
	cylinder1.destroy();
	box1.destroy();

	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		int mode) {
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

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;


	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, 0.01);
	offsetX = 0;
	offsetY = 0;

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
		rot3pd += 0.001;
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
		rot2pi += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !sentido)
		rot2pi -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && sentido)
		rot3pi += 0.01;
	else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !sentido)
		rot3pi -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && sentido)
		rot4pi += 0.01;
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

	glm::mat4 model = glm::mat4(1.0f);
	float offX = 0.0;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		//Descomentar
		//settea la matriz de la vista y projeccion al nuevo shader
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		//box1.enableWireMode();
		//Descomentar
		//Usamos la textura ID 1
		glBindTexture(GL_TEXTURE_2D, textureID1);
		shaderTexture.setFloat("offsetX", 0);
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Usamos la textura ID 1 para los cilindros de los brazos
		glBindTexture(GL_TEXTURE_2D, textureID1);
		shaderTexture.setFloat("offsetX", 0);
		cylinder1.render(glm::scale(model, glm::vec3(0.1, 0.5, 0.1)));
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Descomentar
		//No utilizar ninguna textura
		//glBindTexture(GL_TEXTURE_2D, 0);

		//articulacion
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));//se agrego esta linea para empezar a modelar a la esponja
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));
		j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
		j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));

		//hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0f, 0.0f));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion 2
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5f, 0.0f, 0.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));
		j2 = glm::rotate(j2, rot3, glm::vec3(0, 0, 1));
		j2 = glm::rotate(j2, rot4, glm::vec3(0, 1, 0));

/*
		//Nariz
		glm::mat4 n1 = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.05f));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(n1, glm::vec3(0.20f, 0.20f, 0.10f)));
*/
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////Parte izquierda//////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//articulacion
		glm::mat4 ji1 = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));//se agrego esta linea para empezar a modelar a la esponja
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ji1, glm::vec3(0.1, 0.1, 0.1)));
		ji1 = glm::rotate(ji1, rot1d, glm::vec3(0, 0, 1));
		ji1 = glm::rotate(ji1, rot2d, glm::vec3(0, 1, 0));

		//hueso 1
		glm::mat4 li1 = glm::translate(ji1, glm::vec3(-0.25f, 0.0f, 0.0f));
		li1 = glm::rotate(li1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(li1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion 2
		glm::mat4 ji2 = glm::translate(ji1, glm::vec3(-0.5f, 0.0f, 0.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ji2, glm::vec3(-0.1, 0.1, 0.1)));
		ji2 = glm::rotate(ji2, rot3d, glm::vec3(0, 0, 1));
		ji2 = glm::rotate(ji2, rot4d, glm::vec3(0, 1, 0));

		//hueso 2
		glm::mat4 li2 = glm::translate(ji2, glm::vec3(-0.25, 0.0, 0.0));
		li2 = glm::rotate(li2, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(li2, glm::vec3(-0.1, 0.5, 0.1)));

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////Parte Abajo///////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

		//articulacion
		glm::mat4 ja1 = glm::translate(model, glm::vec3(-0.25f, -0.5f, 0.0f));//se agrego esta linea para empezar a modelar a la esponja
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ja1, glm::vec3(-0.1, 0.1, 0.1)));
		ja1 = glm::rotate(ja1, rot1pd, glm::vec3(0, 0, 1));
		ja1 = glm::rotate(ja1, rot2pd, glm::vec3(0, 1, 0));

		//hueso 1
		glm::mat4 la1 = glm::translate(ja1, glm::vec3(-0.015f, -0.25f, 0.0f));
		la1 = glm::rotate(la1, glm::radians(180.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(la1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion 2
		glm::mat4 ja2 = glm::translate(ja1, glm::vec3(-0.015f, -0.5f, 0.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ja2, glm::vec3(0.1, 0.1, 0.1)));
		ja2 = glm::rotate(ja2, rot3pd, glm::vec3(0, 0, 1));
		ja2 = glm::rotate(ja2, rot4pd, glm::vec3(0, 1, 0));
		//hueso 2
		glm::mat4 la2 = glm::translate(ja2, glm::vec3(-0.015f, -0.25f, 0.0f));
		la2 = glm::rotate(la2, glm::radians(180.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(la2, glm::vec3(0.1, 0.5, 0.1)));
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//articulacion 2
		glm::mat4 jaa1 = glm::translate(model, glm::vec3(0.25f, -0.5f, 0.0f));//se agrego esta linea para empezar a modelar a la esponja
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(jaa1, glm::vec3(-0.1, 0.1, 0.1)));
		jaa1 = glm::rotate(jaa1, rot1pi, glm::vec3(0, 0, 1));
		jaa1 = glm::rotate(jaa1, rot2pi, glm::vec3(0, 1, 0));

		//hueso 12
		glm::mat4 laa1 = glm::translate(jaa1, glm::vec3(0.015f, -0.25f, 0.0f));
		laa1 = glm::rotate(laa1, glm::radians(180.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(laa1, glm::vec3(0.1, 0.5, 0.1)));

		//articulacion 22
		glm::mat4 jaa2 = glm::translate(jaa1, glm::vec3(0.015f, -0.5f, 0.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(jaa2, glm::vec3(0.1, 0.1, 0.1)));
		jaa2 = glm::rotate(jaa2, rot3pi, glm::vec3(0, 0, 1));
		jaa2 = glm::rotate(jaa2, rot4pi, glm::vec3(0, 1, 0));

		//hueso 22
		glm::mat4 laa2 = glm::translate(jaa2, glm::vec3(0.015f, -0.25f, 0.0f));
		laa2 = glm::rotate(laa2, glm::radians(180.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(laa2, glm::vec3(0.1, 0.5, 0.1)));

		glm::mat4 modelAgua = glm::mat4(1.0);
		modelAgua = glm::translate(modelAgua, glm::vec3(0.0, -3.0, 0.0));
		modelAgua = glm::scale(modelAgua, glm::vec3(5.0, 0.01, 5.0));
		//// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID2);
		shaderTexture.setFloat("offsetX", offX);
		box2.render(modelAgua);
		glBindTexture(GL_TEXTURE_2D, 0);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////7
		glm::mat4 modelSphere = glm::mat4(1.0);
		modelSphere = glm::translate(modelSphere, glm::vec3(3.0, 0.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID3);
		shaderTexture.setFloat("offsetX", 0);
		sphere3.render(modelSphere);
		glBindTexture(GL_TEXTURE_2D, 0);
		//hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));

		//ojo 1
		glm::mat4 o1 = glm::translate(model, glm::vec3(0.25f, 0.25f, 0.05f));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setFloat("offsetX", 0);
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(o1, glm::vec3(0.20f, 0.20f, 0.10f)));

		//ojo 2
		glm::mat4 o2 = glm::translate(model, glm::vec3(-0.25f, 0.25f, 0.05f));
		//sphere1.enableWireMode();
		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setFloat("offsetX", 0);
		sphere1.render(glm::scale(o2, glm::vec3(0.20f, 0.20f, 0.10f)));

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////7
		glm::mat4 modelCylinder = glm::mat4(1.0);
		modelCylinder = glm::translate(modelCylinder, glm::vec3(-3.0, 0.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setFloat("offsetX", 0);
		////////////////////////////////////////////////////////////
		cylinder2.render(0,cylinder2.getSlices() * cylinder2.getStacks()*6 ,modelCylinder);
		glBindTexture(GL_TEXTURE_2D, textureID2);
		// Se usa la textura 1 ( Bob sponja)
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder2.render(0, cylinder2.getSlices() * cylinder2.getStacks() * 6,
			modelCylinder);
		///////////////////Pantalones
		glm::mat4 pantalones = glm::translate(model, glm::vec3(0.0, -0.3, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		shaderTexture.setFloat("offsetX", 0);
		box1.render(glm::scale(pantalones, glm::vec3(1.0, 0.45, 0.21)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////
		// Envolvente desde el indice 0, el tamanio es 20 * 20 * 6
		
		// Tapa Superior desde el indice : 20 * 20 * 6, el tamanio de indices es 20 * 3
		// Se usa la textura 2 ( Agua )
		glBindTexture(GL_TEXTURE_2D, textureID5);
		cylinder2.render(cylinder2.getSlices() * cylinder2.getStacks() * 6,
			cylinder2.getSlices() * 3,
			modelCylinder);
		// Tapa inferior desde el indice : 20 * 20 * 6 + 20 * 3, el tamanio de indices es 20 * 3
		// Se usa la textura 3 ( Goku )
		glBindTexture(GL_TEXTURE_2D, textureID6);
		cylinder2.render(cylinder2.getSlices() * cylinder2.getStacks() * 6 + cylinder2.getSlices() * 3,
			cylinder2.getSlices() * 3,
			modelCylinder);

		glBindTexture(GL_TEXTURE_2D, 0);

		shader.turnOff();

		dz = 0;
		rot0 = 0;
		offX += 0.001;
		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
