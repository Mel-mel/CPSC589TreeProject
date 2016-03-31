#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <iostream>
#include <vector> 
#include <cmath> 
#include <random>

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
int numOfBranches = 250;
float tree_height = 0;
Leaf aLeaf;

//Values to change how the branches are drawn
float x_change = 0.1f;
float y_change = 0.1f;

float radiusInfluence = 5.0f;	//If a trunk node has a close neighbor to some branch point
float killRadius = 3.0f;		//If a trunk node is too close to a branch point then that branch point will get removed
float maxValue = 5.0f;
float minValue = -5.0f;

struct branches
{
	vec3 p1;
	vec3 p2;
};


vector<glm::vec2> splitPoints; // keeps track of point where one branch becomes two 
vector<glm::vec3> allPoints;	//This is for the space algorithm to store all trunk, branch, and connected points
vector<glm::vec3> neighbors; //Contains only branch points (from user input?)
vector<glm::vec3> randBranch;
vector<glm::vec3> holding;
vector<glm::vec3> removable;
vector<branches> treeNodes;

GLUquadricObj *quadratic;

/*void Tree::initializeCylinders()
{
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
}*/
Tree::Tree()
{
	//blank initializer
}

Tree::Tree(float height)
{
	if(height > 0.5f && height < 5.0f)
	{
		tree_height = height;
	}
	else
	{
		tree_height = 2.0f;
	}
}

//This is the constructor for a tree (need to find the radius later. make into vec3 later)
Tree::Tree(vec2 hBase, vec2 hTop, vec2 w1, vec2 w2)
{
	heightBase = hBase;
	heightTop = hTop;
	width1 = w1;
	width2 = w2;

	/*
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
	glEnd();*/
	
	//fractals(index);
	//x_change = 0.5f;
	//y_change = 0.5f;
	
}

void Tree::setValues(vec3 hBase, vec3 hTop, vec3 w1, vec3 w2)
{
	
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
	glMatrixMode(GL_MODELVIEW);
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

//Call this every time a new tree node is added to allPoints
void Tree::genRandomBranch()
{
	float randX, randY, randZ;
	randBranch.clear();
    srand(time(NULL));
    for (int i = 0; i < numOfBranches; i++)
    {
		
        randX = ((float)rand() / (float)(RAND_MAX)) * (maxValue - minValue) + minValue;
        randY = ((float)rand() / (float)(RAND_MAX)) * maxValue * tree_height; //User input for height
        cout << "tree height" << "\t" << tree_height << endl;
        randZ = ((float)rand() / (float)(RAND_MAX)) * (maxValue - minValue) + minValue;
     
		//Make sure that the branches are at least above the highest trunk point
		if(randY <= -1.0f)
		{
			 randY += 1.0f;
		}
        randBranch.push_back({ randX, randY, randZ });
        holding.push_back({randX, randY, randZ});
        srand(i+1);
      //  cout << randX << " " << randY << " " << randZ << endl;
    }
}

void Tree::drawTree(int stage)
{
	if(stage == 1)
	{
		glBegin(GL_POINTS);
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.5f, 0.4f, 0.2f);
			glVertex3f(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z);
			glVertex3f(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z);
			//cout << allPoints[i].x << " " << allPoints[i].y << " " << allPoints[i].z << endl;
		}
		glEnd();
	}	
	
	if(stage == 2)
	{
		glBegin(GL_LINES);
		
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.5f, 0.4f, 0.2f);
			glVertex3f(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z);
			glColor3f(0.5f, 0.4f, 0.2f);
			glVertex3f(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z);
		}
		
		glEnd();
	}
	if(stage == 3)
	{
		// cylinders
	}
	if(stage == 4)
	{
		//add leaves **** should be adding to cylinders but for testing we are adding to the lines
		glBegin(GL_LINES);
		
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.5f, 0.4f, 0.2f);
			glVertex3f(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z);
			glVertex3f(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z);
		}
		for(int j = 30; j < treeNodes.size(); j++)
		{
			aLeaf.drawLeaf(treeNodes[j].p1.x, treeNodes[j].p1.y, treeNodes[j].p1.z);
			aLeaf.drawLeaf(treeNodes[j].p2.x, treeNodes[j].p2.y, treeNodes[j].p2.z);
		}
		
		glEnd();
		
	}
	//cout << treeNodes.size() << endl;
	
	//allPoints.clear();
}

//Initializing the trunk points with 6 points (to start off) this would need the user input later
void Tree::initTrunk()
{
	float x, y, z, value;
	x = 0.0f;
	y = -1.0f;
	z = 0.0f;
	value = 0.25f;
	
	vec3 temp(x, y, z);//First position
	vec3 temp2(x, y + value, z);//Second position
	for(int l = 0; l < 6; l++)
	{
		treeNodes.push_back({temp, temp2});
		temp.y = temp.y + value;
		temp2.y = temp2.y + value;
		/*cout << "p1" << endl;
			cout << treeNodes[l].p1.x << " " << treeNodes[l].p1.y << " " << treeNodes[l].p1.z << endl;
			cout << "p2" << endl;
			cout << treeNodes[l].p2.x << " " << treeNodes[l].p2.y << " " << treeNodes[l].p2.z << endl;
			cout << endl;*/
		/*cout << allPoints[l].x << endl;
		cout << allPoints[l].y << endl;
		cout << allPoints[l].z << endl;*/
	}
}

