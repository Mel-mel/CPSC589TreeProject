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

int numOfBranches = 300;
float tree_height = 0;
Leaf aLeaf;
int degreeX, degreeY, degreeZ;
int randomLeaf = 0;
int randomX, randomY, randomZ;

float radiusInfluence = 5.0f;	//The radius of a branch point. Determines if a branch point is a neighbor to a tree node
float killRadius = 3.0f;		//If a trunk node is too close to a branch point then that branch point will get removed
float maxValue = 5.0f;
float minValue = -5.0f;

struct branches
{
	vec3 p1;
	vec3 p2;
};

vector<glm::vec3> neighbors; 	//Contains only branch points (from user input?)
vector<glm::vec3> randBranch;	//Contains random branch points to allow the tree to grow branches
vector<glm::vec3> leaves;		//Holds new leaf positions to be used in rendering the leaves
vector<branches> treeNodes;		//Contains tree nodes generated from the space algorithm

GLUquadricObj *quadratic;		//This is used for drawing cylinders

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

void Tree::clearArray()
{
	treeNodes.clear();
	leaves.clear();
}


/*The following two functions below (renderCylinder and renderCylinder_convenient) 
  were used to create a custom cylinder based on this website
  Kelleher. C, (2008, July 30), Rendering a Cylinder Between Two Points in OpenGL, Accessed on Mar 17, 2016,
  Retrieved from http://lifeofaprogrammergeek.blogspot.ca/2008/07/rendering-cylinder-between-two-points.html
*/
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

/*Function takes in two positions each with their own x, y, and z, a radius and the number of subdivisions to determine
  how smooth the cylinder will be. Then it'll call renderCylinder to actually create a new object and render it to the screen. 
*/
void Tree::renderCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
	//the same quadric can be re-used for drawing many cylinders
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
	gluDeleteQuadric(quadric);
}	

/*This function generates a random set of numbers from a max and min value which creates a position in the 3D space.
  That position is a branch point which the tree will "grow" towards when used in the spaceAlgorithm() function.
  The if statement ensures that the points don't go far below the trunk to prevent the tree from looking unrealistic.
*/
void Tree::genRandomBranch()
{
	float randX, randY, randZ;
	randBranch.clear();
    srand(time(NULL));	//Set the seed for the random number generator
    for (int i = 0; i < numOfBranches; i++)
    {
		//Generate a random point
        randX = ((float)rand() / (float)(RAND_MAX)) * (maxValue - minValue) + minValue;
        randY = ((float)rand() / (float)(RAND_MAX)) * maxValue * tree_height; //User input for height
        cout << "tree height" << "\t" << tree_height << endl;
        randZ = ((float)rand() / (float)(RAND_MAX)) * (maxValue - minValue) + minValue;
     
		//Make sure that the branch points are at least above the highest trunk point
		if(randY <= -1.0f)
		{
			 randY += 1.0f;
		}
        randBranch.push_back({ randX, randY, randZ });
        
        srand(i+2);
      //  cout << randX << " " << randY << " " << randZ << endl;
    }
}

