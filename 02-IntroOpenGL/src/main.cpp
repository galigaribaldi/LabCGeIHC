//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

int screenWidth;
int screenHeight;

GLFWwindow * window;
bool dibujo1 = true;
bool dibujo2 = false;
bool exitApp = false;
int lastMousePosX;
int lastMousePosY;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

GLint vertexShaderID, fragmentShaderID, shaderProgramID;
GLuint VAO[2], VBO[2];
typedef struct _Vertex {
	float m_Pos[3];
	float m_Color[3];
} Vertex;
// Codigo de los shaders, por ahora se crean en una cadena de texto
// Shader de vertices
const GLchar * vertexShaderSource = "#version 330 core\n"
		"layout (location=0) in vec3 in_position;\n"
		"layout (location=1) in vec3 in_color;\n"
		"out vec3 our_color;\n"
		"void main(){\n"
		"gl_Position = vec4(in_position, 1.0);\n"
		"our_color = in_color;\n"
		"}\0";
// Shader de fragmento
const GLchar * fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"in vec3 our_color;\n"
		"void main(){\n"
		"color = vec4(our_color, 1.0);\n"
		"}\0";

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

	// Compilacion de los shaders

	// Se crea el id del Vertex Shader
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	// Se agrega el codigo fuente al ID
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	// Compilación de Vertex Shader
	glCompileShader(vertexShaderID);
	GLint success;
	GLchar infoLog[512];
	// Se obtiene el estatus de la compilacion del vertex shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if(!success){
		// En caso de error se obtiene el error y lanza mensaje con error
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << "Error al compilar el VERTEX_SHADER." << infoLog << std::endl;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Codigo de la practica
	// Se crea el id del Fragment Shader
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	// Se agrega el codigo fuente al ID
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	// Compilacion de Fragment Shader
	glCompileShader(fragmentShaderID);
	// Se obtiene el estatus de la compilacion del Fragment shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		// En caso de error se obtiene el error y lanza mensaje con error
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cout << "Error al compilar el FRAGMENT_SHADER." << infoLog << std::endl;
	}
	////
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Programa con los shaders
	shaderProgramID = glCreateProgram();
	// Se agregan el vertex y fragment shader al program
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	// Proceso de linkeo
	glLinkProgram(shaderProgramID);
	// Revision de error de linkeo del programa
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR al linkear el programa." << infoLog << std::endl;
	}

	// Se definen los vertices de la geometria a dibujar
	//GLfloat vertices[] = {-0.5, -0.5, 0.0, 0.5, -0.5, 0.0, 0.0, 0.5, 0.0};
	// Se definen los vertices de la geometria a dibujar
	/*Vertex vertices[] ={
			{{-0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }},
			{ { 0.5f, -0.5f, 0.0f}, { 0.0f, 1.0f, 0.0f }},
			{ { 0.5f, 0.5f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
			{ {-0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f, 0.0f }},
			{ { 0.5f, 0.5f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}}
	};*/
	Vertex vertex_estrella[] = { {{0.0,0.0,0.0},{0.2,0.3,0.5}},
		{{0.2,0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,0.7,0.0},{0.2,0.4,0.1}},
		{{0.0,0.7,0.0},{0.1,0.6,0.45}},
		{{0.2,0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,0.85,0.0},{0.4,0.1,0.0}},
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		//Parte de abajo
		{{-0.2,0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,0.7,0.0},{0.2,0.4,0.1}},
		{{0.0,0.7,0.0},{0.1,0.6,0.45}},
		{{-0.2,0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,0.85,0.0},{0.5,0.3,0.7}},
		//// Fin de estrella 1

		//// Estrella 2
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		{{0.2,-0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,-0.7,0.0},{0.2,0.4,0.1}},
		{{0.0,-0.7,0.0},{0.1,0.6,0.45}},
		{{0.2,-0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,-0.85,0.0},{0.4,0.1,0.0}},
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		//Parte de abajo
		{{-0.2,-0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,-0.7,0.0},{0.2,0.4,0.1}},
		{{0.0,-0.7,0.0},{0.1,0.6,0.45}},
		{{-0.2,-0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,-0.85,0.0},{0.5,0.3,0.7}},
		//// Fin de estrella 2

		//// Estrella 3
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		{{0.7,0.2,0.0},{0.5,0.3,0.7}},
		{{0.7,0.0,0.0},{0.2,0.4,0.1}},
		{{0.7,0.0,0.0},{0.1,0.6,0.45}},
		{{0.7,0.2,0.0},{0.5,0.3,0.7}},
		{{0.85,0.0,0.0},{0.4,0.1,0.0}},
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		//Parte de abajo
		{{0.7,-0.2,0.0},{0.5,0.3,0.7}},
		{{0.7,0.0,0.0},{0.2,0.4,0.1}},
		{{0.7,0.0,0.0},{0.1,0.6,0.45}},
		{{0.7,-0.2,0.0},{0.5,0.3,0.7}},
		{{0.85,0.0,0.0},{0.5,0.3,0.7}},
		//// Fin de estrella 3

		//// Estrella 4
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		{{-0.7,0.2,0.0},{0.5,0.3,0.7}},
		{{-0.7,0.0,0.0},{0.2,0.4,0.1}},
		{{-0.7,0.0,0.0},{0.1,0.6,0.45}},
		{{-0.7,0.2,0.0},{0.5,0.3,0.7}},
		{{-0.85,0.0,0.0},{0.4,0.1,0.0}},
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		//Parte de abajo

		{{-0.7,-0.2,0.0},{0.5,0.3,0.7}},
		{{-0.7,0.0,0.0},{0.2,0.4,0.1}},
		{{-0.7,0.0,0.0},{0.1,0.6,0.45}},
		{{-0.7,-0.2,0.0},{0.5,0.3,0.7}},
		{{-0.85,0.0,0.0},{0.5,0.3,0.7}}

		//// Fin de estrella 4
	};
	/* Estrella
	Vertex vertices[] = {
		///Estrella 1
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		{{0.2,0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,0.7,0.0},{0.2,0.4,0.1}},
		{{0.0,0.7,0.0},{0.1,0.6,0.45}},
		{{0.2,0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,0.85,0.0},{0.4,0.1,0.0}},
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		//Parte de abajo
		{{-0.2,0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,0.7,0.0},{0.2,0.4,0.1}},
		{{0.0,0.7,0.0},{0.1,0.6,0.45}},
		{{-0.2,0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,0.85,0.0},{0.5,0.3,0.7}},
		//// Fin de estrella 1

		//// Estrella 2
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		{{0.2,-0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,-0.7,0.0},{0.2,0.4,0.1}},
		{{0.0,-0.7,0.0},{0.1,0.6,0.45}},
		{{0.2,-0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,-0.85,0.0},{0.4,0.1,0.0}},
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		//Parte de abajo
		{{-0.2,-0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,-0.7,0.0},{0.2,0.4,0.1}},
		{{0.0,-0.7,0.0},{0.1,0.6,0.45}},
		{{-0.2,-0.7,0.0},{0.5,0.3,0.7}},
		{{0.0,-0.85,0.0},{0.5,0.3,0.7}},
		//// Fin de estrella 2

		//// Estrella 3
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		{{0.7,0.2,0.0},{0.5,0.3,0.7}},
		{{0.7,0.0,0.0},{0.2,0.4,0.1}},
		{{0.7,0.0,0.0},{0.1,0.6,0.45}},
		{{0.7,0.2,0.0},{0.5,0.3,0.7}},
		{{0.85,0.0,0.0},{0.4,0.1,0.0}},
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		//Parte de abajo
		{{0.7,-0.2,0.0},{0.5,0.3,0.7}},
		{{0.7,0.0,0.0},{0.2,0.4,0.1}},
		{{0.7,0.0,0.0},{0.1,0.6,0.45}},
		{{0.7,-0.2,0.0},{0.5,0.3,0.7}},
		{{0.85,0.0,0.0},{0.5,0.3,0.7}},
		//// Fin de estrella 3

		//// Estrella 4
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		{{-0.7,0.2,0.0},{0.5,0.3,0.7}},
		{{-0.7,0.0,0.0},{0.2,0.4,0.1}},
		{{-0.7,0.0,0.0},{0.1,0.6,0.45}},
		{{-0.7,0.2,0.0},{0.5,0.3,0.7}},
		{{-0.85,0.0,0.0},{0.4,0.1,0.0}},
		{{0.0,0.0,0.0},{0.2,0.3,0.5}},
		//Parte de abajo
		
		{{-0.7,-0.2,0.0},{0.5,0.3,0.7}},
		{{-0.7,0.0,0.0},{0.2,0.4,0.1}},
		{{-0.7,0.0,0.0},{0.1,0.6,0.45}},
		{{-0.7,-0.2,0.0},{0.5,0.3,0.7}},
		{{-0.85,0.0,0.0},{0.5,0.3,0.7}}
		
		//// Fin de estrella 4
	};
	*/
	Vertex vertices[] = {
		///Rectangulo de la casa
		
			{{-0.4, -0.4, 0.0}, { 1.0, 0.0, 0.0 }}, //A
			{ { 0.4, -0.4, 0.0}, { 0.0, 1.0, 0.0 }},//B
			{ { 0.4, 0.4, 0.0}, { 0.0, 0.0, 1.0 }}, //C
			{ {-0.4, -0.4, 0.0}, { 1.0, 0.0, 0.0 }},//A
			{ { 0.4, 0.4, 0.0}, { 0.0, 0.0, 1.0 }},//B
			{{-0.4, 0.4, 0.0}, {1.0, 0.0, 1.0}},//D
			
		/// Triangulo de la casa
		
			{{0.4, 0.4, 0.0}, { 1.0, 0.0, 0.0 }}, //D
			{ { -0.4, 0.4, 0.0}, { 0.0, 1.0, 0.0 }},//B
			{ { 0.0, 0.8, 0.0}, { 0.0, 0.0, 1.0 }}, //E
		/// Fachada de la casa
			//PARTE 1
			{{-0.47, 0.40, 0.0}, { 1.0, 1.0, 1.0 }}, //F
			{ { -0.4, 0.4, 0.0}, { 1.0, 1.0, 1.0 }},//D
			{ { 0.0, 0.86, 0.0}, { 1.0, 1.0, 1.0 }}, //G

			{{0.0, 0.86, 0.0}, { 1.0, 1.0, 1.0 }}, //G
			{ { -0.4, 0.4, 0.0}, { 1.0, 1.0, 1.0 }},//D
			{ { 0.0, 0.8, 0.0}, { 1.0, 1.0, 1.0 }}, //E

			// PARTE 2
			{ {0.47, 0.40, 0.0}, { 1.0, 1.0, 1.0 }}, //F
			{ { 0.4, 0.4, 0.0}, { 1.0, 1.0, 1.0 }},//D
			{ { 0.0, 0.86, 0.0}, { 1.0, 1.0, 1.0 }}, //G
			{{0.0, 0.86, 0.0}, { 1.0, 1.0, 1.0 }}, //G
			{ { 0.4, 0.4, 0.0}, { 1.0, 1.0, 1.0 }},//D
			{ { 0.0, 0.8, 0.0}, { 1.0, 1.0, 1.0 }}, //E
		/// VENTANA Y PARTE ALTA
			// VENTANA
			{ { -0.4, 0.4, 0.0}, { 1.0, 1.0, 1.0 }},//D
			{ {0.4, 0.3, 0.0}, { 1.0, 1.0, 1.0 }}, //M
			{ { -0.4, 0.3, 0.0}, { 1.0, 1.0, 1.0 }}, //L
			{ { -0.4, 0.4, 0.0}, { 1.0, 1.0, 1.0 }},//D
			{ { 0.4, 0.4, 0.0}, { 1.0, 1.0, 1.0 }}, //B
			{ {0.4, 0.3, 0.0}, { 1.0, 1.0, 1.0 }}, //M

			//PARTE ALTA

			{ { -0.1013, 0.619, 0.0}, { 1.0, 0.0, 0.0 }},//1
			{ {-0.1013, 0.4811, 0.0}, { 1.0, 0.0, 0.0 }}, //2
			{ { 0.1000, 0.48115, 0.0}, { 1.0, 0.0, 0.0 }}, //3
			{ { 0.1000, 0.48115, 0.0}, { 1.0, 0.0, 0.0 }}, //3
			{ { 0.0986, 0.619, 0.0}, { 1.0, 0.0, 0.0 }},//4
			{ { -0.1013, 0.619, 0.0}, { 1.0, 0.0, 0.0 }},//1


			{ { -0.0792, 0.601, 0.0}, { 1.0, 1.0, 1.0 }},//H
			{ {-0.0792, 0.503, 0.0}, { 1.0, 1.0, 1.0 }}, //I
			{ { 0.076, 0.502, 0.0}, { 1.0, 1.0, 1.0 }}, //J
			{ { 0.076, 0.502, 0.0}, { 1.0, 1.0, 1.0 }}, //J
			{ { 0.076, 0.6004, 0.0}, { 1.0, 1.0, 1.0 }}, //K
			{ { -0.0792, 0.601, 0.0}, { 1.0, 1.0, 1.0 }},//H
		/// PUERTA
			//Puerta principal

			{ { -0.3409, 0.2201, 0.0}, { 1.0, 0.0,0.0 }},//k
			{ {-0.3421, -0.3798, 0.0}, { 1.0, 0.0, 0.0 }}, //l
			{ { -0.0605, -0.3798, 0.0}, { 1.0, 0.0, 0.0 }}, //m
			{ { -0.0605, -0.3798, 0.0}, { 1.0, 0.0, 0.0 }}, //m
			{ { -0.0605, 0.2201, 0.0}, { 1.0, 0.0,0.0 }},//N
			{ { -0.3409, 0.2201, 0.0}, { 1.0, 0.0,0.0 }},//k


			{ { -0.3008, -0.3577, 0.0}, { 1.0, 1.0, 1.0 }},//N
			{ {-0.1, -0.3577, 0.0}, { 1.0, 1.0, 1.0 }}, //O
			{ { -0.1, 0.2, 0.0}, { 1.0, 1.0, 1.0 }}, //P
			{ { -0.1, 0.2, 0.0}, { 1.0, 1.0, 1.0 }}, //P
			{ { -0.3, 0.2, 0.0}, { 1.0, 1.0, 1.0 }}, //Q
			{ { -0.3008, -0.3577, 0.0}, { 1.0, 1.0, 1.0 }},//N
			//Marco de la puerta
			///BAJO
			/*
			{ {-0.1028, -0.3577, 0.0}, { 1.0, 1.0, 1.0 }}, //O
			{ { -0.3208, -0.3806, 0.0}, { 1.0, 1.0, 1.0 }},//S
			{ { -0.0809, -0.3783, 0.0}, { 1.0, 1.0, 1.0 }}, //R
			{ {-0.1028, -0.3577, 0.0}, { 1.0, 1.0, 1.0 }}, //O
			{ { -0.3208, -0.3806, 0.0}, { 1.0, 1.0, 1.0 }},//S
			{ { -0.3008, -0.3577, 0.0}, { 1.0, 1.0, 1.0 }}//N
			///DERECHA
			*/
		/// VENTANA
			/// principal arriba
			{ {0.1, 0.2, 0.0}, { 1.0, 1.0, 1.0 }}, //R
			{ { 0.3, 0.2, 0.0}, { 1.0, 1.0, 1.0 }},//S
			{ { 0.1, 0.1, 0.0}, { 1.0, 1.0, 1.0 }}, //T
			{ { 0.1, 0.1, 0.0}, { 1.0, 1.0, 1.0 }}, //T
			{ { 0.3, 0.2, 0.0}, { 1.0, 1.0, 1.0 }},//S
			{ { 0.3, 0.1, 0.0}, { 1.0, 1.0, 1.0 }},//U			
			///marco medio
			{ { 0.1, 0.1, 0.0}, { 1.0, 0.0, 0.0 }}, //T
			{ { 0.3, 0.1, 0.0}, { 1.0, 0.0, 0.0 }},//U
			{ { 0.3, 0.08, 0.0}, { 1.0, 0.0, 0.0 }},//V
			{ { 0.3, 0.08, 0.0}, { 1.0, 0.0, 0.0 }},//V
			{ { 0.1, 0.08, 0.0}, { 1.0, 0.0, 0.0 }},//W
			{ { 0.1, 0.1, 0.0}, { 1.0, 0.0, 0.0 }}, //T
			///principal derecha

			{ { 0.2, 0.08, 0.0}, { 1.0, 1.0, 1.0 }},//C
			{ { 0.3, 0.08, 0.0}, { 1.0, 1.0, 1.0 }},//V
			{ { 0.2, -0.1, 0.0}, { 1.0, 1.0, 1.0 }},//B1

			{ { 0.3, 0.08, 0.0}, { 1.0, 1.0, 1.0 }},//V
			{ { 0.3, -0.1, 0.0}, { 1.0, 1.0, 1.0 }},//E1
			{ { 0.2, -0.1, 0.0}, { 1.0, 1.0, 1.0 }},//B1
			///Principal Izquierda
			{ { 0.1, 0.08, 0.0}, { 1.0, 1.0, 1.0 }},//W
			{ { 0.2, 0.08, 0.0}, { 1.0, 1.0, 1.0 }},//Z
			{ { 0.2, -0.1, 0.0}, { 1.0, 1.0, 1.0 }},//A1

			{ { 0.1, 0.08, 0.0}, { 1.0, 1.0, 1.0 }},//W
			{ { 0.2, -0.1, 0.0}, { 1.0, 1.0, 1.0 }},//A1
			{ { 0.1, -0.1, 0.0}, { 1.0, 1.0, 1.0 }},//D1
			///Marco intermedio
			{ { 0.1896, 0.0804, 0.0}, { 1.0, 0.0, 0.0 }},//Z
			{ { 0.190, -0.1, 0.0}, { 1.0, 0.0, 0.0 }},//A1
			{ { 0.2087, -0.1, 0.0}, { 1.0, 0.0, 0.0 }},//B1
			{ { 0.2087, -0.1, 0.0}, { 1.0, 0.0, 0.0 } },//B1
			{ { 0.1896, 0.0804, 0.0}, { 1.0, 0.0, 0.0 } },//Z
			{ { 0.2075, 0.0803, 0.0}, { 1.0, 0.0, 0.0 } },//C
			///Marco Bajo
			{ { 0.06, -0.1, 0.0}, { 1.0, 0.0, 0.0 } },//F1
			{ { 0.34, -0.12, 0.0}, { 1.0, 0.0, 0.0 } },//G1
			{ { 0.06, -0.12, 0.0}, { 1.0, 0.0, 0.0 } },//H1

			{ { 0.34, -0.12, 0.0}, { 1.0, 0.0, 0.0 } },//G1
			{ { 0.06, -0.1, 0.0}, { 1.0, 0.0, 0.0 } },//F1
			{ { 0.34,-0.1, 0.0}, { 1.0, 0.0, 0.0 } },//I1

	};
	size_t bufferSize = sizeof(vertices);
	size_t vertexSize = sizeof(vertices[0]);//24
	size_t rgbOffset = sizeof(vertices[0].m_Pos); //12 bytes
	std::cout << "Vertices:" << std::endl;
	std::cout << "bufferSize:" << bufferSize << std::endl;
	std::cout << "vertexSize:" << vertexSize << std::endl;
	std::cout << "rgbOffset:" << rgbOffset << std::endl;
	// Se crea el ID del VAO
	/*
	El VAO es un objeto que nos permite almacenar la estructura de nuestros datos,
	Esto es de gran utilidad debido a que solo se configura la estructura una vez
	y se puede utilizar en el loop de renderizado
	*/
	glGenVertexArrays(2, VAO);
	// Cambiamos el estado para indicar que usaremos el id del VAO
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_estrella), vertex_estrella, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)rgbOffset);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	///////////////////////////////////////////
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)rgbOffset);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// Se crea el VBO (buffer de datos) asociado al VAO


	// Cambiamos el estado para indicar que usaremos el id del VBO como Arreglo de vertices (GL_ARRAY_BUFFER)
