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
float theta = M_PI/6;
float temp = 0.5f;

//Values to change how the branches are drawn
float x_change = 0.1f;
float y_change = 0.1f;

vector<glm::vec2> splitPoints; // keeps track of point where one branch becomes two 

GLUquadricObj *quadratic;

/*void Tree::initializeCylinders()
{
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
}*/

Tree::Tree()
{
	//Blank initializer
}

//This is the constructor for a tree (need to find the radius later)
Tree::Tree(vec2 hBase, vec2 hTop, vec2 w1, vec2 w2)
{
	heightBase = hBase;
	heightTop = hTop;
	width1 = w1;
	width2 = w2;

	
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
	glEnd();
	
	fractals(index);
	x_change = 0.5f;
	y_change = 0.5f;
	
}

//The following two functions below were used to create a custom cylinder based on this website
//http://lifeofaprogrammergeek.blogspot.ca/2008/07/rendering-cylinder-between-two-points.html
//Accessed on Mar 17, 2016 (do the proper citing later~)
//renderCylinder and renderCylinder_convenient
void Tree::renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
	float vx = x2-x1;
	float vy = y2-y1;
	float vz = z2-z1;

	//handle the degenerate case of z1 == z2 with an approximation
	if(vz == 0)
		vz = .0001;

	float v = sqrt( vx*vx + vy*vy + vz*vz );
	float ax = 57.2957795*acos( vz/v );
	if ( vz < 0.0 )
		ax = -ax;
	float rx = -vy*vz;
	float ry = vx*vz;
	glPushMatrix();

	//draw the cylinder body
	glTranslatef( x1,y1,z1 );
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);

	//draw the first cap
	gluQuadricOrientation(quadric,GLU_INSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glTranslatef( 0,0,v );

	//draw the second cap
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glPopMatrix();
}
void Tree::renderCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
	//the same quadric can be re-used for drawing many cylinders
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
	gluDeleteQuadric(quadric);
}

void Tree::renderTreeStageTwo()
{
	/*glTranslatef(0.0f,-0.7f,0.0f); //-0.7 should be replaced with first point given as height
	glRotatef(180.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	
	
	glColor3f(1.0f, 0.8f, 0.2f);
	
	gluCylinder(quadratic,0.015f,0.015f,0.9f,50,50);
	
	glEnd();*/
	//vec2 startpoint (heightBase.x, heightBase.y);//This is for the user input
	vec2 startpoint(0.0f, -0.9);
	splitPoints.push_back(vec2(startpoint.x, startpoint.y));
	splitPoints.push_back(vec2(startpoint.x, startpoint.y + temp));
	renderCylinder_convenient(splitPoints[0].x, splitPoints[0].y, 0, splitPoints[1].x, splitPoints[1].y, 0, 0.025, 10);
	fractals(index);
	for(int i = 1; i < splitPoints.size()-stopPoint; i++)
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		renderCylinder_convenient(splitPoints[i].x, splitPoints[i].y, 0, splitPoints[2*i].x, splitPoints[2*i].y, 0, 0.025, 10); 
	} 
	for(int i = 1; i < splitPoints.size()-stopPoint; i++)
	{
		glColor3f(1.0f, 0.8f, 0.2f);
		renderCylinder_convenient(splitPoints[i].x, splitPoints[i].y, 0, splitPoints[2*i+1].x, splitPoints[2*i+1].y, 0, 0.025, 10); 
	} 
	splitPoints.clear();
}

void Tree::renderTreeStageOne()
{	
	for(int j = 0; j < splitPoints.size()- stopPoint; j++)
	{
		cout << "INDEX  " << j << endl;
		cout << "x  " << splitPoints[j].x << "  " << "y  " << splitPoints[j].y << endl;
		cout << "x  " << splitPoints[2*j].x << "  " << "y  " << splitPoints[2*j].y << endl;
		
		cout << endl;
		
		cout << "x  " << splitPoints[j].x << "  " << "y  " << splitPoints[j].y << endl;
		cout << "x  " << splitPoints[2*j+1].x << "  " << "y  " << splitPoints[2*j+1].y << endl;
	}
	glBegin(GL_LINES);
	
	for(int i = 1; i < splitPoints.size()- stopPoint; i++)
	{
		/*if(i % 2 == 1)
		{
			glTranslatef(-splitPoints[i].x, -splitPoints[i].y, 0.0f);
			glRotatef(50, 1.0f, 0.0f,0.0f);
			glTranslatef(splitPoints[i].x, splitPoints[i].y, 0.0f);
			glBegin(GL_LINES);
		}
		else
		{
			glBegin(GL_LINES);
		}*/
		
		
		
		glColor3f(1.0f, 0.0f, 0.0f);
		//First line
		glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[2*i].x, splitPoints[2*i].y);

		
	}
	
	
	for(int i = 1; i < splitPoints.size()- stopPoint; i++)
	{
		//Second line 
//glBegin(GL_LINES);
glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(splitPoints[i].x, splitPoints[i].y);
		glVertex2f(splitPoints[2*i+1].x, splitPoints[2*i+1].y);
//glEnd();
	}
	glEnd();
	splitPoints.clear();
}

//This will run with the trunk already placed inside the splitPoints vector array
void Tree::fractals(int n)
{	
	
	//The if should exit the function once the last set of branches are drawn...?
	if(n == num)
	{
		//Left branch
		vec2 leftTemp = splitPoints[n];
		leftTemp.x = (leftTemp.x - x_change)*sin(theta);
		//leftTemp.y = leftTemp.y + y_change;
		leftTemp.y = (leftTemp.y + y_change)*cos(-theta);
		splitPoints.push_back(leftTemp);
		
		//Right branch
		vec2 rightTemp = splitPoints[n];
		rightTemp.x = (rightTemp.x + x_change)*sin(theta);
		rightTemp.y = (rightTemp.y +  y_change)*cos(theta);
		splitPoints.push_back(rightTemp);
	}
	//Generate the left and right branches
	else
	{	
		if(y_change < 0.01)
		{
			y_change = 0.01;
		}
		if(x_change < 0.01)
		{
			x_change = 0.01;
		}
		
		
		//Calculate cos and sin as individual variables
		//Then take that values and times it by 2?
		//Then add it to the change values
		
		
		//Left branch
		vec2 leftTemp = splitPoints[n];
		leftTemp.x = (leftTemp.x - x_change)*sin(theta);
		leftTemp.y = (leftTemp.y + y_change)*cos(-theta);
		splitPoints.push_back(leftTemp);
		
		//Right branch
		vec2 rightTemp = splitPoints[n];
		rightTemp.x = (rightTemp.x + x_change)*sin(theta);
		//rightTemp.y = rightTemp.y +  y_change;
		rightTemp.y = (rightTemp.y +  y_change)*cos(theta);
		splitPoints.push_back(rightTemp);
		/*if(x_change > 0.02)
		{
			x_change -= 0.01;
		}
		if(y_change > 0.02)
		{
			y_change -= 0.01;
		}*/
		
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

