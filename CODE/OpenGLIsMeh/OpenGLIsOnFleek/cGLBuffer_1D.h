#pragma once

#include "OpenGLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <vector>

class cGLBuffer_1D
{
public:
	bool init(unsigned int shaderID, std::vector< glm::vec4 > &vecTheData);
	bool updateValues(std::vector< glm::vec4 > &vecTheData);

	//bool updateValue(unsigned int index, glm::vec4 newValue);

	bool getData(std::vector< glm::vec4 >& vecTheData, unsigned int maxIndex = 0);

	GLint getInputBufferID(void)
	{
		return this->m_InputBufferID;
	}
	GLint getOutputBufferID(void)
	{
		return this->m_OutputBufferID;
	}
private:
	GLuint m_InputBufferID = 0;
	GLuint m_OutputBufferID = 0;
	
	// Where the data is:
	glm::vec4* m_theInputData = NULL;
	glm::vec4* m_theOutputData = NULL;
	unsigned int m_BufferSize = 0;
};