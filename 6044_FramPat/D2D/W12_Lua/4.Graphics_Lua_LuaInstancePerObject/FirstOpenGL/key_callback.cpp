#include "GlobalStuff.h"

#include <sstream>


bool isShiftDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT )
	{
		return true;
	}
	return false;
}

bool isCrtlDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL )
	{
		return true;
	}
	return false;
}

bool isAltDownAlone( int mods )
{
	if ( (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT )
	{
		return true;
	}
	return false;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

	switch (key)
	{
	case GLFW_KEY_U:
		{// Scope start
			if ( isShiftDownAlone(mods) )	{ ::g_pObjectToRotate->rotSpeed.x += -0.01f; }
			else							{ ::g_pObjectToRotate->z += 0.1f; }
		}// Scope end

		break;
	case GLFW_KEY_J:
		{
			if ( isShiftDownAlone(mods) )	{ ::g_pObjectToRotate->rotSpeed.x -= -0.01f; }
			else							{ ::g_pObjectToRotate->z -= 0.1f; }
			break;
		}// Scope end
	case GLFW_KEY_H:
	{
			glm::quat rotationChange( glm::vec3( 0.0f, 0.05f, 0.0f) );
			// Combine by multiplying (just like a matrix)
			::g_pObjectToRotate->qOrientaion *= rotationChange;
			break;
	}
	case GLFW_KEY_K:
	{
			glm::quat rotationChange( glm::vec3( 0.0f, -0.05f, 0.0f) );
			// Combine by multiplying (just like a matrix)
			::g_pObjectToRotate->qOrientaion *= rotationChange;
			break;
	}
	case GLFW_KEY_Y:
	{
			glm::quat rotationChange( glm::vec3( 0.0f, 0.0f, 0.05f) );
			// Combine by multiplying (just like a matrix)
			::g_pObjectToRotate->qOrientaion *= rotationChange;
			break;
	}
	case GLFW_KEY_I:
	{
			glm::quat rotationChange( glm::vec3( 0.0f, 0.0f, -0.05f) );
			// Combine by multiplying (just like a matrix)
			::g_pObjectToRotate->qOrientaion *= rotationChange;
			break;
	}

	break;



	// Light WSAD, QE, but for the light...
	case GLFW_KEY_A:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.x -= g_CameraMovementSpeed; }
		else										
		{ ::g_CameraX -= g_CameraMovementSpeed; }
		break;
	case GLFW_KEY_D:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.x += g_CameraMovementSpeed; }
		else										
		{ ::g_CameraX += g_CameraMovementSpeed; }
		break;
	case GLFW_KEY_W:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.z -= g_CameraMovementSpeed; }
		else										
		{ ::g_CameraZ -= g_CameraMovementSpeed; }
		break;
	case GLFW_KEY_S:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.z += g_CameraMovementSpeed; }
		else										
		{ ::g_CameraZ += g_CameraMovementSpeed; }
		break;
	case GLFW_KEY_Q:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.y -= g_CameraMovementSpeed; }
		else										
		{ ::g_CameraY -= g_CameraMovementSpeed; }
		break;
	case GLFW_KEY_E:
		if ( isShiftDownAlone(mods) )	
		{ lightPos.y += g_CameraMovementSpeed; }
		else										
		{ ::g_CameraY += g_CameraMovementSpeed; }
		break;

	case GLFW_KEY_N:
		if ( isShiftDownAlone(mods) )	{ ::g_textureMod.x -= 0.01f; }
		else							{ ::g_textureMod.x += 0.01f; }
		break;
	case GLFW_KEY_M:
		if ( isShiftDownAlone(mods) )	{ ::g_textureMod.y -= 0.01f; }
		else							{ ::g_textureMod.y += 0.01f; }
		break;

	// Adjust lights attenuation
	case GLFW_KEY_1:
		if ( isShiftDownAlone(mods) ) {	::lightLinearAtten *= 0.99f; }	// Drop it by 1%
		break;
	case GLFW_KEY_2:
		if ( isShiftDownAlone(mods) ) {	::lightLinearAtten *= 1.01f;}	// Increase by 1%
		break;
	case GLFW_KEY_3:
		if ( isShiftDownAlone(mods) ) {	::lightQuadAtten *= 0.99f;}	
		break;
	case GLFW_KEY_4:
		if ( isShiftDownAlone(mods) ) {	::lightQuadAtten *= 1.01f;}	
		break;
	case GLFW_KEY_5:
		if ( isShiftDownAlone(mods) ) { ::bDrawLightDebugSpheres = false; }
		break;
	case GLFW_KEY_6:
		if ( isShiftDownAlone(mods) ) { ::bDrawLightDebugSpheres = true; }
		break;
	case GLFW_KEY_7:
		if ( isShiftDownAlone(mods) ) { ::bDrawLightDebugLocation = false; }
		break;
	case GLFW_KEY_8:
		if ( isShiftDownAlone(mods) ) { ::bDrawLightDebugLocation = true; }
		break;

	}//switch (key)




	// Update the camera position in the title...
//	glfwSetWindowTitle( window, "Sexy" );

	std::stringstream ssTitle;
	ssTitle << "Camera: " 
		<< ::g_CameraX << ", "
		<< ::g_CameraY << ", "
		<< ::g_CameraZ
		<< "  Light: " 
		<< ::lightPos.x << ", " 
		<< ::lightPos.y << ", " 
		<< ::lightPos.z
		<< " Lin: " << ::lightLinearAtten
		<< " Quad: " << ::lightQuadAtten;
	glfwSetWindowTitle( window, ssTitle.str().c_str() );


	return;
}
