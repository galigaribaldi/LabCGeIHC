#define _USE_MATH_DEFINES
#include <cmath>
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

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader de texturizado
Shader shaderTexture;
//Shader con iluminacion solo color
Shader shaderColorLighting;
//Shader con iluminacion y textura
Shader shaderTextureLighting;
// Descomentar
//Shader con materiales
Shader shaderMaterialLighting;
// Descomentar
//Shader con skybox
Shader shaderSkybox;
// Shader con multiples luces
Shader shaderMulLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere1(20, 20);
Sphere sphere2(20, 20);
Sphere sphere3(20, 20);
///Esfera Negra
Sphere sphere4(20, 20);
Sphere sphereLamp(20, 20);
// Descomentar
Sphere skyboxSphere(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);
Cylinder cylinderMaterials(20, 20, 0.5, 0.5);
Cylinder cylinder12(20, 20, 0.5, 0.5);
Box boxMaterials;
Box box1;
Box box2;
Box box3;
Box boxCesped;
Box boxWall;
// Models complex instances
Model modelRock;
Model modelRailRoad;
Model modelAircraft;

GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, textureID8, textureID9, textureID17, textureID18;
GLuint skyboxTextureID;
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
float rot1_2 = 0.0, rot2_2 = 0.0, rot3_2 = 0.0, rot4_2 = 0.0;
///Brazo 2 (Derecho)
float rot1_2d = 0.0, rot2_2d = 0.0, rot3_2d = 0.0, rot4_2d = 0.0;
////////////Giro general
float rot0 = 0.0, dz = 0.0;
bool sentido = true;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

//float rot0 = 0.0, dz = 0.0;

