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

#include "Headers/AnimationUtils.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow* window;

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

// Descomentar
Sphere skyboxSphere(20, 20);
Cylinder cylinder1(20, 20, 0.5, 0.5);
Cylinder cylinder2(20, 20, 0.5, 0.5);

Box box1;
Box box2;
// Models complex instances
Model modelTV;
Model modelBed;
Model modelChair;
Model modelCurtain;
Model modelPickup;

GLuint textureID1, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, textureID8;
GLuint textureID9, textureID10, textureID11, textureID12, textureID13, textureID14, textureID15, textureID16;
GLuint textureID17, textureID18, textureID19, textureID20, textureID21, textureID22, textureID23, textureID24;
GLuint textureID25, textureID26;

GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };


std::string fileNames[6] = { "../ProyectoFinal/Textures/ame_starfield/starfield_ft.tga",
		"../ProyectoFinal/Textures/ame_starfield/starfield_bk.tga",
		"../ProyectoFinal/Textures/ame_starfield/starfield_up.tga",
		"../ProyectoFinal/Textures/ame_starfield/starfield_dn.tga",
		"../ProyectoFinal/Textures/ame_starfield/starfield_rt.tga",
		"../ProyectoFinal/Textures/ame_starfield/starfield_lf.tga" };


bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

//float rot0 = 0.0, dz = 0.0;
//float rot1 = 0.0, rot2 = 0.0, rot3 = 0.0, rot4 = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

