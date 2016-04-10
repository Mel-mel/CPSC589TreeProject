#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <iostream>
#include <vector> 
#include <cmath> 
#include "Leaf.h"
#include "Tree.h"

using namespace std; 
using namespace glm;

float x_start, y_start, z_start;
float x_delta, y_delta, z_delta;
float stem_length;

int start_index = 1;
int num_recursion = 10;

float leaf_theta = M_PI/4;

vector<glm::vec3> splitLeafPoints;

Leaf::Leaf()
{
	//Blank initializer
}

/*
 * The following function draws a leaf given a point on 
 * a branch. All other points on the leaf are based off of 
 * the initial start point. 
 */
void Leaf::drawLeaf(float x, float y, float z)
{
	//Storing the start point defined by a point on a branch
	x_start = x;
	y_start = y;
	z_start = z;
	
	x_delta = y_delta = z_delta = stem_length = 0.1; // if you change this value, you change the size of the leaf
	
	splitLeafPoints.push_back(vec3(x_start, y_start, z_start));
	splitLeafPoints.push_back(vec3(x_start, y_start + stem_length, z_start + z_delta));	
	
	//Drawing the initial stem
	glBegin(GL_LINES);
	glColor3f(0.184f, 0.409f, 0.184f);
	glVertex3f(splitLeafPoints[0].x, splitLeafPoints[0].y, splitLeafPoints[0].z);
	glVertex3f(splitLeafPoints[1].x, splitLeafPoints[1].y, splitLeafPoints[1].z);
	glEnd();
	
	//Generating all the points before we draw them
	fractals(start_index);
	
	/*
	 * Drawing the main shape of the leaf from the given start index, 
	 * the lines must loop back on each other to enclose the polygon
	 */
	glBegin(GL_POLYGON);
	glColor3f(0.184f, 0.409f, 0.184f);
	int i = start_index;
	glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
	glVertex3f(splitLeafPoints[i+9].x, splitLeafPoints[i+9].y, splitLeafPoints[i+9].z);
	
	glVertex3f(splitLeafPoints[i+9].x, splitLeafPoints[i+9].y, splitLeafPoints[i+9].z);
	glVertex3f(splitLeafPoints[i+7].x, splitLeafPoints[i+7].y, splitLeafPoints[i+7].z);
	
	glVertex3f(splitLeafPoints[i+7].x, splitLeafPoints[i+7].y, splitLeafPoints[i+7].z);
	glVertex3f(splitLeafPoints[i+4].x, splitLeafPoints[i+4].y, splitLeafPoints[i+4].z);
	
	glVertex3f(splitLeafPoints[i+4].x, splitLeafPoints[i+4].y, splitLeafPoints[i+4].z);
	glVertex3f(splitLeafPoints[i+1].x, splitLeafPoints[i+1].y, splitLeafPoints[i+1].z);
	
	glVertex3f(splitLeafPoints[i+1].x, splitLeafPoints[i+1].y, splitLeafPoints[i+1].z);
	glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
	
	glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
	glVertex3f(splitLeafPoints[i+9].x, splitLeafPoints[i+9].y, splitLeafPoints[i+9].z);
	
	glVertex3f(splitLeafPoints[i+9].x, splitLeafPoints[i+9].y, splitLeafPoints[i+9].z);
	glVertex3f(splitLeafPoints[i+8].x, splitLeafPoints[i+8].y, splitLeafPoints[i+8].z);
	
	glVertex3f(splitLeafPoints[i+8].x, splitLeafPoints[i+8].y, splitLeafPoints[i+8].z);
	glVertex3f(splitLeafPoints[i+5].x, splitLeafPoints[i+5].y, splitLeafPoints[i+5].z);
	
	glVertex3f(splitLeafPoints[i+5].x, splitLeafPoints[i+5].y, splitLeafPoints[i+5].z);
	glVertex3f(splitLeafPoints[i+2].x, splitLeafPoints[i+2].y, splitLeafPoints[i+2].z);
	
	glVertex3f(splitLeafPoints[i+2].x, splitLeafPoints[i+2].y, splitLeafPoints[i+2].z);
	glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
	glEnd();
	
	//Drawing the veins of the leaf
	glBegin(GL_LINES);
	for(int i = 1; i < 9; i+=3)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[i+1].x, splitLeafPoints[i+1].y, splitLeafPoints[i+1].z);
		
		glVertex3f(splitLeafPoints[i+1].x, splitLeafPoints[i+1].y, splitLeafPoints[i+1].z);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[i+2].x, splitLeafPoints[i+2].y, splitLeafPoints[i+2].z);
		
		glVertex3f(splitLeafPoints[i+2].x, splitLeafPoints[i+2].y, splitLeafPoints[i+2].z);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[i+3].x, splitLeafPoints[i+3].y, splitLeafPoints[i+3].z);
	
		glVertex3f(splitLeafPoints[i+3].x, splitLeafPoints[i+3].y, splitLeafPoints[i+3].z);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
	}
	glEnd();
	splitLeafPoints.clear();
}

