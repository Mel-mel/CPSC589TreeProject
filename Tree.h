#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
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
	
	Tree(vec2 height, vec2 width);
	void renderTree();
	//^^whenever the fractal branches off, decrease the radius by some number
	//In addition, the trunk of tree, define sections by height / num. Make
	//sure to decrease sections with very small increments
	void generateLeaves();
	//^^use fractals here too.
	void applyColor();
	//void applyBumpMap();//Apply random bitmap to the tree to have to "texture" to it
	
private:
	glm::vec2 height, width;
	float radius;
};
