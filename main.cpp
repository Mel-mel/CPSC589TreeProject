#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <GL/gl.h>
#include <GL/glu.h> 
#include <GL/glut.h>
#include <iostream>
#include <vector> 
#include <cmath> 

#include "Tree.h"
#include "Leaf.h"
/*nice colors
glColor3f(0.2f, 0.6f, 0.5f);
glColor3f(0.9f, 0.9f, 0.7f);
*/
using namespace std;

GLFWwindow *window; 
int w, h;
double mouseX, mouseY;
float yRotate = 0;
int count = 0; // initial order of user input for dimensions
float zoom = 1;
std::string str = "Click again to render first stage!";
bool getDimensions = true;
bool doneDimensions = false;
bool stageOne = false;
bool stageTwo = false;
bool stageThree = false;
bool stageFour = false;
bool stageFive = false;
bool renderLeaf = false;

Tree aTree;
Leaf leaf;
vector<glm::vec2> treeD;
// leaf span: could possibly use this variable to add slider for amount of leafyness
void justLeaf()
{
	// close up view of one leaf
	leaf.drawGenericLeaf();
}

void fifthStage()
{
	// complete, rendered tree
}

void fourthStage()
{
	// add the leaves
}

void thirdStage()
{
	// draw the tree with cylinders
	aTree.renderTreeStageThree();
}

void secondStage()
{
	// draw the tree as points and lines
	aTree.renderTreeStageTwo();
}

//Render the first stage of the tree (just the outlines of a tree)
void firstStage()
{
	// draw the tree as just points
	Tree aTree(treeD[0], treeD[1], treeD[2], treeD[3]);
	aTree.renderTreeStageOne();
}

void renderBasicHeight()
{
	glPointSize(10.0f);
	glColor3f(0.9f, 0.9f, 0.7f);
	glBegin(GL_POINTS);
	for(int i = 0, j = 1; i < treeD.size(); i++, j++)
	{
		glVertex2f(treeD[i].x, treeD[i].y);
	}
	glEnd();
	
	glBegin(GL_LINES);
		if(treeD.size() >=2 )
		{
			glColor3f(1.0f, 0.8f, 0.2f);
			glVertex2f(treeD[0].x, treeD[0].y);
			glVertex2f(treeD[1].x, treeD[1].y);
		}
		if(treeD.size() >=4 )
		{
			glColor3f(1.0f, 0.8f, 0.2f);
			glVertex2f(treeD[2].x, treeD[2].y);
			glVertex2f(treeD[3].x, treeD[3].y);
		}
		if(treeD.size() == 6)
		{
			glColor3f(0.0f, 0.9f, 0.4f);
			glVertex2f(treeD[4].x, treeD[4].y);
			glVertex2f(treeD[5].x, treeD[5].y);
		}
	glEnd();
	
	
	
}

void textButton(int x_pos, int y_pos)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glColor3f(0.9f, 0.9f, 0.7f);
  	glRasterPos2f(x_pos, y_pos);
  	for (int i = 0; i < str.length(); i++) 
	{
    		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
  	}
}

void renderer () {
	glEnable(GL_DEPTH_TEST); // for 3D
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);//Rotate on the y axis

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-zoom,zoom,-zoom,zoom,-zoom,zoom);
	//glFrustum for perspective or GLU - gluPerspective
	

	if(getDimensions == true)
	{
		if(doneDimensions == true)
		{
			textButton(-0.5f, -0.5f);
		}
		renderBasicHeight();
	}
	if(stageOne == true)
	{
		firstStage();
	} 
	if(stageTwo == true)
	{
		secondStage();
	} 
	if(stageThree == true)
	{
		thirdStage();
	}
	if(stageFour == true)
	{
		fourthStage();
	}
	if(stageFive == true)
	{
		fifthStage();
	}
	if(renderLeaf == true)
	{
		justLeaf();
	}
	
	
}