//float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
//bool sentido = true;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
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
	shaderTexture.initialize("../Shaders/texturizado_res.vs",
			"../Shaders/texturizado_res.fs");
	shaderColorLighting.initialize("../Shaders/iluminacion_color_res.vs",
			"../Shaders/iluminacion_color_res.fs");
	shaderTextureLighting.initialize("../Shaders/iluminacion_texture_res.vs",
			"../Shaders/iluminacion_texture_res.fs");
	shaderMaterialLighting.initialize("../Shaders/iluminacion_material.vs",
			"../Shaders/iluminacion_material_res.fs");
	shaderSkybox.initialize("../Shaders/cubeTexture.vs",
			"../Shaders/cubeTexture.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs",
			"../Shaders/multipleLights.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	// Método setter que colocar el apuntador al shader
	sphere2.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere2.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphereLamp.init();
	// Método setter que colocar el apuntador al shader
	sphereLamp.setShader(&shader);
	//Setter para poner el color de la geometria
	sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinder1.init();
	cylinder1.setShader(&shaderColorLighting);
	cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shaderMulLighting);

	cylinderMaterials.init();
	cylinderMaterials.setShader(&shaderMaterialLighting);
	cylinderMaterials.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));
	////Segundo cilindro
	cylinder12.init();
	cylinder12.setShader(&shader);
	cylinder12.setColor(glm::vec4(255.0, 255.0, 255.0, 1.0));
	/////////////////
	boxMaterials.init();
	boxMaterials.setShader(&shaderMaterialLighting);

	// Inicializacion de la esfera del skybox
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	box1.init();
	// Settea el shader a utilizar
	box1.setShader(&shaderMulLighting);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shaderMulLighting);

	sphere3.init();
	sphere3.setShader(&shaderMulLighting);
	///Esfera Negra
	sphere4.init();
	sphere4.setShader(&shader);
	sphere4.setColor(glm::vec4(0.0, 0.0, 0.0, 0.0));
	///////
	box3.init();
	box3.setShader(&shaderMulLighting);

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWall.init();
	boxWall.setShader(&shaderMulLighting);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelRailRoad.loadModel("../models/railroad/railroad_track.obj");
	modelRailRoad.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));

	// Descomentar
	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	// Definiendo la textura a utilizar
	Texture texture1("../Textures/sponge.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	FIBITMAP *bitmap = texture1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	unsigned char *data = texture1.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID1);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture1.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture2("../Textures/water.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = texture2.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture2.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID2);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture2.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture3("../Textures/goku.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture3.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture3.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID3);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture3.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture texture4("../Textures/texturaLadrillos.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture4.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture4.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID4);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture4.freeImage(bitmap);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Definiendo la textura a utilizar
	Texture texture5("../Textures/piedra.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture5.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture5.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID5);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture5.freeImage(bitmap);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Definiendo la textura a utilizar
	Texture texture6("../Textures/cesped.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture6.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture6.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID6);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture6.freeImage(bitmap);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Definiendo la textura a utilizar
	Texture texture7("../Textures/window2.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture7.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture7.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID7);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID7);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture7.freeImage(bitmap);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Definiendo la textura a utilizar
	Texture texture8("../Textures/door.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture8.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture8.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID8);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID8);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture8.freeImage(bitmap);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Definiendo la textura a utilizar
	Texture texture9("../Textures/door2.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	// Voltear la imagen
	bitmap = texture9.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = texture9.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureID9);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureID9);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	texture9.freeImage(bitmap);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////Textura pantalones//////////////////////////////////////////////
	Texture texture18("../Textures/pantalon.jpg");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	///Esto para voltear mi imagen
	bitmap = texture18.loadImage(true);
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	data = texture18.convertToData(bitmap, imageWidth,
		imageHeight);
	//creando la textura con id 5
	glGenTextures(1, &textureID18);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID18);
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
	texture18.freeImage(bitmap);


	////////////////////////////////////////////////////Textura Ojos//////////////////////////////////////////////
	Texture texture17("../Textures/ojobob.png");
	//Carga el mapa de bits(Es el tipo de dato de la libreria)
	///Esto para voltear mi imagen
	bitmap = texture17.loadImage(true);
	//convertimos el mapa de bits en un arreglo unidimensional de tipo unseigned char
	data = texture17.convertToData(bitmap, imageWidth,
		imageHeight);
	//creando la textura con id 5
	glGenTextures(1, &textureID17);
	//enlazar ese ID o textura a un tipo de textura de 2D
	glBindTexture(GL_TEXTURE_2D, textureID17);
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
	texture17.freeImage(bitmap);

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
				imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

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

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
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
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && sentido)
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

	///Figura 1
	glm::mat4 model = glm::mat4(1.0f);
	/// Figura 2
	glm::mat4 model2 = glm::mat4(1.0f);

	//model = glm::translate(model, glm::vec3(0.0, 1.5, 0.0));
	float offX = 0.0;
	float angle = 0.0;
	float ratio = 30.0;
	glm::mat4 matrixModelAircraft = glm::mat4(1.0);
	matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(8.0, 2.0, -10.0));
	int state = 0;
	float offsetAircraftAdvance = 0.0;
	float offsetairfart = 0.0;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));

		//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		//glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con solo textura
		shaderTexture.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion solo color
		shaderColorLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderColorLighting.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con textura
		shaderTextureLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTextureLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con iluminacion con material
		shaderMaterialLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderMaterialLighting.setMatrix4("view", 1, false,
				 glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		// Propiedades de la luz para objetos con color
		shaderColorLighting.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderColorLighting.setVectorFloat3("light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular",
				glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderTextureLighting.setVectorFloat3("light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.specular",
				glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		// Propiedades de la luz para objetos con textura
		shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));

		// Propiedades de la luz para objetos con multiples luces
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));
		// Esto es para la luz spotlight
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(camera->getFront()));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.01, 0.01, 0.01)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.6)));
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.1);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.001);

		// Esto es para la luces pointlights(Actuan como si fueran un foco)
		// Numero de luces a utiliozar de tipo pointLights  = 3
		shaderMulLighting.setInt("pointLightCount", 5);
		// Posicion de la luz indice [0]
		shaderMulLighting.setVectorFloat3("pointLights[0].position", glm::value_ptr((glm::vec3(-5.1, 4.5, -3.5))));
		// Utiliza lo mismo para la luz Roja
		shaderMulLighting.setVectorFloat3("pointLights[0].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[0].light.specular", glm::value_ptr(glm::vec3(0.0, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[0].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[0].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[0].quadratic", 0.004);

		// Utiliza lo mismo para la luz Roja
		shaderMulLighting.setVectorFloat3("pointLights[1].position", glm::value_ptr((glm::vec3(-5.1, 4.8, -5.0))));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[1].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.00, 0.0))); 
		shaderMulLighting.setVectorFloat3("pointLights[1].light.specular", glm::value_ptr(glm::vec3(0.6, 0.0, 0.0)));
		shaderMulLighting.setFloat("pointLights[1].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[1].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[1].quadratic", 0.004);

		// Utiliza lo mismo para la luz Azul
		shaderMulLighting.setVectorFloat3("pointLights[2].position", glm::value_ptr((glm::vec3(-5.1, 4.5, -6.5))));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[2].light.specular", glm::value_ptr(glm::vec3(0.0, 0.0, 0.6)));
		shaderMulLighting.setFloat("pointLights[2].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[2].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[2].quadratic", 0.004);
		////////////////////////////////////////////////////Amarillo///////////////////////////////////////////////////////////////////////
		shaderMulLighting.setVectorFloat3("pointLights[3].position", glm::value_ptr((glm::vec3(-14.9, 4.8, -5.0))));
		shaderMulLighting.setVectorFloat3("pointLights[3].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[3].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[3].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[3].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[3].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[3].quadratic", 0.004);

		////////////////////////////////////////////////////Amarillo 2///////////////////////////////////////////////////////////////////////
		shaderMulLighting.setVectorFloat3("pointLights[4].position", glm::value_ptr((glm::vec3(-14.9, 4.8, -5.0))));
		shaderMulLighting.setVectorFloat3("pointLights[4].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
		shaderMulLighting.setVectorFloat3("pointLights[4].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		shaderMulLighting.setVectorFloat3("pointLights[4].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		shaderMulLighting.setFloat("pointLights[4].constant", 1.0);
		shaderMulLighting.setFloat("pointLights[4].linear", 0.04);
		shaderMulLighting.setFloat("pointLights[4].quadratic", 0.004);

		//Esto es oara colocar las esferas de las luces
		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.2));
		sphereLamp.setPosition(glm::vec3(-5.1, 4.5, -3.5));
		sphereLamp.setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
		sphereLamp.render();

		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.2));
		sphereLamp.setPosition(glm::vec3(-5.1, 4.8, -5.0));
		sphereLamp.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
		sphereLamp.render();
		///Amarillo
		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.2));
		sphereLamp.setPosition(glm::vec3(-14.9, 4.8, -5.0));
		sphereLamp.setColor(glm::vec4(0.6, 0.6, 0.0, 0.0));
		sphereLamp.render();
		///Amarillo 2
		sphereLamp.setScale(glm::vec3(0.1, 0.1, 0.2));
		sphereLamp.setPosition(glm::vec3(-10.9, 4.8, -5.0));
		sphereLamp.setColor(glm::vec4(0.6, 0.6, 0.0, 0.0));
		sphereLamp.render();

		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle,
				glm::vec3(-14.9, 4.8, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix,
				glm::vec3(0.0f, 0.0f, -ratio));
		// Posicion luz para objetos con color
		shaderColorLighting.setVectorFloat3("light.position",
				glm::value_ptr(
						glm::vec4(
								lightModelmatrix
										* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con textura
		shaderTextureLighting.setVectorFloat3("light.position",
				glm::value_ptr(
						glm::vec4(
								lightModelmatrix
										* glm::vec4(0.0, 0.0, 0.0, 1.0))));

		// Posicion luz para objetos con materiales
		shaderMaterialLighting.setVectorFloat3("light.position",
				glm::value_ptr(
						glm::vec4(
								lightModelmatrix
									* glm::vec4(0.0, 0.0, 0.0, 1.0))));
		sphereLamp.setScale(glm::vec3(1, 1, 1));
		sphereLamp.setPosition(glm::vec3(0, 0, 0));
		sphereLamp.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		sphereLamp.render(lightModelmatrix);

		model = glm::translate(model, glm::vec3(0, 0, dz));
		model = glm::rotate(model, rot0, glm::vec3(0, 1, 0));
		//box1.enableWireMode();
		/*
		// Usamos la textura ID 1
		glBindTexture(GL_TEXTURE_2D, textureID1);
		box1.render(glm::scale(model, glm::vec3(1.0, 1.0, 0.1)));
		// No utilizar ninguna textura
		glBindTexture(GL_TEXTURE_2D, 0);

		// Articulacion 1
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		j1 = glm::rotate(j1, rot1, glm::vec3(0, 0, 1));
		j1 = glm::rotate(j1, rot2, glm::vec3(0, 1, 0));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.25f, 0.0, 0.0));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l1, glm::vec3(0.1, 0.5, 0.1)));

		// Articulacion 2
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.5, 0.0f, 0.0f));
		j2 = glm::rotate(j2, rot3, glm::vec3(0.0, 0.0, 1.0));
		j2 = glm::rotate(j2, rot4, glm::vec3(1.0, 0.0, 0.0));
		sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l2, glm::vec3(0.1, 0.5, 0.1)));

		// Ojo
		glm::mat4 ojo = glm::translate(model, glm::vec3(0.25, 0.25, 0.05));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(ojo, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 ojo2 = glm::translate(model, glm::vec3(-0.25, 0.25, 0.05));
		//sphere2.enableWireMode();
		sphere2.render(glm::scale(ojo2, glm::vec3(0.2, 0.2, 0.1)));

		glm::mat4 modelAgua = glm::mat4(1.0);
		modelAgua = glm::translate(modelAgua, glm::vec3(5.0, 0.01, 5.0));
		modelAgua = glm::scale(modelAgua, glm::vec3(5.0, 0.01, 5.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID2);
		// Le cambiamos el shader con multiples luces NO OLVIDAR
		shaderMulLighting.setFloat("offsetX", offX);
		box2.render(modelAgua);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderMulLighting.setFloat("offsetX", 0);

		glm::mat4 modelCesped = glm::mat4(1.0);
		modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
		modelCesped = glm::scale(modelCesped, glm::vec3(40.0, 0.001, 40.0));
		// Se activa la textura del agua
		glBindTexture(GL_TEXTURE_2D, textureID5);
		boxCesped.render(modelCesped);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelSphere = glm::mat4(1.0);
		modelSphere = glm::translate(modelSphere, glm::vec3(10.0, 0.5, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID3);
		sphere3.render(modelSphere);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelCylinder = glm::mat4(1.0);
		modelCylinder = glm::translate(modelCylinder,
				glm::vec3(-3.0, 0.5, 0.0));
		// Envolvente desde el indice 0, el tamanio es 20 * 20 * 6
		// Se usa la textura 1 ( Bon sponja)
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder2.render(0, cylinder2.getSlices() * cylinder2.getStacks() * 6,
				modelCylinder);
		// Tapa Superior desde el indice : 20 * 20 * 6, el tamanio de indices es 20 * 3
		// Se usa la textura 2 ( Agua )
		glBindTexture(GL_TEXTURE_2D, textureID2);
		cylinder2.render(cylinder2.getSlices() * cylinder2.getStacks() * 6,
				cylinder2.getSlices() * 3, modelCylinder);
		// Tapa inferior desde el indice : 20 * 20 * 6 + 20 * 3, el tamanio de indices es 20 * 3
		// Se usa la textura 3 ( Goku )
		glBindTexture(GL_TEXTURE_2D, textureID3);
		cylinder2.render(
				cylinder2.getSlices() * cylinder2.getStacks() * 6
						+ cylinder2.getSlices() * 3, cylinder2.getSlices() * 3,
				modelCylinder);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Render del cubo con textura de ladrillos y con repeticion en x
		glm::mat4 cubeTextureModel = glm::mat4(1.0);
		cubeTextureModel = glm::translate(cubeTextureModel,
				glm::vec3(-5.0, 0.5, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderMulLighting.setVectorFloat2("scaleUV",
				glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(cubeTextureModel);
		shaderMulLighting.setVectorFloat2("scaleUV",
				glm::value_ptr(glm::vec2(0.0, 0.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		*/
		// Render del cyindro con materiales
/*		glm::mat4 cylinderMaterialModel = glm::mat4(1.0);
		cylinderMaterialModel = glm::translate(cylinderMaterialModel,  glm::vec3(3.0, 0.5, -3.0));
		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.727811f, 0.626959f, 0.626959f)));
		shaderMaterialLighting.setFloat("material.shininess", 76.8f);
		cylinderMaterials.render(cylinderMaterialModel);

		glm::mat4 boxMaterialModel = glm::mat4(1.0f);
		boxMaterialModel = glm::translate(boxMaterialModel, glm::vec3(-3.0, 0.5, -3.0));
		shaderMaterialLighting.setVectorFloat3("material.ambient", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.diffuse", glm::value_ptr(glm::vec3(0.61424f, 0.04136f, 0.04136f)));
		shaderMaterialLighting.setVectorFloat3("material.specular", glm::value_ptr(glm::vec3(0.727811f, 0.626959f, 0.626959f)));
		shaderMaterialLighting.setFloat("material.shininess", 76.8f);
		boxMaterials.render(boxMaterialModel);

		//Models complex render
		glm::mat4 matrixModelRock = glm::mat4(1.0);
		matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 6.0));
		modelRock.render(matrixModelRock);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);
		// Esto es para las vias del tren
		glm::mat4 matrixModelRailroad = glm::mat4(1.0);
		matrixModelRailroad = glm::translate(matrixModelRailroad, glm::vec3(3.0, 0.0, 10.0));
		modelRailRoad.render(matrixModelRailroad);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);
		/*
		* Se construye la casa
		*/

		glBindTexture(GL_TEXTURE_2D, textureID4);
		boxWall.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWall.setPosition(glm::vec3(-10.0, 2.5, 0.0));
		boxWall.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWall.render();
		boxWall.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWall.setPosition(glm::vec3(-15.0, 2.5, -5.0));
		boxWall.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWall.render();
		boxWall.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWall.setPosition(glm::vec3(-5.0, 2.5, -5.0));
		boxWall.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWall.render();
		boxWall.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWall.setPosition(glm::vec3(-10.0, 2.5, -10.0));
		boxWall.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWall.render();
		boxWall.setScale(glm::vec3(12.0, 12.0, 0.05));
		boxWall.setPosition(glm::vec3(-10.0, 5, -5.0));
		boxWall.setOrientation(glm::vec3(90.0, 0.0, 0.0));
		boxWall.render();
		glBindTexture(GL_TEXTURE_2D, 0);


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////PArte Trasera 0//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						// Render del cubo con textura de ladrillos y con repeticion en x
		glm::mat4 cubeTextureModel0 = glm::mat4(3.0);
		cubeTextureModel0 = glm::translate(cubeTextureModel0, glm::vec3(0.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel0, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel2 = glm::mat4(3.0);
		cubeTextureModel2 = glm::translate(cubeTextureModel2, glm::vec3(1.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel2, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel3 = glm::mat4(1.0);
		cubeTextureModel3 = glm::translate(cubeTextureModel3, glm::vec3(2.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel3, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel4 = glm::mat4(1.0);
		cubeTextureModel4 = glm::translate(cubeTextureModel4, glm::vec3(3.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel4, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel5 = glm::mat4(1.0);
		cubeTextureModel5 = glm::translate(cubeTextureModel5, glm::vec3(4.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel5, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel6 = glm::mat4(1.0);
		cubeTextureModel6 = glm::translate(cubeTextureModel6, glm::vec3(5.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel6, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel7 = glm::mat4(1.0);
		cubeTextureModel7 = glm::translate(cubeTextureModel7, glm::vec3(6.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel7, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel8 = glm::mat4(1.0);
		cubeTextureModel8 = glm::translate(cubeTextureModel8, glm::vec3(7.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel8, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel9 = glm::mat4(1.0);
		cubeTextureModel9 = glm::translate(cubeTextureModel9, glm::vec3(8.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel9, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel10 = glm::mat4(1.0);
		cubeTextureModel10 = glm::translate(cubeTextureModel10, glm::vec3(9.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel10, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel11 = glm::mat4(1.0);
		cubeTextureModel11 = glm::translate(cubeTextureModel11, glm::vec3(10.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel11, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel12 = glm::mat4(1.0);
		cubeTextureModel12 = glm::translate(cubeTextureModel12, glm::vec3(11.0, 2.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel12, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////Paredes Internas Bajas/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				////////////////////////////////////////////////////////////
		glm::mat4 pared10 = glm::mat4(1.0);
		pared10 = glm::translate(pared10, glm::vec3(9.0, 2.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(pared10, glm::vec3(4.0, 2.0, 0.1)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 pared20 = glm::mat4(1.0);
		pared20 = glm::translate(pared20, glm::vec3(9.0, 2.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(pared20, glm::vec3(4.0, 2.0, 0.1)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 pared30 = glm::mat4(1.0);
		pared30 = glm::translate(pared30, glm::vec3(3.0, 2.0, 7.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(pared30, glm::vec3(4.0, 2.0, 0.1)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 pared301 = glm::mat4(1.0);
		pared301 = glm::translate(pared301, glm::vec3(5.0, 2.0, 6.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(pared301, glm::vec3(0.1, 2.0, 5.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 pared302 = glm::mat4(1.0);
		pared302 = glm::translate(pared302, glm::vec3(5.0, 2.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(pared302, glm::vec3(0.1, 2.0, 2.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 pared40 = glm::mat4(1.0);
		pared40 = glm::translate(pared40, glm::vec3(7.0, 2.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(pared40, glm::vec3(0.1, 2.0, 3.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		///////////////////////////////////////Puertas//////////////////////////////////////////////////////////
		glm::mat4 puerta1 = glm::mat4(1.0);
		puerta1 = glm::translate(puerta1, glm::vec3(7.0, 2.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(puerta1, glm::vec3(0.1, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta12 = glm::mat4(1.0);
		puerta12 = glm::translate(puerta12, glm::vec3(7.0, 2.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(puerta12, glm::vec3(0.1, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta13 = glm::mat4(1.0);
		puerta13 = glm::translate(puerta13, glm::vec3(5.0, 2.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(puerta13, glm::vec3(0.1, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		///////////////////////////////////////Fin Puertas//////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////Parte Trasera 1//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModell13 = glm::mat4(3.0);
		cubeTextureModell13 = glm::translate(cubeTextureModell13,
			glm::vec3(0.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModell13, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel13 = glm::mat4(3.0);
		cubeTextureModel13 = glm::translate(cubeTextureModel13,
			glm::vec3(1.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel13, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel14 = glm::mat4(1.0);
		cubeTextureModel14 = glm::translate(cubeTextureModel14,
			glm::vec3(2.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel14, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel15 = glm::mat4(1.0);
		cubeTextureModel15 = glm::translate(cubeTextureModel15,
			glm::vec3(3.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel15, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 ccubeTextureModel16 = glm::mat4(1.0);
		ccubeTextureModel16 = glm::translate(ccubeTextureModel16,
			glm::vec3(4.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(ccubeTextureModel16, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel17 = glm::mat4(1.0);
		cubeTextureModel17 = glm::translate(cubeTextureModel17,
			glm::vec3(5.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel17, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 ccubeTextureModel18 = glm::mat4(1.0);
		ccubeTextureModel18 = glm::translate(ccubeTextureModel18,
			glm::vec3(6.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(ccubeTextureModel18, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cucubeTextureModel19 = glm::mat4(1.0);
		cucubeTextureModel19 = glm::translate(cucubeTextureModel19,
			glm::vec3(7.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cucubeTextureModel19, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cucubeTextureModel20 = glm::mat4(1.0);
		cucubeTextureModel20 = glm::translate(cucubeTextureModel20,
			glm::vec3(8.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cucubeTextureModel20, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel21 = glm::mat4(1.0);
		cubeTextureModel21 = glm::translate(cubeTextureModel21, glm::vec3(9.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel21, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel22 = glm::mat4(1.0);
		cubeTextureModel22 = glm::translate(cubeTextureModel22, glm::vec3(10.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel22, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel23 = glm::mat4(1.0);
		cubeTextureModel23 = glm::translate(cubeTextureModel23, glm::vec3(11.0, 3.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel23, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////Parte Trasera 2//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelTAl13 = glm::mat4(3.0);
		cubeTextureModelTAl13 = glm::translate(cubeTextureModelTAl13,
			glm::vec3(0.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelTAl13, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelTA13 = glm::mat4(3.0);
		cubeTextureModelTA13 = glm::translate(cubeTextureModelTA13,
			glm::vec3(1.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelTA13, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelTA14 = glm::mat4(1.0);
		cubeTextureModelTA14 = glm::translate(cubeTextureModelTA14,
			glm::vec3(2.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelTA14, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelTA15 = glm::mat4(1.0);
		cubeTextureModelTA15 = glm::translate(cubeTextureModelTA15,
			glm::vec3(3.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelTA15, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 ccubeTextureModelTA16 = glm::mat4(1.0);
		ccubeTextureModelTA16 = glm::translate(ccubeTextureModelTA16,
			glm::vec3(4.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(ccubeTextureModelTA16, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelTA17 = glm::mat4(1.0);
		cubeTextureModelTA17 = glm::translate(cubeTextureModelTA17,
			glm::vec3(5.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelTA17, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 ccubeTextureModelTA18 = glm::mat4(1.0);
		ccubeTextureModelTA18 = glm::translate(ccubeTextureModelTA18,
			glm::vec3(6.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(ccubeTextureModelTA18, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cucubeTextureModelTA19 = glm::mat4(1.0);
		cucubeTextureModelTA19 = glm::translate(cucubeTextureModelTA19,
			glm::vec3(7.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cucubeTextureModelTA19, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cucubeTextureModelTA20 = glm::mat4(1.0);
		cucubeTextureModelTA20 = glm::translate(cucubeTextureModelTA20,
			glm::vec3(8.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cucubeTextureModelTA20, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelTA21 = glm::mat4(1.0);
		cubeTextureModelTA21 = glm::translate(cubeTextureModelTA21, glm::vec3(9.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelTA21, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelTA22 = glm::mat4(1.0);
		cubeTextureModelTA22 = glm::translate(cubeTextureModelTA22, glm::vec3(10.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelTA22, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelTA23 = glm::mat4(1.0);
		cubeTextureModelTA23 = glm::translate(cubeTextureModelTA23, glm::vec3(11.0, 4.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelTA23, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////PAredes Izquierdas 0////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel24 = glm::mat4(1.0);
		cubeTextureModel24 = glm::translate(cubeTextureModel24, glm::vec3(0.0, 2.0, 4.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel24, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel25 = glm::mat4(1.0);
		cubeTextureModel25 = glm::translate(cubeTextureModel25, glm::vec3(0.0, 2.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel25, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel26 = glm::mat4(1.0);
		cubeTextureModel26 = glm::translate(cubeTextureModel26, glm::vec3(0.0, 2.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel26, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel27 = glm::mat4(1.0);
		cubeTextureModel27 = glm::translate(cubeTextureModel27, glm::vec3(0.0, 2.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel27, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel28 = glm::mat4(1.0);
		cubeTextureModel28 = glm::translate(cubeTextureModel28, glm::vec3(0.0, 2.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel28, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel34 = glm::mat4(1.0);
		cubeTextureModel34 = glm::translate(cubeTextureModel34, glm::vec3(0.0, 2.0, 6.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel34, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel29 = glm::mat4(1.0);
		cubeTextureModel29 = glm::translate(cubeTextureModel29, glm::vec3(0.0, 2.0, 7.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel29, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel30 = glm::mat4(1.0);
		cubeTextureModel30 = glm::translate(cubeTextureModel30, glm::vec3(0.0, 2.0, 8.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel30, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel31 = glm::mat4(1.0);
		cubeTextureModel31 = glm::translate(cubeTextureModel31, glm::vec3(0.0, 2.0, 9.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel31, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel32 = glm::mat4(1.0);
		cubeTextureModel32 = glm::translate(cubeTextureModel32, glm::vec3(0.0, 2.0, 10.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel32, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModel33 = glm::mat4(1.0);
		cubeTextureModel33 = glm::translate(cubeTextureModel33, glm::vec3(0.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModel33, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////Paredes Izqueirda 1/////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI24 = glm::mat4(1.0);
		cubeTextureModelAI24 = glm::translate(cubeTextureModelAI24, glm::vec3(0.0, 3.0, 4.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI24, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI25 = glm::mat4(1.0);
		cubeTextureModelAI25 = glm::translate(cubeTextureModelAI25, glm::vec3(0.0, 3.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI25, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI26 = glm::mat4(1.0);
		cubeTextureModelAI26 = glm::translate(cubeTextureModelAI26, glm::vec3(0.0, 3.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI26, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI27 = glm::mat4(1.0);
		cubeTextureModelAI27 = glm::translate(cubeTextureModelAI27, glm::vec3(0.0, 3.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI27, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI28 = glm::mat4(1.0);
		cubeTextureModelAI28 = glm::translate(cubeTextureModelAI28, glm::vec3(0.0, 3.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI28, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI34 = glm::mat4(1.0);
		cubeTextureModelAI34 = glm::translate(cubeTextureModelAI34, glm::vec3(0.0, 3.0, 6.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI34, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI29 = glm::mat4(1.0);
		cubeTextureModelAI29 = glm::translate(cubeTextureModelAI29, glm::vec3(0.0, 3.0, 7.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI29, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI30 = glm::mat4(1.0);
		cubeTextureModelAI30 = glm::translate(cubeTextureModelAI30, glm::vec3(0.0, 3.0, 8.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI30, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI31 = glm::mat4(1.0);
		cubeTextureModelAI31 = glm::translate(cubeTextureModelAI31, glm::vec3(0.0, 3.0, 9.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI31, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI32 = glm::mat4(1.0);
		cubeTextureModelAI32 = glm::translate(cubeTextureModelAI32, glm::vec3(0.0, 3.0, 10.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI32, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI33 = glm::mat4(1.0);
		cubeTextureModelAI33 = glm::translate(cubeTextureModelAI33, glm::vec3(0.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI33, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////Paredes Izqueirda 2/////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI124 = glm::mat4(1.0);
		cubeTextureModelAI124 = glm::translate(cubeTextureModelAI124, glm::vec3(0.0, 4.0, 4.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI124, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI125 = glm::mat4(1.0);
		cubeTextureModelAI125 = glm::translate(cubeTextureModelAI125, glm::vec3(0.0, 4.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI125, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI126 = glm::mat4(1.0);
		cubeTextureModelAI126 = glm::translate(cubeTextureModelAI126, glm::vec3(-0.1, 4.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI126, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI127 = glm::mat4(1.0);
		cubeTextureModelAI127 = glm::translate(cubeTextureModelAI127, glm::vec3(0.0, 4.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI127, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI128 = glm::mat4(1.0);
		cubeTextureModelAI128 = glm::translate(cubeTextureModelAI128, glm::vec3(0.0, 4.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI128, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI134 = glm::mat4(1.0);
		cubeTextureModelAI134 = glm::translate(cubeTextureModelAI134, glm::vec3(0.0, 4.0, 6.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI134, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI1129 = glm::mat4(1.0);
		cubeTextureModelAI1129 = glm::translate(cubeTextureModelAI1129, glm::vec3(0.0, 4.0, 7.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI1129, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI1130 = glm::mat4(1.0);
		cubeTextureModelAI1130 = glm::translate(cubeTextureModelAI1130, glm::vec3(0.0, 4.0, 8.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI1130, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI131 = glm::mat4(1.0);
		cubeTextureModelAI131 = glm::translate(cubeTextureModelAI131, glm::vec3(-0.1, 4.0, 9.0));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI131, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI132 = glm::mat4(1.0);
		cubeTextureModelAI132 = glm::translate(cubeTextureModelAI132, glm::vec3(0.0, 4.0, 10.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI132, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelAI133 = glm::mat4(1.0);
		cubeTextureModelAI133 = glm::translate(cubeTextureModelAI133, glm::vec3(0.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelAI133, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////Paredes Derecha 0/////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD024 = glm::mat4(1.0);
		cubeTextureModelD024 = glm::translate(cubeTextureModelD024, glm::vec3(11.0, 2.0, 4.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD024, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD025 = glm::mat4(1.0);
		cubeTextureModelD025 = glm::translate(cubeTextureModelD025, glm::vec3(11.0, 2.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD025, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD026 = glm::mat4(1.0);
		cubeTextureModelD026 = glm::translate(cubeTextureModelD026, glm::vec3(11.0, 2.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD026, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD027 = glm::mat4(1.0);
		cubeTextureModelD027 = glm::translate(cubeTextureModelD027, glm::vec3(11.0, 2.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD027, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD028 = glm::mat4(1.0);
		cubeTextureModelD028 = glm::translate(cubeTextureModelD028, glm::vec3(11.0, 2.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD028, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD034 = glm::mat4(1.0);
		cubeTextureModelD034 = glm::translate(cubeTextureModelD034, glm::vec3(11.0, 2.0, 6.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD034, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD029 = glm::mat4(1.0);
		cubeTextureModelD029 = glm::translate(cubeTextureModelD029, glm::vec3(11.0, 2.0, 7.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD029, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD030 = glm::mat4(1.0);
		cubeTextureModelD030 = glm::translate(cubeTextureModelD030, glm::vec3(11.0, 2.0, 8.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD030, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD031 = glm::mat4(1.0);
		cubeTextureModelD031 = glm::translate(cubeTextureModelD031, glm::vec3(11.0, 2.0, 9.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD031, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD032 = glm::mat4(1.0);
		cubeTextureModelD032 = glm::translate(cubeTextureModelD032, glm::vec3(11.0, 2.0, 10.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD032, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD033 = glm::mat4(1.0);
		cubeTextureModelD033 = glm::translate(cubeTextureModelD033, glm::vec3(11.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD033, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////Paredes Derecha 1/////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD124 = glm::mat4(1.0);
		cubeTextureModelD124 = glm::translate(cubeTextureModelD124, glm::vec3(11.0, 3.0, 4.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD124, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD125 = glm::mat4(1.0);
		cubeTextureModelD125 = glm::translate(cubeTextureModelD125, glm::vec3(11.0, 3.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD125, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD126 = glm::mat4(1.0);
		cubeTextureModelD126 = glm::translate(cubeTextureModelD126, glm::vec3(11.0, 3.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD126, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD127 = glm::mat4(1.0);
		cubeTextureModelD127 = glm::translate(cubeTextureModelD127, glm::vec3(11.0, 3.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD127, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD128 = glm::mat4(1.0);
		cubeTextureModelD128 = glm::translate(cubeTextureModelD128, glm::vec3(11.0, 3.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD128, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD134 = glm::mat4(1.0);
		cubeTextureModelD134 = glm::translate(cubeTextureModelD134, glm::vec3(11.0, 3.0, 6.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD134, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD129 = glm::mat4(1.0);
		cubeTextureModelD129 = glm::translate(cubeTextureModelD129, glm::vec3(11.0, 3.0, 7.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD129, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD130 = glm::mat4(1.0);
		cubeTextureModelD130 = glm::translate(cubeTextureModelD130, glm::vec3(11.0, 3.0, 8.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD130, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD131 = glm::mat4(1.0);
		cubeTextureModelD131 = glm::translate(cubeTextureModelD131, glm::vec3(11.0, 3.0, 9.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD131, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD132 = glm::mat4(1.0);
		cubeTextureModelD132 = glm::translate(cubeTextureModelD132, glm::vec3(11.0, 3.0, 10.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD132, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD133 = glm::mat4(1.0);
		cubeTextureModelD133 = glm::translate(cubeTextureModelD133, glm::vec3(11.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD133, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////Paredes Derecha 2/////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD224 = glm::mat4(1.0);
		cubeTextureModelD224 = glm::translate(cubeTextureModelD224, glm::vec3(11.0, 4.0, 4.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD224, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD225 = glm::mat4(1.0);
		cubeTextureModelD225 = glm::translate(cubeTextureModelD225, glm::vec3(11.1, 4.0, 3.0));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD225, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD226 = glm::mat4(1.0);
		cubeTextureModelD226 = glm::translate(cubeTextureModelD226, glm::vec3(11.0, 4.0, 2.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD226, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD227 = glm::mat4(1.0);
		cubeTextureModelD227 = glm::translate(cubeTextureModelD227, glm::vec3(11.0, 4.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD227, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD228 = glm::mat4(1.0);
		cubeTextureModelD228 = glm::translate(cubeTextureModelD228, glm::vec3(11.0, 4.0, 5.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD228, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD234 = glm::mat4(1.0);
		cubeTextureModelD234 = glm::translate(cubeTextureModelD234, glm::vec3(11.0, 4.0, 6.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD234, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD229 = glm::mat4(1.0);
		cubeTextureModelD229 = glm::translate(cubeTextureModelD229, glm::vec3(11.0, 4.0, 7.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD229, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD230 = glm::mat4(1.0);
		cubeTextureModelD230 = glm::translate(cubeTextureModelD230, glm::vec3(11.0, 4.0, 8.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD230, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD231 = glm::mat4(1.0);
		cubeTextureModelD231 = glm::translate(cubeTextureModelD231, glm::vec3(11.0, 4.0, 9.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD231, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD232 = glm::mat4(1.0);
		cubeTextureModelD232 = glm::translate(cubeTextureModelD232, glm::vec3(11.1, 4.0, 10.0));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD232, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelD233 = glm::mat4(1.0);
		cubeTextureModelD233 = glm::translate(cubeTextureModelD233, glm::vec3(11.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelD233, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////Paredes Frontales 0/////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF0 = glm::mat4(3.0);
		cubeTextureModelF0 = glm::translate(cubeTextureModelF0,
			glm::vec3(0.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF0, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF02 = glm::mat4(3.0);
		cubeTextureModelF02 = glm::translate(cubeTextureModelF02,
			glm::vec3(1.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF02, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF03 = glm::mat4(1.0);
		cubeTextureModelF03 = glm::translate(cubeTextureModelF03,
			glm::vec3(2.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF03, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF04 = glm::mat4(1.0);
		cubeTextureModelF04 = glm::translate(cubeTextureModelF04,
			glm::vec3(3.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF04, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF05 = glm::mat4(1.0);
		cubeTextureModelF05 = glm::translate(cubeTextureModelF05,
			glm::vec3(4.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF05, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF06 = glm::mat4(1.0);
		cubeTextureModelF06 = glm::translate(cubeTextureModelF06,
			glm::vec3(5.0, 2.0, 11.1));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF06, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF07 = glm::mat4(1.0);
		cubeTextureModelF07 = glm::translate(cubeTextureModelF07,
			glm::vec3(6.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF07, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF08 = glm::mat4(1.0);
		cubeTextureModelF08 = glm::translate(cubeTextureModelF08,
			glm::vec3(7.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF08, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF09 = glm::mat4(1.0);
		cubeTextureModelF09 = glm::translate(cubeTextureModelF09,
			glm::vec3(8.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF09, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF010 = glm::mat4(1.0);
		cubeTextureModelF010 = glm::translate(cubeTextureModelF010, glm::vec3(9.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF010, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF011 = glm::mat4(1.0);
		cubeTextureModelF011 = glm::translate(cubeTextureModelF011, glm::vec3(10.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF011, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF012 = glm::mat4(1.0);
		cubeTextureModelF012 = glm::translate(cubeTextureModelF012, glm::vec3(11.0, 2.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF012, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////Paredes Frontales 1/////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF1 = glm::mat4(3.0);
		cubeTextureModelF1 = glm::translate(cubeTextureModelF1,
			glm::vec3(0.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF1, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF12 = glm::mat4(3.0);
		cubeTextureModelF12 = glm::translate(cubeTextureModelF12,
			glm::vec3(1.0, 3.0, 11.1));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF12, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF13 = glm::mat4(1.0);
		cubeTextureModelF13 = glm::translate(cubeTextureModelF13,
			glm::vec3(2.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF13, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF14 = glm::mat4(1.0);
		cubeTextureModelF14 = glm::translate(cubeTextureModelF14,
			glm::vec3(3.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF14, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF15 = glm::mat4(1.0);
		cubeTextureModelF15 = glm::translate(cubeTextureModelF15,
			glm::vec3(4.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF15, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF16 = glm::mat4(1.0);
		cubeTextureModelF16 = glm::translate(cubeTextureModelF16,
			glm::vec3(5.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF16, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF17 = glm::mat4(1.0);
		cubeTextureModelF17 = glm::translate(cubeTextureModelF17,
			glm::vec3(6.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF17, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF18 = glm::mat4(1.0);
		cubeTextureModelF18 = glm::translate(cubeTextureModelF18,
			glm::vec3(7.0, 3.0, 11.1));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF18, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF19 = glm::mat4(1.0);
		cubeTextureModelF19 = glm::translate(cubeTextureModelF19,
			glm::vec3(8.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF19, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF110 = glm::mat4(1.0);
		cubeTextureModelF110 = glm::translate(cubeTextureModelF110, glm::vec3(9.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF110, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF111 = glm::mat4(1.0);
		cubeTextureModelF111 = glm::translate(cubeTextureModelF111, glm::vec3(10.0, 3.0, 11.1));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF111, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF112 = glm::mat4(1.0);
		cubeTextureModelF112 = glm::translate(cubeTextureModelF112, glm::vec3(11.0, 3.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF112, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////Paredes Frontales 2/////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF2 = glm::mat4(3.0);
		cubeTextureModelF2 = glm::translate(cubeTextureModelF2,
			glm::vec3(0.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF2, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF22 = glm::mat4(3.0);
		cubeTextureModelF22 = glm::translate(cubeTextureModelF22,
			glm::vec3(1.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF22, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF23 = glm::mat4(1.0);
		cubeTextureModelF23 = glm::translate(cubeTextureModelF23,
			glm::vec3(2.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF23, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF24 = glm::mat4(1.0);
		cubeTextureModelF24 = glm::translate(cubeTextureModelF24,
			glm::vec3(3.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF24, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF25 = glm::mat4(1.0);
		cubeTextureModelF25 = glm::translate(cubeTextureModelF25,
			glm::vec3(4.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF25, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF26 = glm::mat4(1.0);
		cubeTextureModelF26 = glm::translate(cubeTextureModelF26,
			glm::vec3(5.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF26, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF27 = glm::mat4(1.0);
		cubeTextureModelF27 = glm::translate(cubeTextureModelF27,
			glm::vec3(6.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF27, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF28 = glm::mat4(1.0);
		cubeTextureModelF28 = glm::translate(cubeTextureModelF28,
			glm::vec3(7.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF28, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF29 = glm::mat4(1.0);
		cubeTextureModelF29 = glm::translate(cubeTextureModelF29,
			glm::vec3(8.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV",
			glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF29, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF210 = glm::mat4(1.0);
		cubeTextureModelF210 = glm::translate(cubeTextureModelF210, glm::vec3(9.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF210, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF211 = glm::mat4(1.0);
		cubeTextureModelF211 = glm::translate(cubeTextureModelF211, glm::vec3(10.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF211, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		////////////////////////////////////////////////////////////
		glm::mat4 cubeTextureModelF212 = glm::mat4(1.0);
		cubeTextureModelF212 = glm::translate(cubeTextureModelF212, glm::vec3(11.0, 4.0, 11.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box3.render(glm::scale(cubeTextureModelF212, glm::vec3(2.0, 2.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*															Tapa de la casa (Arriba)														*/

		glm::mat4 modelPiedra4 = glm::mat4(1.0);
		modelPiedra4 = glm::translate(modelPiedra4, glm::vec3(5.5, 5.0, 5.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		shaderTexture.setFloat("offsetX", offX);
		box2.render(glm::scale(modelPiedra4, glm::vec3(11.0, 0.01, 11.0)));
		shaderTexture.setFloat("offsetX", 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		/*															Tapa de la casa (Planta Baja)														*/
		glm::mat4 modelPiedra2 = glm::mat4(1.0);
		modelPiedra2 = glm::translate(modelPiedra2, glm::vec3(5.5, 1.0, 5.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		shaderTexture.setFloat("offsetX", offX);
		box2.render(glm::scale(modelPiedra2, glm::vec3(11.0, 0.01, 11.0)));
		shaderTexture.setFloat("offsetX", 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		/*															Tapa de la casa (Piso 1)														*/
		glm::mat4 modelPiedra3 = glm::mat4(1.0);
		modelPiedra3 = glm::translate(modelPiedra3, glm::vec3(5.5, 3.0, 5.5));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		shaderTexture.setFloat("offsetX", offX);
		box2.render(glm::scale(modelPiedra3, glm::vec3(11.0, 0.01, 11.0)));
		shaderTexture.setFloat("offsetX", 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		/*															Tapa de la casa (Cesped)														*/
		glm::mat4 modelPasto2 = glm::mat4(1.0);
		modelPasto2 = glm::translate(modelPasto2, glm::vec3(5.5, 0.8, 5.5));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		shaderTexture.setFloat("offsetX", offX);
		box2.render(glm::scale(modelPasto2, glm::vec3(22.0, 0.01, 22.0)));
		shaderTexture.setFloat("offsetX", 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////// Figura 1 ///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		glBindTexture(GL_TEXTURE_2D, textureID17);
		shaderTexture.setFloat("offsetX", 0);
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(o1, glm::vec3(0.20f, 0.20f, 0.10f)));

		//ojo 2
		glm::mat4 o2 = glm::translate(model, glm::vec3(-0.25f, 0.25f, 0.05f));
		//sphere1.enableWireMode();
		glBindTexture(GL_TEXTURE_2D, textureID17);
		shaderTexture.setFloat("offsetX", 0);
		sphere1.render(glm::scale(o2, glm::vec3(0.20f, 0.20f, 0.10f)));

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////7
		glm::mat4 modelCylinder = glm::mat4(1.0);
		modelCylinder = glm::translate(modelCylinder, glm::vec3(-3.0, 0.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		shaderTexture.setFloat("offsetX", 0);
		////////////////////////////////////////////////////////////
		cylinder2.render(0, cylinder2.getSlices() * cylinder2.getStacks() * 6, modelCylinder);
		glBindTexture(GL_TEXTURE_2D, textureID2);
		// Se usa la textura 1 ( Bob sponja)
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder2.render(0, cylinder2.getSlices() * cylinder2.getStacks() * 6,
			modelCylinder);
		///////////////////Pantalones
		glm::mat4 pantalones = glm::translate(model, glm::vec3(0.0, -0.3, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID18);
		shaderTexture.setFloat("offsetX", 0);
		box1.render(glm::scale(pantalones, glm::vec3(1.0, 0.45, 0.21)));
		glBindTexture(GL_TEXTURE_2D, 0);
		/////////////////////////////////////////////////

		shader.turnOff();
		dz = 0;
		rot0 = 0;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////// Figura 2 ///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4 projection2 = glm::perspective(glm::radians(120.0f), (float)screenWidth / (float)screenHeight, 0.01f, 1000.0f);
		glm::mat4 view2 = camera->getViewMatrix();

		shader.setMatrix4("projection2", 1, false, glm::value_ptr(projection2));
		shader.setMatrix4("view2", 1, false, glm::value_ptr(view2));

		model2 = glm::translate(model2, glm::vec3(0, 0, dz));
		model2 = glm::rotate(model2, rot0, glm::vec3(0, 1, 0));
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
		j111 = glm::rotate(j111, rot4_2d, glm::vec3(0, 1, 0));
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
		j112 = glm::rotate(j112, rot4_2, glm::vec3(0, 1, 0));
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//glm::mat4 matrixModelAircraft = glm::mat4(1.0);
		//matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(8.0, 2.0, -10.0));
		modelAircraft.render(matrixModelAircraft);
		glActiveTexture(GL_TEXTURE0);

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.0001;

		// Se Dibuja el Skybox
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		dz = 0;
		rot0 = 0;
		offX += 0.001;
		switch (state) {
		case 0:
			std::cout << "Advance: " << std::endl;
			//-0.01 debe se rigual
			matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(0.0, 0.0, -0.1));
			offsetAircraftAdvance += 0.01;
			if (offsetAircraftAdvance > 5.0) {
				offsetAircraftAdvance = 0.0;
				state = 1;
			}
			break;
		case 1:
			std::cout << "Turn: " << std::endl;
			matrixModelAircraft = glm::translate(matrixModelAircraft, glm::vec3(0,0,-0.001));
			matrixModelAircraft = glm::rotate(matrixModelAircraft, glm::radians(0.05f), glm::vec3(0, 1, 0));
			offsetairfart += 0.05;
			if (offsetairfart > 90.0) {
				offsetairfart = 0.0;
				state = 0;
			}
			break;

		}

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
