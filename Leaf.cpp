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

float some;

void Leaf::something(float x)
{
	some = x;
}