//	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	// Copiamos los datos de los vertices a memoria del procesador grafico
	//           TIPO DE BUFFER     TAMANIO          DATOS    MODO (No cambian los datos)
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Se crea un indice para el atributo del vertice posicion, debe corresponder al location del atributo del shader
	// indice del atributo, Cantidad de datos, Tipo de dato, Normalizacion, Tamanio del bloque (Stride), offset
	// vertexSize = 6 * sizeof(float) = 24 bytes
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
	// Se habilita el atributo del vertice con indice 0 (posicion)
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)rgbOffset);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);


	// Ya que se configuro, se regresa al estado original
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	glUseProgram(0);
	glDetachShader(shaderProgramID, vertexShaderID);
	glDetachShader(shaderProgramID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glDeleteProgram(shaderProgramID);

	glBindVertexArray(VAO[1]);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VAO[2]);
	glDeleteBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glDeleteVertexArrays(2, VAO);
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
		case GLFW_KEY_0:
			dibujo1 = true;
			dibujo2 = false;
			break;
		case GLFW_KEY_1:
			dibujo2 = true;
			dibujo1 = false;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
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
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);

		// Esta linea esta comentada debido a que de momento no se usan los shaders
		glUseProgram(shaderProgramID);
		if (dibujo1 == true){
			// Se indica el buffer de datos y la estructura de estos utilizando solo el id del VAO
			glBindVertexArray(VAO[0]);
			// Primitiva de ensamble
			glDrawArrays(GL_TRIANGLES, 0, 200);
			glBindVertexArray(0);
		}
		else {
			glBindVertexArray(VAO[1]);
			// Primitiva de ensamble
			glDrawArrays(GL_TRIANGLES, 0, 200);
			glBindVertexArray(0);
		}



		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
