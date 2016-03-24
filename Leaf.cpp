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

int start_index = 1;
int num_recursion = 20;
int stopLeaf = (num_recursion/2)+1;

float leaf_theta = M_PI/4;

vector<glm::vec3> splitLeafPoints;
vector<glm::vec3> splitLeafPointsReverse;

Leaf::Leaf()
{
	
}

void Leaf::drawLeaf(float x_start, float y_start, float z_start, float x_end, float y_end, float z_end)
{
	
}

void Leaf::drawTest()
{
	x_start = z_start = x_end = z_end = 0;
	y_start = -0.75;
	y_end = 0.5;
	
	//x_delta = (x_end - x_start)/num_recursion;
	//y_delta = (y_end - y_start)/num_recursion;
	//z_delta = (z_end - z_start)/num_recursion;
	
	x_delta = y_delta = z_delta = 0.1;
	
	splitLeafPoints.push_back(vec3(x_start, y_start, z_start));
	splitLeafPoints.push_back(vec3(x_start + x_delta, y_start + y_delta, z_start + z_delta));	
	
	fractals(start_index);
	
	splitLeafPointsReverse.push_back(vec3(x_end, y_end, z_end));
	splitLeafPointsReverse.push_back(vec3(x_end + x_delta, y_end - y_delta, z_end + z_delta));
	
	fractalsReverse(start_index);
	
	/*glBegin(GL_POLYGON);
	for(int i = 1; i < splitLeafPoints.size() - stopLeaf; i++)
	{
		glColor3f(0.1f, 0.7f, 0.46f);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[2*i].x, splitLeafPoints[2*i].y, splitLeafPoints[2*i].z);
		
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[2*i+1].x, splitLeafPoints[2*i+1].y, splitLeafPoints[2*i+1].z);
	}
	glEnd();*/
	
	glBegin(GL_LINES);
	for(int i = 1; i < splitLeafPoints.size() - stopLeaf; i++)
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[2*i].x, splitLeafPoints[2*i].y, splitLeafPoints[2*i].z);
		
		glVertex3f(splitLeafPoints[i].x, splitLeafPoints[i].y, splitLeafPoints[i].z);
		glVertex3f(splitLeafPoints[2*i+1].x, splitLeafPoints[2*i+1].y, splitLeafPoints[2*i+1].z);
	}
	glEnd();
	
	//cout << "x   " << splitLeafPointsReverse[splitLeafPoints.size() - stopLeaf-1].x << "y   " << splitLeafPointsReverse[splitLeafPoints.size() - stopLeaf-1].y << "z   " << splitLeafPointsReverse[splitLeafPoints.size() - stopLeaf-1].z << endl;
	//cout << "x   " << splitLeafPointsReverse[2*splitLeafPoints.size() - stopLeaf].x << "y   " << splitLeafPointsReverse[2*splitLeafPoints.size() - stopLeaf].y << "z   " << splitLeafPointsReverse[2*splitLeafPoints.size() - stopLeaf].z << endl;
	//cout << "x   " << splitLeafPointsReverse[2*splitLeafPoints.size() - stopLeaf +1].x << "y   " << splitLeafPointsReverse[2*splitLeafPoints.size() - stopLeaf+1].y << "z   " << splitLeafPointsReverse[2*splitLeafPoints.size() - stopLeaf+1].z << endl;
	
	splitLeafPoints.clear();
	//reverseArray();
	glBegin(GL_LINES);
	for(int i = 1; i < splitLeafPointsReverse.size() - stopLeaf; i++)
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		cout << "x   " << splitLeafPointsReverse[i].x << "y   " << splitLeafPointsReverse[i].y << "z   " << splitLeafPointsReverse[i].z << endl;
		glVertex3f(splitLeafPointsReverse[i].x, splitLeafPointsReverse[i].y, splitLeafPointsReverse[i].z);
		glVertex3f(splitLeafPointsReverse[2*i].x, splitLeafPointsReverse[2*i].y, splitLeafPointsReverse[2*i].z);
		
		glVertex3f(splitLeafPointsReverse[i].x, splitLeafPointsReverse[i].y, splitLeafPointsReverse[i].z);
		glVertex3f(splitLeafPointsReverse[2*i+1].x, splitLeafPointsReverse[2*i+1].y, splitLeafPointsReverse[2*i+1].z);
	}
	glEnd();
	splitLeafPointsReverse.clear();
	
	
}
void Leaf::fractals(int n)
{	
	
	//The if should exit the function once the last set of branches are drawn
	if(n == (num_recursion/2))
	{
		//Left branch
		vec3 leftTemp = splitLeafPoints[n];
		leftTemp.x = (leftTemp.x - x_delta)*sin(leaf_theta);
		leftTemp.y = (leftTemp.y + y_delta)*cos(-leaf_theta);
		leftTemp.z = (leftTemp.z);// + z_delta);
		splitLeafPoints.push_back(leftTemp);
		
		//Right branch
		vec3 rightTemp = splitLeafPoints[n];
		rightTemp.x = (rightTemp.x + x_delta)*sin(leaf_theta);
		rightTemp.y = (rightTemp.y + y_delta)*cos(leaf_theta);
		rightTemp.z = (rightTemp.z);// + z_delta);
		splitLeafPoints.push_back(rightTemp);
	}
	//Generate the left and right branches
	else
	{	
		//Left branch
		vec3 leftTemp = splitLeafPoints[n];
		leftTemp.x = (leftTemp.x - x_delta)*sin(leaf_theta);
		leftTemp.y = (leftTemp.y + y_delta)*cos(-leaf_theta);
		leftTemp.z = (leftTemp.z);// + z_delta);
		splitLeafPoints.push_back(leftTemp);
		
		//Right branch
		vec3 rightTemp = splitLeafPoints[n];
		rightTemp.x = (rightTemp.x + x_delta)*sin(leaf_theta);
		rightTemp.y = (rightTemp.y +  y_delta)*cos(leaf_theta);
		rightTemp.z = (rightTemp.z);// z_delta);
		splitLeafPoints.push_back(rightTemp);
		
		fractals(n+1);
		
	}
}

