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
int num = 4; // this will be determined by leaf span given by user
int stopPoint = num+1;
int index = 1;
float length = 0.5f; //The initial length of the trunk/branch

//Values to change how the branches are drawn
float x_change = 0.5f;
float y_change = 0.5f;

vector<glm::vec2> splitPoints; // keeps track of point where one branch becomes two 



//This is the constructor for a tree (need to find the radius later)
Tree::Tree(vec2 hBase, vec2 hTop, vec2 w1, vec2 w2)
{
	heightBase = hBase;
	cout << hBase.x << "  " << hBase.y << endl;
	heightTop = hTop;
	width1 = w1;
	width2 = w2;
	
}

void Tree::renderTreeStageOne()
{
	float temp = 0.5f;
	//vec2 startpoint (heightBase.x, heightBase.y);
	vec2 startpoint(0.0f, -0.9);
	//glBegin(GL_POINTS);
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
	
	fractals(index);
	
	for(int j = 0; j < splitPoints.size()- stopPoint; j++)
	{
		cout << "INDEX  " << j << endl;
		cout << "x  " << splitPoints[j].x << "  " << "y  " << splitPoints[j].y << endl;
		cout << "x  " << splitPoints[2*j].x << "  " << "y  " << splitPoints[2*j].y << endl;
		
		cout << endl;
		
		cout << "x  " << splitPoints[j].x << "  " << "y  " << splitPoints[j].y << endl;
		cout << "x  " << splitPoints[2*j+1].x << "  " << "y  " << splitPoints[2*j+1].y << endl;
	}
	
	for(int i = 1; i < splitPoints.size()- stopPoint; i++)
	{
		
		//First line
		glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[2*i].x, splitPoints[2*1].y);
		
		//Second line 
		//glVertex2f(splitPoints[i].x, splitPoints[i].y);
		//glVertex2f(splitPoints[2*i+1].x, splitPoints[2*i+1].y);
	}
	
	glEnd();
	//glBegin(GL_POINTS);
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	for(int i = 1; i < splitPoints.size()- stopPoint; i++)
	{
		
		//Second line 
		glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[2*i+1].x, splitPoints[2*i+1].y);
	}
	splitPoints.clear();
	
	
	/*int i = 1;
	splitPoints.push_back({splitPoints[i].x - x_change, splitPoints[i].y + y_change});//Left branch
	splitPoints.push_back({splitPoints[i].x + x_change, splitPoints[i].y + y_change});//Right branch
	glVertex2f(splitPoints[i].x, splitPoints[i].y);
	glVertex2f(splitPoints[2*i].x, splitPoints[2*i].y);
	glVertex2f(splitPoints[i].x, splitPoints[i].y);
	glVertex2f(splitPoints[2*i+1].x, splitPoints[2*i+1].y);
	
	for(i = 1; i < num; i++)
	{
		
		splitPoints.push_back({splitPoints[i].x - x_change, splitPoints[i].y + y_change});//Left branch
		splitPoints.push_back({splitPoints[i].x + x_change, splitPoints[i].y + y_change});//Right branch
		glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[2*i].x, splitPoints[2*i].y);
		glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[2*i+1].x, splitPoints[2*i+1].y);
		
		/*glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[i+1].x, splitPoints[i+1].y);
		glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[i+2].x, splitPoints[i+2].y);
		x_change -= 0.05f;
		y_change += 0.05f;*/
		
	/*}
	
	for(int j = 0; j < splitPoints.size(); j++)
	{
		cout << "j  " << j << endl;
		cout << "x   " << splitPoints[j].x << "  y   ";
		cout << splitPoints[j].y << endl;
		
	}
	splitPoints.clear();*/
	glEnd();
}

//This will run with the trunk already placed inside the splitPoints vector array
void Tree::fractals(int n)
{	
	//The if should exit the function once the last set of branches are drawn...?
	if(n == num)
	{
		//Left branch
		vec2 leftTemp = splitPoints[n];
		leftTemp.x = leftTemp.x - x_change;
		leftTemp.y = leftTemp.y + y_change;
		splitPoints.push_back(leftTemp);
		
		//Right branch
		vec2 rightTemp = splitPoints[n];
		rightTemp.x = rightTemp.x + x_change;
		rightTemp.y = rightTemp.y + y_change;
		splitPoints.push_back(rightTemp);
	}
	//Generate the left and right branches
	else
	{	
		//Left branch
		vec2 leftTemp = splitPoints[n];
		leftTemp.x = leftTemp.x - x_change;
		leftTemp.y = leftTemp.y + y_change;
		splitPoints.push_back(leftTemp);
		
		//Right branch
		vec2 rightTemp = splitPoints[n];
		rightTemp.x = rightTemp.x + x_change;
		rightTemp.y = rightTemp.y + y_change;
		splitPoints.push_back(rightTemp);
		fractals(n+1);
		
	}
	/*
	if(n == 1)
	{
		cout << 1 << endl;
		return 1;
	}
	else
	{
		int value = n * fractals(n-1);
		cout << value << endl;
		return value;
	}
	*/
}




