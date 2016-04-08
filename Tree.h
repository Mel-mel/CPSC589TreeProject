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
	//Defining variables
	glm::vec2 heightBase, heightTop, width1, width2;
	float radius;
	
	//Defining functions
	Tree();
	Tree(float height);
	void renderTreeStageOne();
	void generateLeaves();
	void applyColor();

	void printvalues();
	void fractals(int n);
	void renderTreeStageTwo();

	void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric);
	void renderCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions); 
	
	void setValues(vec3 hBase, vec3 hTop, vec3 w1, vec3 w2);
	
	void spaceAlgorithm();
	void initTrunk();
	void drawTree(int stage, int leafiness);
	void genRandomBranch();
	vec3 avgNormals();
	void removeBranches();
	void createLeafPositions();
	void clearArray();
	
};
