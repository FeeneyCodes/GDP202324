#include "OpenGLCommon.h"

#include <iostream>

void windowSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	std::cout << "Screen is now: "
		<< width << " x " << height << std::endl;

	// TODO: Change any FBOs that need to change here

	return;
}