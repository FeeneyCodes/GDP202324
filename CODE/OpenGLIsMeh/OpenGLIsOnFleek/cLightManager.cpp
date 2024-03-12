#include "cLightManager.h"
#include <sstream> //"string stream"

cLight::cLight()
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	this->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// White light
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// White light

	// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	this->atten = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);	
	// Spot, directional lights
	// (Default is stright down)
	this->direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);	
	// x = lightType, y = inner angle, z = outer angle, w = TBD
	// type = 0 => point light
	this->param1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					// 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	// x = 0 for off, 1 for on
	this->param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


	this->position_UL = -1;
	this->diffuse_UL = -1;
	this->specular_UL = -1;
	this->atten_UL = -1;
	this->direction_UL = -1;
	this->param1_UL = -1;
	this->param2_UL = -1;

}

	// And the uniforms:
void cLight::TurnOn(void)
{
	// x = 0 for off, 1 for on
	this->param2.x = 1.0f;		// Turn on 
	return;
}

void cLight::TurnOff(void)
{
	// x = 0 for off, 1 for on
	this->param2.x = 0.0f;		// Turn off 
	return;
}


void cLightManager::SetUniformLocations(GLuint shaderID)
{

//	std::stringstream ssLight;
//	ssLight << "theLights[" << 0 << "].position";
//	glGetUniformLocation(shaderID, ssLight.str().c_str());

//	// vec4 position;
//	this->theLights[0].position_UL = glGetUniformLocation(shaderID, "theLights[0].position");
//	//        vec4 diffuse;	// Colour of the light (used for diffuse)
//	this->theLights[0].diffuse_UL = glGetUniformLocation(shaderID, "theLights[0].diffuse");
//	//        vec4 specular;	// rgb = highlight colour, w = power
//	this->theLights[0].specular_UL = glGetUniformLocation(shaderID, "theLights[0].specular");
//	//        vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
//	this->theLights[0].atten_UL = glGetUniformLocation(shaderID, "theLights[0].atten");
//	//        vec4 direction;	// Spot, directional lights
//	this->theLights[0].direction_UL = glGetUniformLocation(shaderID, "theLights[0].direction");
//	//        vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//	this->theLights[0].param1_UL = glGetUniformLocation(shaderID, "theLights[0].param1");
//	//        vec4 param2;	// x = 0 for off, 1 for on
//	this->theLights[0].param2_UL = glGetUniformLocation(shaderID, "theLights[0].param2");
//
//
//	// vec4 position;
//	this->theLights[1].position_UL = glGetUniformLocation(shaderID, "theLights[1].position");
//	//        vec4 diffuse;	// Colour of the light (used for diffuse)
//	this->theLights[1].diffuse_UL = glGetUniformLocation(shaderID, "theLights[1].diffuse");
//	//        vec4 specular;	// rgb = highlight colour, w = power
//	this->theLights[1].specular_UL = glGetUniformLocation(shaderID, "theLights[1].specular");
//	//        vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
//	this->theLights[1].atten_UL = glGetUniformLocation(shaderID, "theLights[1].atten");
//	//        vec4 direction;	// Spot, directional lights
//	this->theLights[1].direction_UL = glGetUniformLocation(shaderID, "theLights[1].direction");
//	//        vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//	this->theLights[1].param1_UL = glGetUniformLocation(shaderID, "theLights[1].param1");
//	//        vec4 param2;	// x = 0 for off, 1 for on
//	this->theLights[1].param2_UL = glGetUniformLocation(shaderID, "theLights[1].param2");
//	// And so on, for all the lights.


	return;
}

// This is called every frame
void cLightManager::UpdateUniformValues(GLuint shaderID)
{
//	for ( unsigned int index = 0; index != cLightManager::NUMBER_OF_LIGHTS_IM_USING; index++ )
//	{
//		glUniform4f(theLights[index].position_UL,
//					theLights[index].position.x,
//					theLights[index].position.y,
//					theLights[index].position.z,
//					theLights[index].position.w);
//
//		glUniform4f(theLights[index].diffuse_UL,
//					theLights[index].diffuse.x,
//					theLights[index].diffuse.y,
//					theLights[index].diffuse.z,
//					theLights[index].diffuse.w);
//
//		glUniform4f(theLights[index].specular_UL,
//					theLights[index].specular.x,
//					theLights[index].specular.y,
//					theLights[index].specular.z,
//					theLights[index].specular.w);
//
//		glUniform4f(theLights[index].atten_UL,
//					theLights[index].atten.x,
//					theLights[index].atten.y,
//					theLights[index].atten.z,
//					theLights[index].atten.w);
//
//		glUniform4f(theLights[index].direction_UL,
//					theLights[index].direction.x,
//					theLights[index].direction.y,
//					theLights[index].direction.z,
//					theLights[index].direction.w);
//
//		glUniform4f(theLights[index].param1_UL,
//					theLights[index].param1.x,
//					theLights[index].param1.y,
//					theLights[index].param1.z,
//					theLights[index].param1.w);
//
//		glUniform4f(theLights[index].param2_UL,
//					theLights[index].param2.x,
//					theLights[index].param2.y,
//					theLights[index].param2.z,
//					theLights[index].param2.w);
//	}// for ( unsigned int index...


	return;
}


cLightManager::cLightManager()
{
}