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
int numOfBranches = 300;
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

vector<glm::vec3> neighbors; //Contains only branch points (from user input?)
vector<glm::vec3> randBranch;//Contains random branch points to allow the tree to grow branches
vector<glm::vec3> holding;
vector<branches> leafPosition;//Contains leaf positions to draw a large set of them
vector<branches> treeNodes;	//Contains tree nodes generated from the space algorithm

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
	
}

void Tree::clearArray()
{
	treeNodes.clear();
	leafPosition.clear();
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
        srand(i+2);
      //  cout << randX << " " << randY << " " << randZ << endl;
    }
}

void Tree::drawTree(int stage)
{
	//Just points of the tree
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
	
	//Points and lines of the tree. A line colored from blue to red shows how a line is being 
	//drawn to anothe point
	if(stage == 2)
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
		
		glBegin(GL_LINES);
		
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.0f, 0.5f, 0.8f);
			glVertex3f(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z);
		}
		
		glEnd();
	}
	
	//Drawing the cylinders onto the tree to give it a thickness
	if(stage == 3)
	{
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.5f, 0.4f, 0.2f);
			renderCylinder_convenient(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z, treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z, 0.1f, 10.0f); 
		}
	}
	if(stage == 4)
	{
		//add leaves **** should be adding to cylinders but for testing we are adding to the lines
		glBegin(GL_LINES);
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.5f, 0.4f, 0.2f);
			renderCylinder_convenient(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z, treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z, 0.1f, 10.0f); 
		}
		
		for(int j = 8; j < leafPosition.size(); j++)
		{
			/*glTranslatef(-leafPosition[j].p1.x, -leafPosition[j].p1.y, -leafPosition[j].p1.z);
			glRotatef(30.0f, 0.0, 1.0, 0.0);
			glTranslatef(leafPosition[j].p1.x, leafPosition[j].p1.y, leafPosition[j].p1.z);
			
			glTranslatef(-leafPosition[j].p2.x, -leafPosition[j].p2.y, -leafPosition[j].p2.z);
			glRotatef(30.0f, 0.0, 1.0, 0.0);
			glTranslatef(leafPosition[j].p2.x, leafPosition[j].p2.y, leafPosition[j].p2.z);*/
			
			//aLeaf.drawLeaf(treeNodes[20].p1.x, treeNodes[20].p1.y, treeNodes[20].p1.z);
			/*glBegin(GL_POINTS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(treeNodes[15].p1.x, treeNodes[15].p1.y, treeNodes[15].p1.z);
			glVertex3f(treeNodes[15].p2.x, treeNodes[15].p2.y, treeNodes[15].p2.z);
			glEnd();
			*/
			aLeaf.drawLeaf(leafPosition[j].p1.x, leafPosition[j].p1.y, leafPosition[j].p1.z);
			aLeaf.drawLeaf(leafPosition[j].p2.x, leafPosition[j].p2.y, leafPosition[j].p2.z);
			
			/*aLeaf.drawLeaf(treeNodes[j].p1.x, treeNodes[j].p1.y, treeNodes[j].p1.z);
			aLeaf.drawLeaf(treeNodes[j].p2.x, treeNodes[j].p2.y, treeNodes[j].p2.z);*/
		}
		
		glEnd();
		
	}
	
	//The final rendering, the tree should have shading, a light source to cause the shadow, etc.
	if(stage == 5)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
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

void Tree::createLeafPositions()
{
	for(int i = 0; i < treeNodes.size(); i++)
	{
		leafPosition.push_back({treeNodes[i].p1, treeNodes[i].p2});
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
			norm = temp + (1.5f * norm);//There might be a problem with currDist since it might not be the value going from v to v'

			//Push new positions of p1 and p2 and clear neighbor vector array
			treeNodes.push_back({temp, norm});
			neighbors.clear();
			
			//Removes any branches that are within a kill radius
			//They won't be considered in the next iteration of the for loop
			removeBranches();
		}
	}
	
	
	createLeafPositions();
	randBranch.clear();
	for (int k = 0; k < treeNodes.size(); k++)
	{
		cout << endl;
		cout << "index k " << k << endl;
		cout << treeNodes[k].p2.x << " " << treeNodes[k].p2.y << " " << treeNodes[k].p2.z << endl;
	}
}
	


