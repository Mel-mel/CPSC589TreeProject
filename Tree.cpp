#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <iostream>
#include <vector> 
#include <cmath> 

#include "Leaf.h"
#include "Tree.h"
using namespace std; 
using namespace glm;
int num = 10; // this will be determined by leaf span given by user
float length = 0.5f; //The initial length of the trunk/branch
vector<glm::vec2> splitPoints; // keeps track of point where one branch becomes two 

//This is the constructor for a tree (need to find the radius later)
Tree::Tree(vec2 hBase, vec2 hTop, vec2 w1, vec2 w2)
{
	heightBase = hBase;
	heightTop = hTop;
	width1 = w1;
	width2 = w2;
	
}

void Tree::renderTreeStageOne()
{
	float temp = 0.5f;
	vec2 startpoint (heightBase.x, heightBase.y);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	//for(int i = 0; i < heightTop.y; i++)
	//{
	//Should call fractals here and do stuff
	glVertex2f(startpoint.x, startpoint.y);
	glVertex2f(startpoint.x, startpoint.y + temp);
	//}
	
	
	
	splitPoints.push_back(vec2(startpoint.x, startpoint.y));
	splitPoints.push_back(vec2(startpoint.x, startpoint.y + temp));
	
	//fractals(num);
	float x_change = 0.25f;
	float y_change = 0.25f;
	
	int i = 1;
	splitPoints.push_back({splitPoints[i].x - x_change, splitPoints[i].y + y_change});//Left branch
	splitPoints.push_back({splitPoints[i].x + x_change, splitPoints[i].y + y_change});//Right branch
	glVertex2f(splitPoints[i].x, splitPoints[i].y);
	glVertex2f(splitPoints[i+1].x, splitPoints[i+1].y);
	glVertex2f(splitPoints[i].x, splitPoints[i].y);
	glVertex2f(splitPoints[i+2].x, splitPoints[i+2].y);
	
	for(i = 3; i < num; i+=2)
	{
		
		splitPoints.push_back({splitPoints[i].x - x_change, splitPoints[i].y + y_change});//Left branch
		splitPoints.push_back({splitPoints[i].x + x_change, splitPoints[i].y + y_change});//Right branch
		glVertex2f(splitPoints[i-1].x, splitPoints[i-1].y);
		glVertex2f(splitPoints[i+1].x, splitPoints[i+1].y);
		glVertex2f(splitPoints[i-1].x, splitPoints[i-1].y);
		glVertex2f(splitPoints[i+2].x, splitPoints[i+2].y);
		
		glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[i+1].x, splitPoints[i+1].y);
		glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[i+2].x, splitPoints[i+2].y);
		x_change -= 0.05f;
		y_change += 0.05f;
		
	}
	
	for(int j = 0; j < splitPoints.size(); j++)
	{
		cout << "j  " << j << endl;
		cout << "x   " << splitPoints[j].x << "  y   ";
		cout << splitPoints[j].y << endl;
		
	}
	splitPoints.clear();
	glEnd();
}

/*int Tree::fractals(int n)
{
	//int f = 1;
	for(int i = 0; i < n; i++)
	{
		//f = f*(i+1);
		//Left branch
		glRotatef(M_PI / 6, 1.0f, 0.0f, 0.0f);
		splitPoints.push_back(startpoint.x, startpoint.y)
	}
	return f;
}*/