void Leaf::fractalsReverse(int n)
{	
	
	//The if should exit the function once the last set of branches are drawn
	if(n == (num_recursion/2))
	{
		//Left branch
		vec3 leftTemp = splitLeafPointsReverse[n];
		leftTemp.x = (leftTemp.x + x_delta)*sin(leaf_theta);
		leftTemp.y = (leftTemp.y - y_delta)*cos(-leaf_theta);
		leftTemp.z = (leftTemp.z);// - z_delta);
		splitLeafPointsReverse.push_back(leftTemp);
		
		//Right branch
		vec3 rightTemp = splitLeafPointsReverse[n];
		rightTemp.x = (rightTemp.x - x_delta)*sin(leaf_theta);
		rightTemp.y = (rightTemp.y -  y_delta)*cos(leaf_theta);
		rightTemp.z = (rightTemp.z);// - z_delta);
		splitLeafPointsReverse.push_back(rightTemp);
	}
	//Generate the left and right branches
	else
	{	
		//Left branch
		vec3 leftTemp = splitLeafPointsReverse[n];
		leftTemp.x = (leftTemp.x + x_delta)*sin(leaf_theta);
		leftTemp.y = (leftTemp.y - y_delta)*cos(-leaf_theta);
		leftTemp.z = (leftTemp.z);// - z_delta);
		splitLeafPointsReverse.push_back(leftTemp);
		
		//Right branch
		vec3 rightTemp = splitLeafPointsReverse[n];
		rightTemp.x = (rightTemp.x - x_delta)*sin(leaf_theta);
		rightTemp.y = (rightTemp.y - y_delta)*cos(leaf_theta);
		rightTemp.z = (rightTemp.z); // - z_delta);
		splitLeafPointsReverse.push_back(rightTemp);
		
		fractalsReverse(n+1);

	}
}

void Leaf::reverseArray()
{
	vector<glm::vec3> temp;
	//cout>> temp[0].x >> temp[0].y >> temp[0].z >> endl;
	for(int i = 0; i < splitLeafPointsReverse.size(); i++)
	{
		temp[i].x = splitLeafPointsReverse[i].x;
		temp[i].y = splitLeafPointsReverse[i].y;
		temp[i].z = splitLeafPointsReverse[i].z;
	}
	
	for(int i = 0, j = temp.size(); i < temp.size(); i++, j--)
	{
		splitLeafPointsReverse[i].x = temp[j].x;
		splitLeafPointsReverse[i].y = temp[j].y;
		splitLeafPointsReverse[i].z = temp[j].z;
		
	}
}