void keyboard(GLFWwindow *sender, int key, int scancode, int action, int mods) { // key pressed, action of holding or releasing, mods are if CTRL or SHIFT are being held
	if(key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		exit(0);
	}
	//Rotate to the right on the y-axis
	if(key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		yRotate = yRotate + 15.0f;
	}
	//Rotate to the left on the y-axis
	if(key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		yRotate = yRotate - 15.0f;
	}
	if(key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		zoom += 0.1f;
	}
	if(key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		if(zoom > 0)
		{
			zoom -= 0.1f;
		}
		else
		{
			zoom = zoom;
		}
	}
	if(key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		treeD.clear();
		yRotate = 0;
		count = 0; // initial order of user input for dimensions
		zoom = 1;
		getDimensions = true;
		doneDimensions = false;
		stageOne = false;
		stageTwo = false;
		stageThree = false;
		stageFour = false;
		stageFive = false;
		renderLeaf = false;
	}
	if(key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		getDimensions = false;
		doneDimensions = false;
		stageOne = false;
		stageTwo = false;
		stageThree = false;
		stageFour = false;
		stageFive = false;
		renderLeaf = true;
	}
	if(key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		if(treeD.empty())
		{
			getDimensions = true;
			doneDimensions = false;
			stageOne = false;
			stageTwo = false;
			stageThree = false;
			stageFour = false;
			stageFive = false;
			renderLeaf = false;
		}
		else
		{
			getDimensions = false;
			doneDimensions = false;
			stageOne = true;
			stageTwo = false;
			stageThree = false;
			stageFour = false;
			stageFive = false;
			renderLeaf = false;
		}
	}
	if(key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		if(treeD.empty())
		{
			getDimensions = true;
			doneDimensions = false;
			stageOne = false;
			stageTwo = false;
			stageThree = false;
			stageFour = false;
			stageFive = false;
			renderLeaf = false;
		}
		else
		{
			getDimensions = false;
			doneDimensions = false;
			stageOne = false;
			stageTwo = true;
			stageThree = false;
			stageFour = false;
			stageFive = false;
			renderLeaf = false;
		}
	}
	if(key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		if(treeD.empty())
		{
			getDimensions = true;
			doneDimensions = false;
			stageOne = false;
			stageTwo = false;
			stageThree = false;
			stageFour = false;
			stageFive = false;
			renderLeaf = false;
		}
		else
		{
			getDimensions = false;
			doneDimensions = false;
			stageOne = false;
			stageTwo = false;
			stageThree = true;
			stageFour = false;
			stageFive = false;
			renderLeaf = false;
		}
	}
	if(key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		if(treeD.empty())
		{
			getDimensions = true;
			doneDimensions = false;
			stageOne = false;
			stageTwo = false;
			stageThree = false;
			stageFour = false;
			stageFive = false;
			renderLeaf = false;
		}
		else
		{
			getDimensions = false;
			doneDimensions = false;
			stageOne = false;
			stageTwo = false;
			stageThree = false;
			stageFour = true;
			stageFive = false;
			renderLeaf = false;
		}
	}
	if(key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		if(treeD.empty())
		{
			getDimensions = true;
			doneDimensions = false;
			stageOne = false;
			stageTwo = false;
			stageThree = false;
			stageFour = false;
			stageFive = false;
			renderLeaf = false;
		}
		else
		{
			getDimensions = false;
			doneDimensions = false;
			stageOne = false;
			stageTwo = false;
			stageThree = false;
			stageFour = false;
			stageFive = true;
			renderLeaf = false;
		}
	}
	
}

void mouseClick (GLFWwindow *sender, int button, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		if(button == GLFW_MOUSE_BUTTON_LEFT && count == 0 || count == 1)
		{
			treeD.push_back(glm::vec2(mouseX, mouseY)); //height
			count++;
		}
		else if(button == GLFW_MOUSE_BUTTON_LEFT && count == 2 || count == 3)
		{
			treeD.push_back(glm::vec2(mouseX, mouseY)); // trunk width
			count++;
		}
		else if(button == GLFW_MOUSE_BUTTON_LEFT && count == 4 || count == 5)
		{
			treeD.push_back(glm::vec2(mouseX, mouseY));// leaf span
			
			if (count == 5)
			{
				doneDimensions = true;
			}
			count++;
		}
		else if(count > 5)
		{
			stageOne = true;
			doneDimensions = false; 
			getDimensions = false;
			
		}
		
	}
}

void mousePosition(GLFWwindow *sender, double x, double y){
	mouseX = (2 * (x/ w)) - 1;
	mouseY = (-2 * (y / h)) + 1;

}

int main (int argc, char** argv) {
	if(!glfwInit())
		return 1; // if it is not initialized, exit program
	glutInit(&argc,  argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	window = glfwCreateWindow(800, 900, "My Window", NULL, NULL); // size and name of window
	if (!window)
		return 1;

	glfwMakeContextCurrent(window); // ensure we are rendering to the correct location
	glfwSetKeyCallback(window, keyboard);	
	glfwSetMouseButtonCallback(window, mouseClick);
	glfwSetCursorPosCallback(window, mousePosition);
	while(!glfwWindowShouldClose(window)) { // while not exited
		glfwGetFramebufferSize(window, &w, &h);
		glViewport(0, 0, w, h);

		renderer();

		glfwSwapBuffers(window);
		glfwPollEvents(); // will close window if x is pressed	
			
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}
