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

//This is the constructor for a tree (need to find the radius later)
Tree::Tree(vec2 h, vec2 w)
{
	height = h;
	width = w;
}

void Tree::renderTree()
{
	
}



