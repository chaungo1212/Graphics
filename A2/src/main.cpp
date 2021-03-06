#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <map>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"
#include "Component.h"

using namespace std;
using namespace glm;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "./"; // Where the resources are loaded from
shared_ptr<Program> prog;
shared_ptr<Shape> shape;

Robot robot; // The robot is going to be created.
GLuint progID;
map<string,GLint> attrIDs;
map<string,GLint> unifIDs;
map<string,GLuint> bufIDs;


// This function is called when a GLFW error occurs
static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

// This function is called when a key is pressed
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	// '.': Move forward.  ',': Move backward.
	if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS) { cout << ".\n";
		robot.selectForward();
	}
	if (key == GLFW_KEY_COMMA && action == GLFW_PRESS) { cout << ",\n";
		robot.selectBackward();
	}
	
	if (key == GLFW_KEY_X && action == GLFW_PRESS){
		if(mods == GLFW_MOD_SHIFT){
			//cout << "X\n";
			robot.rotate('X');
		}
		else{
			//cout << "x\n";
			robot.rotate('x');
		}
		
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS){
		if(mods == GLFW_MOD_SHIFT){
			//cout << "Y\n";
			robot.rotate('Y');
		}
		else{
			//cout << "y\n";
			robot.rotate('y');
		}
		
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS){
		if(mods == GLFW_MOD_SHIFT){
			//cout << "Z\n";
			robot.rotate('Z');
		}
		else{
			//cout << "z\n";
			robot.rotate('z');
		}
		
	}
	
	
}

// This function is called when the mouse is clicked
static void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
	// Do nothing
}

// If the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// This function is called once to initialize the scene and OpenGL
static void init()
{
	

	GLSL::checkVersion();

	// Set background color.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	// Initialize the GLSL program.
	prog = make_shared<Program>();
	prog->setVerbose(true); // Set this to true when debugging.
	prog->setShaderNames(RESOURCE_DIR + "vert.glsl", RESOURCE_DIR + "frag.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->setVerbose(false);

	//create the robot with component
	robot.setResourceDir(RESOURCE_DIR);
	robot.create();
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);
}

// This function is called every frame to draw the scene.
static void render()
{

	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = width / (float)height;
	glViewport(0, 0, width, height);

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create matrix stacks.
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	// Apply projection.
	P->pushMatrix();
	P->multMatrix(glm::perspective((float)(45.0*M_PI / 180.0), aspect, 0.01f, 100.0f));

	// Apply camera transform.
	MV->pushMatrix();
	MV->translate(glm::vec3(0, 2.5, -30));
	robot.draw(P, MV, prog);

	// Pop matrix stacks.
	MV->popMatrix();
	P->popMatrix();

	GLSL::checkError(GET_FILE_LINE);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");

	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "CHAU NGO", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	GLSL::checkVersion();
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Set the mouse call back.
	glfwSetMouseButtonCallback(window, mouse_callback);
	// Set the window resize call back.
	glfwSetFramebufferSizeCallback(window, resize_callback);
	// Initialize scene.
	init();
	double startTime = glfwGetTime();
	cout << startTime << endl;
	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