/*
 * The following code draws a generic leaf at a larger scale
 * when the user presses KEY_L
 */
void Leaf::drawGenericLeaf()
{
	//Drawing a generic leaf is hard coded so the user can see it 
	//at a larger scale to see the details
	x_start = z_start = 0;
	y_start = -0.5;
	
	x_delta = y_delta = z_delta = 0.1;
	stem_length = 0.1;
	
	splitLeafPoints.push_back(vec3(x_start, y_start, z_start));
	splitLeafPoints.push_back(vec3(x_start, y_start + stem_length, z_start + z_delta));	
	
	//Defining the stem
	glBegin(GL_LINES);
	glColor3f(0.184f, 0.409f, 0.184f);
	glVertex3f(splitLeafPoints[0].x, splitLeafPoints[0].y, splitLeafPoints[0].z);
	glVertex3f(splitLeafPoints[1].x, splitLeafPoints[1].y, splitLeafPoints[1].z);
	glEnd();
	
	//Generating all the points before we draw them
	fractals(start_index);
	
	/*
	 * Drawing the main shape of the leaf from the given start index, 
	 * the lines must loop back on each other to enclose the polygon
	 */
	glBegin(GL_POLYGON);
	glColor3f(0.184f, 0.409f, 0.184f);
	int i = start_index;
	glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
	glVertex3f(splitLeafPoints[i+9].x, splitLeafPoints[i+9].y, splitLeafPoints[i+9].z);
	
	glVertex3f(splitLeafPoints[i+9].x, splitLeafPoints[i+9].y, splitLeafPoints[i+9].z);
	glVertex3f(splitLeafPoints[i+7].x, splitLeafPoints[i+7].y, splitLeafPoints[i+7].z);
	
	glVertex3f(splitLeafPoints[i+7].x, splitLeafPoints[i+7].y, splitLeafPoints[i+7].z);
	glVertex3f(splitLeafPoints[i+4].x, splitLeafPoints[i+4].y, splitLeafPoints[i+4].z);
	
	glVertex3f(splitLeafPoints[i+4].x, splitLeafPoints[i+4].y, splitLeafPoints[i+4].z);
	glVertex3f(splitLeafPoints[i+1].x, splitLeafPoints[i+1].y, splitLeafPoints[i+1].z);
	
	glVertex3f(splitLeafPoints[i+1].x, splitLeafPoints[i+1].y, splitLeafPoints[i+1].z);
	glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
	
	glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
	glVertex3f(splitLeafPoints[i+9].x, splitLeafPoints[i+9].y, splitLeafPoints[i+9].z);
	
	glVertex3f(splitLeafPoints[i+9].x, splitLeafPoints[i+9].y, splitLeafPoints[i+9].z);
	glVertex3f(splitLeafPoints[i+8].x, splitLeafPoints[i+8].y, splitLeafPoints[i+8].z);
	
	glVertex3f(splitLeafPoints[i+8].x, splitLeafPoints[i+8].y, splitLeafPoints[i+8].z);
	glVertex3f(splitLeafPoints[i+5].x, splitLeafPoints[i+5].y, splitLeafPoints[i+5].z);
	
	glVertex3f(splitLeafPoints[i+5].x, splitLeafPoints[i+5].y, splitLeafPoints[i+5].z);
	glVertex3f(splitLeafPoints[i+2].x, splitLeafPoints[i+2].y, splitLeafPoints[i+2].z);
	
	glVertex3f(splitLeafPoints[i+2].x, splitLeafPoints[i+2].y, splitLeafPoints[i+2].z);
	glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
	glEnd();
	
	//Drawing the veins of the leaf
	glBegin(GL_LINES);
	for(int i = 1; i < 9; i+=3)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[i+1].x, splitLeafPoints[i+1].y, splitLeafPoints[i+1].z);
		
		glVertex3f(splitLeafPoints[i+1].x, splitLeafPoints[i+1].y, splitLeafPoints[i+1].z);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[i+2].x, splitLeafPoints[i+2].y, splitLeafPoints[i+2].z);
		
		glVertex3f(splitLeafPoints[i+2].x, splitLeafPoints[i+2].y, splitLeafPoints[i+2].z);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[i+3].x, splitLeafPoints[i+3].y, splitLeafPoints[i+3].z);
	
		glVertex3f(splitLeafPoints[i+3].x, splitLeafPoints[i+3].y, splitLeafPoints[i+3].z);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		
	}
	glEnd();
	
	splitLeafPoints.clear();
}
void Leaf::fractals(int n)
{	
	if(n == num_recursion)
	{
		//Left branching
		vec3 leftTemp = splitLeafPoints[n];
		leftTemp.x = (leftTemp.x - x_delta);
		leftTemp.y = (leftTemp.y + y_delta);
		splitLeafPoints.push_back(leftTemp);
		
		//Right branching
		vec3 rightTemp = splitLeafPoints[n];
		rightTemp.x = (rightTemp.x + x_delta);
		rightTemp.y = (rightTemp.y + y_delta);
		splitLeafPoints.push_back(rightTemp);
		
		//Main stem
		vec3 straightTemp = splitLeafPoints[n];
		straightTemp.x = straightTemp.x; // moving up, not to the side
		straightTemp.y = straightTemp.y + stem_length;
		straightTemp.z = straightTemp.z + z_delta;
		splitLeafPoints.push_back(straightTemp);
		
	}
	//Generate the left, middle and right portions of the veins 
	else
	{	
		//Left branching
		vec3 leftTemp = splitLeafPoints[n];
		leftTemp.x = (leftTemp.x - x_delta);
		leftTemp.y = (leftTemp.y + y_delta);
		splitLeafPoints.push_back(leftTemp);
		
		//Right branching
		vec3 rightTemp = splitLeafPoints[n];
		rightTemp.x = (rightTemp.x + x_delta);
		rightTemp.y = (rightTemp.y +  y_delta);
		splitLeafPoints.push_back(rightTemp);
		
		//Main stem
		vec3 straightTemp = splitLeafPoints[n];
		straightTemp.x = straightTemp.x; // moving up, not to the side
		straightTemp.y = straightTemp.y + stem_length;
		straightTemp.z = straightTemp.z + z_delta;
		splitLeafPoints.push_back(straightTemp);
		
		//Decrease the change slightly to create 
		//a better leaf shape
		x_delta -= 0.01;
		y_delta -= 0.01;
		
		//The left and right veins do not have futher 
		//lines coming from them so we skip to the next 
		//main portion of the stem
		fractals(n+3);
	}
}





