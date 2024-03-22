#include "cGLBuffer_1D.h"

bool cGLBuffer_1D::init(unsigned int shaderID, std::vector< glm::vec4 >& vecTheData)
{

	if ( vecTheData.empty() )
	{
		return false;
	}

	this->m_BufferSize = (unsigned int)vecTheData.size();

	this->m_InputBufferID = 0;
	glGenTextures(1, &(this->m_InputBufferID));

	// Allocate the data
	this->m_theInputData = new glm::vec4[this->m_BufferSize];

	// Copy the data to the local array
	for ( unsigned int index = 0; index != this->m_BufferSize; index++ )
	{
		this->m_theInputData[index] = vecTheData[index];
	}
	
	//glActiveTexture( textureUnit );	// GL_TEXTURE0, GL_TEXTURE1, etc.
	glBindTexture(GL_TEXTURE_1D, this->m_InputBufferID);

	// Then copy the data over
	//https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage1D.xhtml
	glTexImage1D(GL_TEXTURE_1D,		// target (2D, 3D, etc.)		// OpenGL 2.0 and up
			 0,					// MIP map level 
			 GL_RGBA32F,			// internal format
			 this->m_BufferSize,	// width (pixels)	
			 0,					// border (0 or 1)
			 GL_RGBA,			// format of pixel data
			 GL_FLOAT,			// type of pixel data
			 (void*)this->m_theInputData);	// pointer to data in memory


	glBindTexture(GL_TEXTURE_1D, 0);


	// Set up the output buffer, too (even though there's no data there, yet)

	this->m_OutputBufferID = 0;
	glGenTextures(1, &(this->m_OutputBufferID));

	// Allocate the data
	this->m_theOutputData = new glm::vec4[this->m_BufferSize];

	// Clear it to all zeros...
	memset(this->m_theOutputData, 0, this->m_BufferSize * sizeof(glm::vec4));

	glBindTexture(GL_TEXTURE_1D, this->m_OutputBufferID);

	glTexStorage1D(GL_TEXTURE_1D, 1, GL_RGBA32F, this->m_BufferSize); 

	glBindTexture(GL_TEXTURE_1D, 0);

	return true;
}

bool cGLBuffer_1D::updateValues(std::vector< glm::vec4 >& vecTheData)
{

	// Copy the data to the local array
	for (unsigned int index = 0; index != this->m_BufferSize; index++)
	{
		this->m_theInputData[index] = vecTheData[index];
	}

		//glActiveTexture( textureUnit );	// GL_TEXTURE0, GL_TEXTURE1, etc.
	glBindTexture(GL_TEXTURE_1D, this->m_InputBufferID);

	// Then copy the data over
	//https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexSubImage1D.xhtml
	// 
	// NOTE: glTexImage1D() will work, but will allocate a NEW buffer
	// 
	glTexSubImage1D(GL_TEXTURE_1D,		// target (2D, 3D, etc.)		// OpenGL 2.0 and up
				 0,						// MIP map level 
				 0,						// xoffset
				 this->m_BufferSize,	// width (pixels)	
				 GL_RGBA,			// format of pixel data
				 GL_FLOAT,			// type of pixel data
				 (void*)this->m_theInputData);	// pointer to data in memory


	glBindTexture(GL_TEXTURE_1D, 0);

	return true;
}

bool cGLBuffer_1D::getData(std::vector< glm::vec4 >& vecTheData, unsigned int maxIndex /*=0*/)
{
	if ( maxIndex == 0 )
	{
		return false;
	}

	//glActiveTexture( textureUnit );	// GL_TEXTURE0, GL_TEXTURE1, etc.
	glBindTexture(GL_TEXTURE_1D, this->m_OutputBufferID);


	// Read the image back to the local array
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetTexImage.xhtml
	//
	glGetnTexImage(	GL_TEXTURE_1D,		// GLenum target
					0,					// MipMap level GLint level
					GL_RGBA,			// GLenum format
					GL_FLOAT,			// GLenum type (of each R, G, B, etc.)
					this->m_BufferSize,	// GLsizei bufSize,
					(void*)this->m_theOutputData);

	glBindTexture(GL_TEXTURE_1D, 0);

	return true;
}

//bool cGLBuffer_1D::updateValue(unsigned int index, glm::vec4 newValue)
//{
//	this->m_theInputData[index] = newValue;
//	return true;
//}