double deltaTime;
double currTime, lastTime;

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

	shader.initialize("../ProyectoFinal/Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderTexture.initialize("../ProyectoFinal/Shaders/texturizado_res.vs",
		"../ProyectoFinal/Shaders/texturizado_res.fs");
	shaderColorLighting.initialize("../ProyectoFinal/Shaders/iluminacion_color_res.vs",
		"../ProyectoFinal/Shaders/iluminacion_color_res.fs");
	shaderTextureLighting.initialize("../ProyectoFinal/Shaders/iluminacion_texture_res.vs",
		"../ProyectoFinal/Shaders/iluminacion_texture_res.fs");
	shaderMaterialLighting.initialize("../ProyectoFinal/Shaders/iluminacion_material.vs",
		"../ProyectoFinal/Shaders/iluminacion_material_res.fs");
	shaderSkybox.initialize("../ProyectoFinal/Shaders/cubeTexture.vs",
		"../ProyectoFinal/Shaders/cubeTexture.fs");
	shaderMulLighting.initialize("../ProyectoFinal/Shaders/iluminacion_texture_res.vs",
		"../ProyectoFinal/Shaders/multipleLights.fs");

	// Inicializar los buffers VAO, VBO, EBO
	sphere1.init();
	// Método setter que colocar el apuntador al shader
	sphere1.setShader(&shaderColorLighting);
	//Setter para poner el color de la geometria
	sphere1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	// Inicializar los buffers VAO, VBO, EBO
	sphere2.init();
	sphere2.setShader(&shaderMulLighting);

	cylinder1.init();
	cylinder1.setShader(&shaderColorLighting);
	cylinder1.setColor(glm::vec4(0.3, 0.3, 1.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shaderMulLighting);

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

	modelBed.loadModel("../ProyectoFinal/Models/bed/bed 2.obj");
	modelBed.setShader(&shaderMulLighting);

	modelTV.loadModel("../ProyectoFinal/Models/tv/OBJ/Samsung LED TV.obj");
	modelTV.setShader(&shaderMulLighting);

	modelChair.loadModel("../ProyectoFinal/Models/chair/chair strong_01.obj");
	modelChair.setShader(&shaderMulLighting);

	modelCurtain.loadModel("../ProyectoFinal/Models/curtain/curtain.obj");
	modelCurtain.setShader(&shaderMulLighting);


	modelPickup.loadModel("../ProyectoFinal/Models/car/Pickup/L200-OBJ.obj");
	modelPickup.setShader(&shaderMulLighting);


	camera->setPosition(glm::vec3(0.0, -3.0, 20.0));

	//----------------------------------------------Texturas
	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	// Definiendo la textura a utilizar
	Texture texture1("../ProyectoFinal/Textures/House/carretera.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	FIBITMAP* bitmap = texture1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	unsigned char* data = texture1.convertToData(bitmap, imageWidth,
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

	Texture texture2("../ProyectoFinal/Textures/House/asfalto.jpg");
	bitmap = texture2.loadImage();
	data = texture2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID2);
	glBindTexture(GL_TEXTURE_2D, textureID2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture2.freeImage(bitmap);

	Texture texture3("../ProyectoFinal/Textures/House/pared1.jpg");
	bitmap = texture3.loadImage(true);
	data = texture3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID3);
	glBindTexture(GL_TEXTURE_2D, textureID3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture3.freeImage(bitmap);

	Texture texture4("../ProyectoFinal/Textures/House/azulejo2.jpg");
	bitmap = texture4.loadImage(true);
	data = texture4.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID4);
	glBindTexture(GL_TEXTURE_2D, textureID4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture4.freeImage(bitmap);

	Texture texturePared("../ProyectoFinal/Textures/House/pared4.jpg");
	bitmap = texturePared.loadImage(true);
	data = texturePared.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID5);
	glBindTexture(GL_TEXTURE_2D, textureID5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePared.freeImage(bitmap);

	Texture textureWallOrange("../ProyectoFinal/Textures/House/pared3.jpg");
	bitmap = textureWallOrange.loadImage(true);
	data = textureWallOrange.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID6);
	glBindTexture(GL_TEXTURE_2D, textureID6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureWallOrange.freeImage(bitmap);

	Texture textureAzulejo("../ProyectoFinal/Textures/House/azulejo1.jpg");
	bitmap = textureAzulejo.loadImage(true);
	data = textureAzulejo.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID7);
	glBindTexture(GL_TEXTURE_2D, textureID7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureAzulejo.freeImage(bitmap);

	Texture textureTecho("../ProyectoFinal/Textures/House/techo.jpg");
	bitmap = textureTecho.loadImage(true);
	data = textureTecho.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID8);
	glBindTexture(GL_TEXTURE_2D, textureID8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureTecho.freeImage(bitmap);

	Texture textureEscalon("../ProyectoFinal/Textures/House/escalon.jpg");
	bitmap = textureEscalon.loadImage();
	data = textureEscalon.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID9);
	glBindTexture(GL_TEXTURE_2D, textureID9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureEscalon.freeImage(bitmap);

	Texture texturePuerta1("../ProyectoFinal/Textures/House/puerta1.jpg");
	bitmap = texturePuerta1.loadImage();
	data = texturePuerta1.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID10);
	glBindTexture(GL_TEXTURE_2D, textureID10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePuerta1.freeImage(bitmap);

	Texture texturePuerta2("../ProyectoFinal/Textures/House/puerta2.jpg");
	bitmap = texturePuerta2.loadImage();
	data = texturePuerta2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID11);
	glBindTexture(GL_TEXTURE_2D, textureID11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePuerta2.freeImage(bitmap);

	Texture texturePuerta3("../ProyectoFinal/Textures/House/puerta3.jpg");
	bitmap = texturePuerta3.loadImage();
	data = texturePuerta3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID12);
	glBindTexture(GL_TEXTURE_2D, textureID12);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePuerta3.freeImage(bitmap);

	Texture textureVentana1("../ProyectoFinal/Textures/House/ventana.png");
	bitmap = textureVentana1.loadImage();
	data = textureVentana1.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID13);
	glBindTexture(GL_TEXTURE_2D, textureID13);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureVentana1.freeImage(bitmap);

	Texture textureMadera1("../ProyectoFinal/Textures/House/madera.jpg");
	bitmap = textureMadera1.loadImage(true);
	data = textureMadera1.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID14);
	glBindTexture(GL_TEXTURE_2D, textureID14);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureMadera1.freeImage(bitmap);

	Texture textureVidrio("../ProyectoFinal/Textures/House/vidrioLampara.png");
	bitmap = textureVidrio.loadImage(true);
	data = textureVidrio.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID15);
	glBindTexture(GL_TEXTURE_2D, textureID15);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureVidrio.freeImage(bitmap);

	Texture textureBuro("../ProyectoFinal/Textures/House/cajon1.png");
	bitmap = textureBuro.loadImage(true);
	data = textureBuro.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID18);
	glBindTexture(GL_TEXTURE_2D, textureID18);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureBuro.freeImage(bitmap);

	Texture textureBuro2("../ProyectoFinal/Textures/House/cajon2.jpg");
	bitmap = textureBuro2.loadImage(false);
	data = textureBuro2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID19);
	glBindTexture(GL_TEXTURE_2D, textureID19);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureBuro2.freeImage(bitmap);

	Texture textureRopero("../ProyectoFinal/Textures/House/ropero.jpg");
	bitmap = textureRopero.loadImage(true);
	data = textureRopero.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID20);
	glBindTexture(GL_TEXTURE_2D, textureID20);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureRopero.freeImage(bitmap);

	Texture textureLuna("../ProyectoFinal/Textures/House/luna.jpg");
	bitmap = textureLuna.loadImage(false);
	data = textureLuna.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID21);
	glBindTexture(GL_TEXTURE_2D, textureID21);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureLuna.freeImage(bitmap);

	Texture textureMadera2("../ProyectoFinal/Textures/House/madera2.jpg");
	bitmap = textureMadera2.loadImage(true);
	data = textureMadera2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID22);
	glBindTexture(GL_TEXTURE_2D, textureID22);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureMadera2.freeImage(bitmap);

	Texture textureEspejo("../ProyectoFinal/Textures/House/espejo.jpg");
	bitmap = textureEspejo.loadImage(false);
	data = textureEspejo.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID23);
	glBindTexture(GL_TEXTURE_2D, textureID23);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureEspejo.freeImage(bitmap);

	Texture textureBote("../ProyectoFinal/Textures/House/cafe.jpg");
	bitmap = textureBote.loadImage(false);
	data = textureBote.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID24);
	glBindTexture(GL_TEXTURE_2D, textureID24);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureBote.freeImage(bitmap);

	Texture textureCabecera("../ProyectoFinal/Textures/House/cabecera.png");
	bitmap = textureCabecera.loadImage(false);
	data = textureCabecera.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID25);
	glBindTexture(GL_TEXTURE_2D, textureID25);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureCabecera.freeImage(bitmap);

	Texture texturePared2("../ProyectoFinal/Textures/House/pared2.jpg");
	bitmap = texturePared2.loadImage(false);
	data = texturePared2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID26);
	glBindTexture(GL_TEXTURE_2D, textureID26);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texturePared2.freeImage(bitmap);


	//--------------------------------------------------------------------------------------------------------------------
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
		FIBITMAP* bitmap = skyboxTexture.loadImage(true);
		unsigned char* data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();

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
	/*if (state == GLFW_PRESS) {
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
	}*/
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	/*
	// Condiciones para Mover el modelo
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rot1 += 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rot1 -= 0.01;
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rot2 += 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rot2 -= 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rot3 += 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rot3 -= 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rot4 += 0.01;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rot4 -= 0.01;

	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot0 = 0.1;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot0 = -0.1;
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dz = 0.1;
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dz = -0.1;
	*/


	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 1.5, 0.0));

	glm::mat4 matrixModelPickup = glm::mat4(1.0);
	matrixModelPickup = glm::translate(matrixModelPickup, glm::vec3(1.5, -5.0, 0.0));


	int statePickup = 0;
	int onoff = 0;
	float pickupAdvance = 0.0;
	float pickupRot = 0.0;
	float light = 0.0;
	float contador = 0.0;
	int cerrado = 1;
	float rotPuerta = 0.0;
	float rotLuna = 0.0;


	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<glm::mat4> matrixModel;
		std::vector<float> matrixJoints;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

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

		/*******************************************
		 * Propiedades Luz para objetos con SOLO color
		 *******************************************/
		shaderColorLighting.setVectorFloat3("viewPos",
			glm::value_ptr(camera->getPosition()));
		shaderColorLighting.setVectorFloat3("light.ambient",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.diffuse",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderColorLighting.setVectorFloat3("light.specular",
			glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		/*******************************************
		 * Propiedades Luz para objetos con SOLO texturas
		 *******************************************/
		shaderTextureLighting.setVectorFloat3("viewPos",
			glm::value_ptr(camera->getPosition()));
		shaderTextureLighting.setVectorFloat3("light.ambient",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.diffuse",
			glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTextureLighting.setVectorFloat3("light.specular",
			glm::value_ptr(glm::vec3(0.9, 0.0, 0.0)));

		/*******************************************
		 * Propiedades Luz para objetos con SOLO materiales
		 *******************************************/
		 /*shaderMaterialLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		 shaderMaterialLighting.setVectorFloat3("light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		 shaderMaterialLighting.setVectorFloat3("light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		 shaderMaterialLighting.setVectorFloat3("light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));*/

		 /*******************************************
		  * Propiedades Luz direccional
		  *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(glm::vec3(11.5, 6.8, 5.4)));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0.0, -1.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(light, light, light)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(light, light, light)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(light, light, light)));
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.0);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.009);


		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		 /*shaderMulLighting.setInt("pointLightCount", 3);
		 shaderMulLighting.setVectorFloat3("pointLights[0].position", glm::value_ptr((glm::vec3(-19.5, -2.9, 1.5))));
		 shaderMulLighting.setVectorFloat3("pointLights[0].light.ambient", glm::value_ptr(glm::vec3(0.001, 0.001, 0.0)));
		 shaderMulLighting.setVectorFloat3("pointLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.01, 0.01, 0.0)));
		 shaderMulLighting.setVectorFloat3("pointLights[0].light.specular", glm::value_ptr(glm::vec3(0.6, 0.6, 0.0)));
		 shaderMulLighting.setFloat("pointLights[0].constant", 1.0);
		 shaderMulLighting.setFloat("pointLights[0].linear", 0.04);
		 shaderMulLighting.setFloat("pointLights[0].quadratic", 0.004);


		 /*******************************************
		  * Luz que gira, solo se ocupa con los objetos con un solo color, materiales y con solo textura
		  * Importante: No sirve con objetos con la luz direccional, spots y points
		  *******************************************/
		  /*glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle,
				  glm::vec3(1.0f, 0.0f, 0.0f));
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
									  * glm::vec4(0.0, 0.0, 0.0, 1.0))));*/



									  /*------------------------------------------------------------------------------------------------------------------------
																							  CASA
									  --------------------------------------------------------------------------------------------------------------------------*/
									  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 1 - Pared derecha de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model = glm::mat4(1.0);
		pared1Model = glm::translate(pared1Model, glm::vec3(15.0, 0.0, 0.0));
		pared1Model = glm::rotate(pared1Model, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		pared1Model = glm::scale(pared1Model, glm::vec3(18.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		//shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 1.0)));
		box2.render(pared1Model);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 2 - Pared izquierda de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredModel2 = glm::mat4(1.0);
		paredModel2 = glm::translate(paredModel2, glm::vec3(8.0, 0.0, 0.0));
		paredModel2 = glm::rotate(paredModel2, glm::radians(90.0f), glm::vec3(0, 1.0, 0));
		paredModel2 = glm::scale(paredModel2, glm::vec3(18.0, 10.0, 0.01));
		box2.render(paredModel2);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared 3 - Parte trasera de la casa<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredModel3 = glm::mat4(1.0);
		paredModel3 = glm::translate(paredModel3, glm::vec3(11.5, 0.0, -9.0));
		paredModel3 = glm::scale(paredModel3, glm::vec3(7.0, 10.0, 0.01));
		box2.render(paredModel3);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Techo<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredModel4 = glm::mat4(1.0);
		paredModel4 = glm::translate(paredModel4, glm::vec3(11.5, 5.0, 0.0));
		paredModel4 = glm::rotate(paredModel4, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		paredModel4 = glm::rotate(paredModel4, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		paredModel4 = glm::scale(paredModel4, glm::vec3(18.0, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID8);
		box2.render(0, 8, paredModel4);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID3);
		box2.render(9, 20, paredModel4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Piso planta baja<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredModel5 = glm::mat4(1.0);
		paredModel5 = glm::translate(paredModel5, glm::vec3(11.5, -5.0, 0.0));
		paredModel5 = glm::rotate(paredModel5, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		paredModel5 = glm::rotate(paredModel5, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		paredModel5 = glm::scale(paredModel5, glm::vec3(18.0, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID7);
		box2.render(paredModel5);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Techo 1er Piso<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredModel6 = glm::mat4(1.0);
		paredModel6 = glm::translate(paredModel6, glm::vec3(11.5, 0.0, -5.4));
		paredModel6 = glm::rotate(paredModel6, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		paredModel6 = glm::rotate(paredModel6, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		paredModel6 = glm::scale(paredModel6, glm::vec3(7.2, 7.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID4);
		box2.render(0, 6, paredModel6);

		glm::mat4 paredModel7 = glm::mat4(1.0);
		paredModel7 = glm::translate(paredModel7, glm::vec3(11.5, 0.0, 5.4));
		paredModel7 = glm::rotate(paredModel7, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		paredModel7 = glm::rotate(paredModel7, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		paredModel7 = glm::scale(paredModel7, glm::vec3(7.2, 7.0, 0.01));
		box2.render(0, 6, paredModel7);

		glm::mat4 paredModel8 = glm::mat4(1.0);
		paredModel8 = glm::translate(paredModel8, glm::vec3(9.5, 0.0, 0.0));
		paredModel8 = glm::rotate(paredModel8, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		paredModel8 = glm::rotate(paredModel8, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		paredModel8 = glm::scale(paredModel8, glm::vec3(3.6, 3.0, 0.01));
		box2.render(0, 6, paredModel8);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, textureID3);
		box2.render(6, 35, paredModel6);
		box2.render(6, 35, paredModel7);
		box2.render(6, 35, paredModel8);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Paredes de las habitaciones<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredModel9 = glm::mat4(1.0);
		paredModel9 = glm::translate(paredModel9, glm::vec3(13.0, 0.0, -1.8));
		paredModel9 = glm::scale(paredModel9, glm::vec3(4.0, 10.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(paredModel9);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredModel10 = glm::mat4(1.0);
		paredModel10 = glm::translate(paredModel10, glm::vec3(13.0, 2.5, 1.8));
		paredModel10 = glm::scale(paredModel10, glm::vec3(4.0, 5.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(paredModel10);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 1<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model11 = glm::mat4(1.0);
		pared1Model11 = glm::translate(pared1Model11, glm::vec3(9.5, 4.5, 1.8));
		pared1Model11 = glm::scale(pared1Model11, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID26);
		//shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.4, 0.055)));
		box2.render(pared1Model11);

		glm::mat4 pared1Model12 = glm::mat4(1.0);
		pared1Model12 = glm::translate(pared1Model12, glm::vec3(8.25, 2.0, 1.8));
		pared1Model12 = glm::scale(pared1Model12, glm::vec3(0.5, 4.0, 0.01));
		//shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0005, 0.5)));
		box2.render(pared1Model12);

		glm::mat4 pared1Model13 = glm::mat4(1.0);
		pared1Model13 = glm::translate(pared1Model13, glm::vec3(10.75, 2.0, 1.8));
		pared1Model13 = glm::scale(pared1Model13, glm::vec3(0.5, 4.0, 0.01));
		//shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.0005, 0.5)));
		box2.render(pared1Model13);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta1 = glm::mat4(1.0);
		puerta1 = glm::translate(puerta1, glm::vec3(9.5, 2.0, 1.8));
		puerta1 = glm::translate(puerta1, glm::vec3(-1.0, 0.0, 0.0));
		puerta1 = glm::rotate(puerta1, glm::radians(rotPuerta), glm::vec3(0.0, 1.0, 0.0));
		puerta1 = glm::translate(puerta1, glm::vec3(1.0, 0.0, 0.0));
		puerta1 = glm::scale(puerta1, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID10);
		box2.render(puerta1);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta 2<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model14 = glm::mat4(1.0);
		pared1Model14 = glm::translate(pared1Model14, glm::vec3(9.5, 4.5, -1.8));
		pared1Model14 = glm::scale(pared1Model14, glm::vec3(3.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(pared1Model14);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model15 = glm::mat4(1.0);
		pared1Model15 = glm::translate(pared1Model15, glm::vec3(8.25, 2.0, -1.8));
		pared1Model15 = glm::scale(pared1Model15, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(pared1Model15);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model16 = glm::mat4(1.0);
		pared1Model16 = glm::translate(pared1Model16, glm::vec3(10.75, 2.0, -1.8));
		pared1Model16 = glm::scale(pared1Model16, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(pared1Model16);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta2 = glm::mat4(1.0);
		puerta2 = glm::translate(puerta2, glm::vec3(9.5, 2.0, -1.8));
		puerta2 = glm::translate(puerta2, glm::vec3(-1.0, 0.0, 0.0));
		puerta2 = glm::rotate(puerta2, glm::radians(rotPuerta), glm::vec3(0.0, 1.0, 0.0));
		puerta2 = glm::translate(puerta2, glm::vec3(1.0, 0.0, 0.0));
		puerta2 = glm::scale(puerta2, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		box2.render(puerta2);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 escaleraModel1 = glm::mat4(1.0);
		escaleraModel1 = glm::translate(escaleraModel1, glm::vec3(14.375, -3.75, 0.0));
		escaleraModel1 = glm::scale(escaleraModel1, glm::vec3(1.25, 2.5, 3.6));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box2.render(escaleraModel1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel2 = glm::mat4(1.0);
		escaleraModel2 = glm::translate(escaleraModel2, glm::vec3(13.4, -4.0, 0.9));
		escaleraModel2 = glm::scale(escaleraModel2, glm::vec3(0.7, 2.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box2.render(escaleraModel2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel3 = glm::mat4(1.0);
		escaleraModel3 = glm::translate(escaleraModel3, glm::vec3(12.7, -4.25, 0.9));
		escaleraModel3 = glm::scale(escaleraModel3, glm::vec3(0.7, 1.5, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box2.render(escaleraModel3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel4 = glm::mat4(1.0);
		escaleraModel4 = glm::translate(escaleraModel4, glm::vec3(12.0, -4.5, 0.9));
		escaleraModel4 = glm::scale(escaleraModel4, glm::vec3(0.7, 1.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box2.render(escaleraModel4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel5 = glm::mat4(1.0);
		escaleraModel5 = glm::translate(escaleraModel5, glm::vec3(11.35, -4.75, 0.9));
		escaleraModel5 = glm::scale(escaleraModel5, glm::vec3(0.7, 0.5, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box2.render(escaleraModel5);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel6 = glm::mat4(1.0);
		escaleraModel6 = glm::translate(escaleraModel6, glm::vec3(13.4, -2.1875, -0.9));
		escaleraModel6 = glm::scale(escaleraModel6, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box2.render(escaleraModel6);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel7 = glm::mat4(1.0);
		escaleraModel7 = glm::translate(escaleraModel7, glm::vec3(12.7, -1.5625, -0.9));
		escaleraModel7 = glm::scale(escaleraModel7, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box2.render(escaleraModel7);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel8 = glm::mat4(1.0);
		escaleraModel8 = glm::translate(escaleraModel8, glm::vec3(12.0, -0.9375, -0.9));
		escaleraModel8 = glm::scale(escaleraModel8, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box2.render(escaleraModel8);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 escaleraModel9 = glm::mat4(1.0);
		escaleraModel9 = glm::translate(escaleraModel9, glm::vec3(11.35, -0.3125, -0.9));
		escaleraModel9 = glm::scale(escaleraModel9, glm::vec3(0.7, 0.625, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID9);
		box2.render(escaleraModel9);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared de las escaleras<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredEscaleraModel1 = glm::mat4(1.0);
		paredEscaleraModel1 = glm::translate(paredEscaleraModel1, glm::vec3(11.35, -2.8125, 0.0));
		paredEscaleraModel1 = glm::scale(paredEscaleraModel1, glm::vec3(0.7, 4.375, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(paredEscaleraModel1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel2 = glm::mat4(1.0);
		paredEscaleraModel2 = glm::translate(paredEscaleraModel2, glm::vec3(12.05, -3.125, 0.0));
		paredEscaleraModel2 = glm::scale(paredEscaleraModel2, glm::vec3(0.7, 3.75, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(paredEscaleraModel2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel3 = glm::mat4(1.0);
		paredEscaleraModel3 = glm::translate(paredEscaleraModel3, glm::vec3(12.75, -3.4375, 0.0));
		paredEscaleraModel3 = glm::scale(paredEscaleraModel3, glm::vec3(0.7, 3.125, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(paredEscaleraModel3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModel4 = glm::mat4(1.0);
		paredEscaleraModel4 = glm::translate(paredEscaleraModel4, glm::vec3(13.45, -3.75, 0.0));
		paredEscaleraModel4 = glm::scale(paredEscaleraModel4, glm::vec3(0.7, 2.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(paredEscaleraModel4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 paredEscaleraModell15 = glm::mat4(1.0);
		paredEscaleraModell15 = glm::translate(paredEscaleraModell15, glm::vec3(11.0, -0.4375, -0.9));
		paredEscaleraModell15 = glm::scale(paredEscaleraModell15, glm::vec3(0.01, 0.375, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(paredEscaleraModell15);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Pared frontal<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 paredFrontalModel1 = glm::mat4(1.0);
		paredFrontalModel1 = glm::translate(paredFrontalModel1, glm::vec3(11.5, 4.25, 9.0));
		paredFrontalModel1 = glm::scale(paredFrontalModel1, glm::vec3(7.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box2.render(0, 8, paredFrontalModel1);

		glm::mat4 paredFrontalModel2 = glm::mat4(1.0);
		paredFrontalModel2 = glm::translate(paredFrontalModel2, glm::vec3(11.5, 0.75, 9.0));
		paredFrontalModel2 = glm::scale(paredFrontalModel2, glm::vec3(7.0, 1.5, 0.01));
		box2.render(0, 8, paredFrontalModel2);

		glm::mat4 paredFrontalModel3 = glm::mat4(1.0);
		paredFrontalModel3 = glm::translate(paredFrontalModel3, glm::vec3(14.25, 2.5, 9.0));
		paredFrontalModel3 = glm::scale(paredFrontalModel3, glm::vec3(1.5, 2.0, 0.01));
		box2.render(0, 8, paredFrontalModel3);

		glm::mat4 paredFrontalModel4 = glm::mat4(1.0);
		paredFrontalModel4 = glm::translate(paredFrontalModel4, glm::vec3(9.5, 2.5, 9.0));
		paredFrontalModel4 = glm::scale(paredFrontalModel4, glm::vec3(3.0, 2.0, 0.01));
		box2.render(0, 8, paredFrontalModel4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.02, 0.02)));
		glBindTexture(GL_TEXTURE_2D, textureID26);
		//shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.6, 0.4)));
		box2.render(9, 38, paredFrontalModel1);
		//shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.6, 0.4)));
		box2.render(9, 38, paredFrontalModel2);
		//shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.2, 0.25)));
		box2.render(9, 38, paredFrontalModel3);
		//shaderTexture.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0.4, 0.25)));
		box2.render(9, 38, paredFrontalModel4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Puerta y ventana principal<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 pared1Model17 = glm::mat4(1.0);
		pared1Model17 = glm::translate(pared1Model17, glm::vec3(11.5, -0.5, 9.0));
		pared1Model17 = glm::scale(pared1Model17, glm::vec3(7.0, 1.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box2.render(0, 8, pared1Model17);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(9, 20, pared1Model17);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model18 = glm::mat4(1.0);
		pared1Model18 = glm::translate(pared1Model18, glm::vec3(8.25, -3.0, 9.0));
		pared1Model18 = glm::scale(pared1Model18, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box2.render(0, 8, pared1Model18);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(9, 20, pared1Model18);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model19 = glm::mat4(1.0);
		pared1Model19 = glm::translate(pared1Model19, glm::vec3(10.75, -3.0, 9.0));
		pared1Model19 = glm::scale(pared1Model19, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box2.render(0, 8, pared1Model19);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(9, 20, pared1Model19);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model110 = glm::mat4(1.0);
		pared1Model110 = glm::translate(pared1Model110, glm::vec3(14.75, -3.0, 9.0));
		pared1Model110 = glm::scale(pared1Model110, glm::vec3(0.5, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box2.render(0, 8, pared1Model110);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(9, 20, pared1Model110);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 pared1Model111 = glm::mat4(1.0);
		pared1Model111 = glm::translate(pared1Model111, glm::vec3(12.75, -4.75, 9.0));
		pared1Model111 = glm::scale(pared1Model111, glm::vec3(3.5, 0.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID6);
		box2.render(0, 8, pared1Model111);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID5);
		box2.render(9, 20, pared1Model111);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta3 = glm::mat4(1.0);
		puerta3 = glm::translate(puerta3, glm::vec3(9.5, -3.0, 9.0));
		puerta3 = glm::translate(puerta3, glm::vec3(-1.0, 0.0, 0.0));
		puerta3 = glm::rotate(puerta3, glm::radians(rotPuerta), glm::vec3(0.0, 1.0, 0.0));
		puerta3 = glm::translate(puerta3, glm::vec3(1.0, 0.0, 0.0));
		puerta3 = glm::scale(puerta3, glm::vec3(2.0, 4.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID12);
		box2.render(puerta3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 puerta4 = glm::mat4(1.0);
		puerta4 = glm::translate(puerta4, glm::vec3(11.0, -3.0, -0.9));
		puerta4 = glm::scale(puerta4, glm::vec3(0.01, 4.0, 1.8));
		glBindTexture(GL_TEXTURE_2D, textureID11);
		box2.render(puerta4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Ventanas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glm::mat4 window1 = glm::mat4(1.0);
		window1 = glm::translate(window1, glm::vec3(12.25, 2.5, 9.0));
		window1 = glm::scale(window1, glm::vec3(2.5, 2.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		box2.render(window1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 window2 = glm::mat4(1.0);
		window2 = glm::translate(window2, glm::vec3(12.75, -2.75, 9.0));
		window2 = glm::scale(window2, glm::vec3(3.5, 3.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID13);
		box2.render(window2);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Lamparas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glm::mat4 modelBaseLamp1 = glm::mat4(1.0);
		modelBaseLamp1 = glm::translate(modelBaseLamp1, glm::vec3(11.5, -0.05, 4.0));
		modelBaseLamp1 = glm::scale(modelBaseLamp1, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinder2.render(modelBaseLamp1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp1 = glm::mat4(1.0);
		modelLamp1 = glm::translate(modelLamp1, glm::vec3(11.5, -0.1, 4.0));
		modelLamp1 = glm::scale(modelLamp1, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphere2.render(modelLamp1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp2 = glm::mat4(1.0);
		modelBaseLamp2 = glm::translate(modelBaseLamp2, glm::vec3(11.5, -0.05, -4.0));
		modelBaseLamp2 = glm::scale(modelBaseLamp2, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinder2.render(modelBaseLamp2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp2 = glm::mat4(1.0);
		modelLamp2 = glm::translate(modelLamp2, glm::vec3(11.5, -0.1, -4.0));
		modelLamp2 = glm::scale(modelLamp2, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphere2.render(modelLamp2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp3 = glm::mat4(1.0);
		modelBaseLamp3 = glm::translate(modelBaseLamp3, glm::vec3(11.5, 4.95, -5.4));
		modelBaseLamp3 = glm::scale(modelBaseLamp3, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinder2.render(modelBaseLamp3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp3 = glm::mat4(1.0);
		modelLamp3 = glm::translate(modelLamp3, glm::vec3(11.5, 4.9, -5.4));
		modelLamp3 = glm::scale(modelLamp3, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphere2.render(modelLamp3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelBaseLamp4 = glm::mat4(1.0);
		modelBaseLamp4 = glm::translate(modelBaseLamp4, glm::vec3(11.5, 4.95, 5.4));
		modelBaseLamp4 = glm::scale(modelBaseLamp4, glm::vec3(1.0, 0.1, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID14);
		cylinder2.render(modelBaseLamp4);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelLamp4 = glm::mat4(1.0);
		modelLamp4 = glm::translate(modelLamp4, glm::vec3(11.5, 4.9, 5.4));
		modelLamp4 = glm::scale(modelLamp4, glm::vec3(1.0, 0.2, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID15);
		sphere2.render(modelLamp4);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Terreno<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 modelGround = glm::mat4(1.0);
		modelGround = glm::translate(modelGround, glm::vec3(0.0, -5.3, 0.0));
		modelGround = glm::rotate(modelGround, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelGround = glm::scale(modelGround, glm::vec3(100.0, 100.0, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID2);
		box2.render(modelGround);
		glBindTexture(GL_TEXTURE_2D, 0);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Carretera<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 modelHighway1 = glm::mat4(1.0);
		modelHighway1 = glm::translate(modelHighway1, glm::vec3(2.0, -5.2, 0.0));
		modelHighway1 = glm::scale(modelHighway1, glm::vec3(60.0, 0.01, 50.0));
		glBindTexture(GL_TEXTURE_2D, textureID1);
		box2.render(modelHighway1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 lunaModel = glm::mat4(1.0);
		lunaModel = glm::scale(lunaModel, glm::vec3(4.0, 4.0, 4.0));
		lunaModel = glm::rotate(lunaModel, glm::radians(180.0f), glm::vec3(0, 1.0, 0));

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Muebles<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		glm::mat4 buro = glm::mat4(1.0);
		buro = glm::translate(buro, glm::vec3(14.0, 0.6, 8.0));
		buro = glm::scale(buro, glm::vec3(1.2, 1.2, 1.2));
		buro = glm::rotate(buro, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		glBindTexture(GL_TEXTURE_2D, textureID18);
		box2.render(0, 6, buro);

		glm::mat4 buro2 = glm::mat4(1.0);
		buro2 = glm::translate(buro2, glm::vec3(14.0, 0.6, 4.0));
		buro2 = glm::scale(buro2, glm::vec3(1.2, 1.2, 1.2));
		buro2 = glm::rotate(buro2, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		box2.render(0, 6, buro2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, textureID14);
		box2.render(6, 35, buro);
		box2.render(6, 35, buro2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 buro3 = glm::mat4(1.0);
		buro3 = glm::translate(buro3, glm::vec3(9.0, 1.0, 6.0));
		buro3 = glm::rotate(buro3, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		buro3 = glm::scale(buro3, glm::vec3(3.0, 1.5, 1.5));
		glBindTexture(GL_TEXTURE_2D, textureID19);
		box2.render(0, 6, buro3);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box2.render(6, 35, buro3);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 espejo = glm::mat4(1.0);
		espejo = glm::translate(espejo, glm::vec3(8.5, 2.75, 6.0));
		espejo = glm::rotate(espejo, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		espejo = glm::scale(espejo, glm::vec3(2.0, 2.0, 0.1));
		glBindTexture(GL_TEXTURE_2D, textureID23);
		box2.render(0, 6, espejo);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID14);
		box2.render(6, 35, espejo);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 ropero2 = glm::mat4(1.0);
		ropero2 = glm::translate(ropero2, glm::vec3(13.0, 2.0, 2.5));
		ropero2 = glm::scale(ropero2, glm::vec3(4.0, 4.0, 0.8));
		glBindTexture(GL_TEXTURE_2D, textureID20);
		box2.render(0, 6, ropero2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID22);
		box2.render(6, 35, ropero2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 baseBote = glm::mat4(1.0);
		baseBote = glm::translate(baseBote, glm::vec3(10.5, 0.4, 8.5));
		baseBote = glm::scale(baseBote, glm::vec3(0.7, 0.7, 0.6));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		box2.render(baseBote);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 tapaBote = glm::mat4(1.0);
		tapaBote = glm::translate(tapaBote, glm::vec3(10.5, 0.8, 8.5));
		tapaBote = glm::scale(tapaBote, glm::vec3(0.7, 0.7, 0.6));
		tapaBote = glm::rotate(tapaBote, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		glBindTexture(GL_TEXTURE_2D, textureID24);
		cylinder2.render(tapaBote);
		glBindTexture(GL_TEXTURE_2D, 0);

		glm::mat4 modelCabecera = glm::mat4(1.0);
		modelCabecera = glm::translate(modelCabecera, glm::vec3(14.7, 1.5, 6.0));
		modelCabecera = glm::rotate(modelCabecera, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
		modelCabecera = glm::scale(modelCabecera, glm::vec3(3.0, 1.5, 0.01));
		glBindTexture(GL_TEXTURE_2D, textureID25);
		box2.render(0, 8, modelCabecera);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, textureID25);
		box2.render(9, 20, modelCabecera);
		glBindTexture(GL_TEXTURE_2D, 0);


		//----------------------------------------------Modelos----------------------------------------------------
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Camas<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		glm::mat4 matrixModelBed = glm::mat4(1.0);
		matrixModelBed = glm::translate(matrixModelBed, glm::vec3(14.9, 0.0, 6.5));
		matrixModelBed = glm::rotate(matrixModelBed, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
		modelBed.render(matrixModelBed);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelTV = glm::mat4(1.0);
		matrixModelTV = glm::translate(matrixModelTV, glm::vec3(9.5, 1.75, 7.0));
		matrixModelTV = glm::rotate(matrixModelTV, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		matrixModelTV = glm::rotate(matrixModelTV, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		modelTV.render(matrixModelTV);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelChair = glm::mat4(1.0);
		matrixModelChair = glm::translate(matrixModelChair, glm::vec3(12.0, 0.0, 9.0));
		matrixModelChair = glm::rotate(matrixModelChair, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		modelChair.render(matrixModelChair);
		glActiveTexture(GL_TEXTURE0);

		glm::mat4 matrixModelCurtain = glm::mat4(1.0);
		matrixModelCurtain = glm::translate(matrixModelCurtain, glm::vec3(12.0, 1.5, 8.8));
		matrixModelCurtain = glm::rotate(matrixModelCurtain, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		modelCurtain.render(matrixModelCurtain);
		glActiveTexture(GL_TEXTURE0);


		/*******************************************
		 * Skybox
		 *******************************************/
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

		/*******************************************
				 * State machines
		*******************************************/

		//Maquina de estados mover la Pickup
		switch (statePickup)
		{
		case 0:
			//std::cout << "Advance Pickup:" << offsetPickupAdvance << std::endl;
			matrixModelPickup = glm::translate(matrixModelPickup, glm::vec3(0.0, 0.0, 0.1));
			pickupAdvance += 0.1;
			if (pickupAdvance > 6.0) {
				pickupAdvance = 0.0;
				statePickup = 1;
			}
			break;
		case 1:
			//std::cout << "Turn Pickup" << offsetPickupRot << std::endl;
			matrixModelPickup = glm::translate(matrixModelPickup, glm::vec3(0.0, 0.0, 0.1));
			matrixModelPickup = glm::rotate(matrixModelPickup, glm::radians(0.5f), glm::vec3(0, 1, 0));
			pickupRot += 0.5;
			if (pickupRot > 90) {
				pickupRot = 0.0;
				statePickup = 0;
			}
		default:
			break;
		}

		modelPickup.render(matrixModelPickup);
		glActiveTexture(GL_TEXTURE0);

		//Maquina para para prender y apagar las luces
		switch (onoff)
		{
		case 0:
			contador += 0.1;
			if (contador > 5.0) {
				onoff = 1;
				light = 1.0;
				contador = 0.0;
			}
			break;
		case 1:
			contador += 0.1;
			if (contador > 1.0) {
				onoff = 0;
				light = 0.006;
				contador = 0.0;
			}
		default:
			break;
		}

		//Maquina para abrir y cerrar puertas
		switch (cerrado)
		{
		case 0:
			rotPuerta -= 1.0;
			if (rotPuerta < 0.0) {
				cerrado = 1;
			}
			break;
		case 1:
			rotPuerta += 1.0;
			if (rotPuerta > 90.0) {
				cerrado = 0;
			}
		default:
			break;
		}

		//Maquina para mover la luna
		rotLuna = rotLuna + 0.1;
		lunaModel = glm::translate(lunaModel, glm::vec3(0.0, 0.0, 0.0));
		lunaModel = glm::rotate(lunaModel, glm::radians(rotLuna), glm::vec3(1.0, 0.0, 0.0));
		lunaModel = glm::translate(lunaModel, glm::vec3(0.0, 8.0, 0.0));
		if (rotLuna > 360.0) {
			rotLuna = 0.0;
		}
		glBindTexture(GL_TEXTURE_2D, textureID21);
		sphere2.render(lunaModel);
		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(window);
	}
}

int main(int argc, char** argv) {
	init(800, 700, "Proyecto Final Lab CGIHC", false);
	applicationLoop();
	destroy();
	return 1;
}