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
float x_end, y_end, z_end;

float x_delta, y_delta, z_delta;
float stem_length;

int start_index = 1;
int num_recursion = 10;

float leaf_theta = M_PI/4;

vector<glm::vec3> splitLeafPoints;
vector<glm::vec3> splitLeafPointsReverse;

Leaf::Leaf()
{
	
}

void Leaf::drawLeaf(float x, float y, float z)
{
	x_start = x;
	y_start = y;
	z_start = z;
	
	x_delta = y_delta = z_delta = stem_length = 0.1; // if you change this value, you change the size of the leaf
	
	splitLeafPoints.push_back(vec3(x_start, y_start, z_start));
	splitLeafPoints.push_back(vec3(x_start, y_start + stem_length, z_start + z_delta));	
	
	glTranslatef(-x_start, -y_start, -z_start);
	//glRotatef(15.0f, 0.0, 1.0, 0.0);
	glTranslatef(x_start, y_start, z_start);
	
	
	glBegin(GL_LINES);
	glColor3f(0.184f, 0.409f, 0.184f);
	glVertex3f(splitLeafPoints[0].x, splitLeafPoints[0].y, splitLeafPoints[0].z);
	glVertex3f(splitLeafPoints[1].x, splitLeafPoints[1].y, splitLeafPoints[1].z);
	glEnd();
	
	fractals(start_index);
	
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

void Leaf::drawGenericLeaf()
{
	x_start = z_start = 0;
	y_start = -0.5;
	
	x_delta = y_delta = z_delta = 0.1;
	stem_length = 0.1;
	
	splitLeafPoints.push_back(vec3(x_start, y_start, z_start));
	splitLeafPoints.push_back(vec3(x_start, y_start + stem_length, z_start + z_delta));	
	
	glBegin(GL_LINES);
	glColor3f(0.184f, 0.409f, 0.184f);
	glVertex3f(splitLeafPoints[0].x, splitLeafPoints[0].y, splitLeafPoints[0].z);
	glVertex3f(splitLeafPoints[1].x, splitLeafPoints[1].y, splitLeafPoints[1].z);
	glEnd();
	
	fractals(start_index);
	
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
		//Left branch
		vec3 leftTemp = splitLeafPoints[n];
		leftTemp.x = (leftTemp.x - x_delta);
		leftTemp.y = (leftTemp.y + y_delta);
		splitLeafPoints.push_back(leftTemp);
		
		//Right branch
		vec3 rightTemp = splitLeafPoints[n];
		rightTemp.x = (rightTemp.x + x_delta);
		rightTemp.y = (rightTemp.y + y_delta);
		splitLeafPoints.push_back(rightTemp);
		
		vec3 straightTemp = splitLeafPoints[n];
		straightTemp.x = straightTemp.x; // moving up, not to the side
		straightTemp.y = straightTemp.y + stem_length;
		straightTemp.z = straightTemp.z + z_delta;
		splitLeafPoints.push_back(straightTemp);
		
		x_delta -= 0.01;
		y_delta -= 0.01;
	}
	//Generate the left and right branches
	else
	{	
		//Left branch
		vec3 leftTemp = splitLeafPoints[n];
		leftTemp.x = (leftTemp.x - x_delta);
		leftTemp.y = (leftTemp.y + y_delta);
		splitLeafPoints.push_back(leftTemp);
		
		//Right branch
		vec3 rightTemp = splitLeafPoints[n];
		rightTemp.x = (rightTemp.x + x_delta);
		rightTemp.y = (rightTemp.y +  y_delta);
		splitLeafPoints.push_back(rightTemp);
		
		vec3 straightTemp = splitLeafPoints[n];
		straightTemp.x = straightTemp.x; // moving up, not to the side
		straightTemp.y = straightTemp.y + stem_length;
		straightTemp.z = straightTemp.z + z_delta;
		splitLeafPoints.push_back(straightTemp);
		
		x_delta -= 0.01;
		y_delta -= 0.01;
		
		fractals(n+3);
	}
}





