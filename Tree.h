#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <iostream>
#include <vector> 
#include <cmath> 

using namespace std; 
using namespace glm;

class Tree
{
public:
	//Defining constructors
	Tree();
	Tree(float height);
	
	//Defining initializers
	void initTrunk();
	void genRandomBranch();
	void createLeafPositions();
	
	//Defining drawing functions
	void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric);
	void renderCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions); 
	void drawTree(int stage, int leafiness);
	
	//Defining calculation functions
	void spaceAlgorithm();
	void removeBranches();
	vec3 avgNormals();
	void clearArray();
	
};
