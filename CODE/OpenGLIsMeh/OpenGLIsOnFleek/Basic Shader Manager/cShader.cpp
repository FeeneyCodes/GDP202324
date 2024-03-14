#include "cShaderManager.h"
#include "../OpenGLCommon.h"	// glfw.h, etc. (openGL.h)

#include <iostream>

cShaderManager::cShader::cShader()
{
	this->ID = 0;
	this->shaderType = cShader::UNKNOWN;
	return;
}

cShaderManager::cShader::~cShader() 
{
	return;
}

std::string cShaderManager::cShader::getShaderTypeString(void)
{
	switch ( this->shaderType )
	{
	case cShader::VERTEX_SHADER:
		return "VERTEX_SHADER";
		break;
	case cShader::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
		break;
	//
	case cShader::GEOMETRY_SHADER:
		return "GEOMETRY_SHADER";
		break;
	//
	case cShader::UNKNOWN:
	default:
		return "UNKNOWN_SHADER_TYPE";
		break;
	}
	// Should never reach here...
	return "UNKNOWN_SHADER_TYPE";
}


//// TODO: For the students to do, because it's FUN, FUN, FUN
//std::map< std::string /*name of uniform variable*/,
//		    unsigned int /* uniform location ID */ > 
//						mapUniformName_to_UniformLocation;

// Look up the uniform inside the shader, then save it, if it finds it
bool cShaderManager::cShaderProgram::LoadUniformLocation(std::string variableName)
{
	// 
	GLint uniLocation = glGetUniformLocation(this->ID, 
											 variableName.c_str() );
	// Did it find it (not -1)
	if ( uniLocation == -1 )
	{	// Nope.
		return false;
	}
	// Save it
	this->mapUniformName_to_UniformLocation[variableName.c_str()] = uniLocation;

	return true;	
}

// Look up the uniform location and save it.
int cShaderManager::cShaderProgram::getUniformID_From_Name(std::string name)
{
	std::map< std::string /*name of uniform variable*/,
			  int /* uniform location ID */ >::iterator 
		itUniform = this->mapUniformName_to_UniformLocation.find(name);

	if ( itUniform == this->mapUniformName_to_UniformLocation.end() )
	{
		return -1;		// OpenGL uniform not found value
	}

	return itUniform->second;		// second if the "int" value
}


bool cShaderManager::cShaderProgram::LoadActiveUniforms(void)
{
	// Get the number of active uniforms in this shader
	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetProgram.xhtml
	// https://docs.gl/gl3/glGetActiveUniformsiv
	//
	//	void glGetProgramiv(GLuint program,
	//						GLenum pname,
	//						GLint * params);
	// 
	// glGetProgram with argument GL_ACTIVE_UNIFORMS or GL_ACTIVE_UNIFORM_MAX_LENGTH.

	GLint numberOfActiveUniforms = 0;
	glGetProgramiv(this->ID,	// Shader program ID ('name')
				   GL_ACTIVE_UNIFORMS,		// Give me how many active uniforms there are
				   &numberOfActiveUniforms);

	// Set aside space to hold all the uniform names
	GLint uniformStringNameBufferLength = 0;
	glGetProgramiv(this->ID,	// Shader program ID ('name')
				   GL_ACTIVE_UNIFORM_MAX_LENGTH,		// Give me how many active uniforms there are
				   &uniformStringNameBufferLength);



	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
	// 
	//	void glGetActiveUniform(GLuint program,
	//							GLuint index,
	//							GLsizei bufSize,
	//							GLsizei * length,
	//							GLint * size,
	//							GLenum * type,
	//							GLchar * name);

	std::cout << std::endl;
	std::cout << "Active uniforms in this shader: " << std::endl;
	std::cout << std::endl;

	// String buffer to hold the uniform name
	char* uniformNameBuffer = new char[uniformStringNameBufferLength];

	for ( unsigned int uniformIndex = 0; uniformIndex != numberOfActiveUniforms; uniformIndex++ )
	{
		GLsizei uniformNameActualStringLength = 0;
		GLint uniformVariableNumberBytes = 0;
		GLenum uniformTypeEnum = 0;

		glGetActiveUniform(this->ID,						// Shader ID ('name')
						   uniformIndex,					// Index from 0 to glGetProgramiv(GL_ACTIVE_UNIFORMS) returned
						   uniformStringNameBufferLength,	// Size of string buffer
						   &uniformNameActualStringLength,	// Actual uniform name string length
						   &uniformVariableNumberBytes,		// Size (an array or not)
						   &uniformTypeEnum,					// Uniform type (float, vec2, etc.)
						   uniformNameBuffer);

		std::string strUniformName(uniformNameBuffer);

		// Save the uniform location
		this->LoadUniformLocation(strUniformName);

		// Print out all this goodness:
		std::cout
			<< "location: " << uniformIndex << "  "
			<< strUniformName
			<< " size: " << uniformVariableNumberBytes
			<< " type: " << this->getTypeFromGLenum(uniformTypeEnum) << std::endl;

	}//for ( unsigned int uniformIndex..
					    

	return true;
}