/*This function draws the five stages of the tree
*/
void Tree::drawTree(int stage, int leafiness)
{
	//Render only the points of the tree
	if(stage == 1)
	{
		glBegin(GL_POINTS);
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.5f, 0.4f, 0.2f);
			glVertex3f(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z);
			glVertex3f(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z);
		}
		glEnd();
	}	
	
	//Render only the points and lines of the tree. A line colored from blue to red shows how a line is being 
	//drawn to another point
	if(stage == 2)
	{
		//Render the points
		glBegin(GL_POINTS);
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.5f, 0.4f, 0.2f);
			glVertex3f(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z);
			glVertex3f(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z);
		}
		glEnd();
		
		//Render the colored lines
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
	
	//Drawing the cylinders onto the tree to give it a thickness. No leaves are generated here.
	if(stage == 3)
	{
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.5f, 0.4f, 0.2f);
			renderCylinder_convenient(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z, treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z, 0.07f, 10.0f); 
		}
	}
	
	//Render the cylinders and leaves onto the tree.
	if(stage == 4)
	{
		//Render the cylinders
		glBegin(GL_LINES);
		for(int i = 0; i < treeNodes.size(); i++)
		{
			glColor3f(0.5f, 0.4f, 0.2f);
			renderCylinder_convenient(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z, treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z, 0.07f, 10.0f); 
		}
		
		//Render the leaves onto the tree
		for(int j = 0; j < leaves.size(); j++)
		{
			if(leaves[j].x < 0.5 && leaves[j].x > -0.5 && leaves[j].y < 3.0)
			{
				// do not draw leaves on the main trunk of the tree
			}
			else
			{
				aLeaf.drawLeaf(leaves[j].x, leaves[j].y, leaves[j].z);
			}
		}
		glEnd();
	}
	
	//The final rendering, the tree should have shading, a light source to cause the shadow, etc.
	if(stage == 5)
	{
		//Render the color, ambient, diffuse, and specular lighting for the cylinder
		glPushMatrix();
		GLfloat diffuse[]={0.9, 0.9, 0.9, 1.0};
		GLfloat specular[]={0.3,0.3, 0.3, 1.0};
		GLfloat mat_shininess[] = { 15.0 };
		GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glShadeModel (GL_SMOOTH);
		
		GLfloat brown[] = {0.7f, 0.5f, 0.2f, 1.f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, brown);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		glFlush ();
		
		//Render the cylinders (the color for the cylinder is already set above)
		glBegin(GL_LINES);
		for(int i = 0; i < treeNodes.size(); i++)
		{
			renderCylinder_convenient(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z, treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z, 0.08f, 10.0f); 
		}	
		glEnd();
		
		//Disable the lighting for the leaves so that the leaves stay green
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		
		//Generate a random number so that the rotation of the leaves will be different
		srand(time(NULL));
		randomX = (rand() / RAND_MAX) * (maxValue - minValue) + minValue;
		randomY = (rand() / RAND_MAX) * (maxValue - minValue) + minValue;
		randomZ = (rand() / RAND_MAX) * (maxValue - minValue) + minValue;
		
		//Render the leaves onto the tree
		for(int j = 0; j < leaves.size(); j+= leafiness)
		{
			if(leaves[j].x < 0.5 && leaves[j].x > -0.5 && leaves[j].y < 2.0)
			{
				// do not draw leaves on the main trunk of the tree
			}
			else
			{
				/*
				 * The following code places the individual leaves on points on
				 * the branch. The transformations apply to the individual leaves to make
				 * them random in their orientation on the tree. 
				 */
				if(((int)leaves[j].y)%randomX == 0){degreeX = 90;}
				if(((int)leaves[j].x)%randomY == 0){degreeY = 120;}
				if(((int)leaves[j].z)%randomZ == 0){degreeZ = 70;}
				glColor3f(0.184f, 0.409f, 0.184f);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glTranslatef(leaves[j].x, leaves[j].y, leaves[j].z);
				if(((int)leaves[j].z)%randomX == 0){glRotatef(degreeZ, 0.0f, 0.0f, 1.0f);}
				if(((int)leaves[j].y)%randomY == 0){glRotatef(degreeX, 1.0f, 0.0f, 0.0f);}
				if(((int)leaves[j].x)%randomZ == 0){glRotatef(degreeY, 0.0f, 1.0f, 0.0f);}
				glTranslatef(-leaves[j].x, -leaves[j].y, -leaves[j].z);
				aLeaf.drawLeaf(leaves[j].x, leaves[j].y, leaves[j].z);
				glPopMatrix();
				
				if(((int)leaves[j].x)%2 == 0){degreeZ = 90;}
				if(((int)leaves[j].z)%3 == 0){degreeX = 120;}
				if(((int)leaves[j].y)%5 == 0){degreeY = 70;}
				glColor3f(0.184f, 0.409f, 0.184f);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glTranslatef(leaves[j].x, leaves[j].y, leaves[j].z);
				if(((int)leaves[j].z)%2 == 0){glRotatef(degreeX, 0.0f, 0.0f, 1.0f);}
				if(((int)leaves[j].x)%3 == 0){glRotatef(degreeY, 1.0f, 0.0f, 0.0f);}
				if(((int)leaves[j].y)%5 == 0){glRotatef(degreeZ, 0.0f, 1.0f, 0.0f);}
				glTranslatef(-leaves[j].x, -leaves[j].y, -leaves[j].z);
				aLeaf.drawLeaf(leaves[j].x, leaves[j].y, leaves[j].z);
				glPopMatrix();
			}
		}
	}
}

