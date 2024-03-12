#include "cLightManager.h"
#include <sstream> //"string stream"


cLightManager::cLlightNUB::cLlightNUB()
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
};

bool cLightManager::SetUpLightNUB(GLuint shaderID)
{
    // Please give us one buffer "name" (i.e. number)
    glGenBuffers(1, &(this->m_lightNUB_Buffer_ID) );

    // Init the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, this->m_lightNUB_Buffer_ID);

    // How big should this buffer be, in bytes
    // 
    // cLlightNUB theLightsForNUB[NUMBER_OF_LIGHTS_IM_USING];
    //
    unsigned int size_in_bytes_light_buffer = sizeof(cLlightNUB) * NUMBER_OF_LIGHTS_IM_USING;

    glBufferData(GL_UNIFORM_BUFFER,                 // Type of buffer
                 size_in_bytes_light_buffer,          // Number of bytes in size
                 NULL,                              // If NOT null, then copy the data
                 GL_DYNAMIC_DRAW);                  // Hint for how this will be used


    // Unbind 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);     // 0 releases any binding


    // Connect this buffer to a binding point

    // Get the ID of the Named Uniform Block
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetUniformBlockIndex.xhtml
    // NOTE: glGetUniformBlockIndex returns the special identifier, GL_INVALID_INDEX
    GLuint lightDataNUB_ID = glGetUniformBlockIndex(shaderID, "lightDataNUB");

    if (lightDataNUB_ID == GL_INVALID_INDEX)
    {
        return false;
    }



    // Now bind the buffer to the binding point (which you pick)
    const GLuint lightDataNUB_ID_binding_Point_number = 12;     // <-- I picked 12 for no reason
    glUniformBlockBinding(shaderID, lightDataNUB_ID,
                          lightDataNUB_ID_binding_Point_number);

    // Tie the buffer we created to the same binding point
    // Uniform block binding indices refer to blocks bound to indices in the 
    //  GL_UNIFORM_BUFFER indexed target with glBindBufferRange.
    glBindBufferRange(GL_UNIFORM_BUFFER,                                // Type of buffer
                      lightDataNUB_ID_binding_Point_number,             // Binding point
                      this->m_lightNUB_Buffer_ID,                      // Buffer ID ('name')
                      0,                                                // Offset (in bytes)
                      size_in_bytes_light_buffer);


	return true;
}

// NOTE: This will also copy the values from the theLights array
//          to the theLightsNUB array
// (So we can keep the code that updates the theLights array)
void cLightManager::UpdateLightNUB(GLuint shaderID)
{
	// Copy the data fromt the theLights to the theLightsNUB
	for (unsigned int index = 0; index != NUMBER_OF_LIGHTS_IM_USING; index++)
	{
		this->theLightsForNUB->position = this->theLights->position;
		this->theLightsForNUB->atten = this->theLights->atten;
		this->theLightsForNUB->diffuse = this->theLights->diffuse;
		this->theLightsForNUB->direction = this->theLights->diffuse;
		this->theLightsForNUB->param1 = this->theLights->param1;
		this->theLightsForNUB->param2 = this->theLights->param2;
		this->theLightsForNUB->specular = this->theLights->specular;
	}

    // Connect ('bind') to this buffer
    glBindBuffer(GL_UNIFORM_BUFFER, this->m_lightNUB_Buffer_ID);

    // Copy the data from the C++ side
    // 
    // cLlightNUB theLightsForNUB[NUMBER_OF_LIGHTS_IM_USING];
    //
    unsigned int size_in_bytes_light_buffer = sizeof(cLlightNUB) * NUMBER_OF_LIGHTS_IM_USING;

    glBufferSubData(GL_UNIFORM_BUFFER,
                    0,
                    size_in_bytes_light_buffer,
                    &(this->theLightsForNUB) ); // Pointer to base of the light array

    // Release the binding
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return;
}
