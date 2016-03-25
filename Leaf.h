#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <iostream>
#include <vector> 
#include <cmath> 

using namespace std; 
using namespace glm;

class Leaf
{
public:

	Leaf();
	void drawLeaf(float x_start, float y_start, float z_start);
	void drawLargeLeaf();
	void fractals(int n);
	//void fractalsReverse(int n);
	//void reverseArray();

};