//Initializing the trunk points with 6 points. This will be the default trunk for all tree renderings.
void Tree::initTrunk()
{
	float x, y, z, value;
	x = 0.0f;
	y = -1.0f;
	z = 0.0f;
	value = 0.25f;
	
	vec3 temp(x, y, z);//First position
	vec3 temp2(x, y + value, z);//Second position
	
	//Push the points into treeNodes and update the y position for both points
	for(int l = 0; l < 6; l++)
	{
		treeNodes.push_back({temp, temp2});
		temp.y = temp.y + value;
		temp2.y = temp2.y + value;
	}
}

/*After adding a new tree node, this for loop checks which trunk points in treeNodes are within a 
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
			
			//If kill radius is greater than the calculated distance then remove that branch point
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

/*This function creates new points on the tree to allow more leaves to be placed on the branches.
  It uses subdivision to create new points between a pair of treeNodes which is then stored into 
  a vector array.
*/
void Tree::createLeafPositions()
{
	vec3 tempLeafOne;
	vec3 tempLeafTwo;
	vec3 tempLeafThree;
	for(int i = 0; i < treeNodes.size(); i++)
	{
		leaves.push_back(vec3(treeNodes[i].p1.x, treeNodes[i].p1.y, treeNodes[i].p1.z)); 
		leaves.push_back(vec3(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z));
		
		tempLeafOne.x =  (treeNodes[i].p1.x + treeNodes[i].p2.x)/2;
		tempLeafOne.y =  (treeNodes[i].p1.y + treeNodes[i].p2.y)/2;
		tempLeafOne.z =  (treeNodes[i].p1.z + treeNodes[i].p2.z)/2;
		
		leaves.push_back(vec3(tempLeafOne.x, tempLeafOne.y, tempLeafOne.z));
		
		tempLeafTwo.x =  (treeNodes[i].p1.x + tempLeafOne.x)/2;
		tempLeafTwo.y =  (treeNodes[i].p1.y + tempLeafOne.y)/2;
		tempLeafTwo.z =  (treeNodes[i].p1.z + tempLeafOne.z)/2;
		
		leaves.push_back(vec3(tempLeafTwo.x, tempLeafTwo.y, tempLeafTwo.z));
		
		tempLeafThree.x =  (treeNodes[i].p2.x + tempLeafOne.x)/2;
		tempLeafThree.y =  (treeNodes[i].p2.y + tempLeafOne.y)/2;
		tempLeafThree.z =  (treeNodes[i].p2.z + tempLeafOne.z)/2;
		
		leaves.push_back(vec3(tempLeafThree.x, tempLeafThree.y, tempLeafThree.z));
		
	}
}

/*This function calculates and returns an average normal from all neighbours to a tree node. The normalized vectors
  are then added together and normalized again.
*/
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

/*The nested loop simply adds new tree nodes to treeNodes (which have some points initialized
 * to it already). It determines any branch points that are closest to a trunk point in treeNodes
 * and calculates the direction of those points. In turn, it determines the new position of the tree 
 * node by averaging the normals of all nearby branch points which is then added into treeNodes
 */
void Tree::spaceAlgorithm()
{
	float currDist, x, y, z;
	int i, j, shortIndex;
	for(i = 4; i < treeNodes.size(); i++)
	{
		vec3 temp(treeNodes[i].p2.x, treeNodes[i].p2.y, treeNodes[i].p2.z);
		for(j = 0; j < randBranch.size(); j++)
		{
			//Get the distance from the current tree node to a random branch point
			x = temp.x - randBranch[j].x;
			y = temp.y - randBranch[j].y;
			z = temp.z - randBranch[j].z;
			
			x = x * x;
			y = y * y;
			z = z * z;
			
			float distlength = x + y + z;
			currDist = sqrt(distlength);
			
			/*The current trunk point in treeNodes is within the radius of influence of a branch point.
			 * If the radiusInfluence is greater than the currDist then that means the current
			 * trunk point is one of the closest neighbors to a branch point.*/
			if(radiusInfluence > currDist)
			{	
				neighbors.push_back(temp);
				neighbors.push_back(randBranch[j]);
			}
		}
		if(!neighbors.empty())
		{
			vec3 norm = avgNormals();
			norm = temp + (1.0f * norm);//There might be a problem with currDist since it might not be the value going from v to v'

			//Push new positions of p1 and p2 and clear neighbor vector array
			treeNodes.push_back({temp, norm});
			neighbors.clear();
			
			//Removes any branches that are within a kill radius
			//They won't be considered in the next iteration of the for loop
			removeBranches();
		}
	}
	
	//Create new leaf positions to be rendered
	createLeafPositions();
	randBranch.clear();
}
	