/*After adding a new tree node, this for loop checks which trunk points in allPoints are within a 
 * killRadius that a branch point has. If the trunk point is within that radius of some branch
 * point then that branch point is removed.  
 */ 
void Tree::removeBranches()
{
	float x, y, z, distance;
	
	for (int i = 0; i < treeNodes.size(); i++)
	{
		for (int j = 0; j < randBranch.size(); j++)
		{
			//Get distance from a trunk point to a branch point
			x = treeNodes[i].p2.x - randBranch[j].x;
			y = treeNodes[i].p2.y - randBranch[j].y;
			z = treeNodes[i].p2.z - randBranch[j].z;
			
			x = x * x;
			y = y * y;
			z = z * z;
			
			distance = x + y + z;
			distance = sqrt(distance);
			
			if(killRadius > distance)
			{
				randBranch.erase(randBranch.begin() + j);
			}
			else 
			{
				continue;
			}
		}
	}
}

//Calculates the total sum of branch and trunk points
vec3 Tree::avgNormals()
{
	vec3 normal, totalNormals;
	for(int i = 0; i < neighbors.size(); i+=2)
	{
		normal = normalize(neighbors[i+1] - neighbors[i]);//Branch point - trunk point
		totalNormals = totalNormals + normal;
	}
	
	//Get length of totalNormals to normalize totalNormals
	float x = totalNormals.x * totalNormals.x;
	float y = totalNormals.y * totalNormals.y;
	float z = totalNormals.z * totalNormals.z;
	float normlength = x + y + z;
	normlength = sqrt(normlength);
	
	return totalNormals / normlength;
}

/*The nested loop simply adds new tree nodes to allPoints (which have some points initialized
 * to it already). It determines any branch points that are closest to a trunk point in allPoints
 * and calculates the direction of those points. In turn, it determines the new position of the tree 
 * node by averaging the normals of all nearby branch points which is then added into allPoints
 */
void Tree::spaceAlgorithm()
{
	float currDist, x, y, z;
	int i, j, shortIndex;
	vec3 temp(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z);
	cout << "WHY?" << endl;
	for(i = 4; i < treeNodes.size(); i++)
	{
		//cout << i << endl;
		vec3 temp(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z);
		//cout << temp.x << temp.y << temp.z <<endl;
		for(j = 0; j < randBranch.size(); j++)
		{
			
			x = temp.x - randBranch[j].x;
			y = temp.y - randBranch[j].y;
			z = temp.z - randBranch[j].z;
			
			x = x * x;
			y = y * y;
			z = z * z;
			
			float distlength = x + y + z;
			currDist = sqrt(distlength);
			
			/*The current trunk point in allPoints is within the radius of influence of a branch point.
			 * If the radiusInfluence is greater than the currDist then that means the current
			 * trunk point is one of the closest neighbors to a branch point.*/
			if(radiusInfluence > currDist)
			{	
				neighbors.push_back(temp);
				neighbors.push_back(randBranch[j]);
				//treeNodes.push_back({i, norm});//Problem here?
			}
		}
		if(!neighbors.empty())
		{
			vec3 norm = avgNormals();
			
			/*cout << "p1" << endl;
			cout << treeNodes[i].p1.x << " " << treeNodes[i].p1.y << " " << treeNodes[i].p1.z << endl;
			cout << "p2" << endl;
			cout << treeNodes[i].p2.x << " " << treeNodes[i].p2.y << " " << treeNodes[i].p2.z << endl;*/
			//There might be a problem with currDist since it might not be the value going from v to v'
			//Should fix this or look over again
			norm = temp + (1.5f * norm);

			treeNodes.push_back({temp, norm});//Problem here?
			neighbors.clear();
			
			//Removes any branches that are within a trunk point (re-word later)~~~
			removeBranches();
		}
	}
	
	randBranch.clear();
	for (int k = 0; k < treeNodes.size(); k++)
	{
		cout << endl;
		cout << "index k " << k << endl;
		cout << treeNodes[k].p2.x << " " << treeNodes[k].p2.y << " " << treeNodes[k].p2.z << endl;
	}
	
	//IMPORTANT SIDE NOTE: When randBranch becomes null, that's when the algorithm terminates.
	
	/*PSEUDOCODE (not complete...)
	 * dist = default distance value
	 * shortest is a vector array. Should hold the new tree nodes (probs dont need to add trunk points to it)
	 * avgNormals is a function to calculate the "normalized vectors towards all the sources s in S(v)"
	 * for i = 0 to branch.size() i++
	 *    for j = 0 to trunk.size() j++
	 *       currDist = abs(length.branch[i] - length.trunk[j])
	 *       if(currDist < dist && radiusInfluence < currDist)
	 *          dist = currDist
	 *          newTreeNode = trunk[j] + currDist * avgNormals //avgNormals should be another function
	 *          shortest.push_back(trunk[j])
	 *     end
	 * end
	 */
}
	