// Translates the GLenum uniform type to a human readable string
// GL_FLOAT, GL_FLOAT_VEC2, GL_DOUBLE, etc.
std::string cShaderManager::cShaderProgram::getTypeFromGLenum(unsigned int typeEnum)
{
	switch (typeEnum) 
	{
	case GL_FLOAT: return "float"; break;
	case GL_FLOAT_VEC2: return "vec2"; break;
	case GL_FLOAT_VEC3: return "vec3"; break;
	case GL_FLOAT_VEC4: return "vec4"; break;
	case GL_DOUBLE: return "double"; break;
	case GL_DOUBLE_VEC2: return "dvec2"; break;
	case GL_DOUBLE_VEC3: return "dvec3"; break;
	case GL_DOUBLE_VEC4: return "dvec4"; break;
	case GL_INT: return "int"; break;
	case GL_INT_VEC2: return "ivec2"; break;
	case GL_INT_VEC3: return "ivec3"; break;
	case GL_INT_VEC4: return "ivec4"; break;
	case GL_UNSIGNED_INT: return "unsigned"; break;
	case GL_UNSIGNED_INT_VEC2: return "uvec2"; break;
	case GL_UNSIGNED_INT_VEC3: return "uvec3"; break;
	case GL_UNSIGNED_INT_VEC4: return "uvec4"; break;
	case GL_BOOL: return "bool"; break;
	case GL_BOOL_VEC2: return "bvec2"; break;
	case GL_BOOL_VEC3: return "bvec3"; break;
	case GL_BOOL_VEC4: return "bvec4"; break;
	case GL_FLOAT_MAT2: return "mat2"; break;
	case GL_FLOAT_MAT3: return "mat3"; break;
	case GL_FLOAT_MAT4: return "mat4"; break;
	case GL_FLOAT_MAT2x3: return "mat2x3"; break;
	case GL_FLOAT_MAT2x4: return "mat2x4"; break;
	case GL_FLOAT_MAT3x2: return "mat3x2"; break;
	case GL_FLOAT_MAT3x4: return "mat3x4"; break;
	case GL_FLOAT_MAT4x2: return "mat4x2"; break;
	case GL_FLOAT_MAT4x3: return "mat4x3"; break;
	case GL_DOUBLE_MAT2: return "dmat2"; break;
	case GL_DOUBLE_MAT3: return "dmat3"; break;
	case GL_DOUBLE_MAT4: return "dmat4"; break;
	case GL_DOUBLE_MAT2x3: return "dmat2x3"; break;
	case GL_DOUBLE_MAT2x4: return "dmat2x4"; break;
	case GL_DOUBLE_MAT3x2: return "dmat3x2"; break;
	case GL_DOUBLE_MAT3x4: return "dmat3x4"; break;
	case GL_DOUBLE_MAT4x2: return "dmat4x2"; break;
	case GL_DOUBLE_MAT4x3: return "dmat4x3"; break;
	case GL_SAMPLER_1D: return "sampler1D"; break;
	case GL_SAMPLER_2D: return "sampler2D"; break;
	case GL_SAMPLER_3D: return "sampler3D"; break;
	case GL_SAMPLER_CUBE: return "samplerCube"; break;
	case GL_SAMPLER_1D_SHADOW: return "sampler1DShadow"; break;
	case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow"; break;
	case GL_SAMPLER_1D_ARRAY: return "sampler1DArray"; break;
	case GL_SAMPLER_2D_ARRAY: return "sampler2DArray"; break;
	case GL_SAMPLER_1D_ARRAY_SHADOW: return "sampler1DArrayShadow"; break;
	case GL_SAMPLER_2D_ARRAY_SHADOW: return "sampler2DArrayShadow"; break;
	case GL_SAMPLER_2D_MULTISAMPLE: return "sampler2DMS"; break;
	case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return "sampler2DMSArray"; break;
	case GL_SAMPLER_CUBE_SHADOW: return "samplerCubeShadow"; break;
	case GL_SAMPLER_BUFFER: return "samplerBuffer"; break;
	case GL_SAMPLER_2D_RECT: return "sampler2DRect"; break;
	case GL_SAMPLER_2D_RECT_SHADOW: return "sampler2DRectShadow"; break;
	case GL_INT_SAMPLER_1D: return "isampler1D"; break;
	case GL_INT_SAMPLER_2D: return "isampler2D"; break;
	case GL_INT_SAMPLER_3D: return "isampler3D"; break;
	case GL_INT_SAMPLER_CUBE: return "isamplerCube"; break;
	case GL_INT_SAMPLER_1D_ARRAY: return "isampler1DArray"; break;
	case GL_INT_SAMPLER_2D_ARRAY: return "isampler2DArray"; break;
	case GL_INT_SAMPLER_2D_MULTISAMPLE: return "isampler2DMS"; break;
	case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return "isampler2DMSArray"; break;
	case GL_INT_SAMPLER_BUFFER: return "isamplerBuffer"; break;
	case GL_INT_SAMPLER_2D_RECT: return "isampler2DRect"; break;
	case GL_UNSIGNED_INT_SAMPLER_1D: return "usampler1D"; break;
	case GL_UNSIGNED_INT_SAMPLER_2D: return "usampler2D"; break;
	case GL_UNSIGNED_INT_SAMPLER_3D: return "usampler3D"; break;
	case GL_UNSIGNED_INT_SAMPLER_CUBE: return "usamplerCube"; break;
	case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return "usampler2DArray"; break;
	case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return "usampler2DArray"; break;
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return "usampler2DMS"; break;
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return "usampler2DMSArray"; break;
	case GL_UNSIGNED_INT_SAMPLER_BUFFER: return "usamplerBuffer"; break;
	case GL_UNSIGNED_INT_SAMPLER_2D_RECT: return "usampler2DRect"; break;
	case GL_IMAGE_1D: return "image1D"; break;
	case GL_IMAGE_2D: return "image2D"; break;
	case GL_IMAGE_3D: return "image3D"; break;
	case GL_IMAGE_2D_RECT: return "image2DRect"; break;
	case GL_IMAGE_CUBE: return "imageCube"; break;
	case GL_IMAGE_BUFFER: return "imageBuffer"; break;
	case GL_IMAGE_1D_ARRAY: return "image1DArray"; break;
	case GL_IMAGE_2D_ARRAY: return "image2DArray"; break;
	case GL_IMAGE_2D_MULTISAMPLE: return "image2DMS"; break;
	case GL_IMAGE_2D_MULTISAMPLE_ARRAY: return "image2DMSArray"; break;
	case GL_INT_IMAGE_1D: return "iimage1D"; break;
	case GL_INT_IMAGE_2D: return "iimage2D"; break;
	case GL_INT_IMAGE_3D: return "iimage3D"; break;
	case GL_INT_IMAGE_2D_RECT: return "iimage2DRect"; break;
	case GL_INT_IMAGE_CUBE: return "iimageCube"; break;
	case GL_INT_IMAGE_BUFFER: return "iimageBuffer"; break;
	case GL_INT_IMAGE_1D_ARRAY: return "iimage1DArray"; break;
	case GL_INT_IMAGE_2D_ARRAY: return "iimage2DArray"; break;
	case GL_INT_IMAGE_2D_MULTISAMPLE: return "iimage2DMS"; break;
	case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return "iimage2DMSArray"; break;
	case GL_UNSIGNED_INT_IMAGE_1D: return "uimage1D"; break;
	case GL_UNSIGNED_INT_IMAGE_2D: return "uimage2D"; break;
	case GL_UNSIGNED_INT_IMAGE_3D: return "uimage3D"; break;
	case GL_UNSIGNED_INT_IMAGE_2D_RECT: return "uimage2DRect"; break;
	case GL_UNSIGNED_INT_IMAGE_CUBE: return "uimageCube"; break;
	case GL_UNSIGNED_INT_IMAGE_BUFFER: return "uimageBuffer"; break;
	case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return "uimage1DArray"; break;
	case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return "uimage2DArray"; break;
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return "uimage2DMS"; break;
	case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return "uimage2DMSArray"; break;
	case GL_UNSIGNED_INT_ATOMIC_COUNTER: return "atomic_uint"; break;
	}
	return "UNKNOWN";
}

