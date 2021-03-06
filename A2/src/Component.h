#include <cassert>
#include <cstring>
#include <cmath>
#include <iostream>
#include <iostream>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Shape.h"

using namespace glm;
using namespace std;

#define PI 3.14159265

class Component{
public:
	int ID;
	vector<vec3> compDef;   //0: translationWithParentJoint - translation of the component's joint with respect to the parent component's joint.
				//1: jointAngle - current joint angles about the X, Y, and Z axes of the component's joint.
				//2: translationMeshToJoint - translation of the component's mesh with respect to its joint
				//3: scaleMesh - X, Y, and Z scaling factors for the mesh.
	vector<Component*> children;//holds the branches of the parent component, torso has 5, upper limbs have 1, head has 0
	shared_ptr<Shape> shape;
	Component(){};
	Component(vector<vec3> icompDef, int id, string RESOURCE_DIR);
	void addChild(Component* childComp);
	void draw(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, const shared_ptr<Program> prog);
	virtual ~Component(){};	
};

class Robot{  //collection of all components of the robot
private: 
	string resourceDir;
	int N; // # of elements.
	vector<Component*> trajectory;
	void addRoot(Component R);
	void addHead(Component* head);
	void addUpArm(Component* UpArm); // UpArm already links to its lower arm.
	void addUpLeg(Component* UpLeg); // UpLeg already links to its lower leg.
	void rotateX(int flag); // 'x' -> flag > 0:increment  'X' -> flag <= 0:decrement
	void rotateY(int flag); // 'y' -> flag > 0:increment  'Y' -> flag <= 0:decrement
	void rotateZ(int flag); // 'z' -> flag > 0:increment  'Z' -> flag <= 0:decrement
	
	vector<vector<int>> adj;
	void DFSUtil(Component* comp, vector<bool> &visited);
public:
	int selectID; // -1: No component has been selected yet.
	Robot(){this->resourceDir = "../resources/"; selectID = -1;};
	Robot(string RESOURCE_DIR){this->resourceDir = RESOURCE_DIR;  selectID = -1;};
	virtual ~Robot();
	void setResourceDir(string RESOURCE_DIR);
	Component* root; // Here we utilize Torso.
	void draw(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, const shared_ptr<Program> prog);
	void create();	
	void selectForward();
	void selectBackward();
	void rotate(char flag);
	void DFS();
};
